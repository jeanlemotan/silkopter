#include "Comms.h"

#include "common/node/INode.h"
#include "common/node/IPilot.h"

#include "common/Comm_Data.h"

#include "utils/RCP_UDP_Socket.h"
#include "utils/RCP_RFMON_Socket.h"
#include "utils/RCP_RF4463F30_Socket.h"

using namespace silk;
//using namespace boost::asio;

constexpr uint8_t PILOT_CHANNEL = 15;
constexpr uint8_t VIDEO_CHANNEL = 16;
constexpr uint8_t TELEMETRY_CHANNEL = 20;


Comms::Comms()
    : m_pilot_channel(PILOT_CHANNEL)
    , m_video_channel(VIDEO_CHANNEL)
    , m_telemetry_channel(TELEMETRY_CHANNEL)
{
}

auto Comms::start_udp(boost::asio::ip::address const& address, uint16_t send_port, uint16_t receive_port) -> bool
{
    try
    {
        auto s = new util::RCP_UDP_Socket();
        m_rc_socket.reset(s);
        m_rcp.reset(new util::RCP());

        util::RCP::Socket_Handle handle = m_rcp->add_socket(m_rc_socket.get());
        if (handle < 0)
        {
            QASSERT(0);
            throw std::exception();
        }

        m_rcp->set_internal_socket_handle(handle);
        m_rcp->set_socket_handle(PILOT_CHANNEL, handle);
        m_rcp->set_socket_handle(VIDEO_CHANNEL, handle);
        m_rcp->set_socket_handle(TELEMETRY_CHANNEL, handle);

        s->open(send_port, receive_port);
        s->set_send_endpoint(address, send_port);
        s->start_listening();

        QLOGI("Started sending on port {} and receiving on port {}", send_port, receive_port);
    }
    catch(...)
    {
        m_rc_socket.reset();
        m_rcp.reset();
        QLOGW("Connect failed");
        return false;
    }

    configure_channels();
    reset();

    return true;
}

auto Comms::start_rfmon(std::string const& interface, uint8_t id) -> bool
{
    bool is_connected = false;
    try
    {
        m_rcp.reset(new util::RCP);

        {
            auto s = new util::RCP_RFMON_Socket(interface, id);
            m_video_socket.reset(s);
            if (!s->start())
            {
                throw std::exception();
            }

            util::RCP::Socket_Handle handle = m_rcp->add_socket(m_video_socket.get());
            if (handle >= 0)
            {
                m_rcp->set_socket_handle(VIDEO_CHANNEL, handle);
                m_rcp->set_socket_handle(TELEMETRY_CHANNEL, handle);
            }
        }

        {
            auto s = new util::RCP_RF4463F30_Socket("/dev/spidev1.0", 10000000, true);
            m_rc_socket.reset(s);
            if (!s->start())
            {
                throw std::exception();
            }

            util::RCP::Socket_Handle handle = m_rcp->add_socket(m_rc_socket.get());
            if (handle >= 0)
            {
                m_rcp->set_internal_socket_handle(handle);
                m_rcp->set_socket_handle(PILOT_CHANNEL, handle);
            }
        }

        is_connected = true;
    }
    catch(std::exception e)
    {
        is_connected = false;
    }

    if (!is_connected)
    {
        m_video_socket.reset();
        m_rc_socket.reset();
        m_rcp.reset();
        QLOGW("Cannot start comms on interface {}", interface);
        return false;
    }

    QLOGI("Started sending on interface {}", interface);
    configure_channels();

    return true;
}

void Comms::disconnect()
{
    reset();
    m_rc_socket.reset();
    m_video_socket.reset();
}

auto Comms::is_connected() const -> bool
{
    return m_rcp != nullptr && m_rcp->is_connected();
}


void Comms::configure_channels()
{
    {
        util::RCP::Send_Params params;
        params.is_compressed = true;
        params.is_reliable = false;
        params.importance = 10;
        m_rcp->set_send_params(TELEMETRY_CHANNEL, params);
    }

    {
        util::RCP::Send_Params params;
        params.is_compressed = true;
        params.is_reliable = true;
        params.importance = 100;
        params.cancel_after = std::chrono::milliseconds(100);
        m_rcp->set_send_params(PILOT_CHANNEL, params);
    }

    {
        util::RCP::Receive_Params params;
        params.max_receive_time = std::chrono::milliseconds(300);
        m_rcp->set_receive_params(PILOT_CHANNEL, params);
    }
    {
        util::RCP::Receive_Params params;
        params.max_receive_time = std::chrono::milliseconds(300);
        m_rcp->set_receive_params(VIDEO_CHANNEL, params);
    }

    {
        util::RCP::Receive_Params params;
        params.max_receive_time = std::chrono::milliseconds(500);
        m_rcp->set_receive_params(TELEMETRY_CHANNEL, params);
    }

}

void Comms::reset()
{
    m_last_req_id = 0;
}

void Comms::handle_multirotor_state()
{
    auto& channel = m_pilot_channel;

    stream::IMultirotor_State::Sample sample;
    if (!channel.unpack_param(sample))
    {
        QLOGE("Error in unpacking multirotor state");
        return;
    }

    std::lock_guard<std::mutex> lg(m_samples_mutex);
    m_multirotor_state_samples.push_back(sample);
}
void Comms::handle_video()
{
    auto& channel = m_video_channel;

    stream::IVideo::Sample sample;
    if (!channel.unpack_param(sample))
    {
        QLOGE("Error in unpacking video");
        return;
    }

    std::lock_guard<std::mutex> lg(m_samples_mutex);
    m_video_samples.push_back(sample);
}

auto Comms::get_video_samples() -> std::vector<stream::IVideo::Sample>
{
    std::lock_guard<std::mutex> lg(m_samples_mutex);
    std::vector<stream::IVideo::Sample> samples = std::move(m_video_samples);
    m_video_samples.clear();
    return samples;
}
auto Comms::get_multirotor_state_samples() -> std::vector<stream::IMultirotor_State::Sample>
{
    std::lock_guard<std::mutex> lg(m_samples_mutex);
    std::vector<stream::IMultirotor_State::Sample> samples = std::move(m_multirotor_state_samples);
    m_multirotor_state_samples.clear();
    return samples;
}
void Comms::send_multirotor_commands_value(stream::IMultirotor_Commands::Value const& value)
{
    m_pilot_channel.pack_all(silk::rc_comms::Pilot_Message::MULTIROTOR_COMMANDS, value);
    m_pilot_channel.try_sending(*m_rcp);
}

void Comms::process_rcp()
{
    if (!m_rcp)
    {
        return;
    }

    if (m_video_socket)
    {
        auto result = m_video_socket->process();
        if (result != util::RCP_Socket::Result::OK)
        {
//            m_rcp->reconnect();
        }
    }

    if (m_rc_socket)
    {
        auto result = m_rc_socket->process();
        if (result != util::RCP_Socket::Result::OK)
        {
            m_rcp->reconnect();
        }
    }

    m_rcp->process();
}

void Comms::process()
{
    if (!m_rcp)
    {
        return;
    }

//    {
//        std::lock_guard<std::mutex> lg(m_samples_mutex);
//        m_multirotor_state_samples.clear();
//        m_video_samples.clear();
//    }

    while (m_pilot_channel.get_next_message(*m_rcp))
    {
        rc_comms::Pilot_Message msg;
        if (!m_pilot_channel.begin_unpack() || !m_pilot_channel.unpack_param(msg))
        {
            QASSERT(0);
            m_pilot_channel.end_unpack();
            break;
        }

        switch (msg)
        {
        case rc_comms::Pilot_Message::MULTIROTOR_STATE : handle_multirotor_state(); break;
        default: break;
        }

        m_pilot_channel.end_unpack();
    }
    while (m_video_channel.get_next_message(*m_rcp))
    {
        rc_comms::Video_Message msg;
        if (!m_video_channel.begin_unpack() || !m_video_channel.unpack_param(msg))
        {
            QASSERT(0);
            m_video_channel.end_unpack();
            break;
        }

        switch (msg)
        {
        case rc_comms::Video_Message::FRAME_DATA : handle_video(); break;
        default: break;
        }
    }

    auto start = q::Clock::now();
    while (m_telemetry_channel.get_next_message(*m_rcp))
    {
        //process only the first 10 ms worh of data and discard the rest
        if (q::Clock::now() - start < std::chrono::milliseconds(100))
        {
//            switch (msg.get())
//            {
////            case comms::Telemetry_Message::STREAM_DATA : handle_stream_data(); break;
//            default: break;
//            }
        }
    }

    m_telemetry_channel.try_sending(*m_rcp);
}

