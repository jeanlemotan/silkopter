#pragma once

#include <memory>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "common/stream/IMultirotor_State.h"


namespace silk
{

class Remote_Viewer;

class Remote_Viewer_Server
{
public:
    Remote_Viewer_Server();
    ~Remote_Viewer_Server();

    void send_data(uint8_t const* video_data, size_t video_data_size, math::vec2u16 const& resolution,
                   stream::IMultirotor_State::Value const& multirotor_state);

    void process();

private:
    void start_accept();

    boost::asio::io_service m_io_service;
    std::unique_ptr<boost::asio::io_service::work> m_io_service_work;
    boost::asio::ip::tcp::endpoint m_endpoint;
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::thread m_io_service_thread;

    std::vector<std::unique_ptr<Remote_Viewer>> m_viewers;
};

}
