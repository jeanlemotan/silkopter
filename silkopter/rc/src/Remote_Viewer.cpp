#include "Remote_Viewer.h"

namespace silk
{

///////////////////////////////////////////////////////////////////////////////////////////////////

Remote_Viewer::Remote_Viewer(boost::asio::ip::tcp::socket socket, bool master)
    : m_socket(std::move(socket))
    , m_is_master(master)
    , m_socket_adapter(m_socket)
    , m_channel(m_socket_adapter)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Remote_Viewer::~Remote_Viewer()
{
    m_socket.close();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Remote_Viewer::start()
{
    m_socket_adapter.start();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool Remote_Viewer::is_alive() const
{
    return m_socket.is_open();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Remote_Viewer::send_data(void const* video_data, size_t video_data_size, math::vec2u16 const& resolution,
               stream::IMultirotor_State::Value const& multirotor_state)
{
    {
        size_t offset = 0;
        util::serialization::serialize(m_serialization_buffer, resolution, offset);
        m_serialization_buffer.resize(offset + video_data_size);
        memcpy(m_serialization_buffer.data() + offset, video_data, video_data_size);
        m_channel.send(viewer::Packet_Type::VIDEO_DATA, m_serialization_buffer.data(), m_serialization_buffer.size());
    }

    {
        size_t offset = 0;
        util::serialization::serialize(m_serialization_buffer, multirotor_state, offset);
        m_channel.send(viewer::Packet_Type::MULTIROTOR_STATE, m_serialization_buffer.data(), m_serialization_buffer.size());
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Remote_Viewer::process()
{
    m_channel.process();
}

///////////////////////////////////////////////////////////////////////////////////////////////////


}
