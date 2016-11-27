#include "RC.h"
#include "utils/hw/RF4463F30.h"
#include <string>
#include "util/murmurhash.h"

namespace util
{
namespace comms
{

constexpr size_t MTU = 64;
constexpr uint8_t CHANNEL = 0;
constexpr q::Clock::duration MIN_TX_DURATION = std::chrono::milliseconds(5);



struct RC::HW
{
    hw::RF4463F30 chip;
};



RC::RC(bool master)
    : m_is_master(master)
{
    m_hw.reset(new HW);
    set_rate(30);
}

RC::~RC()
{
    m_exit = true;
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

bool RC::init(std::string const& device, uint32_t speed, uint8_t sdn_gpio, uint8_t nirq_gpio)
{
    if (!m_hw->chip.init(device, speed, sdn_gpio, nirq_gpio))
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

    return true;
}

size_t RC::get_mtu() const
{
    return MTU - sizeof(Header);
}

void RC::set_rate(size_t rate)
{
    m_desired_rate = std::max(rate, 1u);
    m_desired_duration = std::chrono::microseconds(1000000 / rate);
    if (m_desired_duration < MIN_TX_DURATION)
    {
        m_desired_duration = MIN_TX_DURATION;
    }
}

void RC::set_tx_data(void const* data, size_t size)
{
    std::lock_guard<std::mutex> lg(m_mutex);
    m_tx_data.resize(size);

    if (size > 0 && data)
    {
        memcpy(m_tx_data.data(), data, size);
    }
}

void RC::get_rx_data(Data& data) const
{
    std::lock_guard<std::mutex> lg(m_mutex);
    data = m_rx_data;
}

void RC::master_thread_proc()
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
                std::lock_guard<std::mutex> lg(m_mutex);
                m_tx_buffer.resize(1 + sizeof(Header) + m_tx_data.size());
                memcpy(m_tx_buffer.data() + 1 + sizeof(Header), m_tx_data.data(), m_tx_data.size());
                m_tx_buffer[0] = sizeof(Header) + m_tx_data.size();
            }

            Header& header = *reinterpret_cast<Header*>(m_tx_buffer.data() + 1);
            header.crc = 0;
            m_hw->chip.get_dBm(header.dBm);
            header.crc = q::util::compute_murmur_hash16(m_tx_buffer.data() + 1, m_tx_buffer.size() - 1);

            //wait a bit so the other end has time to setup its RX state
            std::this_thread::sleep_for(std::chrono::microseconds(500));

            //The first byte is the length. The rest are payload
            if (m_hw->chip.write_tx_fifo(m_tx_buffer.data(), m_tx_buffer.size()))
            {
                //The first byte is the length. The actual payload is size - 1
                if (m_hw->chip.tx(m_tx_buffer.size() - 1, CHANNEL))
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

void RC::slave_thread_proc()
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
                std::lock_guard<std::mutex> lg(m_mutex);
                m_tx_buffer.resize(1 + sizeof(Header) + m_tx_data.size());
                memcpy(m_tx_buffer.data() + 1 + sizeof(Header), m_tx_data.data(), m_tx_data.size());
                m_tx_buffer[0] = sizeof(Header) + m_tx_data.size();
            }

            //auto start_tx_tp = q::Clock::now();

            Header& header = *reinterpret_cast<Header*>(m_tx_buffer.data() + 1);
            header.crc = 0;
            m_hw->chip.get_dBm(header.dBm);
            header.crc = q::util::compute_murmur_hash16(m_tx_buffer.data() + 1, m_tx_buffer.size() - 1);

            //wait a bit so the other end has time to setup its RX state
            std::this_thread::sleep_for(std::chrono::microseconds(500));

            //The first byte is the length. The rest are payload
            if (m_hw->chip.write_tx_fifo(m_tx_buffer.data(), m_tx_buffer.size()))
            {
                //The first byte is the length. The actual payload is size - 1
                if (m_hw->chip.tx(m_tx_buffer.size() - 1, CHANNEL))
                {

                }
            }

            m_rx_data.tx_timepoint = q::Clock::now();
        }
    }
}

bool RC::read_fifo(size_t rx_size)
{
    if (rx_size == 0)
    {
        return true;
    }

    if (rx_size >= sizeof(Header) && rx_size <= MTU)
    {
        m_rx_buffer.resize(rx_size);
        if (m_hw->chip.read_rx_fifo(m_rx_buffer.data(), rx_size))
        {
            Header& header = *reinterpret_cast<Header*>(m_rx_buffer.data());
            uint16_t crc = header.crc;
            header.crc = 0;
            uint16_t computed_crc = q::util::compute_murmur_hash16(m_rx_buffer.data(), m_rx_buffer.size());
            if (crc == computed_crc)
            {
                int8_t dBm = 0;
                m_hw->chip.get_dBm(dBm);

                {
                    std::lock_guard<std::mutex> lg(m_mutex);
                    m_rx_data.tx_dBm = header.dBm;
                    m_rx_data.rx_dBm = dBm;
                    m_rx_data.rx_timepoint = q::Clock::now();
                    m_rx_data.rx_data.resize(m_rx_buffer.size() - sizeof(Header));
                    if (!m_rx_data.rx_data.empty())
                    {
                        memcpy(m_rx_data.rx_data.data(), m_rx_buffer.data() + sizeof(Header), m_rx_data.rx_data.size());
                    }
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
