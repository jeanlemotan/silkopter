#include "Video_Streamer.h"
#include <pcap.h>
#include "utils/hw/radiotap/radiotap.h"
#include <mutex>
#include <condition_variable>
#include <deque>
#include <atomic>
#include <boost/intrusive_ptr.hpp>
#include "utils/Pool.h"
#include "fec.h"

#include "QBase.h"

namespace util
{
namespace comms
{

static constexpr unsigned BLOCK_NUMS[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                           10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                                           21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };

static constexpr size_t MAX_PACKET_SIZE = 4192;
static constexpr size_t MAX_USER_PACKET_SIZE = 1470;

static constexpr size_t DEFAULT_RATE_HZ = 26000000;

static std::vector<uint8_t> RADIOTAP_HEADER;

static constexpr size_t SRC_MAC_LASTBYTE  = 15;
static constexpr size_t DST_MAC_LASTBYTE  = 21;

// Penumbra IEEE80211 header
static uint8_t IEEE_HEADER[] =
{
    0x08, 0x01, 0x00, 0x00,
    0x13, 0x22, 0x33, 0x44, 0x55, 0x66,
    0x13, 0x22, 0x33, 0x44, 0x55, 0x66,
    0x13, 0x22, 0x33, 0x44, 0x55, 0x66,
    0x10, 0x86,
};

#pragma pack(push, 1)

struct Penumbra_Radiotap_Header
{
    int32_t channel = 0;
    int32_t channel_flags = 0;
    int32_t rate = 0;
    int32_t antenna = 0;
    int32_t radiotap_flags = 0;
};

struct Datagram_Header
{
//    uint32_t crc = 0;
    uint32_t block_index : 24;
    uint32_t datagram_index : 8;
    uint16_t is_fec : 1;
    uint16_t size : 15;
    uint16_t width;
    uint16_t height;
};

#pragma pack(pop)

//A     B       C       D       E       F
//A     Bx      Cx      Dx      Ex      Fx


struct TX
{
    struct Datagram : public Pool_Item_Base
    {
        std::vector<uint8_t> data;
    };

    typedef Pool<Datagram>::Ptr Datagram_ptr;

    Pool<Datagram> datagram_pool;

    ////////
    std::mutex datagram_queue_mutex;
    std::deque<Datagram_ptr> datagram_queue;
    std::condition_variable datagram_queue_cv;
    ////////

    std::vector<Datagram_ptr> block_datagrams;
    std::vector<Datagram_ptr> block_fec_datagrams;

    Datagram_ptr crt_datagram;

    uint32_t last_block_index = 1;
};


struct RX
{
    struct Datagram : public Pool_Item_Base
    {
        bool is_processed = false;
        uint32_t index = 0;
        math::vec2u16 resolution;
        std::vector<uint8_t> data;
    };

    typedef Pool<Datagram>::Ptr Datagram_ptr;
    Pool<Datagram> datagram_pool;

    struct Block : public Pool_Item_Base
    {
        uint32_t index = 0;

        std::vector<Datagram_ptr> datagrams;
        std::vector<Datagram_ptr> fec_datagrams;
    };

    typedef Pool<Block>::Ptr Block_ptr;
    Pool<Block> block_pool;

    std::mutex mutex;
    std::deque<Block_ptr> block_queue;

    q::Clock::time_point last_block_tp = q::Clock::now();
    q::Clock::time_point last_datagram_tp = q::Clock::now();

    uint32_t next_block_index = 0;
};


static void seal_datagram(TX::Datagram& datagram, size_t header_offset, uint32_t block_index, uint8_t datagram_index, math::vec2u16 const& resolution, bool is_fec)
{
    QASSERT(datagram.data.size() >= header_offset + sizeof(TX::Datagram));

    Datagram_Header& header = *reinterpret_cast<Datagram_Header*>(datagram.data.data() + header_offset);
//    header.crc = 0;
    header.size = datagram.data.size() - header_offset;
    header.block_index = block_index;
    header.datagram_index = datagram_index;
    header.is_fec = is_fec ? 1 : 0;
    header.width = resolution.x;
    header.height = resolution.y;

//    header.crc = q::util::murmur_hash(datagram.data.data() + header_offset, header.size, 0);
}

struct Video_Streamer::Impl
{
    std::mutex pcap_mutex;
    pcap_t* pcap = nullptr;
    int rx_pcap_selectable_fd = 0;

    size_t _80211_header_length = 0;
    size_t tx_packet_header_length = 0;

//    std::mutex tx_buffer_mutex;
//    std::vector<uint8_t> tx_buffer;
//    bool tx_buffer_has_data = false;

    TX tx;
    RX rx;
};

////////////////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> Video_Streamer::enumerate_interfaces()
{
    std::vector<std::string> res;

    char error_buf[PCAP_ERRBUF_SIZE];
    pcap_if_t* head = nullptr;
    if (pcap_findalldevs(&head, error_buf) == -1)
    {
        QLOGE("Error enumerating rfmon interfaces: {}", error_buf);
        return {};
    }

    pcap_if_t* crt = head;
    while (crt)
    {
        if (crt->flags & PCAP_IF_LOOPBACK)
        {
            QLOGI("Skipping {} because it's a loppback interface", crt->name);
        }
        else if ((crt->flags & PCAP_IF_UP) == 0)
        {
            QLOGI("Skipping {} because it's down", crt->name);
        }
        else
        {
            res.push_back(crt->name);
        }
        crt = crt->next;
    }

    pcap_freealldevs(head);

    return res;
}

////////////////////////////////////////////////////////////////////////////////////////////

Video_Streamer::Video_Streamer(std::string const& interface, Master_Descriptor const& descriptor)
    : m_interface(interface)
    , m_is_master(true)
    , m_master_descriptor(descriptor)
{
}

////////////////////////////////////////////////////////////////////////////////////////////

Video_Streamer::Video_Streamer(std::string const& interface, Slave_Descriptor const& descriptor)
    : m_interface(interface)
    , m_is_master(false)
    , m_slave_descriptor(descriptor)
{
}

////////////////////////////////////////////////////////////////////////////////////////////

Video_Streamer::~Video_Streamer()
{
    m_exit = true;

    m_impl->tx.datagram_queue_cv.notify_all();

    if (m_thread.joinable())
    {
        m_thread.join();
    }

    fec_free(m_fec);
}

////////////////////////////////////////////////////////////////////////////////////////////

bool Video_Streamer::prepare_filter()
{
    struct bpf_program program;
    char program_src[512];

    int link_encap = pcap_datalink(m_impl->pcap);

    switch (link_encap)
    {
    case DLT_PRISM_HEADER:
        QLOGI("DLT_PRISM_HEADER Encap");
        m_impl->_80211_header_length = 0x20; // ieee80211 comes after this
        sprintf(program_src, "radio[0x4a:4]==0x13223344 && radio[0x4e:2] == 0x5566");
        break;

    case DLT_IEEE802_11_RADIO:
        QLOGI("DLT_IEEE802_11_RADIO Encap");
        m_impl->_80211_header_length = 0x18; // ieee80211 comes after this
        sprintf(program_src, "ether[0x0a:4]==0x13223344 && ether[0x0e:2] == 0x5566");
        break;

    default:
        QLOGE("!!! unknown encapsulation");
        return false;
    }

    if (pcap_compile(m_impl->pcap, &program, program_src, 1, 0) == -1)
    {
        QLOGE("Failed to compile program: {} : {}", program_src, pcap_geterr(m_impl->pcap));
        return false;
    }
    if (pcap_setfilter(m_impl->pcap, &program) == -1)
    {
        pcap_freecode(&program);
        QLOGE("Failed to set program: {} : {}", program_src, pcap_geterr(m_impl->pcap));
        return false;
    }
    pcap_freecode(&program);

    m_impl->rx_pcap_selectable_fd = pcap_get_selectable_fd(m_impl->pcap);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////

static void radiotap_add_u8(uint8_t*& dst, size_t& idx, uint8_t data)
{
    *dst++ = data;
    idx++;
}

////////////////////////////////////////////////////////////////////////////////////////////

static void radiotap_add_u16(uint8_t*& dst, size_t& idx, uint16_t data)
{
    if ((idx & 1) == 1) //not aligned, pad first
    {
        radiotap_add_u8(dst, idx, 0);
    }
    *reinterpret_cast<uint16_t*>(dst) = data;
    dst += 2;
    idx += 2;
}

////////////////////////////////////////////////////////////////////////////////////////////

void Video_Streamer::prepare_radiotap_header(size_t rate_hz)
{
    RADIOTAP_HEADER.resize(1024);
    ieee80211_radiotap_header& hdr = reinterpret_cast<ieee80211_radiotap_header&>(*RADIOTAP_HEADER.data());
    hdr.it_version = 0;
    hdr.it_present = 0
                    | (1 << IEEE80211_RADIOTAP_RATE)
                    | (1 << IEEE80211_RADIOTAP_TX_FLAGS)
                    | (1 << IEEE80211_RADIOTAP_RTS_RETRIES)
                    | (1 << IEEE80211_RADIOTAP_DATA_RETRIES)
                    | (1 << IEEE80211_RADIOTAP_CHANNEL)
//                    | (1 << IEEE80211_RADIOTAP_MCS)
                    ;

    auto* dst = RADIOTAP_HEADER.data() + sizeof(ieee80211_radiotap_header);
    size_t idx = dst - RADIOTAP_HEADER.data();

    if (hdr.it_present & (1 << IEEE80211_RADIOTAP_RATE))
    {
        radiotap_add_u8(dst, idx, std::min(static_cast<uint8_t>(rate_hz / 500000), uint8_t(1)));
    }
    if (hdr.it_present & (1 << IEEE80211_RADIOTAP_TX_FLAGS))
    {
        radiotap_add_u16(dst, idx, IEEE80211_RADIOTAP_F_TX_NOACK); //used to be 0x18
    }
    if (hdr.it_present & (1 << IEEE80211_RADIOTAP_RTS_RETRIES))
    {
        radiotap_add_u8(dst, idx, 0x0);
    }
    if (hdr.it_present & (1 << IEEE80211_RADIOTAP_DATA_RETRIES))
    {
        radiotap_add_u8(dst, idx, 0x0);
    }
    if (hdr.it_present & (1 << IEEE80211_RADIOTAP_MCS))
    {
        radiotap_add_u8(dst, idx, IEEE80211_RADIOTAP_MCS_HAVE_MCS);
        radiotap_add_u8(dst, idx, 0);
        radiotap_add_u8(dst, idx, 18);
    }
    if (hdr.it_present & (1 << IEEE80211_RADIOTAP_CHANNEL))
    {
        radiotap_add_u16(dst, idx, 2467);
        radiotap_add_u16(dst, idx, 0);
    }

    //finish it
    hdr.it_len = static_cast<__le16>(idx);
    RADIOTAP_HEADER.resize(idx);


//    RADIOTAP_HEADER.resize(sizeof(RADIOTAP_HEADER_original));
//    memcpy(RADIOTAP_HEADER.data(), RADIOTAP_HEADER_original, sizeof(RADIOTAP_HEADER_original));
}

////////////////////////////////////////////////////////////////////////////////////////////

void Video_Streamer::prepare_tx_packet_header(uint8_t* buffer)
{
    //prepare the buffers with headers
    uint8_t* pu8 = buffer;

    memcpy(pu8, RADIOTAP_HEADER.data(), RADIOTAP_HEADER.size());
    pu8 += RADIOTAP_HEADER.size();

    memcpy(pu8, IEEE_HEADER, sizeof (IEEE_HEADER));
    pu8 += sizeof (IEEE_HEADER);
}

////////////////////////////////////////////////////////////////////////////////////////////

bool Video_Streamer::process_rx_packet()
{
    struct pcap_pkthdr* pcap_packet_header = nullptr;

    uint8_t payload_buffer[MAX_PACKET_SIZE];
    uint8_t* payload = payload_buffer;

    while (true)
    {
        {
            std::lock_guard<std::mutex> lg(m_impl->pcap_mutex);
            int retval = pcap_next_ex(m_impl->pcap, &pcap_packet_header, (const u_char**)&payload);
            if (retval < 0)
            {
                QLOGE("Socket broken: {}", pcap_geterr(m_impl->pcap));
                return false;
            }
            if (retval != 1)
            {
                break;
            }
        }

        size_t header_len = (payload[2] + (payload[3] << 8));
        if (pcap_packet_header->len < (header_len + m_impl->_80211_header_length))
        {
            QLOGW("packet too small");
            return true;
        }

        size_t bytes = pcap_packet_header->len - (header_len + m_impl->_80211_header_length);

        ieee80211_radiotap_iterator rti;
        if (ieee80211_radiotap_iterator_init(&rti, (struct ieee80211_radiotap_header *)payload, pcap_packet_header->len) < 0)
        {
            QLOGE("iterator null");
            return true;
        }

        int n = 0;
        Penumbra_Radiotap_Header prh;
        while ((n = ieee80211_radiotap_iterator_next(&rti)) == 0)
        {

            switch (rti.this_arg_index)
            {
            case IEEE80211_RADIOTAP_RATE:
                prh.rate = (*rti.this_arg);
                break;

            case IEEE80211_RADIOTAP_CHANNEL:
                prh.channel = (*((uint16_t *)rti.this_arg));
                prh.channel_flags = (*((uint16_t *)(rti.this_arg + 2)));
                break;

            case IEEE80211_RADIOTAP_ANTENNA:
                prh.antenna = (*rti.this_arg) + 1;
                break;

            case IEEE80211_RADIOTAP_FLAGS:
                prh.radiotap_flags = *rti.this_arg;
                break;
            }
        }
        payload += header_len + m_impl->_80211_header_length;

        if (prh.radiotap_flags & IEEE80211_RADIOTAP_F_FCS)
        {
            bytes -= 4;
        }

        bool checksum_correct = (prh.radiotap_flags & 0x40) == 0;

        //    block_num = seq_nr / param_retransmission_block_size;//if retr_block_size would be limited to powers of two, this could be replaced by a logical AND operation

        //printf("rec %x bytes %d crc %d\n", seq_nr, bytes, checksum_correct);

#ifdef DEBUG_PCAP
        std::cout << "PCAP RX>>";
        std::copy(payload, payload + bytes, std::ostream_iterator<uint8_t>(std::cout));
        std::cout << "<<PCAP RX";
#endif
        if (!checksum_correct)
        {
            QLOGW("invalid checksum.");
            return true;
        }

        {
            RX& rx = m_impl->rx;

            Datagram_Header& header = *reinterpret_cast<Datagram_Header*>(payload);
            uint32_t block_index = header.block_index;
            uint32_t datagram_index = header.datagram_index;
            if (datagram_index >= m_coding_n)
            {
                QLOGE("datagram index out of range: {} > {}", datagram_index, m_coding_n);
                return true;
            }

            std::lock_guard<std::mutex> lg(rx.mutex);

            if (block_index < rx.next_block_index)
            {
                //QLOGW("Old datagram: {} < {}", block_index, rx.next_block_index);
                return true;
            }

            RX::Block_ptr block;

            //find the block
            {
                auto iter = std::lower_bound(rx.block_queue.begin(), rx.block_queue.end(), block_index, [](RX::Block_ptr const& l, uint32_t index) { return l->index < index; });
                if (iter != rx.block_queue.end() && (*iter)->index == block_index)
                {
                    block = *iter;
                }
                else
                {
                    block = rx.block_pool.acquire();
                    block->index = block_index;
                    rx.block_queue.insert(iter, block);
                }
            }

            RX::Datagram_ptr datagram = rx.datagram_pool.acquire();
            datagram->data.resize(bytes - sizeof(Datagram_Header));
            datagram->index = datagram_index;
            datagram->resolution.set(header.width, header.height);
            memcpy(datagram->data.data(), payload + sizeof(Datagram_Header), bytes - sizeof(Datagram_Header));

            //store datagram
            if (header.is_fec)
            {
                block->fec_datagrams.push_back(datagram);
            }
            else
            {
                auto iter = std::lower_bound(block->datagrams.begin(), block->datagrams.end(), datagram_index, [](RX::Datagram_ptr const& l, uint32_t index) { return l->index < index; });
                if (iter != block->datagrams.end() && (*iter)->index == datagram_index)
                {
                    //QLOGW("Duplicated datagram {} from block {} (index {})", datagram_index, block_index, block_index * m_coding_k + datagram_index);
                    return true;
                }
                else
                {
                    block->datagrams.insert(iter, datagram);
                }
            }
        }

        //m_impl->rx_queue.enqueue(payload, bytes);
//        if (receive_callback && bytes > 0)
//        {
//            receive_callback(payload, bytes);
//        }

#ifdef DEBUG_THROUGHPUT
        {
            static int xxx_data = 0;
            static std::chrono::system_clock::time_point xxx_last_tp = std::chrono::system_clock::now();
            xxx_data += bytes;
            auto now = std::chrono::system_clock::now();
            if (now - xxx_last_tp >= std::chrono::seconds(1))
            {
                float r = std::chrono::duration<float>(now - xxx_last_tp).count();
                QLOGI("Received: {} KB/s", float(xxx_data)/r/1024.f);
                xxx_data = 0;
                xxx_last_tp = now;
            }
        }
#endif
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////

bool Video_Streamer::init(uint32_t coding_k, uint32_t coding_n)
{
    if (coding_k == 0 || coding_n < coding_k || coding_k > m_fec_src_datagram_ptrs.size() || coding_n > m_fec_dst_datagram_ptrs.size())
    {
        QLOGE("Invalid coding params: {} / {}" ,coding_k, coding_n);
        return false;
    }

    m_coding_k = coding_k;
    m_coding_n = coding_n;

    m_fec = fec_new(m_coding_k, m_coding_n);


    char pcap_error[PCAP_ERRBUF_SIZE] = {0};

    m_impl.reset(new Impl);

//    IEEE_HEADER[SRC_MAC_LASTBYTE] = 0;
//    IEEE_HEADER[DST_MAC_LASTBYTE] = 0;

    prepare_radiotap_header(DEFAULT_RATE_HZ);
    m_impl->tx_packet_header_length = RADIOTAP_HEADER.size() + sizeof(IEEE_HEADER);
    QLOGI("Radiocap header size: {}, IEEE header size: {}", RADIOTAP_HEADER.size(), sizeof(IEEE_HEADER));


    /////////////////////
    //calculate some offsets and sizes
    m_datagram_header_offset = m_impl->tx_packet_header_length;
    m_payload_offset = m_datagram_header_offset + sizeof(Datagram_Header);

    m_transport_datagram_size = m_payload_offset + get_mtu();
    m_streaming_datagram_size = m_transport_datagram_size - m_impl->tx_packet_header_length;
    m_payload_size = get_mtu();
    /////////////////////


    m_impl->tx.datagram_pool.on_acquire = [this](TX::Datagram& datagram)
    {
        if (datagram.data.empty())
        {
            datagram.data.resize(m_payload_offset);
            prepare_tx_packet_header(datagram.data.data());
        }
        else
        {
            datagram.data.resize(m_payload_offset);
        }
    };

    m_impl->rx.datagram_pool.on_acquire = [this](RX::Datagram& datagram)
    {
        datagram.index = 0;
        datagram.is_processed = false;
        datagram.data.clear();
        datagram.data.reserve(m_transport_datagram_size);
    };
    m_impl->rx.block_pool.on_acquire = [this](RX::Block& block)
    {
        block.index = 0;

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


//    m_impl->pcap = pcap_open_live(m_interface.c_str(), 2048, 1, -1, pcap_error);
//    if (m_impl->pcap == nullptr)
//    {
//        QLOGE("Unable to open interface {} in pcap: {}", m_interface, pcap_error);
//        return false;
//    }

////    if (pcap_setnonblock(m_impl->pcap, 1, pcap_error) < 0)
////    {
////        QLOGE("Error setting {} to nonblocking mode: {}", m_interface, pcap_error);
////        return false;
////    }

//    if (pcap_setdirection(m_impl->pcap, PCAP_D_IN) < 0)
//    {
//        QLOGE("Error setting {} to IN capture only: {}", m_interface, pcap_geterr(m_impl->pcap));
//        return false;
//    }

    m_impl->pcap = pcap_create(m_interface.c_str(), pcap_error);
    if (m_impl->pcap == nullptr)
    {
        QLOGE("Unable to open interface {}: {}", m_interface, pcap_geterr(m_impl->pcap));
        return false;
    }
    if (pcap_set_snaplen(m_impl->pcap, 1800) < 0)
    {
        QLOGE("Error setting pcap_set_snaplen: {}", pcap_geterr(m_impl->pcap));
        return false;
    }
    if (pcap_set_promisc(m_impl->pcap, 1) < 0)
    {
        QLOGE("Error setting pcap_set_promisc: {}", pcap_geterr(m_impl->pcap));
        return false;
    }
    if (pcap_set_rfmon(m_impl->pcap, 1) < 0)
    {
        QLOGE("Error setting pcap_set_rfmon: {}", pcap_geterr(m_impl->pcap));
        return false;
    }
    if (pcap_set_timeout(m_impl->pcap, -1) < 0)
    {
        QLOGE("Error setting pcap_set_timeout: {}", pcap_geterr(m_impl->pcap));
        return false;
    }
    if (pcap_set_immediate_mode(m_impl->pcap, 1) < 0)
    {
        QLOGE("Error setting pcap_set_immediate_mode: {}", pcap_geterr(m_impl->pcap));
        return false;
    }
    if (pcap_set_buffer_size(m_impl->pcap, 16000000) < 0)
    {
        QLOGE("Error setting pcap_set_buffer_size: {}", pcap_geterr(m_impl->pcap));
        return false;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if (pcap_activate(m_impl->pcap) < 0)
    {
        QLOGE("Error in pcap_activate: {}", pcap_geterr(m_impl->pcap));
        return false;
    }
//    if (pcap_setnonblock(m_impl->pcap, 1, pcap_error) < 0)
//    {
//        QLOGE("Error setting pcap_set_snaplen: {}", pcap_geterr(m_impl->pcap));
//        return false;
//    }
    if (pcap_setdirection(m_impl->pcap, PCAP_D_IN) < 0)
    {
        QLOGE("Error setting pcap_setdirection: {}", pcap_geterr(m_impl->pcap));
        return false;
    }

    if (!prepare_filter())
    {
        return false;
    }

    if (m_is_master)
    {
        m_thread = boost::thread([this]() { master_thread_proc(); });
    }
    else
    {
        m_thread = boost::thread([this]() { slave_thread_proc(); });
    }

#if defined RASPBERRY_PI
    {
//        int policy = SCHED_OTHER;
//        struct sched_param param;
//        param.sched_priority = 0;
        int policy = SCHED_FIFO;
        struct sched_param param;
        param.sched_priority = sched_get_priority_max(policy);
        if (pthread_setschedparam(m_thread.native_handle(), policy, &param) != 0)
        {
            perror("Cannot set TX thread priority - using normal");
        }
    }
#endif

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////

void Video_Streamer::slave_thread_proc()
{
    while (!m_exit)
    {
        fd_set readset;
        struct timeval to;

        to.tv_sec = 0;
        to.tv_usec = 1e3;

        FD_ZERO(&readset);
        FD_SET(m_impl->rx_pcap_selectable_fd, &readset);

        int n = select(30, &readset, nullptr, nullptr, &to);
        if (n != 0)
        {
            if (FD_ISSET(m_impl->rx_pcap_selectable_fd, &readset))
            {
                process_rx_packet();
                //std::this_thread::yield();
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////

void Video_Streamer::master_thread_proc()
{
    TX& tx = m_impl->tx;

    while (!m_exit)
    {
        TX::Datagram_ptr datagram;
        {
            //wait for data
            std::unique_lock<std::mutex> lg(tx.datagram_queue_mutex);
            if (tx.datagram_queue.empty())
            {
                tx.datagram_queue_cv.wait(lg, [this, &tx]{ return tx.datagram_queue.empty() == false || m_exit == true; });
            }
            if (m_exit)
            {
                break;
            }

            //inject packets
            if (!tx.datagram_queue.empty())
            {
                datagram = tx.datagram_queue.front();
                tx.datagram_queue.pop_front();
            }
        }

        if (datagram)
        {
            std::lock_guard<std::mutex> lg(m_impl->pcap_mutex);

            int isize = static_cast<int>(datagram->data.size());
            int r = pcap_inject(m_impl->pcap, datagram->data.data(), isize);
//                    if (r <= 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
//                    {
//                        break;
//                    }
//                    else
            {
                if (r <= 0)
                {
                    QLOGW("Trouble injecting packet: {} / {}: {}", r, isize, pcap_geterr(m_impl->pcap));
                    //result = Result::ERROR;
                }
                if (r > 0 && r != isize)
                {
                    QLOGW("Incomplete packet sent: {} / {}", r, isize);
                    //result = Result::ERROR;
                }
            }
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

void Video_Streamer::send(void const* _data, size_t size, math::vec2u16 const& resolution)
{
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
            seal_datagram(*datagram, m_datagram_header_offset, tx.last_block_index, tx.block_datagrams.size(), resolution, false);
            tx.block_datagrams.push_back(datagram);

            //send the current datagram
            {
                std::unique_lock<std::mutex> lg(tx.datagram_queue_mutex);
                tx.datagram_queue.push_back(datagram);
            }
            datagram = tx.datagram_pool.acquire();


            if (tx.block_datagrams.size() >= m_coding_k)
            {
                if (1)
                {
                    auto start = q::Clock::now();

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
                        seal_datagram(*tx.block_fec_datagrams[i], m_datagram_header_offset, tx.last_block_index, m_coding_k + i, resolution, true);
                    }

                    //send
                    {
                        std::unique_lock<std::mutex> lg(tx.datagram_queue_mutex);
                        for (size_t i = 0; i < fec_count; i++)
                        {
                            tx.datagram_queue.push_back(tx.block_fec_datagrams[i]);
                        }
                    }

                    //QLOGI("Encoded fec: {}", q::Clock::now() - start);
                }

                tx.block_datagrams.clear();
                tx.block_fec_datagrams.clear();
                tx.last_block_index++;
            }

            tx.datagram_queue_cv.notify_all();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////

size_t Video_Streamer::get_mtu() const
{
    return std::min(400u, MAX_USER_PACKET_SIZE);
}

////////////////////////////////////////////////////////////////////////////////////////////

void Video_Streamer::process()
{
    if (!m_impl || m_is_master)
    {
        return;
    }

    RX& rx = m_impl->rx;
    std::lock_guard<std::mutex> lg(rx.mutex);

    if (q::Clock::now() - rx.last_datagram_tp > m_slave_descriptor.reset_duration)
    {
        rx.next_block_index = 0;
    }

    if (rx.block_queue.empty())
    {
        return;
    }

    RX::Block_ptr block = rx.block_queue.front();

    //entire block received
    if (block->datagrams.size() >= m_coding_k)
    {
        for (RX::Datagram_ptr const& d: block->datagrams)
        {
            uint32_t seq_number = block->index * m_coding_k + d->index;
            if (!d->is_processed)
            {
                //QLOGI("Datagram {}", seq_number);
                if (on_data_received)
                {
                    on_data_received(d->data.data(), d->data.size(), d->resolution);
                }
                rx.last_datagram_tp = q::Clock::now();
                d->is_processed = true;
            }
        }

        rx.last_block_tp = q::Clock::now();

        rx.next_block_index = block->index + 1;
        rx.block_queue.pop_front();
        return;
    }

    //try to process consecutive datagrams before the block is finished to minimize latency
    for (size_t i = 0; i < block->datagrams.size(); i++)
    {
        RX::Datagram_ptr const& d = block->datagrams[i];
        if (d->index == i)
        {
            uint32_t seq_number = block->index * m_coding_k + d->index;
            if (!d->is_processed)
            {
                //QLOGI("Datagram {}", seq_number);
                if (on_data_received)
                {
                    on_data_received(d->data.data(), d->data.size(), d->resolution);
                }
                rx.last_datagram_tp = q::Clock::now();
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
        auto start = q::Clock::now();

        std::array<unsigned int, 32> indices;
        size_t primary_index = 0;
        size_t used_fec_index = 0;
        for (size_t i = 0; i < m_coding_k; i++)
        {
            if (primary_index < block->datagrams.size() && i == block->datagrams[primary_index]->index)
            {
                m_fec_src_datagram_ptrs[i] = block->datagrams[primary_index]->data.data();
                indices[i] = i;
                primary_index++;
            }
            else
            {
                m_fec_src_datagram_ptrs[i] = block->fec_datagrams[used_fec_index]->data.data();
                indices[i] = block->fec_datagrams[used_fec_index]->index;
                used_fec_index++;
            }
        }

        //insert the missing datagrams, they will be filled with data by the fec_decode below
        size_t fec_index = 0;
        for (size_t i = 0; i < m_coding_k; i++)
        {
            if (i >= block->datagrams.size() || i != block->datagrams[i]->index)
            {
                block->datagrams.insert(block->datagrams.begin() + i, rx.datagram_pool.acquire());
                block->datagrams[i]->data.resize(m_payload_size);
                block->datagrams[i]->index = i;
                m_fec_dst_datagram_ptrs[fec_index++] = block->datagrams[i]->data.data();
            }
        }

        fec_decode(m_fec, m_fec_src_datagram_ptrs.data(), m_fec_dst_datagram_ptrs.data(), indices.data(), m_payload_size);

        //now dispatch them
        for (size_t i = 0; i < block->datagrams.size(); i++)
        {
            RX::Datagram_ptr const& d = block->datagrams[i];
            uint32_t seq_number = block->index * m_coding_k + d->index;
            if (!d->is_processed)
            {
                //QLOGI("Datagram F {}", seq_number);
                if (on_data_received)
                {
                    on_data_received(d->data.data(), d->data.size(), d->resolution);
                }
                rx.last_datagram_tp = q::Clock::now();
                d->is_processed = true;
            }
        }

        //QLOGI("Decoded fac: {}", q::Clock::now() - start);

        rx.last_block_tp = q::Clock::now();
        rx.next_block_index = block->index + 1;
        rx.block_queue.pop_front();
        return;
    }

    //skip if too much buffering
    if (rx.block_queue.size() > 3)
    {
        for (size_t i = 0; i < block->datagrams.size(); i++)
        {
            RX::Datagram_ptr const& d = block->datagrams[i];
            if (!d->is_processed)
            {
                uint32_t seq_number = block->index * m_coding_k + d->index;
                //QLOGI("Skipping {}", seq_number);
            }
        }
        rx.next_block_index = block->index + 1;
        rx.block_queue.pop_front();
    }

}

////////////////////////////////////////////////////////////////////////////////////////////


}
}
