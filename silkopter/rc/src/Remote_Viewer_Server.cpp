#include "Remote_Viewer_Server.h"
#include "Remote_Viewer.h"

namespace silk
{

const uint16_t k_port = 3333;

///////////////////////////////////////////////////////////////////////////////////////////////////

Remote_Viewer_Server::Remote_Viewer_Server()
    : m_io_service()
    , m_io_service_work(new asio::io_service::work(m_io_service))
    , m_endpoint(asio::ip::tcp::v4(), k_port)
    , m_socket(m_io_service)
    , m_acceptor(m_io_service, m_endpoint)
{
    m_io_service_thread = std::thread([this]()
    {
       m_io_service.run();
    });

    start_accept();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Remote_Viewer_Server::~Remote_Viewer_Server()
{
    m_io_service_work.reset();
    m_io_service.stop();
    m_io_service_thread.join();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

size_t Remote_Viewer_Server::get_remote_viewer_count() const
{
    return m_viewers.size();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Remote_Viewer_Server::send_video_data(void const* video_data, size_t video_data_size, math::vec2u16 const& resolution)
{
    for (std::unique_ptr<Remote_Viewer>& viewer: m_viewers)
    {
        viewer->send_video_data(video_data, video_data_size, resolution);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Remote_Viewer_Server::send_telemetry(stream::IMultirotor_Commands::Value const& multirotor_commands, stream::IMultirotor_State::Value const& multirotor_state)
{
    for (std::unique_ptr<Remote_Viewer>& viewer: m_viewers)
    {
        viewer->send_telemetry(multirotor_commands, multirotor_state);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Remote_Viewer_Server::start_accept()
{
    m_acceptor.async_accept(m_socket, [this](asio::error_code ec)
    {
        if (!ec)
        {
            QLOGI("Remote Viewer connected");
            bool master = m_viewers.empty();
            std::unique_ptr<Remote_Viewer> viewer(new Remote_Viewer(std::move(m_socket), master));
            viewer->start();
            m_viewers.push_back(std::move(viewer));
        }
        else
        {
            QLOGI("Remote Viewer failed to connect");
        }
        start_accept();
    });
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Remote_Viewer_Server::process()
{
    for (auto it = m_viewers.begin(); it != m_viewers.end();)
    {
        Remote_Viewer& viewer = *(*it);
        if (viewer.is_alive())
        {
            viewer.process();
            ++it;
        }
        else
        {
            QLOGI("Remote Viewer disconnected");
            m_viewers.erase(it);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////


}
