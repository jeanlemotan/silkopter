#pragma once

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

#include "common/node/INode.h"
#include "common/node/IPilot.h"

#include "common/config/Multi.h"
#include "common/Comm_Data.h"
#include "common/Manual_Clock.h"

#include "utils/Json_Util.h"

namespace silk
{

class HAL;
class Comms;




namespace node
{
namespace gs
{
struct Node;
}
}


namespace stream
{
namespace gs
{

struct Stream : public IStream
{
    node::gs::Node_wptr node;
    std::string name;
    uint32_t rate = 0;
    int telemetry_active_req = 0;
    bool is_telemetry_active = false;
};


template<class Base>
struct Typed_Stream : public Stream
{
    static constexpr Semantic TYPE = Base::TYPE;

    typedef typename Base::Value Value;
    typedef typename Base::Sample Sample;
    typedef std::vector<Sample> Samples;

    Samples samples;
    q::util::Signal<void(Samples const&)> samples_available_signal;

    virtual auto get_rate() const -> uint32_t override { return rate; }
    virtual auto get_semantic() const -> Semantic override { return TYPE; }
};

using Acceleration = Typed_Stream<IAcceleration>;
using ENU_Acceleration = Typed_Stream<IENU_Acceleration>;
//using ECEF_Acceleration = Typed_Stream<IECEF_Acceleration>;
using ADC = Typed_Stream<IADC>;
using Angular_Velocity = Typed_Stream<IAngular_Velocity>;
using ENU_Angular_Velocity = Typed_Stream<IENU_Angular_Velocity>;
using ECEF_Angular_Velocity = Typed_Stream<IECEF_Angular_Velocity>;
using Battery_State = Typed_Stream<IBattery_State>;
using Current = Typed_Stream<ICurrent>;
using Distance = Typed_Stream<IDistance>;
using ENU_Distance = Typed_Stream<IENU_Distance>;
using ECEF_Distance = Typed_Stream<IECEF_Distance>;
using Float = Typed_Stream<IFloat>;
using Bool = Typed_Stream<IBool>;
using Force = Typed_Stream<IForce>;
using ENU_Force = Typed_Stream<IENU_Force>;
using ECEF_Force = Typed_Stream<IECEF_Force>;
using UAV_Frame = Typed_Stream<IUAV_Frame>;
using Gimbal_Frame = Typed_Stream<IGimbal_Frame>;
//using ENU_Frame = GS_Stream<IENU_Frame>;
using GPS_Info = Typed_Stream<IGPS_Info>;
using Linear_Acceleration = Typed_Stream<ILinear_Acceleration>;
using ENU_Linear_Acceleration = Typed_Stream<IENU_Linear_Acceleration>;
using ECEF_Linear_Acceleration = Typed_Stream<IECEF_Linear_Acceleration>;
using ECEF_Position = Typed_Stream<IECEF_Position>;
using Magnetic_Field = Typed_Stream<IMagnetic_Field>;
using ENU_Magnetic_Field = Typed_Stream<IENU_Magnetic_Field>;
using ECEF_Magnetic_Field = Typed_Stream<IECEF_Magnetic_Field>;
using Pressure = Typed_Stream<IPressure>;
using PWM = Typed_Stream<IPWM>;
using Temperature = Typed_Stream<ITemperature>;
using Throttle = Typed_Stream<IThrottle>;
using Torque = Typed_Stream<ITorque>;
using ENU_Torque = Typed_Stream<IENU_Torque>;
using ECEF_Torque = Typed_Stream<IECEF_Torque>;
using Velocity = Typed_Stream<IVelocity>;
using ENU_Velocity = Typed_Stream<IENU_Velocity>;
using ECEF_Velocity = Typed_Stream<IECEF_Velocity>;
using Voltage = Typed_Stream<IVoltage>;
using Video = Typed_Stream<IVideo>;
using Multirotor_State = Typed_Stream<IMultirotor_State>;
using Multirotor_Commands = Typed_Stream<IMultirotor_Commands>;
using Proximity = Typed_Stream<IProximity>;

}
}



namespace node
{
namespace gs
{

struct Node_Def
{
    std::string name;
    node::Type type;
    rapidjson::Document default_init_params;

    struct Input
    {
        stream::Semantic type;
        std::string name;
        uint32_t rate = 0;
    };
    std::vector<Input> inputs;
    struct Output
    {
        stream::Semantic type;
        std::string name;
        uint32_t rate = 0;
    };
    std::vector<Output> outputs;
};

struct Node
{
    std::string name;
    node::Type type;

    rapidjson::Document init_params;
    rapidjson::Document config;

    struct Input
    {
        q::Path stream_path;
        //stream::gs::Stream_wptr stream;
        stream::Semantic type;
        std::string name;
        uint32_t rate = 0;
    };
    std::vector<Input> inputs;
    struct Output
    {
        //stream::gs::Stream_ptr stream;
        stream::Semantic type;
        std::string name;
        uint32_t rate = 0;
    };
    std::vector<Output> outputs;

    q::util::Signal<void()> changed_signal;
    q::util::Signal<void(rapidjson::Document const& json)> message_received_signal;
};

}
}


}
