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
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Remote_Viewer::send_data(uint8_t const* video_data, size_t video_data_size, math::vec2u16 const& resolution,
               stream::IMultirotor_State::Value const& multirotor_state)
{
    m_socket_adapter.write(video_data, video_data_size);
//    m_channel.send(Message::RESOLUTION, &resolution, sizeof(resolution));
//    m_channel.send(Message::VIDEO_DATA, video_data, video_data_size);

//    size_t off = 0;
//    util::serialization::serialize(m_serialization_buffer, multirotor_state, off);

//    m_channel.send(Message::MULTIROTOR_STATE, m_serialization_buffer.data(), m_serialization_buffer.size());
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Remote_Viewer::process()
{
    m_channel.process();
}

///////////////////////////////////////////////////////////////////////////////////////////////////


}
