#pragma once

#include <memory>
#include <boost/asio.hpp>
#include "common/stream/IMultirotor_State.h"
#include "utils/comms/Channel.h"
#include "utils/comms/ASIO_Socket_Adapter.h"

namespace silk
{

class Remote_Viewer_Server;

class Remote_Viewer
{
    friend class Remote_Viewer_Server;
public:
    ~Remote_Viewer();

protected:
    Remote_Viewer(boost::asio::ip::tcp::socket socket, bool master);

    void start();
    bool is_alive() const;

    void send_data(void const* video_data, size_t video_data_size, math::vec2u16 const& resolution,
                   stream::IMultirotor_State::Value const& multirotor_state);

    void process();

private:
    boost::asio::ip::tcp::socket m_socket;
    bool m_is_master;

    enum class Message
    {
        RESOLUTION,
        VIDEO_DATA,
        MULTIROTOR_STATE
    };

    typedef util::comms::ASIO_Socket_Adapter<boost::asio::ip::tcp::socket> Socket_Adapter;
    Socket_Adapter m_socket_adapter;
    util::comms::Channel<Message, Socket_Adapter> m_channel;

    std::vector<uint8_t> m_serialization_buffer;
};

}
