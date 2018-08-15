#pragma once

#include <mutex>
#include <deque>
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

    //----------------------------------------------------------------------

    Phy const& get_phy() const;
    Phy& get_phy();

    Clock::time_point get_last_rx_tp() const;

    void set_single_phy(bool yes);

    std::function<void(void const*, size_t, math::vec2u16 const&)> on_video_data_received;

    stream::IMultirotor_State::Value get_multirotor_state() const;

    struct Stats
    {
        float packets_dropped_per_second = 0;
        float packets_received_per_second = 0;
        float packets_sent_per_second = 0;
        float data_received_per_second = 0;
        float data_sent_per_second = 0;
        int16_t rx_rssi = -1000;
        int16_t tx_rssi = -1000;
    };

    const Stats& get_stats() const;

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
    rc_comms::Packet_Header prepare_packet_header(rc_comms::Packet_Type packet_type, uint32_t packet_index) const;

//    util::comms::RC_Phy m_rc_phy;
//    util::comms::RC_Protocol m_rc_protocol;
//    util::comms::RC_Protocol::RX_Packet m_rx_packet;

//    util::comms::Video_Streamer m_video_streamer;

    uint16_t m_station_id = 0;
    std::vector<std::unique_ptr<Phy>> m_phys;
    bool m_single_phy = false;

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

    Stats m_stats;
    Clock::time_point m_last_stats_tp = Clock::now();

    struct Raw_Stats
    {
        size_t packets_dropped = 0;
        size_t packets_received = 0;
        size_t packets_sent = 0;
        size_t data_received = 0;
        size_t data_sent = 0;
        int32_t rx_rssi_accumulated = 0;
        size_t rx_rssi_count = 0;
    };

    void add_raw_stats(Raw_Stats& dst, Raw_Stats const& src) const;

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

        ///////////////////////////////////////////////////////
        //thread_proc only
        struct Parked_Packet
        {
            uint32_t packet_index;
            Packet_ptr packet;
        };
        std::deque<Parked_Packet> parked_packets;
        uint32_t last_sent_packet_index = 0;
        uint32_t last_received_packet_index = 0;
        Clock::time_point last_received_packet_tp = Clock::now();
        ///////////////////////////////////////////////////////

        mutable std::mutex raw_stats_mutex;
        Raw_Stats raw_stats;

        bool thread_exit = false;
        std::thread thread;
    } m_phy_data;

    void phy_thread_proc();
};

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

