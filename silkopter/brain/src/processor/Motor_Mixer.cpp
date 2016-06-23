#include "BrainStdAfx.h"
#include "Motor_Mixer.h"
#include "uav_properties/IMultirotor_Properties.h"

#include "uav.def.h"
//#include "sz_Motor_Mixer.hpp"

namespace silk
{
namespace node
{

Motor_Mixer::Motor_Mixer(UAV& uav)
    : m_uav(uav)
    , m_descriptor(new uav::Motor_Mixer_Descriptor())
    , m_config(new uav::Motor_Mixer_Config())
{
}

auto Motor_Mixer::init(uav::INode_Descriptor const& descriptor) -> bool
{
    QLOG_TOPIC("motor_mixer::init");

    auto specialized = dynamic_cast<uav::Motor_Mixer_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }
    *m_descriptor = *specialized;

    return init();
}

auto Motor_Mixer::init() -> bool
{
    std::shared_ptr<const IMultirotor_Properties> multirotor_properties = m_uav.get_specialized_uav_properties<IMultirotor_Properties>();
    if (!multirotor_properties)
    {
        QLOGE("No multirotor properties found");
        return false;
    }

    //check symmetry
    math::vec3f center;
    math::vec3f torque;
    for (auto& mc: multirotor_properties->get_motors())
    {
        center += mc.position;
        torque += math::cross(mc.position, mc.thrust_vector) + mc.thrust_vector*(multirotor_properties->get_motor_z_torque() * (mc.clockwise ? 1 : -1));
    }
    if (!math::is_zero(center, 0.05f))
    {
        QLOGE("Motors are not a symmetrical");
        return false;
    }
    if (!math::is_zero(torque, 0.05f))
    {
        QLOGE("Motors don't produce symmetrical thrust");
        return false;
    }

    m_outputs.resize(multirotor_properties->get_motors().size());
    for (auto& os: m_outputs)
    {
        os = std::make_shared<Stream>();
        os->rate = m_descriptor->get_rate();
    }

    //m_config->output_streams.throttles.resize(multirotor_descriptor->motors.size());

    return true;
}

auto Motor_Mixer::start(q::Clock::time_point tp) -> bool
{
    //TODO - use an basic_output_stream instead of the ad-hoc streams
    return true;
}

auto Motor_Mixer::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::ITorque::TYPE, m_descriptor->get_rate(), "Torque", m_accumulator.get_stream_path(0) },
        { stream::IFloat::TYPE, m_descriptor->get_rate(), "Collective Thrust", m_accumulator.get_stream_path(1) }
    }};
    return inputs;
}
auto Motor_Mixer::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(m_outputs.size());
    for (size_t i = 0; i < m_outputs.size(); i++)
    {
        outputs[i].name = q::util::format<std::string>("Throttle {}", i);
        outputs[i].stream = m_outputs[i];
    }
    return outputs;
}

void Motor_Mixer::process()
{
    QLOG_TOPIC("motor_mixer::process");

    for (auto& os: m_outputs)
    {
        os->samples.clear();
    }

    std::shared_ptr<const IMultirotor_Properties> multirotor_properties = m_uav.get_specialized_uav_properties<IMultirotor_Properties>();
    if (!multirotor_properties)
    {
        return;
    }
    if (multirotor_properties->get_motors().size() != m_outputs.size())
    {
        QLOGE("Motor count changed since initialization!!!! Case not handled");
        return;
    }

//    for (auto& os: m_outputs)
//    {
//        os->samples.resize(count);
//    }

    m_accumulator.process([this, &multirotor_properties](stream::ITorque::Sample const& t_sample,
                                                stream::IFloat::Sample const& f_sample)
    {
        compute_throttles(*multirotor_properties, f_sample.value, t_sample.value);

        for (size_t mi = 0; mi < m_outputs.size(); mi++)
        {
            auto& sample = m_outputs[mi]->last_sample;
            sample.value = m_outputs[mi]->throttle;
            sample.is_healthy = true;
            m_outputs[mi]->samples.push_back(sample);
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
//static float compute_thrust_from_throttle(float max_thrust, float throttle)
//{
//    auto thrust = math::square(throttle * max_thrust);
//    return thrust;
//}

constexpr float MIN_THRUST = 0.f;
constexpr float DYN_RANGE_FACTOR = 1.1f;//allow a bit more dyn range than normal to get better torque resolution at the expense of collective force


void Motor_Mixer::compute_throttles(IMultirotor_Properties const& multirotor_properties, stream::IFloat::Value const& collective_thrust, stream::ITorque::Value const& _target)
{
    auto target = _target;

    //precalculate some data
    for (size_t i = 0; i < m_outputs.size(); i++)
    {
        auto const& mc = multirotor_properties.get_motors()[i];
        auto& out = m_outputs[i];
        out->config.position = mc.position;

        out->config.max_torque = math::cross(out->config.position, mc.thrust_vector * multirotor_properties.get_motor_thrust());
        out->config.max_torque += mc.thrust_vector * (multirotor_properties.get_motor_z_torque() * (mc.clockwise ? 1 : -1));
        out->config.torque_vector = math::normalized<float, math::safe>(out->config.max_torque);

        out->thrust = MIN_THRUST;
    }


    //apply the desired thrust
    float min_thrust = MIN_THRUST;
    float max_thrust = MIN_THRUST;
    float target_thrust = collective_thrust;
    if (target_thrust >= 0.01f)
    {
        auto th = math::clamp(target_thrust / float(m_outputs.size()), m_config->get_armed_thrust(), multirotor_properties.get_motor_thrust());
        for (auto& out: m_outputs)
        {
            out->thrust = th; //take into account only motors that produce useful thrust
        }

        float dyn_range = math::min(th - m_config->get_armed_thrust(), multirotor_properties.get_motor_thrust() - th);
        dyn_range *= DYN_RANGE_FACTOR;
        min_thrust = math::max(th - dyn_range, m_config->get_armed_thrust());
        max_thrust = math::min(th + dyn_range, multirotor_properties.get_motor_thrust());
    }


    //compute thrusts for the target torque
    float STEP = 0.9f;
    size_t iteration = 0;
    math::vec3f old_crt;
    while (true)
    {
        //calculate motor torques
        math::vec3f crt;
        for (auto& out: m_outputs)
        {
            float ratio = out->thrust / multirotor_properties.get_motor_thrust();
            out->torque = out->config.max_torque * ratio;
            crt += out->torque;
        }

        if (iteration > 0)
        {
            if (math::equals(crt, old_crt, math::epsilon<float>()))
            {
//                QLOGI("{}: Stabilized in {} iterations: {} - {}", STEP, iteration, crt, target);
                break;
            }
        }
        old_crt = crt;

        //check if we're done
        if (math::equals(crt, target, 0.01f))
        {
//            QLOGI("{}: Done in {} iterations", STEP, iteration);
            break;
        }

        //how far are we for the target?
        //divide by motor count because I want to distribute the difference to all motors
        auto diff = (target - crt) / float(m_outputs.size());

        //distribute the diff to all motors
        for (auto& out: m_outputs)
        {
            //figure out how much each motor can influence the target torque
            //  by doing a dot product with the normalized torque vector
            auto f = math::dot(out->config.torque_vector, diff) * STEP; //go toqards the difference in small steps so we can converge
//            auto th = math::cross(diff, out->config.position);
//            auto f = math::dot(th, THRUST_VECTOR) * STEP; //go toqards the difference in small steps so we can converge
            out->thrust = math::clamp(out->thrust + f, min_thrust, max_thrust);
        }

        iteration++;
        if (iteration > 5000)
        {
//            QLOGW("Too many iterations!!! {}", iteration);
        }
        if (iteration > 50000)
        {
            QLOGW("Too many iterations!!! {}", iteration);
            break;
        }
    }

    for (auto& out: m_outputs)
    {
        out->thrust = math::clamp(out->thrust, min_thrust, max_thrust);
    }

    //convert thrust to throttle and clip
    for (auto& out: m_outputs)
    {
        out->throttle = compute_throttle_from_thrust(multirotor_properties.get_motor_thrust(), out->thrust);
    }

}
//void Motor_Mixer::compute_throttles(config::Multirotor const& multirotor_properties, stream::IFloat::Value const& collective_thrust, stream::ITorque::Value const& _target)
//{
////    auto max_throttle = m_config->max_throttle;
////    auto min_throttle = m_config->min_throttle;

//    auto target = _target;

//    //precalculate some data
//    for (size_t i = 0; i < m_outputs.size(); i++)
//    {
//        auto const& mc = multirotor_properties.motors[i];
//        auto& out = m_outputs[i];
//        out->config.position = mc.position;
//        out->config.max_thrust = mc.max_thrust;

//        out->config.max_torque = math::cross(out->config.position, THRUST_VECTOR * mc.max_thrust);
//        out->config.max_torque += THRUST_VECTOR * mc.max_z_torque;
//        out->config.torque_vector = math::normalized<float, math::safe>(out->config.max_torque);

//        out->thrust = MIN_THRUST;
//    }

//    //compute torque range
////    {
////        auto target_normalized = math::normalized(target);
////        float max_torque = 0;
////        //calculate motor torques
////        for (auto& out: m_outputs)
////        {
////            math::vec3f torque = math::cross(out->config.position, THRUST_VECTOR * out->config.max_thrust);
////            //add z torque which is torque along the thrust axis
////            torque += THRUST_VECTOR * out->config.max_z_torque;
////            auto dot = math::dot(torque, target_normalized);
////            if (dot > 0)
////            {
////                max_torque += dot;
////            }
////        }
////        max_torque *= 0.75f; //don't use the whole range to leave some room for mechanical issues with the motors and props
////        if (math::length_sq(target) > math::square(max_torque))
////        {
////            int a = 0;
////            //target.set_length(max_torque);
////        }
////    }


//    //compute thrusts for the target torque
//    float STEP = 0.9f;
//    size_t iteration = 0;
//    math::vec3f old_crt;
//    while (true)
//    {
//        //calculate motor torques
//        math::vec3f crt;
//        for (auto& out: m_outputs)
//        {
//            float ratio = out->thrust / out->config.max_thrust;
//            out->torque = out->config.max_torque * ratio;
//            crt += out->torque;
//        }

//        if (iteration > 0)
//        {
//            if (math::equals(crt, old_crt, math::epsilon<float>()))
//            {
//                QLOGI("{}: Stabilized in {} iterations: {} - {}", STEP, iteration, crt, target);
//                break;
//            }
//        }
//        old_crt = crt;

//        //check if we're done
//        if (math::equals(crt, target, 0.01f))
//        {
//            QLOGI("{}: Done in {} iterations", STEP, iteration);
//            break;
//        }

//        //how far are we for the target?
//        //divide by motor count because I want to distribute the difference to all motors
//        auto diff = (target - crt) / float(m_outputs.size());

//        //distribute the diff to all motors
//        for (auto& out: m_outputs)
//        {
//            //figure out how much each motor can influence the target torque
//            //  by doing a dot product with the normalized torque vector
//            auto f = math::dot(out->config.torque_vector, diff) * STEP; //go toqards the difference in small steps so we can converge
////            auto th = math::cross(diff, out->config.position);
////            auto f = math::dot(th, THRUST_VECTOR) * STEP; //go toqards the difference in small steps so we can converge
//            out->thrust = math::clamp(out->thrust + f, MIN_THRUST, out->config.max_thrust);
//        }

//        iteration++;
//        if (iteration > 5000)
//        {
//            QLOGW("Too many iterations!!! {}", iteration);
//        }
//        if (iteration > 50000)
//        {
//            QLOGW("Too many iterations!!! {}", iteration);
//            break;
//        }
//    }

//    for (auto& out: m_outputs)
//    {
//        out->thrust = math::clamp(out->thrust, MIN_THRUST, out->config.max_thrust);
//    }

//    float target_thrust = math::dot(THRUST_VECTOR, collective_thrust);

//    //calculate total thrust
//    float total_thrust = 0;
//    float max_inc = std::numeric_limits<float>::max(); //how much can we increase throttle before clipping
//    float max_dec = std::numeric_limits<float>::max(); //max decrease before clipping
//    for (auto& out: m_outputs)
//    {
//        total_thrust += out->thrust; //take into account only motors that produce useful thrust
//        max_dec = math::clamp(max_dec, 0.f, out->thrust - MIN_THRUST);
//        max_inc = math::min(max_inc, out->config.max_thrust - out->thrust);
//    }

//    //either increase or decrease if needed
//    float diff = (target_thrust - total_thrust) / float(m_outputs.size());
//    if (diff > 0)
//    {
//        diff = math::min(diff, max_inc);
//        for (auto& out: m_outputs)
//        {
//            out->thrust += diff;
//            QASSERT(out->thrust >= MIN_THRUST && out->thrust <= out->config.max_thrust);
//        }
//    }
//    else if (diff < 0)
//    {
//        diff = math::min(-diff, max_dec);
//        for (auto& out: m_outputs)
//        {
//            out->thrust -= diff;
//            QASSERT(out->thrust >= MIN_THRUST && out->thrust <= out->config.max_thrust);
//        }
//    }

//    //convert thrust to throttle and clip
//    for (auto& out: m_outputs)
//    {
//        out->throttle = compute_throttle_from_thrust(out->config.max_thrust, out->thrust);
//    }

//}

void Motor_Mixer::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_descriptor->get_rate(), m_uav);
}

auto Motor_Mixer::set_config(uav::INode_Config const& config) -> bool
{
    QLOG_TOPIC("motor_mixer::set_config");

    auto specialized = dynamic_cast<uav::Motor_Mixer_Config const*>(&config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }
    *m_config = *specialized;

    return true;
}
auto Motor_Mixer::get_config() const -> std::shared_ptr<const uav::INode_Config>
{
    return m_config;
}

auto Motor_Mixer::get_descriptor() const -> std::shared_ptr<const uav::INode_Descriptor>
{
    return m_descriptor;
}
//auto Motor_Mixer::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}


}
}
