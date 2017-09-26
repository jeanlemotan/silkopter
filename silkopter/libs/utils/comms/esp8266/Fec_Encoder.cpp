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
    uint16_t is_fec : 1;
    uint16_t size : 15;
};

#pragma pack(pop)

static_assert(Fec_Encoder::PAYLOAD_OVERHEAD == sizeof(Datagram_Header), "Check the PAYLOAD_OVERHEAD size");

//A     B       C       D       E       F
//A     Bx      Cx      Dx      Ex      Fx

struct Fec_Encoder::TX
{
    struct Datagram
    {
        std::vector<uint8_t> data;
    };
    typedef Pool<Datagram>::Ptr Datagram_ptr;

    Pool<Datagram> datagram_pool;

    ////////
    //These are accessed by both the TX thread and the main thread
    std::mutex datagram_queue_mutex;
    std::deque<Datagram_ptr> datagram_queue;
    std::condition_variable datagram_queue_cv;
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
    struct Datagram
    {
        bool is_fec = false;
        bool is_processed = false;
        uint32_t block_index = 0;
        uint32_t datagram_index = 0;
        std::vector<uint8_t> data;
    };
    typedef Pool<Datagram>::Ptr Datagram_ptr;

    Pool<Datagram> datagram_pool;

    ////////
    //These are accessed by both the RX thread and the main thread
    std::mutex datagram_queue_mutex;
    std::deque<Datagram_ptr> datagram_queue;
    std::condition_variable datagram_queue_cv;
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


static void seal_datagram(Fec_Encoder::TX::Datagram& datagram, size_t header_offset, uint32_t block_index, uint8_t datagram_index, bool is_fec)
{
    assert(datagram.data.size() >= header_offset + sizeof(Fec_Encoder::TX::Datagram));

    Datagram_Header& header = *reinterpret_cast<Datagram_Header*>(datagram.data.data() + header_offset);
//    header.crc = 0;
    header.size = datagram.data.size() - header_offset;
    header.block_index = block_index;
    header.datagram_index = datagram_index;
    header.is_fec = is_fec ? 1 : 0;

//    header.crc = q::util::murmur_hash(datagram.data.data() + header_offset, header.size, 0);
}

struct Fec_Encoder::Impl
{
    size_t tx_packet_header_length = 0;

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
    if (m_is_tx)
    {
        {
            std::lock_guard<std::mutex> lg(m_impl->tx.datagram_queue_mutex);
            m_exit = true;
        }
        m_impl->tx.datagram_queue_cv.notify_all();
    }
    else
    {
        {
            std::lock_guard<std::mutex> lg(m_impl->rx.datagram_queue_mutex);
            m_exit = true;
        }
        m_impl->rx.datagram_queue_cv.notify_all();
    }

    if (m_thread.joinable())
    {
        m_thread.join();
    }

    fec_free(m_fec);
}

////////////////////////////////////////////////////////////////////////////////////////////

bool Fec_Encoder::add_rx_packet(void const* _data, size_t size)
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
        std::lock_guard<std::mutex> lg(rx.datagram_queue_mutex);

        RX::Datagram_ptr datagram = rx.datagram_pool.acquire();
        datagram->data.resize(size - sizeof(Datagram_Header));
        datagram->block_index = block_index;
        datagram->datagram_index = datagram_index;
        datagram->is_fec = header.is_fec;
        memcpy(datagram->data.data(), data + sizeof(Datagram_Header), size - sizeof(Datagram_Header));

        rx.datagram_queue.push_back(datagram);
    }
    rx.datagram_queue_cv.notify_all();

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////

bool Fec_Encoder::init_tx(TX_Descriptor const& descriptor)
{
    m_is_tx = true;

    m_tx_descriptor = descriptor;

    m_coding_k = descriptor.coding_k;
    m_coding_n = descriptor.coding_n;

    return init();
}

////////////////////////////////////////////////////////////////////////////////////////////

bool Fec_Encoder::init_rx(RX_Descriptor const& descriptor)
{
    m_is_tx = false;

    m_rx_descriptor = descriptor;

    m_coding_k = descriptor.coding_k;
    m_coding_n = descriptor.coding_n;

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

    m_impl.reset(new Impl);

    /////////////////////
    //calculate some offsets and sizes
    m_payload_offset = sizeof(Datagram_Header);

    m_transport_datagram_size = m_payload_offset + get_mtu();
    m_payload_size = get_mtu();
    /////////////////////


    m_impl->tx.datagram_pool.on_acquire = [this](TX::Datagram& datagram)
    {
        datagram.data.resize(m_payload_offset);
    };

    m_impl->rx.datagram_pool.on_acquire = [this](RX::Datagram& datagram)
    {
        datagram.block_index = 0;
        datagram.datagram_index = 0;
        datagram.is_processed = false;
        datagram.is_fec = false;
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

void Fec_Encoder::tx_thread_proc()
{
    TX& tx = m_impl->tx;

#if 0 //TEST THROUGHPUT
    TX::Datagram_ptr datagram = tx.datagram_pool.acquire();

    size_t s = std::min(8192u, m_transport_datagram_size - datagram->data.size());
    size_t offset = datagram->data.size();
    datagram->data.resize(offset + s);

    auto start = Clock::now();

    size_t packets = 0;
    size_t total_size = 0;

    while (!m_exit)
    {
        int isize = static_cast<int>(datagram->data.size());

        std::lock_guard<std::mutex> lg(tx.pcap.mutex);
        int r = pcap_inject(tx.pcap.pcap, datagram->data.data(), isize);
        if (r <= 0)
        {
            QLOGW("Trouble injecting packet: {} / {}: {}", r, isize, pcap_geterr(tx.pcap.pcap));
            //result = Result::ERROR;
        }
        if (r > 0)
        {
            if (r != isize)
            {
                QLOGW("Incomplete packet sent: {} / {}", r, isize);
            }
            else
            {
                packets++;
                total_size += isize;
            }
        }

        auto now = Clock::now();
        if (now - start > std::chrono::seconds(1))
        {
            float f = std::chrono::duration<float>(now - start).count();
            start = now;

            QLOGI("Packets: {}, Size: {}MB", packets / f, (total_size / (1024.f * 1024.f)) / f);
            packets = 0;
            total_size = 0;
        }
    }
#endif


    while (!m_exit)
    {
        {
            //wait for data
            std::unique_lock<std::mutex> lg(tx.datagram_queue_mutex);
            while (tx.datagram_queue.empty() && !m_exit)
            {
                tx.datagram_queue_cv.wait(lg);
            }
            if (m_exit)
            {
                break;
            }

            //consume as many as it can
            while (!tx.datagram_queue.empty() && tx.block_datagrams.size() < m_coding_k)
            {
                TX::Datagram_ptr datagram = tx.datagram_queue.front();
                tx.datagram_queue.pop_front();

                if (datagram)
                {
                    seal_datagram(*datagram, m_datagram_header_offset, tx.last_block_index, tx.block_datagrams.size(), false);

                    if (on_tx_data_encoded)
                    {
                        on_tx_data_encoded(datagram->data.data(), datagram->data.size());
                    }

                    tx.block_datagrams.push_back(datagram);
                }
            }
        }
        tx.datagram_queue_cv.notify_all();

        //compute fec datagrams
        if (tx.block_datagrams.size() >= m_coding_k)
        {
            if (1)
            {
                //auto start = Clock::now();

                //init data for the fec_encode
                for (size_t i = 0; i < m_coding_k; i++)
                {
                    m_fec_src_datagram_ptrs[i] = tx.block_datagrams[i]->data.data() + m_payload_offset;
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
                    seal_datagram(*tx.block_fec_datagrams[i], m_datagram_header_offset, tx.last_block_index, m_coding_k + i, true);

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

bool Fec_Encoder::add_tx_packet(void const* _data, size_t size)
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

        bool limit_reached = false;
        if (datagram->data.size() >= m_transport_datagram_size)
        {
            //send the current datagram
            {
                std::unique_lock<std::mutex> lg(tx.datagram_queue_mutex);
                if (m_tx_descriptor.blocking)
                {
                    while (tx.datagram_queue.size() >= m_tx_descriptor.max_enqueued_packets && !m_exit)
                    {
                        tx.datagram_queue_cv.wait(lg);
                    }
                    if (m_exit)
                    {
                        return false;
                    }
                }
                if (tx.datagram_queue.size() < m_tx_descriptor.max_enqueued_packets)
                {
                    tx.datagram_queue.push_back(datagram);
                }
                else
                {
                    limit_reached = true;
                }
            }
            datagram = tx.datagram_pool.acquire();

            tx.datagram_queue_cv.notify_all();
        }

        if (limit_reached)
        {
            return false;
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

void Fec_Encoder::rx_thread_proc()
{
    RX& rx = m_impl->rx;

    while (true)
    {
        std::unique_lock<std::mutex> lg(rx.datagram_queue_mutex);
        while (rx.datagram_queue.empty() && !m_exit)
        {
            rx.datagram_queue_cv.wait(lg);
        }
        if (m_exit)
        {
            break;
        }

        while (!rx.datagram_queue.empty())
        {
            RX::Datagram_ptr datagram = rx.datagram_queue.back();
            rx.datagram_queue.pop_back();

            uint32_t block_index = datagram->block_index;
            uint32_t datagram_index = datagram->datagram_index;
            if (datagram_index >= m_coding_n)
            {
                //QLOGE("datagram index out of range: {} > {}", datagram_index, m_coding_n);
                continue;
            }
            if (block_index < rx.next_block_index)
            {
                //QLOGW("Old datagram: {} < {}", block_index, rx.next_block_index);
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
            if (datagram->is_fec)
            {
                auto iter = std::lower_bound(block->fec_datagrams.begin(), block->fec_datagrams.end(), datagram_index, [](RX::Datagram_ptr const& l, uint32_t index) { return l->datagram_index < index; });
                if (iter != block->fec_datagrams.end() && (*iter)->datagram_index == datagram_index)
                {
                    //QLOGW("Duplicated datagram {} from block {} (index {})", datagram_index, block_index, block_index * m_coding_k + datagram_index);
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
                    //QLOGW("Duplicated datagram {} from block {} (index {})", datagram_index, block_index, block_index * m_coding_k + datagram_index);
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
            rx.next_block_index = 0;
        }

        while (!rx.block_queue.empty())
        {
            RX::Block_ptr block = rx.block_queue.front();

            //entire block received
            if (block->datagrams.size() >= m_coding_k)
            {
                for (RX::Datagram_ptr const& d: block->datagrams)
                {
                    uint32_t seq_number = block->block_index * m_coding_k + d->datagram_index;
                    if (!d->is_processed)
                    {
                        //QLOGI("Datagram {}", seq_number);
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
                        //QLOGI("Datagram {}", seq_number);
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
                        //QLOGI("Datagram F {}", seq_number);
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
                for (size_t i = 0; i < block->datagrams.size(); i++)
                {
                    RX::Datagram_ptr const& d = block->datagrams[i];
                    if (!d->is_processed)
                    {
                        uint32_t seq_number = block->block_index * m_coding_k + d->datagram_index;
                        //QLOGI("Skipping {}", seq_number);
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
