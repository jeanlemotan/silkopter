#include "RCP_RF4463F30_Socket.h"
#include "RF4463F30.h"

namespace util
{

constexpr uint8_t CHANNEL = 0;

constexpr q::Clock::duration TX_DURATION = std::chrono::milliseconds(5);
constexpr q::Clock::duration RX_DURATION = std::chrono::milliseconds(5);

struct RCP_RF4463F30_Socket::Impl
{
    RF4463F30 rf_chip;

    std::mutex tx_buffer_mutex;
    std::vector<uint8_t> tx_buffer;
    bool tx_buffer_has_data = false;
    std::condition_variable tx_buffer_cv;

    std::vector<uint8_t> rx_buffer;
};


///////////////////////////////////////////////////////////////////////////////////////

RCP_RF4463F30_Socket::RCP_RF4463F30_Socket(std::string const& device, uint32_t speed, bool master)
    : m_impl(new Impl)
    , m_device(device)
    , m_speed(speed)
    , m_is_master(master)
{

}

RCP_RF4463F30_Socket::~RCP_RF4463F30_Socket()
{
    m_exit = true;
    m_impl->tx_buffer_cv.notify_all(); //to wake up the thread

    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

RCP_RF4463F30_Socket::Result RCP_RF4463F30_Socket::process()
{
    return Result::OK;
}

bool RCP_RF4463F30_Socket::start()
{
    if (m_is_initialized)
    {
        return true;
    }
    if (!m_impl->rf_chip.init(m_device, m_speed, 6, 5))
    {
        return false;
    }

//    auto start = std::chrono::high_resolution_clock::now();

//    size_t i = 0;
//    while (true)
//    {
//        std::string str = q::util::format<std::string>("counter: {}", i);

//        //QLOGI("FIFO {}", i);
//        if (!m_impl->rf_chip.write_tx_fifo(str.data(), str.size()))
//        {
//            QLOGE("Failed to fill fifo");
//            std::this_thread::sleep_for(std::chrono::milliseconds(100));
//        }
//        //QLOGI("Send {}", i);
//        if (!m_impl->rf_chip.start_tx(str.size(), 0))
//        {
//            QLOGE("Failed to send");
//            std::this_thread::sleep_for(std::chrono::milliseconds(100));
//        }
//        //QLOGI("Done {}", i);
//        std::this_thread::sleep_for(std::chrono::microseconds(3000));
//        i++;

//        if (std::chrono::high_resolution_clock::now() - start > std::chrono::milliseconds(1000))
//        {
//            start = std::chrono::high_resolution_clock::now();
//            QLOGI("{} / {} bytes per second", i, i * str.size());
//            i = 0;
//        }
//    }

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

void RCP_RF4463F30_Socket::master_thread_proc()
{
    while (!m_exit)
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
            Result result = Result::OK;

            auto now = q::Clock::now();
            auto diff = now - m_last_tx_tp;
            if (diff < TX_DURATION)
            {
                std::this_thread::sleep_for(diff);
            }

            //wait a bit so the other end has time to setup its RX state
            std::this_thread::sleep_for(std::chrono::microseconds(500));

            if (m_impl->rf_chip.write_tx_fifo(m_impl->tx_buffer.data(), m_impl->tx_buffer.size()))
            {
                if (m_impl->rf_chip.tx(m_impl->tx_buffer.size(), CHANNEL))
                {
                    result = Result::OK;
                }
                else
                {
                    result = Result::ERROR;
                }
            }
            else
            {
                result = Result::ERROR;
            }

            m_last_tx_tp = q::Clock::now();
            m_impl->tx_buffer_has_data = false;

            if (send_callback)
            {
                send_callback(result);
            }
        }

        {
            if (m_impl->rf_chip.begin_rx(CHANNEL))
            {
                std::this_thread::sleep_for(RX_DURATION);

                size_t rx_size = 0;
                if (m_impl->rf_chip.end_rx(rx_size))
                {
                    if (rx_size > 0)
                    {
                        m_impl->rx_buffer.resize(rx_size);
                        if (m_impl->rf_chip.read_rx_fifo(m_impl->rx_buffer.data(), rx_size))
                        {
                            if (receive_callback)
                            {
                                receive_callback(m_impl->rx_buffer.data(), m_impl->rx_buffer.size());
                            }

                            std::string str(m_impl->rx_buffer.begin(), m_impl->rx_buffer.end());
                            QLOGI("RCV: {}", str);
                        }
                    }
                }
            }
        }
    }
}

void RCP_RF4463F30_Socket::slave_thread_proc()
{
    while (!m_exit)
    {
        //wait for a packet indefinitely
        {
            size_t rx_size = 0;
            if (m_impl->rf_chip.begin_rx(CHANNEL))
            {
                while (!m_exit && rx_size == 0)
                {
                    if (m_impl->rf_chip.end_rx(rx_size))
                    {
                        if (rx_size > 0)
                        {
                            m_impl->rx_buffer.resize(rx_size);
                            if (m_impl->rf_chip.read_rx_fifo(m_impl->rx_buffer.data(), rx_size))
                            {
                                receive_callback(m_impl->rx_buffer.data(), m_impl->rx_buffer.size());

                                std::string str(m_impl->rx_buffer.begin(), m_impl->rx_buffer.end());
                                QLOGI("RCV: {}", str);
                            }
                        }
                    }
                    if (rx_size == 0)
                    {
                        std::this_thread::sleep_for(std::chrono::microseconds(100));
                    }
                }
            }
        }

        if (m_exit)
        {
            break;
        }

        //if packet received, respond
        {
            std::unique_lock<std::mutex> lg(m_impl->tx_buffer_mutex);
            if (m_impl->tx_buffer_has_data)
            {
                Result result = Result::OK;

                auto now = q::Clock::now();
                auto diff = now - m_last_tx_tp;
                if (diff < TX_DURATION)
                {
                    std::this_thread::sleep_for(diff);
                }

                //wait a bit so the other end has time to setup its RX state
                std::this_thread::sleep_for(std::chrono::microseconds(500));

                if (m_impl->rf_chip.write_tx_fifo(m_impl->tx_buffer.data(), m_impl->tx_buffer.size()))
                {
                    if (m_impl->rf_chip.tx(m_impl->tx_buffer.size(), CHANNEL))
                    {
                        result = Result::OK;
                    }
                    else
                    {
                        result = Result::ERROR;
                    }
                }
                else
                {
                    result = Result::ERROR;
                }

                m_last_tx_tp = q::Clock::now();
                m_impl->tx_buffer_has_data = false;

                if (send_callback)
                {
                    send_callback(result);
                }
            }
        }
    }
}

void RCP_RF4463F30_Socket::async_send(void const* _data, size_t size)
{
    QASSERT(size <= get_mtu());
    QASSERT(m_send_in_progress == true);

    uint8_t const* data = reinterpret_cast<uint8_t const*>(_data);

    {
        std::unique_lock<std::mutex> lg(m_impl->tx_buffer_mutex);

        QASSERT(!m_impl->tx_buffer_has_data);
        m_impl->tx_buffer.resize(size);
        std::copy(data, data + size, m_impl->tx_buffer.begin());
    }

    m_impl->tx_buffer_has_data = true;
    if (m_is_master)
    {
        m_impl->tx_buffer_cv.notify_all();
    }
}

size_t RCP_RF4463F30_Socket::get_mtu() const
{
    return 64;
}

bool RCP_RF4463F30_Socket::lock()
{
    if (m_send_in_progress.exchange(true))
    {
        //already locked
        return false;
    }
    return true;
}

void RCP_RF4463F30_Socket::unlock()
{
    m_send_in_progress = false;
}


}
