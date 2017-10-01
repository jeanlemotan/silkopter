#include "Fec_Encoder.h"
#include <mutex>
#include <condition_variable>
#include <deque>
#include <atomic>
#include <chrono>
#include <algorithm>
#include <cassert>
#include <cstring>
#include "Pool.h"
#include "utils/fec.h"

static constexpr unsigned BLOCK_NUMS[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                           10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                                           21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };

const uint8_t Fec_Encoder::MAX_CODING_K;
const uint8_t Fec_Encoder::MAX_CODING_N;
const size_t Fec_Encoder::PAYLOAD_OVERHEAD;

#pragma pack(push, 1)

struct Datagram_Header
{
//    uint32_t crc = 0;
    uint32_t block_index : 24;
    uint32_t datagram_index : 8;
    uint16_t size : 16;
};

#pragma pack(pop)

static_assert(Fec_Encoder::PAYLOAD_OVERHEAD == sizeof(Datagram_Header), "Check the PAYLOAD_OVERHEAD size");

//A     B       C       D       E       F
//A     Bx      Cx      Dx      Ex      Fx

struct Fec_Encoder::TX
{
    TX(size_t max_queue_length)
        : datagram_queue(max_queue_length)
    {}

    struct Datagram
    {
        std::vector<uint8_t> data;
    };
    typedef Pool<Datagram>::Ptr Datagram_ptr;

    Pool<Datagram> datagram_pool;

    ////////
    //These are accessed by both the TX thread and the main thread
    Queue<Datagram_ptr> datagram_queue;
    ////////

    ////////
    //these live in the TX thread only
    std::vector<Datagram_ptr> block_datagrams;
    std::vector<Datagram_ptr> block_fec_datagrams;
    ///////

    Datagram_ptr crt_datagram;

    uint32_t last_block_index = 1;
};


struct Fec_Encoder::RX
{
    RX(size_t max_queue_length)
        : datagram_queue(max_queue_length)
    {}

    struct Datagram
    {
        bool is_processed = false;
        uint32_t block_index = 0;
        uint32_t datagram_index = 0;
        std::vector<uint8_t> data;
    };
    typedef Pool<Datagram>::Ptr Datagram_ptr;

    Pool<Datagram> datagram_pool;

    ////////
    //These are accessed by both the RX thread and the main thread
    Queue<Datagram_ptr> datagram_queue;
    ////////

    struct Block
    {
        uint32_t block_index = 0;

        std::vector<Datagram_ptr> datagrams;
        std::vector<Datagram_ptr> fec_datagrams;
    };
    typedef Pool<Block>::Ptr Block_ptr;

    Pool<Block> block_pool;
    std::deque<Block_ptr> block_queue;

    Clock::time_point last_block_tp = Clock::now();
    Clock::time_point last_datagram_tp = Clock::now();

    std::atomic_uint next_block_index = { 0 };
};


static void seal_datagram(Fec_Encoder::TX::Datagram& datagram, size_t header_offset, uint32_t block_index, uint8_t datagram_index)
{
    assert(datagram.data.size() >= header_offset + sizeof(Fec_Encoder::TX::Datagram));

    Datagram_Header& header = *reinterpret_cast<Datagram_Header*>(datagram.data.data() + header_offset);
//    header.crc = 0;
    header.size = datagram.data.size() - header_offset;
    header.block_index = block_index;
    header.datagram_index = datagram_index;

//    header.crc = q::util::murmur_hash(datagram.data.data() + header_offset, header.size, 0);
}

struct Fec_Encoder::Impl
{
    Impl(size_t max_queue_length)
        : tx(max_queue_length)
        , rx(max_queue_length)
    {}

    TX tx;
    RX rx;
};

////////////////////////////////////////////////////////////////////////////////////////////

Fec_Encoder::Fec_Encoder()
{
}

////////////////////////////////////////////////////////////////////////////////////////////

Fec_Encoder::~Fec_Encoder()
{
    m_exit = true;
    m_impl->tx.datagram_queue.exit();
    m_impl->rx.datagram_queue.exit();
    if (m_thread.joinable())
    {
        m_thread.join();
    }

    if (m_fec)
    {
        fec_free(m_fec);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////

bool Fec_Encoder::add_rx_packet(void const* _data, size_t size, bool block)
{
    if (m_exit)
    {
        return false;
    }

    if (!_data || size == 0)
    {
        return false;
    }

    RX& rx = m_impl->rx;
    uint8_t const* data = reinterpret_cast<uint8_t const*>(_data);

    const Datagram_Header& header = *reinterpret_cast<const Datagram_Header*>(data);
    uint32_t block_index = header.block_index;
    uint32_t datagram_index = header.datagram_index;
    if (datagram_index >= m_coding_n)
    {
        //QLOGE("datagram index out of range: {} > {}", datagram_index, m_coding_n);
        return true;
    }

    {
        RX::Datagram_ptr datagram = rx.datagram_pool.acquire();
        datagram->data.resize(size - sizeof(Datagram_Header));
        datagram->block_index = block_index;
        datagram->datagram_index = datagram_index;
        memcpy(datagram->data.data(), data + sizeof(Datagram_Header), size - sizeof(Datagram_Header));

        rx.datagram_queue.push_back(datagram, block);
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////

bool Fec_Encoder::init_tx(TX_Descriptor const& descriptor)
{
    m_is_tx = true;

    m_tx_descriptor = descriptor;

    m_coding_k = descriptor.coding_k;
    m_coding_n = descriptor.coding_n;

    m_impl.reset(new Impl(m_tx_descriptor.max_enqueued_packets));

    return init();
}

////////////////////////////////////////////////////////////////////////////////////////////

bool Fec_Encoder::init_rx(RX_Descriptor const& descriptor)
{
    m_is_tx = false;

    m_rx_descriptor = descriptor;

    m_coding_k = descriptor.coding_k;
    m_coding_n = descriptor.coding_n;

    m_impl.reset(new Impl(m_rx_descriptor.max_enqueued_packets));

    return init();
}

////////////////////////////////////////////////////////////////////////////////////////////

bool Fec_Encoder::init()
{
    if (m_coding_k == 0 || m_coding_n < m_coding_k || m_coding_k > m_fec_src_datagram_ptrs.size() || m_coding_n > m_fec_dst_datagram_ptrs.size())
    {
        //QLOGE("Invalid coding params: {} / {}" , m_coding_k, m_coding_n);
        return false;
    }

    if (m_fec)
    {
        fec_free(m_fec);
    }
    m_fec = fec_new(m_coding_k, m_coding_n);


    /////////////////////
    //calculate some offsets and sizes
    m_payload_offset = sizeof(Datagram_Header);

    m_transport_datagram_size = m_payload_offset + get_mtu();
    m_payload_size = get_mtu();
    /////////////////////


    m_impl->tx.datagram_pool.on_acquire = [this](TX::Datagram& datagram)
    {
        if (datagram.data.size() < m_payload_offset)
        {
            datagram.data.resize(m_payload_offset);
        }
    };

    m_impl->rx.datagram_pool.on_acquire = [this](RX::Datagram& datagram)
    {
        datagram.block_index = 0;
        datagram.datagram_index = 0;
        datagram.is_processed = false;
        datagram.data.clear();
        datagram.data.reserve(m_transport_datagram_size);
    };
    m_impl->rx.block_pool.on_acquire = [this](RX::Block& block)
    {
        block.block_index = 0;

        block.datagrams.clear();
        block.datagrams.reserve(m_coding_k);

        block.fec_datagrams.clear();
        block.fec_datagrams.reserve(m_coding_n - m_coding_k);
    };
    m_impl->rx.block_pool.on_release = [this](RX::Block& block)
    {
        block.datagrams.clear();
        block.fec_datagrams.clear();
    };


    if (m_is_tx)
    {
        m_thread = std::thread([this]() { tx_thread_proc(); });
    }
    else
    {
        m_thread = std::thread([this]() { rx_thread_proc(); });
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////

Fec_Encoder::Descriptor const& Fec_Encoder::get_descriptor() const
{
    return m_is_tx ? static_cast<Fec_Encoder::Descriptor const&>(m_tx_descriptor) : static_cast<Fec_Encoder::Descriptor const&>(m_rx_descriptor);
}

////////////////////////////////////////////////////////////////////////////////////////////

void Fec_Encoder::tx_thread_proc()
{
    TX& tx = m_impl->tx;

    while (!m_exit)
    {
        size_t start = tx.block_datagrams.size();
        tx.datagram_queue.pop_front(tx.block_datagrams, m_coding_k, true);

        //seal and send the newly added ones
        for (size_t i = start; i < tx.block_datagrams.size(); i++)
        {
            TX::Datagram_ptr datagram = tx.block_datagrams[i];
            seal_datagram(*datagram, m_datagram_header_offset, tx.last_block_index, i);
            if (on_tx_data_encoded)
            {
                on_tx_data_encoded(datagram->data.data(), datagram->data.size());
            }
        }

        //compute fec datagrams
        if (tx.block_datagrams.size() >= m_coding_k)
        {
            if (1)
            {
                //auto start = Clock::now();

                //init data for the fec_encode
                for (size_t i = 0; i < m_coding_k; i++)
                {
                    TX::Datagram_ptr datagram = tx.block_datagrams[i];
                    m_fec_src_datagram_ptrs[i] = datagram->data.data() + m_payload_offset;
                }

                size_t fec_count = m_coding_n - m_coding_k;
                tx.block_fec_datagrams.resize(fec_count);
                for (size_t i = 0; i < fec_count; i++)
                {
                    tx.block_fec_datagrams[i] = tx.datagram_pool.acquire();
                    tx.block_fec_datagrams[i]->data.resize(m_transport_datagram_size);
                    m_fec_dst_datagram_ptrs[i] = tx.block_fec_datagrams[i]->data.data() + m_payload_offset;
                }

                //encode
                fec_encode(m_fec, m_fec_src_datagram_ptrs.data(), m_fec_dst_datagram_ptrs.data(), BLOCK_NUMS + m_coding_k, m_coding_n - m_coding_k, m_payload_size);

                //seal the result
                for (size_t i = 0; i < fec_count; i++)
                {
                    seal_datagram(*tx.block_fec_datagrams[i], m_datagram_header_offset, tx.last_block_index, m_coding_k + i);

                    if (on_tx_data_encoded)
                    {
                        TX::Datagram& datagram = *tx.block_fec_datagrams[i];
                        on_tx_data_encoded(datagram.data.data(), datagram.data.size());
                    }
                }

                //QLOGI("Encoded fec: {}", Clock::now() - start);
            }

            tx.block_datagrams.clear();
            tx.block_fec_datagrams.clear();
            tx.last_block_index++;
        }

        {
            //std::this_thread::sleep_for(std::chrono::milliseconds(1));
            //std::this_thread::yield();

#ifdef DEBUG_THROUGHPUT
            {
                static size_t xxx_data = 0;
                static size_t xxx_real_data = 0;
                static std::chrono::system_clock::time_point xxx_last_tp = std::chrono::system_clock::now();
                xxx_data += tx_buffer.size();
                xxx_real_data += MAX_USER_PACKET_SIZE;
                auto now = std::chrono::system_clock::now();
                if (now - xxx_last_tp >= std::chrono::seconds(1))
                {
                    float r = std::chrono::duration<float>(now - xxx_last_tp).count();
                    QLOGI("Sent: {} KB/s / {} KB/s", float(xxx_data)/r/1024.f, float(xxx_real_data)/r/1024.f);
                    xxx_data = 0;
                    xxx_real_data = 0;
                    xxx_last_tp = now;
                }
            }
#endif

        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////

bool Fec_Encoder::add_tx_packet(void const* _data, size_t size, bool block)
{
    if (m_exit)
    {
        return false;
    }

    TX& tx = m_impl->tx;

    TX::Datagram_ptr& datagram = tx.crt_datagram;

    uint8_t const* data = reinterpret_cast<uint8_t const*>(_data);

    while (size > 0)
    {
        if (!datagram)
        {
            datagram = tx.datagram_pool.acquire();
        }

        size_t s = std::min(size, m_transport_datagram_size - datagram->data.size());
        size_t offset = datagram->data.size();
        datagram->data.resize(offset + s);
        memcpy(datagram->data.data() + offset, data, s);
        data += s;
        size -= s;

        if (datagram->data.size() >= m_transport_datagram_size)
        {
            tx.datagram_queue.push_back(datagram, block);
            datagram = tx.datagram_pool.acquire();
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////

size_t Fec_Encoder::get_mtu() const
{
    return m_is_tx ? m_tx_descriptor.mtu : m_rx_descriptor.mtu;
}

////////////////////////////////////////////////////////////////////////////////////////////

size_t Fec_Encoder::compute_mtu_from_packet_size(size_t packet_size)
{
    if (packet_size < sizeof(Datagram_Header))
    {
        return 0;
    }
    return packet_size - sizeof(Datagram_Header);
}

////////////////////////////////////////////////////////////////////////////////////////////

static size_t s_last_seq_number = 0;

void Fec_Encoder::rx_thread_proc()
{
    RX& rx = m_impl->rx;

    while (!m_exit)
    {
        RX::Datagram_ptr datagram;
        rx.datagram_queue.pop_front(datagram, true);
        if (datagram)
        {
            uint32_t block_index = datagram->block_index;
            uint32_t datagram_index = datagram->datagram_index;
            if (datagram_index >= m_coding_n)
            {
//                printf("datagram index out of range: %d > %d\n", datagram_index, m_coding_n);
                continue;
            }
            if (block_index < rx.next_block_index)
            {
                //printf("Old datagram: %d < %d\n", block_index, rx.next_block_index.load());
                continue;
            }

            RX::Block_ptr block;

            //find the block
            {
                auto iter = std::lower_bound(rx.block_queue.begin(), rx.block_queue.end(), block_index, [](RX::Block_ptr const& l, uint32_t index) { return l->block_index < index; });
                if (iter != rx.block_queue.end() && (*iter)->block_index == block_index)
                {
                    block = *iter;
                }
                else
                {
                    block = rx.block_pool.acquire();
                    block->block_index = block_index;
                    rx.block_queue.insert(iter, block);
                }
            }

            //store datagram
            if (datagram->datagram_index >= m_coding_k)
            {
                auto iter = std::lower_bound(block->fec_datagrams.begin(), block->fec_datagrams.end(), datagram_index, [](RX::Datagram_ptr const& l, uint32_t index) { return l->datagram_index < index; });
                if (iter != block->fec_datagrams.end() && (*iter)->datagram_index == datagram_index)
                {
//                    printf("Duplicated datagram %d from block %d (index %d)\n", datagram_index, block_index, block_index * m_coding_k + datagram_index);
                    continue;
                }
                else
                {
                    block->fec_datagrams.insert(iter, datagram);
                }
            }
            else
            {
                auto iter = std::lower_bound(block->datagrams.begin(), block->datagrams.end(), datagram_index, [](RX::Datagram_ptr const& l, uint32_t index) { return l->datagram_index < index; });
                if (iter != block->datagrams.end() && (*iter)->datagram_index == datagram_index)
                {
//                    printf("Duplicated datagram %d from block %d (index %d)\n", datagram_index, block_index, block_index * m_coding_k + datagram_index);
                    continue;
                }
                else
                {
                    block->datagrams.insert(iter, datagram);
                }
            }
        }


        if (Clock::now() - rx.last_datagram_tp > m_rx_descriptor.reset_duration)
        {
//            printf("Reset block index\n");
            rx.next_block_index = 0;
        }

        while (!rx.block_queue.empty())
        {
            RX::Block_ptr block = rx.block_queue.front();

            //entire block received
            if (block->datagrams.size() >= m_coding_k)
            {
                //printf("Complete block\n");
                for (RX::Datagram_ptr const& d: block->datagrams)
                {
                    uint32_t seq_number = block->block_index * m_coding_k + d->datagram_index;
                    if (!d->is_processed)
                    {
//                        if (s_last_seq_number + 1 != seq_number)
//                            printf("Datagram C %d: %s\n", seq_number, s_last_seq_number + 1 == seq_number ? "Ok" : "Skipped");
//                        s_last_seq_number = seq_number;

                        m_video_stats_data_accumulated += d->data.size();
                        if (on_rx_data_decoded)
                        {
                            on_rx_data_decoded(d->data.data(), d->data.size());
                        }
                        rx.last_datagram_tp = Clock::now();
                        d->is_processed = true;
                    }
                }

                rx.last_block_tp = Clock::now();

                rx.next_block_index = block->block_index + 1;
                rx.block_queue.pop_front();
                continue;
            }

            //try to process consecutive datagrams before the block is finished to minimize latency
            for (size_t i = 0; i < block->datagrams.size(); i++)
            {
                RX::Datagram_ptr const& d = block->datagrams[i];
                if (d->datagram_index == i)
                {
                    uint32_t seq_number = block->block_index * m_coding_k + d->datagram_index;
                    if (!d->is_processed)
                    {
//                        if (s_last_seq_number + 1 != seq_number)
//                            printf("Datagram E %d: %s\n", seq_number, s_last_seq_number + 1 == seq_number ? "Ok" : "Skipped");
//                        s_last_seq_number = seq_number;

                        m_video_stats_data_accumulated += d->data.size();
                        if (on_rx_data_decoded)
                        {
                            on_rx_data_decoded(d->data.data(), d->data.size());
                        }
                        rx.last_datagram_tp = Clock::now();
                        d->is_processed = true;
                    }
                }
                else
                {
                    break;
                }
            }

            //can we fec decode?
            if (block->datagrams.size() + block->fec_datagrams.size() >= m_coding_k)
            {
                //printf("Complete FEC block\n");
                //auto start = Clock::now();

                std::array<unsigned int, 32> indices;
                size_t primary_index = 0;
                size_t used_fec_index = 0;
                for (size_t i = 0; i < m_coding_k; i++)
                {
                    if (primary_index < block->datagrams.size() && i == block->datagrams[primary_index]->datagram_index)
                    {
                        m_fec_src_datagram_ptrs[i] = block->datagrams[primary_index]->data.data();
                        indices[i] = block->datagrams[primary_index]->datagram_index;
                        primary_index++;
                    }
                    else
                    {
                        m_fec_src_datagram_ptrs[i] = block->fec_datagrams[used_fec_index]->data.data();
                        indices[i] = block->fec_datagrams[used_fec_index]->datagram_index;
                        used_fec_index++;
                    }
                }

                //insert the missing datagrams, they will be filled with data by the fec_decode below
                size_t fec_index = 0;
                for (size_t i = 0; i < m_coding_k; i++)
                {
                    if (i >= block->datagrams.size() || i != block->datagrams[i]->datagram_index)
                    {
                        block->datagrams.insert(block->datagrams.begin() + i, rx.datagram_pool.acquire());
                        block->datagrams[i]->data.resize(m_payload_size);
                        block->datagrams[i]->datagram_index = i;
                        m_fec_dst_datagram_ptrs[fec_index++] = block->datagrams[i]->data.data();
                    }
                }

                fec_decode(m_fec, m_fec_src_datagram_ptrs.data(), m_fec_dst_datagram_ptrs.data(), indices.data(), m_payload_size);

                //now dispatch them
                for (size_t i = 0; i < block->datagrams.size(); i++)
                {
                    RX::Datagram_ptr const& d = block->datagrams[i];
                    uint32_t seq_number = block->block_index * m_coding_k + d->datagram_index;
                    if (!d->is_processed)
                    {
//                        if (s_last_seq_number + 1 != seq_number)
//                            printf("Datagram F %d: %s\n", seq_number, s_last_seq_number + 1 == seq_number ? "Ok" : "Skipped");
//                        s_last_seq_number = seq_number;

                        m_video_stats_data_accumulated += d->data.size();
                        if (on_rx_data_decoded)
                        {
                            on_rx_data_decoded(d->data.data(), d->data.size());
                        }
                        rx.last_datagram_tp = Clock::now();
                        d->is_processed = true;
                    }
                }

                //QLOGI("Decoded fac: {}", Clock::now() - start);

                rx.last_block_tp = Clock::now();
                rx.next_block_index = block->block_index + 1;
                rx.block_queue.pop_front();
                continue;
            }

            //skip if too much buffering
            if (rx.block_queue.size() > 3)
            {
                //printf("Skipping block\n");
                for (size_t i = 0; i < block->datagrams.size(); i++)
                {
                    RX::Datagram_ptr const& d = block->datagrams[i];
                    if (!d->is_processed)
                    {
                        uint32_t seq_number = block->block_index * m_coding_k + d->datagram_index;
//                        printf("Skipping %d\n", seq_number);
                    }
                }
                rx.next_block_index = block->block_index + 1;
                rx.block_queue.pop_front();
                continue;
            }

            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
