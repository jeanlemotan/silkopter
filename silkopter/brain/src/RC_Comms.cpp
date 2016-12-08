#include "BrainStdAfx.h"
#include "RC_Comms.h"
#include "utils/Timed_Scope.h"

#include "utils/comms/RC.h"
#include "utils/comms/Video_Streamer.h"

namespace silk
{

constexpr uint8_t SDN_GPIO = 6;
constexpr uint8_t NIRQ_GPIO = 5;
constexpr char const* SPI_DEVICE = "/dev/spidev1.0";
constexpr size_t SPEED = 2000000;


struct RC_Comms::Impl
{
    Impl()
        : rc(false)
        , video_streamer()
    {}

    util::comms::RC rc;
    util::comms::Video_Streamer video_streamer;

    std::vector<uint8_t> serialization_buffer;
    util::comms::RC::Data rc_rx_data;
};

RC_Comms::RC_Comms(HAL& hal)
    : m_hal(hal)
    , m_impl(new Impl())
{
}

auto RC_Comms::start(std::string const& interface, uint8_t id) -> bool
{
    try
    {
//        m_rcp.reset(new util::comms::RCP);

//        {
//            auto s = new util::comms::RFMON_Socket(interface, id);
//            m_video_socket.reset(s);
//            if (!s->start())
//            {
//                throw std::exception();
//            }

//            util::RCP::Socket_Handle handle = m_rcp->add_socket(m_video_socket.get());
//            if (handle >= 0)
//            {
//                m_rcp->set_socket_handle(VIDEO_CHANNEL, handle);
//                m_rcp->set_socket_handle(TELEMETRY_CHANNEL, handle);
//            }
//        }

//        {
//            auto s = new util::RCP_RF4463F30_Socket("/dev/spidev1.0", 16000000, false);
//            m_rc_socket.reset(s);
//            if (!s->start())
//            {
//                throw std::exception();
//            }

//            util::RCP::Socket_Handle handle = m_rcp->add_socket(m_rc_socket.get());
//            if (handle >= 0)
//            {
//                m_rcp->set_internal_socket_handle(handle);
//                m_rcp->set_socket_handle(PILOT_CHANNEL, handle);
//            }
//        }

        util::comms::Video_Streamer::TX_Descriptor descriptor;
        descriptor.interface = "wlan1";
        descriptor.coding_k = 12;
        descriptor.coding_n = 20;

        m_is_connected = m_impl->rc.init(SPI_DEVICE, SPEED, SDN_GPIO, NIRQ_GPIO) && m_impl->video_streamer.init_tx(descriptor);
    }
    catch(std::exception e)
    {
        m_is_connected = false;
    }

    if (!m_is_connected)
    {
        QLOGW("Cannot start comms on interface {}", interface);
        return false;
    }

    QLOGI("Started sending on interface {}", interface);

    return true;
}

auto RC_Comms::is_connected() const -> bool
{
    return m_is_connected;
}

auto RC_Comms::get_multirotor_commands_values() const -> std::vector<stream::IMultirotor_Commands::Value> const&
{
    return m_multirotor_commands_values;
}
void RC_Comms::add_multirotor_state_sample(stream::IMultirotor_State::Sample const& sample)
{
//    m_channels->pilot.pack_all(silk::rc_comms::Pilot_Message::MULTIROTOR_STATE, sample);
//    m_channels->pilot.send(*m_rcp);
}
void RC_Comms::add_video_sample(stream::IVideo::Sample const& sample)
{
    //size_t off = 0;
    //util::serialization::serialize(m_impl->serialization_buffer, sample, off);
    m_impl->video_streamer.send(sample.value.data.data(), sample.value.data.size(), sample.value.resolution);
}

void RC_Comms::handle_multirotor_commands()
{
//    auto& channel = m_channels->pilot;

//    stream::IMultirotor_Commands::Value value;
//    if (!channel.unpack_param(value))
//    {
//        QLOGE("Error in unpacking multirotor commands");
//        return;
//    }

//    m_multirotor_commands_values.push_back(value);
}


void RC_Comms::process()
{
    if (!is_connected())
    {
        return;
    }

    m_multirotor_commands_values.clear();

    {
        silk::stream::IMultirotor_Commands::Value commands;

        util::comms::RC::Data& rc_rx_data = m_impl->rc_rx_data;
        m_impl->rc.get_rx_data(rc_rx_data);

        size_t off = 0;
        if (util::serialization::deserialize(rc_rx_data.rx_data, commands, off))
        {
            m_multirotor_commands_values.push_back(commands);
            QLOGI("^{}dBm, v{}dBm, throttle: {}", rc_rx_data.tx_dBm, rc_rx_data.tx_dBm, commands.sticks.pitch);
        }
        else
        {
            QLOGW("error deserializing commands");
        }
    }

//    while (m_channels->pilot.get_next_message(*m_rcp))
//    {
//        rc_comms::Pilot_Message msg;
//        if (!m_channels->pilot.begin_unpack() || !m_channels->pilot.unpack_param(msg))
//        {
//            QASSERT(0);
//            m_channels->pilot.end_unpack();
//            break;
//        }

//        switch (msg)
//        {
//        case rc_comms::Pilot_Message::MULTIROTOR_COMMANDS: handle_multirotor_commands(); break;
//        default: QLOGE("Received unrecognised pilot message: {}", static_cast<int>(msg)); break;
//        }
//    }


//    if (m_video_socket)
//    {
//        auto result = m_video_socket->process();
//        if (result != util::comms::ISocket::Result::OK)
//        {
////            m_rcp->reconnect();
//        }
//    }

//    if (m_rc_socket)
//    {
//        auto result = m_rc_socket->process();
//        if (result != util::comms::ISocket::Result::OK)
//        {
//            m_rcp->reconnect();
//        }
//    }

//    m_rcp->process();

//    auto now = q::Clock::now();
//    if (now - m_last_rcp_tp >= RCP_PERIOD)
//    {
//        m_last_rcp_tp = now;

//        //m_channels->setup.send(*m_rcp);
//        m_channels->telemetry.try_sending(*m_rcp);
//    }
}

}
