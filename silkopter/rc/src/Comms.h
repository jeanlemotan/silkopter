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

#include "utils/RCP.h"
#include "utils/Channel.h"
#include "common/Manual_Clock.h"

#include "common/node/INode.h"
#include "common/stream/IVideo.h"
#include "common/stream/IMultirotor_State.h"

#include "common/Comm_Data.h"

#include <boost/asio.hpp>

namespace silk
{

class Comms : q::util::Noncopyable
{
public:
    Comms();

    auto start_udp(boost::asio::ip::address const& address, uint16_t send_port, uint16_t receive_port) -> bool;
    auto start_rfmon(std::string const& interface, uint8_t id) -> bool;

    void disconnect();
    auto is_connected() const -> bool;

    //----------------------------------------------------------------------

    typedef util::Channel<uint32_t> Pilot_Channel;
    typedef util::Channel<uint32_t> Video_Channel;
    typedef util::Channel<uint32_t> Telemetry_Channel;

    auto get_video_samples() const -> std::vector<stream::IVideo::Sample> const&;
    auto get_multirotor_state_samples() const -> std::vector<stream::IMultirotor_State::Sample> const&;
    void send_multirotor_commands_value(stream::IMultirotor_Commands::Value const& value);

    void process_rcp();
    void process();

private:
    void configure_channels();

    void reset();

    std::shared_ptr<util::RCP_Socket> m_socket;
    std::shared_ptr<util::RCP> m_rcp;

    uint32_t m_last_req_id = 0;

    //mutable Setup_Channel m_setup_channel;
    mutable Pilot_Channel m_pilot_channel;
    mutable Video_Channel m_video_channel;
    mutable Telemetry_Channel m_telemetry_channel;

    std::vector<stream::IVideo::Sample> m_video_samples;
    std::vector<stream::IMultirotor_State::Sample> m_multirotor_state_samples;

    void handle_multirotor_state();
    void handle_video();
};

}
