#pragma once

#include "common/Manual_Clock.h"
#include "common/Comm_Data.h"
#include "utils/PID.h"

#include "common/node/stream/IAcceleration.h"
#include "common/node/stream/IAngular_Velocity.h"
#include "common/node/stream/IADC_Value.h"
#include "common/node/stream/IBattery_State.h"
#include "common/node/stream/ICardinal_Points.h"
#include "common/node/stream/ICurrent.h"
#include "common/node/stream/IDistance.h"
#include "common/node/stream/ILinear_Acceleration.h"
#include "common/node/stream/ILocation.h"
#include "common/node/stream/IMagnetic_Field.h"
#include "common/node/stream/IPressure.h"
#include "common/node/stream/IPWM_Value.h"
#include "common/node/stream/IReference_Frame.h"
#include "common/node/stream/ITemperature.h"
#include "common/node/stream/IVideo.h"
#include "common/node/stream/IVoltage.h"

#include "common/node/processor/IMultirotor.h"

namespace silk
{

class Comms : q::util::Noncopyable
{
public:
    Comms(boost::asio::io_service& io_service);

    auto start(boost::asio::ip::address const& address, uint16_t send_port, uint16_t receive_port) -> bool;
    void disconnect();

    auto is_connected() const -> bool;
    auto get_remote_address() const -> boost::asio::ip::address;

    auto get_remote_clock() const -> Manual_Clock const&;
    auto get_rudp() -> util::RUDP&;

    void process();

    q::util::Signal<void(node::stream::IAcceleration::Sample const&)> acceleration_received_signal;

    //----------------------------------------------------------------------

private:
    boost::asio::io_service& m_io_service;
    boost::asio::ip::udp::socket m_socket;
    boost::asio::ip::udp::endpoint m_remote_endpoint;

    util::RUDP m_rudp;

    typedef util::Channel<comms::Setup_Message, uint16_t> Setup_Channel;
    typedef util::Channel<comms::Input_Message, uint16_t> Input_Channel;
    typedef util::Channel<uint32_t, uint16_t> Telemetry_Channel;
    mutable Setup_Channel m_setup_channel;
    mutable Input_Channel m_input_channel;
    mutable Telemetry_Channel m_telemetry_channel;

    Manual_Clock m_remote_clock;
};

}
