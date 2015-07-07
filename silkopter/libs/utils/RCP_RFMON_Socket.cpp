#include "RCP_RFMON_Socket.h"

#include <pcap.h>

#include "utils/radiotap/radiotap.h"


namespace util
{

//#define DEBUG_PCAP
//#define DEBUG_ASIO
#define DEBUG_THROUGHPUT



static constexpr size_t MAX_PACKET_SIZE = 4192;
static constexpr size_t MAX_USER_PACKET_SIZE = 1470;

static constexpr size_t DEFAULT_RATE_HZ = 26000000;

std::vector<uint8_t> RADIOTAP_HEADER;

static constexpr size_t SRC_MAC_LASTBYTE  = 15;
static constexpr size_t DST_MAC_LASTBYTE  = 21;

// Penumbra IEEE80211 header
static uint8_t IEEE_HEADER[] =
{
    0x08, 0x01, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
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

#pragma pack(pop)

typedef std::shared_ptr<std::vector<uint8_t>> Buffer;


struct RCP_RFMON_Socket::Impl
{
    std::mutex pcap_mutex;
    pcap_t* pcap = nullptr;
    int rx_pcap_selectable_fd = 0;

    size_t _80211_header_length = 0;
    size_t tx_packet_header_length = 0;

    uint8_t const* tx_buffer = nullptr;
    size_t tx_buffer_size = 0;
    std::mutex tx_buffer_mutex;
    std::condition_variable tx_buffer_cv;
};

//////////////////////////////////////////////

RCP_RFMON_Socket::RCP_RFMON_Socket(std::string const& interface, uint8_t id)
    : m_interface(interface)
    , m_id(id)
{
    m_impl.reset(new Impl);
}

RCP_RFMON_Socket::~RCP_RFMON_Socket()
{
    m_exit = true;
    if (m_tx_thread.joinable())
    {
        m_tx_thread.join();
    }
    if (m_rx_thread.joinable())
    {
        m_rx_thread.join();
    }
}

auto RCP_RFMON_Socket::prepare_filter() -> bool
{
    struct bpf_program program;
    char program_src[512];

    int link_encap = pcap_datalink(m_impl->pcap);

    switch (link_encap)
    {
    case DLT_PRISM_HEADER:
        QLOGI("DLT_PRISM_HEADER Encap");
        m_impl->_80211_header_length = 0x20; // ieee80211 comes after this
        sprintf(program_src, "radio[0x4a:4]==0x13223344 && radio[0x4e:2] != 0x55%.2x", m_id);
        break;

    case DLT_IEEE802_11_RADIO:
        QLOGI("DLT_IEEE802_11_RADIO Encap");
        m_impl->_80211_header_length = 0x18; // ieee80211 comes after this
        sprintf(program_src, "ether[0x0a:4]==0x13223344 && ether[0x0e:2] != 0x55%.2x", m_id);
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


static void radiotap_add_u8(uint8_t*& dst, size_t& idx, uint8_t data)
{
    *dst++ = data;
    idx++;
}
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

void RCP_RFMON_Socket::prepare_radiotap_header(size_t rate_hz)
{
    RADIOTAP_HEADER.resize(1024);
    ieee80211_radiotap_header& hdr = reinterpret_cast<ieee80211_radiotap_header&>(*RADIOTAP_HEADER.data());
    hdr.it_version = 0;
    hdr.it_present = 0
                    | (1 << IEEE80211_RADIOTAP_RATE)
                    | (1 << IEEE80211_RADIOTAP_TX_FLAGS)
                    | (1 << IEEE80211_RADIOTAP_RTS_RETRIES)
                    | (1 << IEEE80211_RADIOTAP_DATA_RETRIES)
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

    //finish it
    hdr.it_len = static_cast<__le16>(idx);
    RADIOTAP_HEADER.resize(idx);


//    RADIOTAP_HEADER.resize(sizeof(RADIOTAP_HEADER_original));
//    memcpy(RADIOTAP_HEADER.data(), RADIOTAP_HEADER_original, sizeof(RADIOTAP_HEADER_original));
}

void RCP_RFMON_Socket::prepare_tx_packet_header(uint8_t* buffer)
{
    //prepare the buffers with headers
    uint8_t* pu8 = buffer;

    memcpy(pu8, RADIOTAP_HEADER.data(), RADIOTAP_HEADER.size());
    pu8 += RADIOTAP_HEADER.size();

    memcpy(pu8, IEEE_HEADER, sizeof (IEEE_HEADER));
    pu8 += sizeof (IEEE_HEADER);
}

auto RCP_RFMON_Socket::process_rx_packet() -> bool
{
    std::lock_guard<std::mutex> lg(m_impl->pcap_mutex);

    struct pcap_pkthdr* pcap_packet_header = nullptr;

    uint8_t payload_buffer[MAX_PACKET_SIZE];
    uint8_t* payload = payload_buffer;

    while (true)
    {
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

        //m_impl->rx_queue.enqueue(payload, bytes);
        if (receive_callback && bytes > 0)
        {
            receive_callback(payload, bytes);
        }

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


auto RCP_RFMON_Socket::start() -> bool
{
    IEEE_HEADER[SRC_MAC_LASTBYTE] = m_id;
    IEEE_HEADER[DST_MAC_LASTBYTE] = m_id;

    prepare_radiotap_header(DEFAULT_RATE_HZ);
    m_impl->tx_packet_header_length = RADIOTAP_HEADER.size() + sizeof(IEEE_HEADER);
    QLOGI("Radiocap header size: {}, IEEE header size: {}", RADIOTAP_HEADER.size(), sizeof(IEEE_HEADER));

    char pcap_error[PCAP_ERRBUF_SIZE] = {0};

    m_impl->pcap = pcap_open_live(m_interface.c_str(), 2048, 1, -1, pcap_error);
    if (m_impl->pcap == nullptr)
    {
        QLOGE("Unable to open interface {} in pcap: {}", m_interface, pcap_error);
        return false;
    }

//    if (pcap_setnonblock(m_impl->pcap, 1, pcap_error) < 0)
//    {
//        QLOGE("Error setting {} to nonblocking mode: {}", m_interface, pcap_error);
//        return false;
//    }

    if (pcap_setdirection(m_impl->pcap, PCAP_D_IN) < 0)
    {
        QLOGE("Error setting {} to IN capture only: {}", m_interface, pcap_geterr(m_impl->pcap));
        return false;
    }

    if (!prepare_filter())
    {
        return false;
    }

//    m_impl->tx_buffer.resize(MAX_PACKET_SIZE);
//    prepare_tx_packet_header(m_impl->tx_buffer.data());

    m_rx_thread = std::thread([this]()
    {
        while (!m_exit)
        {
            fd_set readset;
            struct timeval to;

            to.tv_sec = 0;
            to.tv_usec = 1e5;

            FD_ZERO(&readset);
            FD_SET(m_impl->rx_pcap_selectable_fd, &readset);

            int n = select(30, &readset, nullptr, nullptr, &to);
            if (n != 0)
            {
                if (FD_ISSET(m_impl->rx_pcap_selectable_fd, &readset))
                {
                    process_rx_packet();
                }
            }
        }
    });

    m_tx_thread = std::thread([this]()
    {
        while (!m_exit)
        {
            bool sent = false;
            Result result = Result::OK;

            {
                //wait for data
                std::unique_lock<std::mutex> lg(m_impl->tx_buffer_mutex);
                if (!m_impl->tx_buffer)
                {
                    m_impl->tx_buffer_cv.wait(lg, [this]{ return m_impl->tx_buffer != nullptr; });
                }

                //inject packets
                if (m_impl->tx_buffer)
                {
                    std::lock_guard<std::mutex> lg(m_impl->pcap_mutex);
                    int isize = static_cast<int>(m_impl->tx_buffer_size);
                    int r = pcap_inject(m_impl->pcap, m_impl->tx_buffer, isize);
                    if (r <= 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
                    {
                        break;
                    }
                    else
                    {
                        if (r <= 0)
                        {
                            QLOGW("Trouble injecting packet: {} / {}: {}", r, isize, pcap_geterr(m_impl->pcap));
                            result = Result::ERROR;
                        }
                        if (r > 0 && r != isize)
                        {
                            QLOGW("Incomplete packet sent: {} / {}", r, isize);
                            result = Result::ERROR;
                        }

                        sent = true;
                    }

                    m_impl->tx_buffer = nullptr;
                }
            }

            if (sent)
            {
                if (send_callback)
                {
                    send_callback(result);
                }
                //std::this_thread::sleep_for(std::chrono::milliseconds(1));
                //std::this_thread::yield();

#ifdef DEBUG_THROUGHPUT
                {
                    static size_t xxx_data = 0;
                    static std::chrono::system_clock::time_point xxx_last_tp = std::chrono::system_clock::now();
                    xxx_data += m_impl->tx_buffer_size;
                    auto now = std::chrono::system_clock::now();
                    if (now - xxx_last_tp >= std::chrono::seconds(1))
                    {
                        float r = std::chrono::duration<float>(now - xxx_last_tp).count();
                        QLOGI("Sent: {} KB/s", float(xxx_data)/r/1024.f);
                        xxx_data = 0;
                        xxx_last_tp = now;
                    }
                }
#endif

            }
        }
    });

    return true;
}

void RCP_RFMON_Socket::async_send(uint8_t const* data, size_t size)
{
    QASSERT(size < m_impl->tx_packet_header_length + MAX_USER_PACKET_SIZE);

    {
        std::unique_lock<std::mutex> lg(m_impl->tx_buffer_mutex);

        QASSERT(!m_impl->tx_buffer);
        m_impl->tx_buffer = data;
        m_impl->tx_buffer_size = size;
    }

    m_impl->tx_buffer_cv.notify_all();
}

size_t RCP_RFMON_Socket::prepare_buffer(std::vector<uint8_t>& buffer)
{
    buffer.resize(MAX_PACKET_SIZE);
    prepare_tx_packet_header(buffer.data());
    return m_impl->tx_packet_header_length;
}

auto RCP_RFMON_Socket::process() -> Result
{
    return Result::OK;
}


}
