#include "BrainStdAfx.h"
#include "Video_Server.h"
#include "utils/RUDP.h"

constexpr uint8_t VIDEO_CHANNEL = 4;

using namespace silk;
using namespace boost::asio;

Video_Server::Video_Server(util::RUDP& rudp)
    : m_rudp(rudp)
{
    util::RUDP::Send_Params sparams;
    sparams.is_compressed = true;
    sparams.is_reliable = false;
    sparams.importance = 0;
    sparams.cancel_on_new_data = true;
    m_rudp.set_send_params(VIDEO_CHANNEL, sparams);

    SILK_INFO("Video server created");
}

auto Video_Server::send_frame(Flags flags, uint8_t const* data, size_t size) -> bool
{
    if (size == 0 || !data)
    {
        return true;
    }
//    if (!m_socket)
//    {
//        SILK_WARNING("Cannot send frame as the streamer is not connected");
//        return false;
//    }

    return m_rudp.try_sending(VIDEO_CHANNEL, data, size);
}
