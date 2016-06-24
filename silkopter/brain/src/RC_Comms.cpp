#include "BrainStdAfx.h"
#include "RC_Comms.h"
#include "utils/Timed_Scope.h"

//#include "common/stream/IAcceleration.h"
//#include "common/stream/IAngular_Velocity.h"
//#include "common/stream/IADC.h"
//#include "common/stream/IFloat.h"
//#include "common/stream/IBool.h"
//#include "common/stream/IBattery_State.h"
//#include "common/stream/ICurrent.h"
//#include "common/stream/IDistance.h"
//#include "common/stream/IPosition.h"
//#include "common/stream/ILinear_Acceleration.h"
//#include "common/stream/IMagnetic_Field.h"
//#include "common/stream/IPressure.h"
//#include "common/stream/IPWM.h"
//#include "common/stream/IFrame.h"
//#include "common/stream/IGPS_Info.h"
//#include "common/stream/ITemperature.h"
//#include "common/stream/IVideo.h"
//#include "common/stream/IForce.h"
//#include "common/stream/IVelocity.h"
//#include "common/stream/IThrottle.h"
//#include "common/stream/ITorque.h"
//#include "common/stream/IVoltage.h"
//#include "common/stream/IProximity.h"
//#include "common/stream/IMultirotor_State.h"
//#include "common/stream/IMultirotor_Commands.h"

//#include "common/node/IBrain.h"

#include "utils/RCP.h"
#include "utils/RCP_UDP_Socket.h"
#include "utils/RCP_RFMON_Socket.h"
#include "utils/Channel.h"

//#include "hal.def.h"

//#include <boost/asio.hpp>

using namespace silk;

constexpr uint8_t PILOT_CHANNEL = 15;
constexpr uint8_t VIDEO_CHANNEL = 16;
constexpr uint8_t TELEMETRY_CHANNEL = 20;

constexpr q::Clock::duration RCP_PERIOD = std::chrono::milliseconds(30);

struct RC_Comms::Channels
{
    typedef util::Channel<rc_comms::Pilot_Message, uint32_t> Pilot;
    typedef util::Channel<rc_comms::Video_Message, uint32_t> Video;
    typedef util::Channel<rc_comms::Telemetry_Message, uint32_t> Telemetry;

    Channels()
        : pilot(PILOT_CHANNEL)
        , telemetry(TELEMETRY_CHANNEL)
        , video(VIDEO_CHANNEL)
    {}

    Pilot pilot;
    Telemetry telemetry;
    Video video;
};

RC_Comms::RC_Comms(HAL& hal)
    : m_hal(hal)
    , m_channels(new Channels())
{
}

auto RC_Comms::start_udp(uint16_t send_port, uint16_t receive_port) -> bool
{
    try
    {
        auto s = new util::RCP_UDP_Socket();
        m_socket.reset(s);
        m_rcp.reset(new util::RCP());

        util::RCP::Socket_Handle handle = m_rcp->add_socket(m_socket.get());
        if (handle >= 0)
        {
            m_rcp->set_internal_socket_handle(handle);
            m_rcp->set_socket_handle(PILOT_CHANNEL, handle);
            m_rcp->set_socket_handle(VIDEO_CHANNEL, handle);
            m_rcp->set_socket_handle(TELEMETRY_CHANNEL, handle);

            s->open(send_port, receive_port);
            s->start_listening();
//            s->set_send_endpoint(boost::asio::ip::address::from_string("127.0.0.1"), send_port);
            m_is_connected = true;
        }
    }
    catch(std::exception e)
    {
        m_is_connected = false;
        return false;
    }

    if (!m_is_connected)
    {
        m_socket.reset();
        m_rcp.reset();
        QLOGW("Cannot start comms on ports s:{} r:{}", send_port, receive_port);
        return false;
    }

    QLOGI("Started sending on ports s:{} r:{}", send_port, receive_port);

    configure_channels();

    return true;
}

auto RC_Comms::start_rfmon(std::string const& interface, uint8_t id) -> bool
{
    try
    {
        auto s = new util::RCP_RFMON_Socket(interface, id);
        m_socket.reset(s);
        m_rcp.reset(new util::RCP);

        util::RCP::Socket_Handle handle = m_rcp->add_socket(m_socket.get());
        if (handle >= 0)
        {
            m_rcp->set_internal_socket_handle(handle);
            m_rcp->set_socket_handle(PILOT_CHANNEL, handle);
            m_rcp->set_socket_handle(VIDEO_CHANNEL, handle);
            m_rcp->set_socket_handle(TELEMETRY_CHANNEL, handle);

            m_is_connected = s->start();
        }
    }
    catch(std::exception e)
    {
        m_is_connected = false;
    }

    if (!m_is_connected)
    {
        m_socket.reset();
        m_rcp.reset();
        QLOGW("Cannot start comms on interface {}", interface);
        return false;
    }

    QLOGI("Started sending on interface {}", interface);
    configure_channels();

    return true;
}

void RC_Comms::configure_channels()
{
    {
        util::RCP::Send_Params params;
        params.is_compressed = true;
        params.is_reliable = true;
        params.importance = 10;
        params.cancel_after = std::chrono::milliseconds(150);
        m_rcp->set_send_params(TELEMETRY_CHANNEL, params);
    }
    {
        util::RCP::Send_Params params;
        params.is_compressed = true;
        params.is_reliable = true;
        params.importance = 100;
        params.cancel_previous_data = true;
        params.cancel_after = std::chrono::milliseconds(150);
        m_rcp->set_send_params(PILOT_CHANNEL, params);
    }
    {
        util::RCP::Send_Params params;
        params.is_compressed = false;
        params.is_reliable = true;
        params.importance = 100;
        params.cancel_after = std::chrono::milliseconds(150);
        m_rcp->set_send_params(VIDEO_CHANNEL, params);
    }

    {
        util::RCP::Receive_Params params;
        params.max_receive_time = std::chrono::milliseconds(100);
        m_rcp->set_receive_params(PILOT_CHANNEL, params);
    }
    {
        util::RCP::Receive_Params params;
        params.max_receive_time = std::chrono::milliseconds(500);
        m_rcp->set_receive_params(TELEMETRY_CHANNEL, params);
    }
}

auto RC_Comms::is_connected() const -> bool
{
    return m_is_connected;
}

//auto Comms::send_video_stream(Stream_Telemetry_Data& ts, stream::IStream const& _stream) -> bool
//{
//    if (_stream.get_type() != stream::IVideo::TYPE)
//    {
//        return false;
//    }

//    auto const& stream = static_cast<stream::IVideo const&>(_stream);
//    auto const& samples = stream.get_samples();

//    for (auto const& s: samples)
//    {
//        m_channels->video.begin_pack(comms::Video_Message::FRAME_DATA);
//        m_channels->video.pack_param(ts.stream_name);
//        m_channels->video.pack_param(s);
//        m_channels->video.end_pack();
//        m_channels->video.try_sending(*m_rcp);
//    }
//    return true;
//}

auto RC_Comms::get_multirotor_commands_values() const -> std::vector<stream::IMultirotor_Commands::Value> const&
{
    return m_multirotor_commands_values;
}
void RC_Comms::add_multirotor_state_sample(stream::IMultirotor_State::Sample const& sample)
{
    m_channels->pilot.pack_all(silk::rc_comms::Pilot_Message::MULTIROTOR_STATE, sample);
    m_channels->pilot.send(*m_rcp);
}
void RC_Comms::add_video_sample(stream::IVideo::Sample const& sample)
{
    m_channels->video.pack_all(silk::rc_comms::Video_Message::FRAME_DATA, sample);
    m_channels->video.send(*m_rcp);
}

//void Comms::handle_multirotor_commands()
//{
//    auto& channel = m_channels->pilot;

//    stream::IMultirotor_Commands::Value value;
//    if (!channel.unpack_all(value))
//    {
//        QLOGE("Error in unpacking multirotor commands");
//        return;
//    }

//    m_multirotor_commands_values.push_back(value);
//}


void RC_Comms::process()
{
    if (!is_connected())
    {
        return;
    }

//    m_multirotor_commands_values.clear();

//    while (auto msg = m_channels->pilot.get_next_message(*m_rcp))
//    {
//        switch (msg.get())
//        {
//        case comms::Pilot_Message::MULTIROTOR_COMMANDS: handle_multirotor_commands(); break;

//        default: QLOGE("Received unrecognised pilot message: {}", static_cast<int>(msg.get())); break;
//        }
//    }


    auto result = m_socket->process();
    if (result != util::RCP_Socket::Result::OK)
    {
        m_rcp->reconnect();
    }

    m_rcp->process();

    auto now = q::Clock::now();
    if (now - m_last_rcp_tp >= RCP_PERIOD)
    {
        m_last_rcp_tp = now;

        //m_channels->setup.send(*m_rcp);
        m_channels->telemetry.try_sending(*m_rcp);
    }

//    static std::vector<uint8_t> buf;
//    if (buf.empty())
//    {
//        buf.resize(16000000);
//        std::generate(buf.begin(), buf.end(), [](){ return rand() % 50;});
//    }

//    while(true)
//    {
//        m_rcp.send(12, buf.data(), buf.size());

//        m_rcp.process();
//        //m_channel.send(COMMS_CHANNEL);
//        static int xxx = 0;
//        LOG_INFO("{}", xxx);
//        xxx++;
//        std::this_thread::sleep_for(std::chrono::milliseconds(30));
//    }
}

