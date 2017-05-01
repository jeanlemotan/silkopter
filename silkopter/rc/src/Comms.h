#pragma once

#include "common/Comm_Data.h"

#include "common/stream/IAcceleration.h"
#include "common/stream/IADC.h"
#include "common/stream/IFloat.h"
#include "common/stream/IBool.h"
#include "common/stream/IAngular_Velocity.h"
#include "common/stream/IBattery_State.h"
#include "common/stream/ICurrent.h"
#include "common/stream/IDistance.h"
#include "common/stream/IForce.h"
#include "common/stream/IFrame.h"
#include "common/stream/IGPS_Info.h"
#include "common/stream/ILinear_Acceleration.h"
#include "common/stream/IPosition.h"
#include "common/stream/IMagnetic_Field.h"
#include "common/stream/IPressure.h"
#include "common/stream/IPWM.h"
#include "common/stream/ITemperature.h"
#include "common/stream/IThrottle.h"
#include "common/stream/ITorque.h"
#include "common/stream/IVelocity.h"
#include "common/stream/IVideo.h"
#include "common/stream/IVoltage.h"
#include "common/stream/IProximity.h"
#include "common/stream/IMultirotor_Commands.h"
#include "common/stream/IMultirotor_State.h"

#include "utils/Clock.h"

#include "common/node/INode.h"
#include "common/stream/IVideo.h"
#include "common/stream/IMultirotor_State.h"

#include "common/Comm_Data.h"
#include "utils/comms/RC_Phy.h"
#include "utils/comms/RC_Protocol.h"
#include "utils/comms/Video_Streamer.h"
#include "Remote_Viewer_Server.h"

#include <boost/asio.hpp>

namespace silk
{

class HAL;

class Comms : q::util::Noncopyable
{
public:
    Comms(HAL& hal);

    bool start();

    void disconnect();
    bool is_connected() const;

    //----------------------------------------------------------------------

    int8_t get_rx_dBm() const;
    int8_t get_tx_dBm() const;

    Clock::time_point get_last_rx_tp() const;

    Remote_Viewer_Server const& get_remote_viewer_server() const;
    Remote_Viewer_Server& get_remote_viewer_server();

    util::comms::Video_Streamer const& get_video_streamer() const;
    util::comms::Video_Streamer& get_video_streamer();

//    void get_video_data(std::vector<uint8_t>& dst, math::vec2u16& resolution);
    stream::IMultirotor_State::Value get_multirotor_state() const;

    void send_multirotor_commands_value(stream::IMultirotor_Commands::Value const& value);

    void process();

private:
    HAL& m_hal;

    void reset();

    bool compute_multirotor_commands_packet(uint8_t* data, size_t& size, uint8_t& packet_type);
    void process_rx_packet(util::comms::RC_Protocol::RX_Packet const& packet);

    Remote_Viewer_Server m_remote_viewer_server;

    util::comms::RC_Phy m_rc_phy;
    util::comms::RC_Protocol m_rc_protocol;
    util::comms::RC_Protocol::RX_Packet m_rx_packet;

    util::comms::Video_Streamer m_video_streamer;

    bool m_is_connected = false;

    uint32_t m_last_req_id = 0;

    mutable std::mutex m_samples_mutex;
    math::vec2u16 m_video_resolution;
    std::vector<uint8_t> m_video_data;
    std::vector<uint8_t> m_serialization_buffer;
    stream::IMultirotor_State::Value m_multirotor_state;
    stream::IMultirotor_Commands::Value m_multirotor_commands;

    //mark the commands as infinitely old
    Clock::time_point m_multirotor_commands_tp = Clock::time_point(Clock::duration::zero());

    void send_video_to_viewers(void const* data, size_t size, math::vec2u16 const& resolution);

    Clock::time_point m_telemetry_tp = Clock::time_point(Clock::duration::zero());
    void send_telemetry_to_viewers();
};

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

