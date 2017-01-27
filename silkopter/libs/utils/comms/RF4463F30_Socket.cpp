#include "RF4463F30_Socket.h"
#include "utils/hw/RF4463F30.h"

namespace util
{
namespace comms
{

constexpr uint8_t CHANNEL = 0;

constexpr Clock::duration TX_DURATION = std::chrono::milliseconds(5);
constexpr Clock::duration RX_DURATION = std::chrono::milliseconds(5);

struct RF4463F30_Socket::Impl
{
    hw::RF4463F30 rf_chip;

    std::mutex tx_buffer_mutex;
    std::vector<uint8_t> tx_buffer;
    bool tx_buffer_has_data = false;
    std::condition_variable tx_buffer_cv;

    std::vector<uint8_t> rx_buffer;
};


///////////////////////////////////////////////////////////////////////////////////////

RF4463F30_Socket::RF4463F30_Socket(std::string const& device, uint32_t speed, bool master)
    : m_impl(new Impl)
    , m_device(device)
    , m_speed(speed)
    , m_is_master(master)
{

}

RF4463F30_Socket::~RF4463F30_Socket()
{
    m_exit = true;
    m_impl->tx_buffer_cv.notify_all(); //to wake up the thread

    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

RF4463F30_Socket::Result RF4463F30_Socket::process()
{
    return Result::OK;
}

bool RF4463F30_Socket::start()
{
    if (m_is_initialized)
    {
        return true;
    }
    if (!m_impl->rf_chip.init(m_device, m_speed, 6, 5))
    {
        return false;
    }

    if (m_is_master)
    {
        auto start = Clock::now();
        size_t i = 0;

        std::vector<char> data;

        while (true)
        {
            std::string str = q::util::format<std::string>("counter: {} 123 456 789 abc def ghi jkl mno pqr stu vxy zx", i & 7);

            data.resize(str.size() + 1);
            data[0] = str.size();
            std::copy(str.begin(), str.end(), data.begin() + 1);

            //QLOGI("FIFO {}", i);
            if (!m_impl->rf_chip.write_tx_fifo(data.data(), data.size()))
            {
                QLOGE("Failed to fill fifo");
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            //QLOGI("Send {}", i);
            if (!m_impl->rf_chip.tx(data.size() - 1, 0))
            {
                QLOGE("Failed to send");
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

//            size_t rx_size = 0;
//            if (!m_impl->rf_chip.rx(rx_size, CHANNEL, std::chrono::milliseconds(1)))
//            {
//                QLOGE("Cannot enter RX state");
//            }

            //QLOGI("Done {}", i);
            //std::this_thread::sleep_for(std::chrono::milliseconds(0));
            i++;

            if (Clock::now() - start > std::chrono::milliseconds(1000))
            {
                start = Clock::now();
                QLOGI("{} / {} bytes per second", i, i * str.size());
                i = 0;
            }
        }
    }

    else
    {
        auto start = Clock::now();
        size_t i = 0;
        size_t bps = 0;
        size_t err_invs = 0;

        size_t rx_size = 0;
        while (!m_exit)
        {
            if (m_impl->rf_chip.rx(rx_size, CHANNEL, std::chrono::milliseconds(1000)))
            {
                if (rx_size > 0 && rx_size <= get_mtu())
                {
                    m_impl->rx_buffer.resize(rx_size);
                    if (m_impl->rf_chip.read_rx_fifo(m_impl->rx_buffer.data(), rx_size))
                    {
                        i++;
                        bps += rx_size;
                        std::string str(m_impl->rx_buffer.begin(), m_impl->rx_buffer.end());
                        QLOGI("RCV: {} / {}", rx_size, str);
                    }
                    else
                    {
                        QLOGE("Failed to read fifo");
                    }
                }
                else
                {
                    err_invs++;
                    //QLOGE("Invalid packet size: {}", rx_size);
                }
            }
            else
            {
                QLOGE("Failed to retrieve packet info");
            }

            //std::this_thread::sleep_for(std::chrono::microseconds(100));

            if (Clock::now() - start > std::chrono::milliseconds(1000))
            {
                start = Clock::now();
                QLOGI("{} / {} bytes per second. {}", i, bps, err_invs);
                i = 0;
                bps = 0;
                err_invs = 0;
            }
        }
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

void RF4463F30_Socket::master_thread_proc()
{
    while (!m_exit)
    {
        Result result = Result::ERROR;
        bool sent = false;

        {
            //wait for data
            std::unique_lock<std::mutex> lg(m_impl->tx_buffer_mutex);
            if (!m_impl->tx_buffer_has_data)
            {
                m_impl->tx_buffer_cv.wait(lg, [this]{ return m_impl->tx_buffer_has_data == true || m_exit == true; });
            }
            if (m_exit)
            {
                break;
            }

            if (m_impl->tx_buffer_has_data)
            {
                auto now = Clock::now();
                auto diff = now - m_last_tx_tp;
                if (diff < TX_DURATION)
                {
                    std::this_thread::sleep_for(diff);
                }

                //wait a bit so the other end has time to setup its RX state
                std::this_thread::sleep_for(std::chrono::microseconds(500));

                //The first byte is the length. The rest are payload
                if (m_impl->rf_chip.write_tx_fifo(m_impl->tx_buffer.data(), m_impl->tx_buffer.size()))
                {
                    //The first byte is the length. The actual payload is size - 1
                    if (m_impl->rf_chip.tx(m_impl->tx_buffer.size() - 1, CHANNEL))
                    {
                        result = Result::OK;
                    }
                }

                m_last_tx_tp = Clock::now();
                m_impl->tx_buffer_has_data = false;
                sent = true;
            }
        }

        if (sent && send_callback)
        {
            send_callback(result);
        }


        {
            size_t rx_size = 0;
            if (m_impl->rf_chip.rx(rx_size, CHANNEL, RX_DURATION))
            {
                if (rx_size > 0 && rx_size <= get_mtu())
                {
                    m_impl->rx_buffer.resize(rx_size);
                    if (m_impl->rf_chip.read_rx_fifo(m_impl->rx_buffer.data(), rx_size))
                    {
                        if (receive_callback)
                        {
                            receive_callback(m_impl->rx_buffer.data(), m_impl->rx_buffer.size());
                        }

                        std::string str(m_impl->rx_buffer.begin(), m_impl->rx_buffer.end());
                        QLOGI("RCV: {}: {}", rx_size, str);
                    }
                }
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }
        }
    }
}

void RF4463F30_Socket::slave_thread_proc()
{
    while (!m_exit)
    {
        //wait for a packet indefinitely
        {
            size_t rx_size = 0;
            while (!m_exit)
            {
                if (m_impl->rf_chip.rx(rx_size, CHANNEL, std::chrono::seconds(500)))
                {
                    if (rx_size > 0 && rx_size <= get_mtu())
                    {
                        m_impl->rx_buffer.resize(rx_size);
                        if (m_impl->rf_chip.read_rx_fifo(m_impl->rx_buffer.data(), rx_size))
                        {
                            if (receive_callback)
                            {
                                receive_callback(m_impl->rx_buffer.data(), m_impl->rx_buffer.size());
                            }

                            std::string str(m_impl->rx_buffer.begin(), m_impl->rx_buffer.end());
                            QLOGI("RCV: {}: {}", rx_size, str);
                            break;
                        }
                    }
                }
            }
        }

        if (m_exit)
        {
            break;
        }

        Result result = Result::ERROR;
        bool sent = false;

        //if packet received, respond
        {
            std::unique_lock<std::mutex> lg(m_impl->tx_buffer_mutex);
            if (m_impl->tx_buffer_has_data)
            {
                auto now = Clock::now();
                auto diff = now - m_last_tx_tp;
                if (diff < TX_DURATION)
                {
                    std::this_thread::sleep_for(diff);
                }

                //wait a bit so the other end has time to setup its RX state
                std::this_thread::sleep_for(std::chrono::microseconds(500));

                //The first byte is the length. The rest are payload
                if (m_impl->rf_chip.write_tx_fifo(m_impl->tx_buffer.data(), m_impl->tx_buffer.size()))
                {
                    //The first byte is the length. The actual payload is size - 1
                    if (m_impl->rf_chip.tx(m_impl->tx_buffer.size() - 1, CHANNEL))
                    {
                        result = Result::OK;
                    }
                }

                m_last_tx_tp = Clock::now();
                m_impl->tx_buffer_has_data = false;
                sent = true;
            }
        }

        if (sent && send_callback)
        {
            send_callback(result);
        }
    }
}

void RF4463F30_Socket::async_send(void const* _data, size_t size)
{
    QASSERT(size <= get_mtu());
    QASSERT(m_is_locked == true);

    {
        std::unique_lock<std::mutex> lg(m_impl->tx_buffer_mutex);

        QASSERT(!m_impl->tx_buffer_has_data);
        m_impl->tx_buffer.resize(1 + size);

        m_impl->tx_buffer[0] = size;

        uint8_t const* data = reinterpret_cast<uint8_t const*>(_data);
        std::copy(data, data + size, m_impl->tx_buffer.data() + 1);

        m_impl->tx_buffer_has_data = true;
    }

    if (m_is_master)
    {
        m_impl->tx_buffer_cv.notify_all();
    }
}

size_t RF4463F30_Socket::get_mtu() const
{
    return 63;
}

bool RF4463F30_Socket::lock()
{
    if (m_is_locked.exchange(true))
    {
        //already locked
        return false;
    }

    //now locked
    return true;
}

void RF4463F30_Socket::unlock()
{
    if (m_is_locked.exchange(false) == false)
    {
        QASSERT(false);
    }
}


}
}
