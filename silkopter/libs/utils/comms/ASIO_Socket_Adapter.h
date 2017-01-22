#pragma once

#include <vector>
#include <deque>
#include <mutex>

namespace util
{
namespace comms
{

template<class S>
class ASIO_Socket_Adapter : q::util::Noncopyable
{
    typedef std::vector<uint8_t> TX_Buffer_t;
    typedef std::vector<uint8_t> RX_Buffer_t;

public:
    typedef S Socket_t;

    ASIO_Socket_Adapter(Socket_t& s)
        : m_socket(s)
    {
    }

    void start()
    {
        m_socket.async_read_some(boost::asio::buffer(m_rx_temp_buffer.data(), m_rx_temp_buffer.size()),
                                 boost::bind(&ASIO_Socket_Adapter<Socket_t>::handle_receive, this,
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred));
        //            m_socket.async_receive(
        //                boost::asio::buffer(m_rx_temp_buffer.data(), m_rx_temp_buffer.size()),
        //                boost::bind(&Asio_Socket_Adapter<Socket_t>::handle_receive, this,
        //				boost::asio::placeholders::error,
        //				boost::asio::placeholders::bytes_transferred));
    }

    template<class Container>
    void read(Container& dst)
    {
        std::lock_guard<std::mutex> lg(m_rx_mutex);
        size_t off = dst.size();
        dst.resize(off + m_rx_buffer.size());
        std::copy(m_rx_buffer.begin(), m_rx_buffer.end(), dst.begin() + off);
        m_rx_buffer.clear();
    }
    void write(void const* data, size_t size)
    {
        if (size)
        {
            std::shared_ptr<TX_Buffer_t> buffer_ptr = get_tx_buffer();
            buffer_ptr->resize(size);
            std::copy(reinterpret_cast<uint8_t const*>(data), reinterpret_cast<uint8_t const*>(data) + size, buffer_ptr->begin());

            {
                std::lock_guard<std::mutex> lg(m_tx_buffer_queue_mutex);
                m_tx_buffer_queue.push_back(buffer_ptr);
                //QLOGI("++ buffer queue {}", m_tx_buffer_queue.size());
            }

            if (m_is_sending.exchange(true) == false)
            {
                m_socket.async_write_some(boost::asio::buffer(*buffer_ptr),
                                          boost::bind(&ASIO_Socket_Adapter<Socket_t>::handle_send, this,
                                                      buffer_ptr,
                                                      boost::asio::placeholders::error,
                                                      boost::asio::placeholders::bytes_transferred));
            }
        }
    }

private:
    void handle_send(std::shared_ptr<TX_Buffer_t> buffer_ptr,
                     const boost::system::error_code& error,
                     std::size_t bytes_transferred)
    {
        if (error)
        {
            m_socket.close();
        }
        else
        {
            {
                std::lock_guard<std::mutex> lg(m_tx_buffer_pool_mutex);
                m_tx_buffer_pool.push_back(buffer_ptr);
            }

            {
                std::lock_guard<std::mutex> lg(m_tx_buffer_queue_mutex);
                QASSERT(m_tx_buffer_queue.front() == buffer_ptr);
                m_tx_buffer_queue.pop_front();
                //QLOGI("-- buffer queue {}", m_tx_buffer_queue.size());

                if (!m_tx_buffer_queue.empty())
                {
                    buffer_ptr = m_tx_buffer_queue.front();
                    m_socket.async_write_some(boost::asio::buffer(*buffer_ptr),
                                              boost::bind(&ASIO_Socket_Adapter<Socket_t>::handle_send, this,
                                                          buffer_ptr,
                                                          boost::asio::placeholders::error,
                                                          boost::asio::placeholders::bytes_transferred));
                }
                else
                {
                    m_is_sending.exchange(false);
                }
            }
        }
    }

    void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred)
    {
        if (error)
        {
            m_socket.close();
        }
        else
        {
            if (bytes_transferred > 0)
            {
                std::lock_guard<std::mutex> lg(m_rx_mutex);
                size_t off = m_rx_buffer.size();
                m_rx_buffer.resize(off + bytes_transferred);
                std::copy(m_rx_temp_buffer.begin(), m_rx_temp_buffer.begin() + bytes_transferred, m_rx_buffer.begin() + off);
                //                    std::copy(m_rx_temp_buffer.begin(), m_rx_temp_buffer.begin() + bytes_transferred, std::back_inserter(m_rx_buffer));
            }
            start();
        }
    }

    std::shared_ptr<TX_Buffer_t> get_tx_buffer()
    {
        std::lock_guard<std::mutex> lg(m_tx_buffer_pool_mutex);
        std::shared_ptr<TX_Buffer_t> buffer_ptr;
        if (!m_tx_buffer_pool.empty())
        {
            buffer_ptr = m_tx_buffer_pool.back();
            m_tx_buffer_pool.pop_back();
        }
        else
        {
            buffer_ptr = std::make_shared<TX_Buffer_t>();
        }
        return buffer_ptr;
    }

    Socket_t& m_socket;
    RX_Buffer_t m_rx_buffer;
    std::array<uint8_t, 512> m_rx_temp_buffer;
    std::mutex m_rx_mutex;

    std::atomic_bool m_is_sending = { false };
    std::deque<std::shared_ptr<TX_Buffer_t>> m_tx_buffer_queue;
    std::mutex m_tx_buffer_queue_mutex;

    std::vector<std::shared_ptr<TX_Buffer_t>> m_tx_buffer_pool;
    std::mutex m_tx_buffer_pool_mutex;
};


}
}
