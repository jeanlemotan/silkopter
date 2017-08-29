#pragma once

#include <memory>
#include <asio.hpp>
#include <thread>
#include "common/stream/IMultirotor_State.h"


namespace silk
{

class Remote_Viewer;

class Remote_Viewer_Server
{
public:
    Remote_Viewer_Server();
    ~Remote_Viewer_Server();

    size_t get_remote_viewer_count() const;

    void send_video_data(void const* video_data, size_t video_data_size, math::vec2u16 const& resolution);
    void send_telemetry(stream::IMultirotor_Commands::Value const& multirotor_commands, stream::IMultirotor_State::Value const& multirotor_state);

    void process();

private:
    void start_accept();

    asio::io_service m_io_service;
    std::unique_ptr<asio::io_service::work> m_io_service_work;
    asio::ip::tcp::endpoint m_endpoint;
    asio::ip::tcp::socket m_socket;
    asio::ip::tcp::acceptor m_acceptor;
    std::thread m_io_service_thread;

    std::vector<std::unique_ptr<Remote_Viewer>> m_viewers;
};

}
