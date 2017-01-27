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

class Comms : q::util::Noncopyable
{
public:
    Comms();

    bool start();

    void disconnect();
    bool is_connected() const;

    //----------------------------------------------------------------------

    int8_t get_rx_dBm() const;
    int8_t get_tx_dBm() const;

    Clock::time_point get_last_rx_tp() const;

    Remote_Viewer_Server const& get_remote_viewer_server() const;
    Remote_Viewer_Server& get_remote_viewer_server();

//    void get_video_data(std::vector<uint8_t>& dst, math::vec2u16& resolution);
    stream::IMultirotor_State::Value get_multirotor_state() const;

    struct Home_Data
    {
        util::coordinates::ECEF ecef_position;
    };
    boost::optional<Home_Data> get_home_data() const;

    void send_multirotor_commands_value(stream::IMultirotor_Commands::Value const& value);

    void process();

private:
    void reset();

    size_t compute_multirotor_commands_packet(uint8_t* data, uint8_t& packet_type);
    void process_rx_packet(util::comms::RC_Protocol::RX_Packet const& packet);

    Remote_Viewer_Server m_remote_viewer_server;

    util::comms::RC_Phy m_rc_phy;
    util::comms::RC_Protocol m_rc_protocol;
    util::comms::RC_Protocol::RX_Packet m_rx_packet;

    boost::optional<Home_Data> m_home_data;
    util::comms::Video_Streamer m_video_streamer;

    bool m_is_connected = false;

    uint32_t m_last_req_id = 0;

    mutable std::mutex m_samples_mutex;
    math::vec2u16 m_video_resolution;
    std::vector<uint8_t> m_video_data;
    std::vector<uint8_t> m_serialization_buffer;
    stream::IMultirotor_State::Value m_multirotor_state;
    stream::IMultirotor_Commands::Value m_multirotor_commands;

    void handle_video(void const* data, size_t size, math::vec2u16 const& resolution);
};

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace util
{
namespace serialization
{

template<> inline void serialize(Buffer_t& buffer, silk::Comms::Home_Data const& value, size_t& off)
{
    util::coordinates::LLA lla_position = util::coordinates::ecef_to_lla(value.ecef_position);

    {
        int64_t v = lla_position.latitude * 100000000.0; //8 decimal places
        uint8_t const* data = reinterpret_cast<uint8_t const*>(&v);
        serialize(buffer, data[0], off);
        serialize(buffer, data[1], off);
        serialize(buffer, data[2], off);
        serialize(buffer, data[3], off);
        serialize(buffer, data[4], off);
    }
    {
        int64_t v = lla_position.longitude * 100000000.0; //8 decimal places
        uint8_t const* data = reinterpret_cast<uint8_t const*>(&v);
        serialize(buffer, data[0], off);
        serialize(buffer, data[1], off);
        serialize(buffer, data[2], off);
        serialize(buffer, data[3], off);
        serialize(buffer, data[4], off);
    }
    {
        int16_t v = static_cast<int16_t>(math::clamp(lla_position.altitude, -327.0, 327.0) * 10.0);
        serialize(buffer, v, off);
    }
}

template<> inline bool deserialize(Buffer_t const& buffer, silk::Comms::Home_Data& value, size_t& off)
{
    uint8_t v1, v2, v3, v4, v5;
    int16_t s1;

    util::coordinates::LLA lla_position;

    //latitude
    if (!deserialize(buffer, v1, off) ||
        !deserialize(buffer, v2, off) ||
        !deserialize(buffer, v3, off) ||
        !deserialize(buffer, v4, off) ||
        !deserialize(buffer, v5, off))
    {
        return false;
    }

    {
        int64_t v = 0;
        if (((v5 >> 7) & 1) == 1)
        {
            v = -1;
        }
        uint8_t* data = reinterpret_cast<uint8_t*>(&v);
        data[0] = v1;
        data[1] = v2;
        data[2] = v3;
        data[3] = v4;
        data[4] = v5;
        lla_position.latitude = v / 100000000.0;
    }

    //longitude
    if (!deserialize(buffer, v1, off) ||
        !deserialize(buffer, v2, off) ||
        !deserialize(buffer, v3, off) ||
        !deserialize(buffer, v4, off) ||
        !deserialize(buffer, v5, off))
    {
        return false;
    }

    {
        int64_t v = 0;
        if (((v5 >> 7) & 1) == 1)
        {
            v = -1;
        }
        uint8_t* data = reinterpret_cast<uint8_t*>(&v);
        data[0] = v1;
        data[1] = v2;
        data[2] = v3;
        data[3] = v4;
        data[4] = v5;
        lla_position.longitude = v / 100000000.0;
    }

    //altitude
    if (!deserialize(buffer, s1, off))
    {
        return false;
    }

    {
        double v = s1 / 100.0;
        lla_position.altitude = v;
    }

    value.ecef_position = util::coordinates::lla_to_ecef(lla_position);

    return true;
}

}
}
