#include "RC_Phy.h"
#include <string>
#include <tuple>
#include "util/murmurhash.h"

#define CHIP_RF4463F30  1
#define CHIP_RFM22B     2

#define USE_CHIP CHIP_RFM22B
//#define USE_CHIP CHIP_RF4463F30



#if USE_CHIP == CHIP_RFM22B
#   include "utils/hw/RFM22B.h"
#elif USE_CHIP == CHIP_RF4463F30
#   include "utils/hw/RF4463F30.h"
#endif

namespace util
{
namespace comms
{

//constexpr size_t MTU = 63;
constexpr Clock::duration MIN_TX_DURATION = std::chrono::microseconds(1);


#if USE_CHIP == CHIP_RF4463F30
    typedef hw::RF4463F30 Chip;
#elif USE_CHIP == CHIP_RFM22B
    typedef hw::RFM22B Chip;
#else
#   error "Choose a RF chip"
#endif


struct RC_Phy::HW
{
    Chip chip;
};



RC_Phy::RC_Phy(bool master)
    : m_is_master(master)
    , m_tx_queue(10)
    , m_rx_queue(10)
{
    m_hw.reset(new HW);

    set_master_listen_rate(30);
}

RC_Phy::~RC_Phy()
{
    m_exit = true;
    m_tx_queue.finish();
    m_rx_queue.finish();
    if (m_hw_thread.joinable())
    {
        m_hw_thread.join();
    }
    if (m_pk_tx_thread.joinable())
    {
        m_pk_tx_thread.join();
    }
    if (m_pk_rx_thread.joinable())
    {
        m_pk_rx_thread.join();
    }
}

bool RC_Phy::init(hw::ISPI& spi, uint8_t sdn_gpio)
{
    QLOG_TOPIC("RC_Phy::init");
    if (!m_hw->chip.init(spi, sdn_gpio))
    {
        return false;
    }

#if USE_CHIP == CHIP_RFM22B
    uint8_t modem_config[42] =
    {
        0x16,
        0x40,
        0x3F,
        0x02,
        0x0C,
        0x4A,
        0x07,
        0xFF,
        0x1B,
        0x28,
        0x27,
        0x29,
        0xC9,
        0x00,
        0x02,
        0x08,
        0x22,
        0x2D,
        0xD4,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x40,
        0x00,
        0x00,
        0x00,
        0x00,
        0xFF,
        0xFF,
        0xFF,
        0xFF,
        0x08,
        0x31,
        0x0C,
        0x22,
        0x1A,
        0x53,
        0x64,
        0x00,
    };

    m_hw->chip.set_modem_configuration(modem_config);
    m_hw->chip.set_tx_power_dBm(20);

    QLOGI("TX power: {}dBm", m_hw->chip.get_tx_power_dBm());
#endif

//    m_hw_thread = boost::thread([this]() { master_thread_proc(); });

    if (m_is_master)
    {
        m_hw_thread = boost::thread([this]() { master_thread_proc(); });
    }
    else
    {
        m_hw_thread = boost::thread([this]() { slave_thread_proc(); });
    }

    m_pk_tx_thread = boost::thread([this]() { packet_tx_thread_proc(); });
    m_pk_rx_thread = boost::thread([this]() { packet_rx_thread_proc(); });

    return true;
}

bool RC_Phy::set_channel(uint8_t channel)
{
    QLOGI("Setting channel {}", channel);
    return m_hw->chip.set_channel(channel);
}

void RC_Phy::set_xtal_adjustment(float adjustment)
{
    adjustment = math::clamp(adjustment, -1.f, 1.f);
    QLOGI("Setting xtal adjustment of {}", adjustment);
    m_hw->chip.set_xtal_adjustment(adjustment);
}

void RC_Phy::set_callbacks(TX_Callback txcb, RX_Callback rxcb)
{
    m_tx_callback = txcb;
    m_rx_callback = rxcb;
}

void RC_Phy::set_master_listen_rate(size_t rate)
{
    rate = std::max(rate, 1u);
    m_master_listen_period = std::chrono::microseconds(1000000 / rate);
}

size_t RC_Phy::get_mtu() const
{
#if USE_CHIP == CHIP_RF4463F30
    return 510;
#else
    return 63;
#endif
}

void RC_Phy::master_thread_proc()
{
    QLOG_TOPIC("RC_Phy::master");

//    {
//        const struct sched_param priority = {1};
//        sched_setscheduler(0, SCHED_FIFO, &priority);

//        cpu_set_t cpuset;
//        CPU_ZERO(&cpuset);
//        CPU_SET(3, &cpuset);
//        int s = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
//        if (s != 0)
//        {
//            QLOGW("pthread_setaffinity_np failed: {}", s);
//        }
//    }

    Clock::duration tx_average_duration = Clock::duration::zero();
    //Clock::time_point last_tp = Clock::now();
    size_t tx_packet_count = 0;
    size_t rx_packet_count = 0;
    size_t min_size = 999999;
    size_t max_size = 0;
    size_t average_size = 0;
    size_t crc_failed_count = 0;


#if USE_CHIP == CHIP_RF4463F30
    m_hw->chip.set_fifo_mode(hw::RF4463F30::FIFO_Mode::HALF_DUPLEX);
#endif

    uint8_t last_encoded_dBm = 0;

    while (!m_exit)
    {
        //TX session --------------------------------------------------------
        {
            size_t spin = 0;
            auto start_session_tp = Clock::now();
            while (!m_exit)
            {
                spin++;
                auto start_tx_tp = Clock::now();

                bool keep_tx = Clock::now() - start_session_tp < m_master_listen_period;

                std::this_thread::sleep_for(std::chrono::microseconds(300));

                std::unique_ptr<std::vector<uint8_t>> buffer = m_tx_queue.begin_consuming(false);
                if (!buffer)
                {
                    if (!keep_tx)
                    {
                        //send an empty packet so the other end knows it can transmit
                        uint8_t* tx_fifo = m_hw->chip.get_tx_fifo_payload_ptr(sizeof(Master_Header));
                        Master_Header& header = *reinterpret_cast<Master_Header*>(tx_fifo);
                        header.dBm = last_encoded_dBm;
                        header.more_data = 0;
                        if (m_hw->chip.tx(std::chrono::milliseconds(10)))
                        {
                            tx_packet_count++;
                        }
                        else
                        {
                            QLOGW("TX error");
                        }

                        //go to RX session
                        break;
                    }
                    std::this_thread::sleep_for(std::chrono::microseconds(1000));
                    continue;
                }

                uint8_t* tx_fifo = m_hw->chip.get_tx_fifo_payload_ptr(buffer->size());
                QASSERT(buffer->size() >= sizeof(Master_Header));
                memcpy(tx_fifo, buffer->data(), buffer->size());
                buffer->clear();
                m_tx_queue.end_consuming(std::move(buffer));

                Master_Header& header = *reinterpret_cast<Master_Header*>(tx_fifo);
                header.dBm = last_encoded_dBm;
                if (!keep_tx)
                {
                    header.more_data = 0;
                }

                if (m_hw->chip.tx(std::chrono::milliseconds(10)))
                {
                    tx_packet_count++;
                }
                else
                {
                    QLOGW("TX error");
                }

                Clock::duration tx_duration = Clock::now() - start_tx_tp;
                tx_average_duration += tx_duration;

                if (!keep_tx)
                {
                    break;
                }
            } //TX Session
        }

        //RX session --------------------------------------------------------
        {
            Chip::RX_Result result = m_hw->chip.rx(get_mtu() + sizeof(Slave_Header),
                                                            std::chrono::milliseconds(10),
                                                            std::chrono::milliseconds(100));
            if (result == Chip::RX_Result::OK)
            {
                uint8_t* rx_data = m_hw->chip.get_rx_fifo_payload_ptr();
                size_t rx_size = m_hw->chip.get_rx_fifo_payload_size();
                min_size = std::min(min_size, rx_size);
                max_size = std::max(max_size, rx_size);
                average_size += rx_size;
                if (rx_data && rx_size >= sizeof(Slave_Header))
                {
                    rx_packet_count++;
                    //does the packet contain user data as well? if yes, put it in the queue
                    if (rx_size > sizeof(Master_Header))
                    {
                        std::unique_ptr<std::vector<uint8_t>> buffer = m_rx_queue.begin_producing();
                        if (buffer)
                        {
                            buffer->resize(rx_size);
                            memcpy(buffer->data(), rx_data, buffer->size());
                            m_rx_queue.end_producing(std::move(buffer), false);
                        }
                    }
                }
            }
            else if (result == Chip::RX_Result::CRC_FAILED)
            {
                crc_failed_count++;
            }
            else if (result != Chip::RX_Result::TIMEOUT)
            {
                QLOGW("RX error: {}", result);
            }
        }

        //refresh dBm --------------------------------------------------
        if (Clock::now() - m_last_dBm_tp >= std::chrono::milliseconds(500))
        {
            m_last_dBm_tp = Clock::now();
            m_last_dBm = m_hw->chip.get_input_dBm();
            last_encoded_dBm = -std::min<int8_t>(m_last_dBm, 0);
        }

//        if (Clock::now() - last_tp >= std::chrono::seconds(1))
//        {
//            last_tp = Clock::now();
//            QLOGI("TX packets: {}, average per packet: {}", tx_packet_count, tx_packet_count > 0 ? tx_average_duration / tx_packet_count : Clock::duration::zero());
//            tx_packet_count = 0;
//            tx_average_duration = Clock::duration::zero();

//            QLOGI("RX packets: {}, crc failed {}, {}-{}-{}", rx_packet_count, crc_failed_count, min_size, max_size, rx_packet_count > 0 ? average_size / rx_packet_count : 0);
//            rx_packet_count = 0;
//            crc_failed_count = 0;
//            min_size = 999999;
//            max_size = 0;
//            average_size = 0;
//        }
    }
}

void RC_Phy::slave_thread_proc()
{
    QLOG_TOPIC("RC_Phy::slave");

//    {
//        const struct sched_param priority = {1};
//        sched_setscheduler(0, SCHED_FIFO, &priority);

//        cpu_set_t cpuset;
//        CPU_ZERO(&cpuset);
//        CPU_SET(3, &cpuset);
//        int s = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
//        if (s != 0)
//        {
//            QLOGW("pthread_setaffinity_np failed: {}", s);
//        }
//    }

#if USE_CHIP == CHIP_RF4463F30
    m_hw->chip.set_fifo_mode(hw::RF4463F30::FIFO_Mode::HALF_DUPLEX);
#endif

    Clock::duration tx_average_duration = Clock::duration::zero();
    //Clock::time_point last_tp = Clock::now();
    size_t tx_packet_count = 0;
    size_t rx_packet_count = 0;
    size_t min_size = 999999;
    size_t max_size = 0;
    size_t average_size = 0;
    size_t crc_failed_count = 0;

    uint8_t last_encoded_dBm = 0;

    while (!m_exit)
    {
        //RX session, until the master says to send -----------------------------------
        {
            size_t spin = 0;
            while (!m_exit)
            {
                spin++;
                Chip::RX_Result result = m_hw->chip.rx(get_mtu() + sizeof(Master_Header), std::chrono::seconds(1), std::chrono::milliseconds(20));
                if (result == Chip::RX_Result::OK)
                {
                    uint8_t* rx_data = m_hw->chip.get_rx_fifo_payload_ptr();
                    size_t rx_size = m_hw->chip.get_rx_fifo_payload_size();
                    min_size = std::min(min_size, rx_size);
                    max_size = std::max(max_size, rx_size);
                    average_size += rx_size;
                    if (rx_data && rx_size >= sizeof(Master_Header))
                    {
                        rx_packet_count++;

                        std::unique_ptr<std::vector<uint8_t>> buffer = m_rx_queue.begin_producing();
                        if (buffer)
                        {
                            buffer->resize(rx_size);
                            memcpy(buffer->data(), rx_data, buffer->size());
                            m_rx_queue.end_producing(std::move(buffer), false);
                        }

                        Master_Header& header = *reinterpret_cast<Master_Header*>(rx_data);
                        if (header.more_data == 0)
                        {
                            break; //TX time!
                        }
                    }
                }
                else if (result == Chip::RX_Result::CRC_FAILED)
                {
                    crc_failed_count++;
                }
                else if (result != Chip::RX_Result::TIMEOUT)
                {
                    QLOGW("RX error: {}", result);
                }
            }
        }

        //refresh dBm
        if (Clock::now() - m_last_dBm_tp >= std::chrono::milliseconds(500))
        {
            m_last_dBm_tp = Clock::now();
            m_last_dBm = m_hw->chip.get_input_dBm();
            last_encoded_dBm = -std::min<int8_t>(m_last_dBm, 0);
        }

        //TX session
        {
            auto start_tx_tp = Clock::now();

            std::this_thread::sleep_for(std::chrono::microseconds(300));

            std::unique_ptr<std::vector<uint8_t>> buffer = m_tx_queue.begin_consuming(false);
            if (buffer)
            {
                uint8_t* tx_fifo = m_hw->chip.get_tx_fifo_payload_ptr(buffer->size());
                QASSERT(buffer->size() >= sizeof(Slave_Header));
                memcpy(tx_fifo, buffer->data(), buffer->size());
                buffer->clear();
                m_tx_queue.end_consuming(std::move(buffer));

                Slave_Header& header = *reinterpret_cast<Slave_Header*>(tx_fifo);
                header.dBm = last_encoded_dBm;

                if (m_hw->chip.tx(std::chrono::milliseconds(10)))
                {
                    tx_packet_count++;
                }
                else
                {
                    QLOGW("TX error");
                }

                Clock::duration tx_duration = Clock::now() - start_tx_tp;
                tx_average_duration += tx_duration;
            }
        }

//        if (Clock::now() - last_tp >= std::chrono::seconds(1))
//        {
//            last_tp = Clock::now();
//            QLOGI("TX packets: {}, average per packet: {}", tx_packet_count, tx_packet_count > 0 ? tx_average_duration / tx_packet_count : Clock::duration::zero());
//            tx_packet_count = 0;
//            tx_average_duration = Clock::duration::zero();

//            QLOGI("RX packets: {}, crc failed {}, {}-{}-{}", rx_packet_count, crc_failed_count, min_size, max_size, rx_packet_count > 0 ? average_size / rx_packet_count : 0);
//            rx_packet_count = 0;
//            crc_failed_count = 0;
//            min_size = 999999;
//            max_size = 0;
//            average_size = 0;
//        }

        if (m_exit)
        {
            break;
        }
    }
}

void RC_Phy::packet_tx_thread_proc()
{
    QLOG_TOPIC("RC_Phy::packet_tx");

    while (!m_exit)
    {
        std::unique_ptr<std::vector<uint8_t>> buffer = m_tx_queue.begin_producing();
        if (!buffer)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(1000));
            continue;
        }
        if (m_is_master)
        {
            buffer->resize(sizeof(Master_Header));
        }
        else
        {
            buffer->resize(sizeof(Slave_Header));
        }

        bool more_data = true;
        if (m_tx_callback)
        {
            if (!m_tx_callback(*buffer, more_data))
            {
                m_tx_queue.cancel_producing(std::move(buffer));

                std::this_thread::sleep_for(std::chrono::microseconds(100));
                continue;
            }
        }

        if (m_is_master)
        {
            QASSERT(buffer->size() >= sizeof(Master_Header));
            Master_Header& header = *reinterpret_cast<Master_Header*>(buffer->data());
            header.more_data = more_data ? 1 : 0;
        }

        m_tx_queue.end_producing(std::move(buffer), true);
    }
}

void RC_Phy::packet_rx_thread_proc()
{
    QLOG_TOPIC("RC_Phy::packet_rx");

    while (!m_exit)
    {
        std::unique_ptr<std::vector<uint8_t>> buffer = m_rx_queue.begin_consuming(true);
        if (!buffer)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(1000));
            continue;
        }

        int8_t header_dBm = 0;
        size_t header_size = 0;
        if (m_is_master)
        {
            if (buffer->size() >= sizeof(Slave_Header))
            {
                Slave_Header const& header = *reinterpret_cast<Slave_Header const*>(buffer->data());
                header_dBm = -static_cast<int8_t>(header.dBm);
                header_size = sizeof(Slave_Header);
            }
        }
        else
        {
            if (buffer->size() >= sizeof(Master_Header))
            {
                Master_Header const& header = *reinterpret_cast<Master_Header const*>(buffer->data());
                header_dBm = -static_cast<int8_t>(header.dBm);
                header_size = sizeof(Master_Header);
            }
        }

        if (header_size > 0)
        {
            std::lock_guard<std::mutex> lg(m_rx_data_mutex);
            m_rx_data.tx_dBm = header_dBm;
            m_rx_data.rx_dBm = m_last_dBm;
            m_rx_data.index++;
            m_rx_data.rx_timepoint = Clock::now();
            buffer->erase(buffer->begin(), buffer->begin() + header_size);
            if (!buffer->empty() && m_rx_callback)
            {
                m_rx_callback(m_rx_data, *buffer);
            }
        }

        buffer->clear();
        m_rx_queue.end_consuming(std::move(buffer));
    }
}


}
}
