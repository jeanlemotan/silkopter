#pragma once

#include <vector>
#include <deque>
#include <mutex>
#include <boost/bind.hpp>

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

    ASIO_Socket_Adapter()
    {
    }

    void start(Socket_t& s)
    {
        m_socket = &s;
        start();
    }

    void stop()
    {
        m_socket = nullptr;
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
        assert(m_socket);
        if (size && m_socket)
        {
            m_pending_to_send_size += size;

            std::lock_guard<std::mutex> lg(m_tx_buffer_mutex);
            size_t offset = m_tx_pending_buffer.size();
            m_tx_pending_buffer.resize(offset + size);
            std::copy(reinterpret_cast<uint8_t const*>(data), reinterpret_cast<uint8_t const*>(data) + size, m_tx_pending_buffer.begin() + offset);

            if (!m_is_sending)
            {
                size_t mtu_size = std::min(32768u, m_tx_pending_buffer.size());
                m_tx_crt_buffer.resize(mtu_size);

                std::copy(m_tx_pending_buffer.begin(), m_tx_pending_buffer.begin() + mtu_size, m_tx_crt_buffer.begin());

                m_is_sending = true;
                m_socket->async_write_some(boost::asio::buffer(m_tx_crt_buffer),
                                          boost::bind(&ASIO_Socket_Adapter<Socket_t>::handle_send, this,
                                                      boost::asio::placeholders::error,
                                                      boost::asio::placeholders::bytes_transferred));
            }
        }
    }

    size_t get_send_buffer_size() const
    {
        return m_pending_to_send_size;
    }

private:
    void start()
    {
        m_socket->async_read_some(boost::asio::buffer(m_rx_temp_buffer.data(), m_rx_temp_buffer.size()),
                                 boost::bind(&ASIO_Socket_Adapter<Socket_t>::handle_receive, this,
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred));
    }

    void handle_send(const boost::system::error_code& error, std::size_t bytes_transferred)
    {
        if (error)
        {
            if (m_socket)
            {
                m_socket->close();
            }
        }
        else
        {
            std::lock_guard<std::mutex> lg(m_tx_buffer_mutex);

            QASSERT(bytes_transferred <= m_tx_pending_buffer.size());
            bytes_transferred = std::min(bytes_transferred, m_tx_pending_buffer.size());
            m_pending_to_send_size -= bytes_transferred;

            m_tx_pending_buffer.erase(m_tx_pending_buffer.begin(), m_tx_pending_buffer.begin() + bytes_transferred);

            if (!m_tx_pending_buffer.empty())
            {
                size_t mtu_size = std::min(32768u, m_tx_pending_buffer.size());
                m_tx_crt_buffer.resize(mtu_size);

                std::copy(m_tx_pending_buffer.begin(), m_tx_pending_buffer.begin() + mtu_size, m_tx_crt_buffer.begin());

                if (m_socket)
                {
                    m_is_sending = true;
                    m_socket->async_write_some(boost::asio::buffer(m_tx_crt_buffer),
                                               boost::bind(&ASIO_Socket_Adapter<Socket_t>::handle_send, this,
                                                           boost::asio::placeholders::error,
                                                           boost::asio::placeholders::bytes_transferred));
                }
            }
            else
            {
                m_is_sending = false;
            }
        }
    }

    void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred)
    {
        if (error)
        {
            if (m_socket)
            {
                m_socket->close();
            }
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

    Socket_t* m_socket = nullptr;
    RX_Buffer_t m_rx_buffer;
    std::array<uint8_t, 512> m_rx_temp_buffer;
    std::mutex m_rx_mutex;

    std::atomic_uint m_pending_to_send_size = { 0 };

    std::mutex m_tx_buffer_mutex;
    std::vector<uint8_t> m_tx_crt_buffer;
    std::deque<uint8_t> m_tx_pending_buffer;
    bool m_is_sending = false;

    std::vector<std::shared_ptr<TX_Buffer_t>> m_tx_buffer_pool;
    std::mutex m_tx_buffer_pool_mutex;
};


}
}
