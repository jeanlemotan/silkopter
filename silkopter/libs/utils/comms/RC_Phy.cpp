#include "RC_Phy.h"
#include "utils/hw/RF4463F30.h"
#include "utils/hw/RFM22B.h"
#include <string>
#include "util/murmurhash.h"

#define CHIP_RF4463F30  1
#define CHIP_RFM22B     2

#define USE_CHIP CHIP_RFM22B


namespace util
{
namespace comms
{

constexpr size_t MTU = 64;
constexpr uint8_t CHANNEL = 0;
constexpr q::Clock::duration MIN_TX_DURATION = std::chrono::microseconds(1);



struct RC_Phy::HW
{

#if USE_CHIP == CHIP_RF4463F30
    hw::RF4463F30 chip;
#elif USE_CHIP == CHIP_RFM22B
    hw::RFM22B chip;
#else
#   error "Choose a RF chip"
#endif
};



RC_Phy::RC_Phy(bool master)
    : m_is_master(master)
{
    m_hw.reset(new HW);
    set_rate(30);
}

RC_Phy::~RC_Phy()
{
    m_exit = true;
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

bool RC_Phy::init(std::string const& device, uint32_t speed, uint8_t sdn_gpio, uint8_t nirq_gpio)
{
    if (!m_hw->chip.init(device, speed, sdn_gpio, nirq_gpio))
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
#endif

    if (m_is_master)
    {
        m_thread = boost::thread([this]() { master_thread_proc(); });
    }
    else
    {
        m_thread = boost::thread([this]() { slave_thread_proc(); });
    }

    return true;
}

void RC_Phy::set_callbacks(TX_Callback txcb, RX_Callback rxcb)
{
    m_tx_callback = txcb;
    m_rx_callback = rxcb;
}


size_t RC_Phy::get_mtu() const
{
    return MTU - sizeof(Header);
}

void RC_Phy::set_rate(size_t rate)
{
    m_desired_rate = std::max(rate, 1u);
    m_desired_duration = std::chrono::microseconds(1000000 / rate);
    if (m_desired_duration < MIN_TX_DURATION)
    {
        m_desired_duration = MIN_TX_DURATION;
    }
}

//void RC::set_tx_data(void const* data, size_t size)
//{
//    std::lock_guard<std::mutex> lg(m_mutex);
//    m_tx_data.resize(size);

//    if (size > 0 && data)
//    {
//        memcpy(m_tx_data.data(), data, size);
//    }
//}

//bool RC::get_rx_data(RX_Data& rx_data) const
//{
//    std::lock_guard<std::mutex> lg(m_mutex);
//    bool is_new = m_rx_data.index > rx_data.index;
//    rx_data = m_rx_data;
//    return is_new;
//}

void RC_Phy::master_thread_proc()
{
    while (!m_exit)
    {
        q::Clock::duration tx_duration;

        {
            auto start_tx_tp = q::Clock::now();
            auto diff = start_tx_tp - m_rx_data.tx_timepoint;
            if (diff < m_desired_duration)
            {
                std::this_thread::sleep_for(diff);
            }

            start_tx_tp = q::Clock::now();

            {
                //std::lock_guard<std::mutex> lg(m_mutex);
                m_tx_buffer.resize(sizeof(Header) + get_mtu());
                size_t size = m_tx_callback(m_tx_buffer.data() + sizeof(Header));
                m_tx_buffer.resize(sizeof(Header) + size);
            }

            Header& header = *reinterpret_cast<Header*>(m_tx_buffer.data());
            header.crc = 0;
            header.dBm = m_hw->chip.get_input_dBm();
            header.crc = q::util::compute_murmur_hash16(m_tx_buffer.data(), m_tx_buffer.size());

            //wait a bit so the other end has time to setup its RX state
            std::this_thread::sleep_for(std::chrono::microseconds(500));

            //The first byte is the length. The rest are payload
            if (m_hw->chip.write_tx_fifo(m_tx_buffer.data(), m_tx_buffer.size()))
            {
                //The first byte is the length. The actual payload is size - 1
                if (m_hw->chip.tx(m_tx_buffer.size(), CHANNEL))
                {

                }
            }

            m_rx_data.tx_timepoint = q::Clock::now();

            tx_duration = q::Clock::now() - start_tx_tp;
        }

        {
            q::Clock::duration rx_duration = m_desired_duration - tx_duration;
            if (rx_duration < m_desired_duration / 2)
            {
                rx_duration = m_desired_duration / 2;
            }

            size_t rx_size = 0;
            if (m_hw->chip.rx(rx_size, CHANNEL, rx_duration))
            {
                read_fifo(rx_size);
            }
            else
            {
                QLOGW("RX Failure");
            }
        }
    }
}

void RC_Phy::slave_thread_proc()
{
    while (!m_exit)
    {
        //wait for a packet indefinitely
        {
            size_t rx_size = 0;
            while (!m_exit)
            {
                if (m_hw->chip.rx(rx_size, CHANNEL, std::chrono::seconds(500)))
                {
                    read_fifo(rx_size);
                    break;
                }
                else
                {
                    QLOGW("RX Failure");
                }
            }
        }

        if (m_exit)
        {
            break;
        }

        //if packet received, respond
        {
            {
//                std::lock_guard<std::mutex> lg(m_mutex);
//                m_tx_buffer.resize(1 + sizeof(Header) + m_tx_data.size());
//                memcpy(m_tx_buffer.data() + sizeof(Header), m_tx_data.data(), m_tx_data.size());
                m_tx_buffer.resize(sizeof(Header) + get_mtu());
                size_t size = m_tx_callback(m_tx_buffer.data() + sizeof(Header));
                m_tx_buffer.resize(sizeof(Header) + size);
            }

            //auto start_tx_tp = q::Clock::now();

            Header& header = *reinterpret_cast<Header*>(m_tx_buffer.data());
            header.crc = 0;
            header.dBm = m_hw->chip.get_input_dBm();
            header.crc = q::util::compute_murmur_hash16(m_tx_buffer.data(), m_tx_buffer.size());

            //wait a bit so the other end has time to setup its RX state
            std::this_thread::sleep_for(std::chrono::microseconds(500));

            //The first byte is the length. The rest are payload
            if (m_hw->chip.write_tx_fifo(m_tx_buffer.data(), m_tx_buffer.size()))
            {
                //The first byte is the length. The actual payload is size - 1
                if (m_hw->chip.tx(m_tx_buffer.size(), CHANNEL))
                {

                }
            }

            m_rx_data.tx_timepoint = q::Clock::now();
        }
    }
}

bool RC_Phy::read_fifo(size_t rx_size)
{
    if (rx_size == 0)
    {
        return true;
    }

    if (rx_size >= sizeof(Header) && rx_size <= MTU)
    {
        //m_rx_buffer.resize(rx_size);
        m_rx_data.payload.resize(rx_size);

        if (m_hw->chip.read_rx_fifo(m_rx_data.payload.data(), rx_size))
        {
            Header& header = *reinterpret_cast<Header*>(m_rx_data.payload.data());
            uint16_t crc = header.crc;
            header.crc = 0;
            uint16_t computed_crc = q::util::compute_murmur_hash16(m_rx_data.payload.data(), m_rx_data.payload.size());
            if (crc == computed_crc)
            {
                int8_t dBm = m_hw->chip.get_input_dBm();

                {
                    //std::lock_guard<std::mutex> lg(m_mutex);
                    m_rx_data.index++;
                    m_rx_data.tx_dBm = header.dBm;
                    m_rx_data.rx_dBm = dBm;
                    m_rx_data.rx_timepoint = q::Clock::now();
                    m_rx_data.payload.erase(m_rx_data.payload.begin(), m_rx_data.payload.begin() + sizeof(Header));
                    m_rx_callback(m_rx_data);
                }
                return true;
            }
            else
            {
                QLOGW("CRC mismatch: {} vs {}", crc, computed_crc);
                return false;
            }
        }
        else
        {
            QLOGW("Failed to read RX fifo");
            return false;
        }
    }
    else
    {
        QLOGW("Invalid size received: {}", rx_size);
        return false;
    }
}


}
}
