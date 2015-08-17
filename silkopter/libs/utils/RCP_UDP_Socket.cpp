#include "RCP_UDP_Socket.h"

namespace util
{

static constexpr uint8_t MARKER_DATA = 0;
static constexpr uint8_t MARKER_ACK = 1;

static constexpr std::chrono::milliseconds MAX_ACK_TIMEOUT(100);

RCP_UDP_Socket::RCP_UDP_Socket()
    : m_io_work(new boost::asio::io_service::work(m_io_service))
    , m_socket(m_io_service)
{
    m_rx_buffer.resize(100 * 1024);
    m_asio_send_callback = boost::bind(&RCP_UDP_Socket::handle_send, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
    m_asio_receive_callback = boost::bind(&RCP_UDP_Socket::handle_receive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);

    m_io_thread = std::thread([this]() { m_io_service.run(); });
}

RCP_UDP_Socket::~RCP_UDP_Socket()
{
    m_io_work.reset();
    m_io_service.stop();
    if (m_io_thread.joinable())
    {
        std::this_thread::yield();
        m_io_thread.join();
    }
}

void RCP_UDP_Socket::open(uint16_t send_port, uint16_t receive_port)
{
    m_socket.open(boost::asio::ip::udp::v4());
    m_socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    m_socket.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), receive_port));

    m_send_port = send_port;
    m_receive_port = receive_port;
}

void RCP_UDP_Socket::start_listening()
{
    m_socket.async_receive_from(boost::asio::buffer(m_rx_buffer), m_rx_endpoint, m_asio_receive_callback);
}

void RCP_UDP_Socket::set_send_endpoint(boost::asio::ip::udp::endpoint endpoint)
{
    m_tx_endpoint = endpoint;
}

auto RCP_UDP_Socket::acquire_tx_buffer_locked(size_t size) -> Buffer
{
    Buffer buffer;
    if (m_tx_buffer_pool.empty())
    {
        buffer = std::make_shared<Buffer::element_type>(size);
    }
    else
    {
        buffer = std::move(m_tx_buffer_pool.back());
        m_tx_buffer_pool.pop_back();
        buffer->resize(size);
    }

    return buffer;
}

void RCP_UDP_Socket::async_send(uint8_t const* data, size_t size)
{
    QASSERT(!m_send_in_progress);
    m_send_in_progress = true;

    std::lock_guard<std::mutex> lg(m_tx_buffer_mutex);

    auto buffer = acquire_tx_buffer_locked(size + 1);

    buffer->front() = MARKER_DATA;
    std::copy(data, data + size, buffer->data() + 1);

    m_tx_buffer_queue.push_back(std::move(buffer));
    m_tx_data_sent_tp = q::Clock::now();

    send_next_packet_locked();
}

void RCP_UDP_Socket::send_next_packet_locked()
{
    if (!m_tx_buffer_queue.empty() && !m_tx_buffer_in_transit)
    {
        //std::cout << "sending\n";
        m_tx_buffer_in_transit = std::move(m_tx_buffer_queue.front());
        m_tx_buffer_queue.pop_front();

        if (m_tx_buffer_in_transit->front() == MARKER_DATA)
        {
            m_tx_data_sent_tp = q::Clock::now();
        }

        m_socket.async_send_to(boost::asio::buffer(*m_tx_buffer_in_transit), m_tx_endpoint, m_asio_send_callback);
    }
}

void RCP_UDP_Socket::handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if (error)
    {
        if (error != boost::asio::error::eof)
        {
            QLOGE("Error on socket receive: {}", error.message());
            //m_socket.close();
        }
    }
    else
    {
        if (bytes_transferred > 0)
        {
            if (m_rx_buffer[0] == MARKER_DATA)
            {
                if (receive_callback)
                {
                    receive_callback(m_rx_buffer.data() + 1, bytes_transferred - 1);
                }

                //send ack
                {
                    std::lock_guard<std::mutex> lg(m_tx_buffer_mutex);
                    auto buffer = acquire_tx_buffer_locked(1);
                    buffer->front() = MARKER_ACK;
                    m_tx_buffer_queue.push_back(std::move(buffer));
                    send_next_packet_locked();
                }
            }
            else if (m_rx_buffer[0] == MARKER_ACK)
            {
                if (m_send_in_progress)
                {
                    QASSERT(bytes_transferred == 1);
                    m_send_in_progress = false;
                    if (send_callback)
                    {
                        send_callback(Result::OK);
                    }
                }
            }
        }
        start_listening();
    }
}
void RCP_UDP_Socket::handle_send(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    std::lock_guard<std::mutex> lg(m_tx_buffer_mutex);

    //put it back in the pool
    if (m_tx_buffer_in_transit)
    {
        m_tx_buffer_pool.push_back(std::move(m_tx_buffer_in_transit));

        m_tx_buffer_in_transit.reset();
    }

    //send the next one
    send_next_packet_locked();

//    if (error)
//    {
//        QLOGE("Error on socket send: {}", error.message());
//        if (send_callback)
//        {
//            send_callback(Result::ERROR);
//        }
//    }
}

size_t RCP_UDP_Socket::prepare_buffer(std::vector<uint8_t>& buffer)
{
    //no header needed
    return 0;
}
auto RCP_UDP_Socket::get_mtu() const -> size_t
{
    return 1450;
}

auto RCP_UDP_Socket::process() -> Result
{
    if (m_tx_endpoint.address().is_unspecified() && !m_rx_endpoint.address().is_unspecified())
    {
        auto endpoint = m_rx_endpoint;
        endpoint.port(m_send_port);
        m_tx_endpoint = endpoint;
        return Result::RECONNECTED;
    }

    //check if the ACK is too late. If it is, we issue an error to give the caller a chance to retry
    //This happens a lot when starting one endpoint before the other. The first endpoint sends some data and waits
    // for the ACK but the second one never received the data (as it was started after) and never sends the ACK
    auto now = q::Clock::now();
    if (m_send_in_progress && now - m_tx_data_sent_tp > MAX_ACK_TIMEOUT)
    {
        m_send_in_progress = false;
        if (send_callback)
        {
            send_callback(Result::ERROR);
        }
    }

    return Result::OK;
}


}
