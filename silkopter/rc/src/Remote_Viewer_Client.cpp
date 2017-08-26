#include "Remote_Viewer_Client.h"
#include "Remote_Viewer.h"

namespace silk
{

const uint16_t k_port = 3333;

///////////////////////////////////////////////////////////////////////////////////////////////////

Remote_Viewer_Client::Remote_Viewer_Client()
    : m_io_service()
    , m_io_service_work(new boost::asio::io_service::work(m_io_service))
    , m_channel(m_socket_adapter)
{
    m_io_service_thread = boost::thread([this]()
    {
       m_io_service.run();
    });

    start_connect();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Remote_Viewer_Client::~Remote_Viewer_Client()
{
    m_io_service_work.reset();
    m_io_service.stop();
    m_io_service_thread.join();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool Remote_Viewer_Client::is_connected() const
{
    return m_socket && m_socket->is_open() && m_is_connected;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Remote_Viewer_Client::send_video_data(void const* video_data, size_t video_data_size, math::vec2u16 const& resolution)
{
    if (is_connected())
    {
        if (video_data && video_data_size)
        {
            m_serialization_buffer.clear();
            size_t offset = 0;
            util::serialization::serialize(m_serialization_buffer, resolution, offset);
            m_serialization_buffer.resize(offset + video_data_size);
            memcpy(m_serialization_buffer.data() + offset, video_data, video_data_size);
            m_channel.send(viewer::Packet_Type::VIDEO_DATA, m_serialization_buffer.data(), m_serialization_buffer.size());
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Remote_Viewer_Client::send_telemetry(stream::IMultirotor_Commands::Value const& multirotor_commands, stream::IMultirotor_State::Value const& multirotor_state)
{
    if (is_connected())
    {
        m_serialization_buffer.clear();
        size_t offset = 0;
        util::serialization::serialize(m_serialization_buffer, multirotor_state, offset);
        util::serialization::serialize(m_serialization_buffer, multirotor_commands, offset);
        m_channel.send(viewer::Packet_Type::TELEMETRY, m_serialization_buffer.data(), offset);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Remote_Viewer_Client::start_connect()
{
    m_last_connection_attempt_tp = Clock::now();

    try
    {
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("192.168.42.129"), k_port);

        if (m_socket)
        {
            m_socket_adapter.stop();
            m_socket->close();
        }

        m_socket.reset(new boost::asio::ip::tcp::socket(m_io_service));
        m_socket->async_connect(endpoint, [this](boost::system::error_code ec)
        {
            try
            {
                if (!ec)
                {
                    QLOGI("Remote Viewer connected");
                    m_socket_adapter.start(*m_socket);
                    m_is_connected = true;
                }
                else
                {
                    QLOGI("Remote Viewer failed to connect. Retrying");
                    if (m_socket)
                    {
                        m_socket_adapter.stop();
                        m_socket->close();
                    }
                }
            }
            catch (std::exception const& e)
            {
                QLOGE("Exception thrown: {}", e.what());
            }
        });
    }
    catch (std::exception const& e)
    {
        QLOGE("Exception thrown: {}", e.what());
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Remote_Viewer_Client::process()
{
    if (!is_connected() && Clock::now() - m_last_connection_attempt_tp >= std::chrono::milliseconds(100))
    {
        start_connect();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////


}
