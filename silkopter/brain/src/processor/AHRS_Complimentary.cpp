#include "BrainStdAfx.h"
#include "AHRS_Complimentary.h"

#include "sz_math.hpp"
#include "sz_hal_nodes.hpp"

namespace silk
{
namespace node
{
namespace processor
{

AHRS_Complimentary::AHRS_Complimentary(HAL& hal)
    : m_hal(hal)
{
}

auto AHRS_Complimentary::get_name() const -> std::string const&
{
    return m_params.name;
}

auto AHRS_Complimentary::init(rapidjson::Value const& json) -> bool
{
//    sz::AHRS_Complimentary sz;
//    autojsoncxx::error::ErrorStack result;
//    if (!autojsoncxx::from_value(sz, json, result))
//    {
//        std::ostringstream ss;
//        ss << result;
//        QLOGE("Cannot deserialize AHRS_Complimentary data: {}", ss.str());
//        return false;
//    }
//    Init_Params params;
//    params.name = sz.name;
//    params.voltage_stream = m_hal.get_streams().find_by_name<stream::IVoltage>(sz.voltage_stream);
//    params.current_stream = m_hal.get_streams().find_by_name<stream::ICurrent>(sz.current_stream);
//    params.full_charge = sz.full_charge;
//    return init(params);
}
auto AHRS_Complimentary::init(Init_Params const& params) -> bool
{
    m_params = params;

    if (!init())
    {
        return false;
    }

    if (!m_params.name.empty())
    {
        m_stream.name = q::util::format2<std::string>("{}/stream", m_params.name);
        if (!m_hal.get_streams().add(m_stream))
        {
            return false;
        }
    }
    return true;
}

auto AHRS_Complimentary::init() -> bool
{
    m_stream.params = &m_params;
    if (!m_params.angular_velocity_stream)
    {
        QLOGE("No input angular velocity stream specified");
        return false;
    }
    if (!m_params.acceleration_stream)
    {
        QLOGE("No input acceleration stream specified");
        return false;
    }
    if (!m_params.magnetic_field_stream)
    {
        QLOGE("No input magnetic field stream specified");
        return false;
    }
    if (m_params.acceleration_stream->get_rate() != m_params.magnetic_field_stream->get_rate() ||
        m_params.acceleration_stream->get_rate() != m_params.angular_velocity_stream->get_rate())
    {
        QLOGE("Angular velocity, Acceleration and Magnetic field streams have different rates: {} != {} != {}",
              m_params.angular_velocity_stream->get_rate(),
              m_params.acceleration_stream->get_rate(),
              m_params.magnetic_field_stream->get_rate());
        return false;
    }

    m_dt = std::chrono::microseconds(1000000 / m_stream.get_rate());
}

auto AHRS_Complimentary::get_input_stream_count() const -> size_t
{
    return 3;
}
auto AHRS_Complimentary::get_input_stream(size_t idx) -> stream::IStream&
{
    QASSERT(idx < get_input_stream_count());
    if (idx == 0)
    {
        return get_input_angular_velocity_stream();
    }
    else if (idx == 1)
    {
        return get_input_acceleration_stream();
    }
    else
    {
        return get_input_magnetic_field_stream();
    }
}
auto AHRS_Complimentary::get_output_stream_count() const -> size_t
{
    return 1;
}
auto AHRS_Complimentary::get_output_stream(size_t idx) -> stream::IStream&
{
    QASSERT(idx < get_output_stream_count());
    return get_output_reference_frame_stream();
}
auto AHRS_Complimentary::get_input_acceleration_stream() -> stream::IAcceleration&
{
    QASSERT(m_params.acceleration_stream);
    return *m_params.acceleration_stream;
}
auto AHRS_Complimentary::get_input_angular_velocity_stream() -> stream::IAngular_Velocity&
{
    QASSERT(m_params.angular_velocity_stream);
    return *m_params.angular_velocity_stream;
}
auto AHRS_Complimentary::get_input_magnetic_field_stream() -> stream::IMagnetic_Field&
{
    QASSERT(m_params.magnetic_field_stream);
    return *m_params.magnetic_field_stream;
}
auto AHRS_Complimentary::get_output_reference_frame_stream() -> stream::IReference_Frame&
{
    return m_stream;
}


void AHRS_Complimentary::process()
{
    m_stream.samples.clear();
}


}
}
}
