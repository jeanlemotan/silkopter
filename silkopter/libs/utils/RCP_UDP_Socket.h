#pragma once

#include "RCP.h"
#include <boost/asio.hpp>

namespace util
{

class RCP_UDP_Socket : public RCP_Socket
{
public:
    RCP_UDP_Socket();
    ~RCP_UDP_Socket();

    auto process() -> Result;

    void set_send_endpoint(boost::asio::ip::udp::endpoint endpoint);
    void open(uint16_t send_port, uint16_t receive_port);
    void start_listening();

    auto prepare_buffer(std::vector<uint8_t>& buffer) -> size_t;
    auto get_mtu() const -> size_t;

private:
    void async_send(uint8_t const* data, size_t size);

    void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred);
    void handle_send(const boost::system::error_code& error, std::size_t bytes_transferred);

    boost::function<void(const boost::system::error_code& error, std::size_t bytes_transferred)> m_asio_send_callback;
    boost::function<void(const boost::system::error_code& error, std::size_t bytes_transferred)> m_asio_receive_callback;

    boost::asio::io_service m_io_service;
    std::thread m_io_thread;
    std::unique_ptr<boost::asio::io_service::work> m_io_work;

    std::atomic_bool m_send_in_progress = {false};
    std::vector<uint8_t> m_tx_buffer;

    boost::asio::ip::udp::endpoint m_tx_endpoint;
    boost::asio::ip::udp::endpoint m_rx_endpoint;
    boost::asio::ip::udp::socket m_socket;
    std::vector<uint8_t> m_rx_buffer;

    uint16_t m_send_port = 0;
    uint16_t m_receive_port = 0;
};

}
