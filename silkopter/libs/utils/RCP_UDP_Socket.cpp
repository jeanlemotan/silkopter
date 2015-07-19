#include "RCP_UDP_Socket.h"

namespace util
{

static constexpr uint8_t MARKER_DATA = 0;
static constexpr uint8_t MARKER_ACK = 1;

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
    m_socket.set_option(boost::asio::socket_base::receive_buffer_size(get_mtu() + 200));
    m_socket.set_option(boost::asio::socket_base::send_buffer_size(get_mtu() + 200));
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

void RCP_UDP_Socket::async_send(uint8_t const* data, size_t size)
{
    QASSERT(!m_send_in_progress);
    m_send_in_progress = true;

    m_tx_buffer.resize(size + 1);
    m_tx_buffer[0] = MARKER_DATA;
    std::copy(data, data + size, m_tx_buffer.data() + 1);
    m_socket.async_send_to(boost::asio::buffer(m_tx_buffer), m_tx_endpoint, m_asio_send_callback);
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
            if (m_rx_buffer[0] == MARKER_DATA && receive_callback)
            {
                receive_callback(m_rx_buffer.data() + 1, bytes_transferred - 1);
            }
            else if (m_rx_buffer[0] == MARKER_ACK)
            {
                QASSERT(bytes_transferred == 1);
                QASSERT(m_send_in_progress);
                m_send_in_progress = false;
                if (send_callback)
                {
                    send_callback(Result::OK);
                }
            }
        }
        start_listening();
    }
}
void RCP_UDP_Socket::handle_send(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    //std::this_thread::sleep_for(std::chrono::milliseconds(5));
    if (error)
    {
        QLOGE("Error on socket send: {}", error.message());
        if (send_callback)
        {
            send_callback(Result::ERROR);
        }
    }
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
    return Result::OK;
}


}
