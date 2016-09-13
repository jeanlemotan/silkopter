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

#include "common/Manual_Clock.h"

#include "common/node/INode.h"
#include "common/stream/IVideo.h"
#include "common/stream/IMultirotor_State.h"

#include "common/Comm_Data.h"
#include "utils/comms/RC.h"
#include "utils/comms/Video_Streamer.h"

#include <boost/asio.hpp>

namespace silk
{

class Comms : q::util::Noncopyable
{
public:
    Comms();

    auto start(std::string const& interface, uint8_t id) -> bool;

    void disconnect();
    auto is_connected() const -> bool;

    //----------------------------------------------------------------------

    void get_video_data(std::vector<uint8_t>& dst);
    auto get_multirotor_state_samples() -> std::vector<stream::IMultirotor_State::Sample>;
    void send_multirotor_commands_value(stream::IMultirotor_Commands::Value const& value);

    void process();

private:
    void reset();

    util::comms::RC m_rc;
    util::comms::Video_Streamer m_video_streamer;

    bool m_is_connected = false;

    uint32_t m_last_req_id = 0;

    mutable std::mutex m_samples_mutex;
    std::vector<uint8_t> m_video_data;
    std::vector<stream::IMultirotor_State::Sample> m_multirotor_state_samples;

    void handle_multirotor_state();
    void handle_video(void const* data, size_t size);
};

}
