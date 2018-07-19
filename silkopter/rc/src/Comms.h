#pragma once

#include <mutex>
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
#include "common/stream/ICamera_Commands.h"
#include "common/stream/IMultirotor_Commands.h"
#include "common/stream/IMultirotor_State.h"

#include "utils/Clock.h"

#include "common/node/INode.h"
#include "common/stream/IVideo.h"
#include "common/stream/IMultirotor_State.h"

#include "common/Comm_Data.h"
#include "utils/comms/esp32/Phy.h"
#include "utils/Pool.h"
#include "utils/Queue.h"

#include <asio.hpp>

namespace silk
{

class IHAL;

class Comms : q::util::Noncopyable
{
public:
    Comms(IHAL& hal);
    ~Comms();

    bool start();

    void disconnect();
    bool is_connected() const;

    //----------------------------------------------------------------------

    int8_t get_rx_dBm() const;
    int8_t get_tx_dBm() const;

    Phy const& get_phy() const;
    Phy& get_phy();

    Clock::time_point get_last_rx_tp() const;

    std::function<void(void const*, size_t, math::vec2u16 const&)> on_video_data_received;

    stream::IMultirotor_State::Value get_multirotor_state() const;

    void send_multirotor_commands_value(stream::IMultirotor_Commands::Value const& value);
    void send_camera_commands_value(stream::ICamera_Commands::Value const& value);

    void process();

private:
    IHAL& m_hal;

    void reset();

    bool sent_multirotor_commands_packet();
    bool send_camera_commands_packet();
    void process_rx_packet(rc_comms::Packet_Type packet_type, std::vector<uint8_t> const& data, size_t offset);
    void process_received_data(std::vector<uint8_t> const& data);

//    util::comms::RC_Phy m_rc_phy;
//    util::comms::RC_Protocol m_rc_protocol;
//    util::comms::RC_Protocol::RX_Packet m_rx_packet;

//    util::comms::Video_Streamer m_video_streamer;

    bool m_is_connected = false;

    uint32_t m_last_req_id = 0;

    Phy m_phy;

    mutable std::mutex m_samples_mutex;
    stream::IMultirotor_State::Value m_multirotor_state;
    stream::IMultirotor_Commands::Value m_multirotor_commands;
    stream::ICamera_Commands::Value m_camera_commands;

    //mark the commands as infinitely old
    Clock::time_point m_multirotor_commands_last_valid_tp = Clock::time_point(Clock::duration::zero());
    Clock::time_point m_multirotor_commands_last_sent_tp = Clock::time_point(Clock::duration::zero());

    Clock::time_point m_camera_commands_last_valid_tp = Clock::time_point(Clock::duration::zero());
    Clock::time_point m_camera_commands_last_sent_tp = Clock::time_point(Clock::duration::zero());

    Clock::time_point m_telemetry_tp = Clock::time_point(Clock::duration::zero());

    struct Phy_Data
    {
        Phy_Data()
            : tx_queue(32)
            , rx_queue(32)
        {}

        typedef std::vector<uint8_t> Packet;
        typedef Pool<Packet>::Ptr Packet_ptr;
        Pool<Packet> packet_pool;
        Queue<Packet_ptr> tx_queue;
        Queue<Packet_ptr> rx_queue;
        bool thread_exit = false;
        std::thread thread;
    } m_phy_data;

    void phy_thread_proc();
};

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

