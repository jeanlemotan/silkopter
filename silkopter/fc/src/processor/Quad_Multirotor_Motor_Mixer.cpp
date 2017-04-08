#include "FCStdAfx.h"
#include "Quad_Multirotor_Motor_Mixer.h"
#include "uav_properties/Quad_Multirotor_Properties.h"
#include "physics/constants.h"

#include "hal.def.h"

namespace silk
{
namespace node
{

Quad_Multirotor_Motor_Mixer::Quad_Multirotor_Motor_Mixer(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::Quad_Multirotor_Motor_Mixer_Descriptor())
    , m_config(new hal::Quad_Multirotor_Motor_Mixer_Config())
{
}

ts::Result<void> Quad_Multirotor_Motor_Mixer::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("Quad_Multirotor_Motor_Mixer::init");

    auto specialized = dynamic_cast<hal::Quad_Multirotor_Motor_Mixer_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}

ts::Result<void> Quad_Multirotor_Motor_Mixer::init()
{
    std::shared_ptr<const Quad_Multirotor_Properties> multirotor_properties = m_hal.get_specialized_uav_properties<Quad_Multirotor_Properties>();
    if (!multirotor_properties)
    {
        return make_error("No quad multirotor properties found");
    }

    if (multirotor_properties->get_motors().size() != 4)
    {
        return make_error("Invalid quad multirotor properties found - motor count != 4");
    }

    const std::vector<Quad_Multirotor_Properties::Motor>& motors = multirotor_properties->get_motors();
    for (Quad_Multirotor_Properties::Motor const& mc: motors)
    {
        if (math::dot(mc.thrust_vector, math::vec3f(0, 0, 1.f)) < 0.9999f)
        {
            return make_error("Bad quad multirotor properties: motors have wrong thrust vectors");
        }
    }
    if (!math::equals(motors[0].position, -motors[2].position, 0.99f) ||
            motors[0].clockwise != motors[2].clockwise)
    {
        return make_error("Bad quad multirotor properties: motors 0 and 2 are not antipodal");
    }
    if (!math::equals(motors[1].position, -motors[3].position, 0.99f) ||
            motors[1].clockwise != motors[3].clockwise)
    {
        return make_error("Bad quad multirotor properties: motors 1 and 3 are not antipodal");
    }

    for (std::shared_ptr<Stream>& os: m_outputs)
    {
        os = std::make_shared<Stream>();
        os->rate = m_descriptor->get_rate();
    }

    return ts::success;
}

ts::Result<void> Quad_Multirotor_Motor_Mixer::start(Clock::time_point tp)
{
    //TODO - use an basic_output_stream instead of the ad-hoc streams
    return ts::success;
}

auto Quad_Multirotor_Motor_Mixer::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::ITorque::TYPE, m_descriptor->get_rate(), "torque", m_accumulator.get_stream_path(0) },
        { stream::IFloat::TYPE, m_descriptor->get_rate(), "collective_thrust", m_accumulator.get_stream_path(1) }
    }};
    return inputs;
}
auto Quad_Multirotor_Motor_Mixer::get_outputs() const -> std::vector<Output>
{
    if (m_outputs.size() == 0 || m_outputs[0] == nullptr)
    {
        return std::vector<Output>();
    }

    std::vector<Output> outputs(m_outputs.size());
    for (size_t i = 0; i < m_outputs.size(); i++)
    {
        outputs[i].name = q::util::format<std::string>("throttle_{}", i);
        outputs[i].stream = m_outputs[i];
    }
    return outputs;
}

void Quad_Multirotor_Motor_Mixer::process()
{
    QLOG_TOPIC("Quad_Multirotor_Motor_Mixer::process");

    for (std::shared_ptr<Stream>& os: m_outputs)
    {
        os->samples.clear();
    }

    std::shared_ptr<const Quad_Multirotor_Properties> multirotor_properties = m_hal.get_specialized_uav_properties<Quad_Multirotor_Properties>();
    if (!multirotor_properties)
    {
        return;
    }
    if (multirotor_properties->get_motors().size() != m_outputs.size())
    {
        QLOGE("Motor count changed since initialization!!!! Case not handled");
        return;
    }

    m_accumulator.process([this, &multirotor_properties](stream::ITorque::Sample const& t_sample,
                                                stream::IFloat::Sample const& f_sample)
    {
        bool is_healthy = false;
        if (t_sample.is_healthy & f_sample.is_healthy)
        {
            compute_throttles(*multirotor_properties, f_sample.value, t_sample.value);
            is_healthy = true;
        }

        for (std::shared_ptr<Stream> output: m_outputs)
        {
            Stream::Sample& sample = output->last_sample;
            sample.value = output->throttle;
            sample.is_healthy = is_healthy;
            output->samples.push_back(sample);
        }
    });
}

static float compute_throttle_from_thrust(float max_thrust, float thrust)
{
    if (math::is_zero(max_thrust, math::epsilon<float>()))
    {
        return 0;
    }
    float ratio = thrust / max_thrust;

    //thrust increases approximately with throttle^2
    float throttle = math::sqrt<float, math::safe>(ratio);
    return throttle;
}
static float compute_thrust_from_throttle(float max_thrust, float throttle)
{
    float thrust = math::square(math::clamp(throttle, 0.f, 1.f)) * max_thrust;
    return thrust;
}

void Quad_Multirotor_Motor_Mixer::compute_throttles(Quad_Multirotor_Properties const& multirotor_properties, stream::IFloat::Value const& collective_thrust, stream::ITorque::Value const& _target)
{
    //T = r X F
    //T = |r| * |F| * sin(a)
    // T - torque
    // r - position of the force relative to the center of mass
    // F - force applied
    // a - angle between the force vector and the position vector (usually 90 degrees)


    if (collective_thrust < std::numeric_limits<float>::epsilon() * 10.f)
    {
        for (size_t i = 0; i < m_outputs.size(); i++)
        {
            Stream& out = *m_outputs[i];
            out.thrust = 0.f;
            out.throttle = 0.f;
        }
        return;
    }

    float motor_count = static_cast<float>(m_outputs.size());

    float collective_thrust_per_motor = collective_thrust / motor_count;

    //min and max allowed thrusts per motor
    float max_thrust_per_motor = multirotor_properties.get_motor_thrust();
    float min_thrust_per_motor = compute_thrust_from_throttle(m_config->get_armed_min_throttle(), max_thrust_per_motor);

    //the min collective thrust per motor is - at minimum - twice the min thrust per motor.
    //this is to that even at minimum, there is still some dynamic range left
    float min_collective_thrust_per_motor = math::max(collective_thrust_per_motor, min_thrust_per_motor * 2.f);
    collective_thrust_per_motor = math::max(collective_thrust_per_motor, min_collective_thrust_per_motor);

    //this is half the dynamic range, computed so that there is no spill either above max or below min
    float max_half_dynamic_range = math::min(
                collective_thrust_per_motor - min_thrust_per_motor, //how much space we have between the collective thrust and the min thrust
                (max_thrust_per_motor - min_thrust_per_motor) / 2.f //the middle thrust point
                );


    //split the target torque in XY (roll, pitch) and Z (yaw)
    stream::ITorque::Value xy_target = stream::ITorque::Value(_target.x, _target.y, 0.f);
    stream::ITorque::Value half_xy_target = xy_target / 2.f; //for xy, the quad has 2 halves participating

    float z_target_per_motor = _target.z / motor_count;

    float motor_z_torque = multirotor_properties.get_motor_z_torque();

    float max_computed_thrust_per_motor = std::numeric_limits<float>::lowest();
    float min_computed_thrust_per_motor = std::numeric_limits<float>::max();
    for (size_t i = 0; i < m_outputs.size(); i++)
    {
        Quad_Multirotor_Properties::Motor const& mc = multirotor_properties.get_motors()[i];
        Stream& out = *m_outputs[i];

        float thrust = 0.f;

        //XY torque
        if (math::length_sq(half_xy_target) > std::numeric_limits<float>::epsilon() * 10.f)
        {
            //the length of F depends on how much this motor can participate in the torque
            //a motor parallel with the torque will have |F| == 0
            //a motor perpendicular will have |F| = 1
            math::vec3f F = math::cross(math::normalized<float, math::safe>(half_xy_target), math::normalized<float, math::safe>(mc.position));

            //doing the dot product dives us directionality for the motor thrust
            float positional_factor = math::dot(F, mc.thrust_vector);

            //|F| = |T| / (|p| * sin a)
            //sin a == 1 (for a quad with the motor thrust pointing upwards)
            //so |F| = |T| / |p|
            thrust = positional_factor * math::length(half_xy_target) / math::length(mc.position);
        }

        //distribute the z torque
        {
            float mu = math::clamp(z_target_per_motor / (multirotor_properties.get_motor_z_torque() * (mc.clockwise ? 1 : -1)), 0.f, 1.f);
            thrust += mu * max_thrust_per_motor;
        }

        //clamp to half dynamic range
        thrust = math::sgn(thrust) * math::min(math::abs(thrust), max_half_dynamic_range);
        thrust += collective_thrust_per_motor;

        max_computed_thrust_per_motor = math::max(max_computed_thrust_per_motor, thrust);
        min_computed_thrust_per_motor = math::min(min_computed_thrust_per_motor, thrust);

        out.thrust = thrust;
    }

    //there could be some spill over the max now. Redistribute it to all motors
    QASSERT(min_computed_thrust_per_motor >= min_thrust_per_motor - math::epsilon<float>());
    if (max_computed_thrust_per_motor > max_thrust_per_motor + math::epsilon<float>())
    {
        float spill = max_computed_thrust_per_motor - max_thrust_per_motor;
        min_computed_thrust_per_motor -= spill;
        QASSERT(min_computed_thrust_per_motor >= min_thrust_per_motor - math::epsilon<float>());

        for (size_t i = 0; i < m_outputs.size(); i++)
        {
            Quad_Multirotor_Properties::Motor const& mc = multirotor_properties.get_motors()[i];
            Stream& out = *m_outputs[i];
            out.thrust = math::max(out.thrust - spill, min_thrust_per_motor);
        }
    }

    //convert thrust to throttle and clip
    for (auto& out: m_outputs)
    {
        out->throttle = compute_throttle_from_thrust(max_thrust_per_motor, out->thrust);
    }
}

ts::Result<void> Quad_Multirotor_Motor_Mixer::set_input_stream_path(size_t idx, std::string const& path)
{
    return m_accumulator.set_stream_path(idx, path, m_descriptor->get_rate(), m_hal);
}

ts::Result<void> Quad_Multirotor_Motor_Mixer::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("Quad_Multirotor_Motor_Mixer::set_config");

    auto specialized = dynamic_cast<hal::Quad_Multirotor_Motor_Mixer_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    return ts::success;
}
auto Quad_Multirotor_Motor_Mixer::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto Quad_Multirotor_Motor_Mixer::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}
ts::Result<std::shared_ptr<messages::INode_Message>> Quad_Multirotor_Motor_Mixer::send_message(messages::INode_Message const& message)
{
    return make_error("Unknown message");
}


}
}
