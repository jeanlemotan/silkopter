#include "BrainStdAfx.h"
#include "Motor_Mixer.h"

#include "sz_math.hpp"
#include "sz_Motor_Mixer.hpp"

namespace silk
{
namespace node
{

Motor_Mixer::Motor_Mixer(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Motor_Mixer::Init_Params())
    , m_config(new sz::Motor_Mixer::Config())
{
}

auto Motor_Mixer::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("motor_mixer::init");

    sz::Motor_Mixer::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Motor_Mixer data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init();
}

constexpr math::vec3d THRUST_VECTOR(0, 0, 1);

auto Motor_Mixer::init() -> bool
{
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    auto multi_config = m_hal.get_multi_config();
    if (!multi_config)
    {
        QLOGE("No multi config found");
        return false;
    }

    //check symmetry
    math::vec3d center;
    math::vec3d torque;
    for (auto& mc: multi_config->motors)
    {
        center += mc.position;
        torque += math::cross(mc.position, THRUST_VECTOR) + THRUST_VECTOR*(multi_config->motor_z_torque * (mc.clockwise ? 1 : -1));
    }
    if (!math::is_zero(center, 0.05))
    {
        QLOGE("Motors are not a symmetrical");
        return false;
    }
    if (!math::is_zero(torque, 0.05))
    {
        QLOGE("Motors don't produce symmetrical thrust");
        return false;
    }

    m_outputs.resize(multi_config->motors.size());
    for (auto& os: m_outputs)
    {
        os = std::make_shared<Stream>();
        os->rate = m_init_params->rate;
    }

    //m_config->output_streams.throttles.resize(multi_config->motors.size());

    return true;
}

auto Motor_Mixer::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::ITorque::TYPE, m_init_params->rate, "Torque", m_accumulator.get_stream_path(0) },
        { stream::IForce::TYPE, m_init_params->rate, "Collective Force", m_accumulator.get_stream_path(1) }
    }};
    return inputs;
}
auto Motor_Mixer::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(m_outputs.size());
    for (size_t i = 0; i < m_outputs.size(); i++)
    {
        outputs[i].name = q::util::format2<std::string>("Throttle {}", i);
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

    auto multi_config = m_hal.get_multi_config();
    if (!multi_config)
    {
        return;
    }
    if (multi_config->motors.size() != m_outputs.size())
    {
        QLOGE("Motor count changed since initialization!!!! Case not handled");
        return;
    }

//    for (auto& os: m_outputs)
//    {
//        os->samples.resize(count);
//    }

    m_accumulator.process([this, &multi_config](
                          size_t idx,
                          stream::ITorque::Sample const& t_sample,
                          stream::IForce::Sample const& f_sample)
    {
        compute_throttles(*multi_config, f_sample.value, t_sample.value);

        for (size_t mi = 0; mi < m_outputs.size(); mi++)
        {
            auto& sample = m_outputs[mi]->last_sample;
            sample.value = m_outputs[mi]->throttle;
            m_outputs[mi]->samples.push_back(sample);
        }
    });
}

static double compute_throttle_from_thrust(double max_thrust, double thrust)
{
    if (math::is_zero(max_thrust, math::epsilon<double>()))
    {
        return 0;
    }
    auto ratio = thrust / max_thrust;

    //thrust increases approximately with throttle^2
    auto throttle = math::sqrt<double, math::safe>(ratio);
    return throttle;
}
//static double compute_thrust_from_throttle(double max_thrust, double throttle)
//{
//    auto thrust = math::square(throttle * max_thrust);
//    return thrust;
//}

constexpr double MIN_THRUST = 0.0;
constexpr double DYN_RANGE_FACTOR = 1.1;//allow a bit more dyn range than normal to get better torque resolution at the expense of collective force


void Motor_Mixer::compute_throttles(config::Multi const& multi_config, stream::IForce::Value const& collective_thrust, stream::ITorque::Value const& _target)
{
    auto target = _target;

    //precalculate some data
    for (size_t i = 0; i < m_outputs.size(); i++)
    {
        auto const& mc = multi_config.motors[i];
        auto& out = m_outputs[i];
        out->config.position = mc.position;

        out->config.max_torque = math::cross(out->config.position, THRUST_VECTOR * multi_config.motor_thrust);
        out->config.max_torque += THRUST_VECTOR * (multi_config.motor_z_torque * (mc.clockwise ? 1 : -1));
        out->config.torque_vector = math::normalized<double, math::safe>(out->config.max_torque);

        out->thrust = MIN_THRUST;
    }


    //apply the desired thrust
    double min_thrust = MIN_THRUST;
    double max_thrust = MIN_THRUST;
    double target_thrust = math::dot(collective_thrust, THRUST_VECTOR);
    if (target_thrust >= 0)
    {
        auto th = math::clamp(target_thrust / double(m_outputs.size()), MIN_THRUST, multi_config.motor_thrust);
        for (auto& out: m_outputs)
        {
            out->thrust = th; //take into account only motors that produce useful thrust
        }

        double dyn_range = math::min(th - MIN_THRUST, multi_config.motor_thrust - th);
        dyn_range *= DYN_RANGE_FACTOR;
        min_thrust = math::max(th - dyn_range, MIN_THRUST);
        max_thrust = math::min(th + dyn_range, multi_config.motor_thrust);
    }


    //compute thrusts for the target torque
    double STEP = 0.9;
    size_t iteration = 0;
    math::vec3d old_crt;
    while (true)
    {
        //calculate motor torques
        math::vec3d crt;
        for (auto& out: m_outputs)
        {
            double ratio = out->thrust / multi_config.motor_thrust;
            out->torque = out->config.max_torque * ratio;
            crt += out->torque;
        }

        if (iteration > 0)
        {
            if (math::equals(crt, old_crt, math::epsilon<double>()))
            {
//                QLOGI("{}: Stabilized in {} iterations: {} - {}", STEP, iteration, crt, target);
                break;
            }
        }
        old_crt = crt;

        //check if we're done
        if (math::equals(crt, target, 0.01))
        {
//            QLOGI("{}: Done in {} iterations", STEP, iteration);
            break;
        }

        //how far are we for the target?
        //divide by motor count because I want to distribute the difference to all motors
        auto diff = (target - crt) / double(m_outputs.size());

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
        out->throttle = compute_throttle_from_thrust(multi_config.motor_thrust, out->thrust);
    }

}
//void Motor_Mixer::compute_throttles(config::Multi const& multi_config, stream::IForce::Value const& collective_thrust, stream::ITorque::Value const& _target)
//{
////    auto max_throttle = m_config->max_throttle;
////    auto min_throttle = m_config->min_throttle;

//    auto target = _target;

//    //precalculate some data
//    for (size_t i = 0; i < m_outputs.size(); i++)
//    {
//        auto const& mc = multi_config.motors[i];
//        auto& out = m_outputs[i];
//        out->config.position = mc.position;
//        out->config.max_thrust = mc.max_thrust;

//        out->config.max_torque = math::cross(out->config.position, THRUST_VECTOR * mc.max_thrust);
//        out->config.max_torque += THRUST_VECTOR * mc.max_z_torque;
//        out->config.torque_vector = math::normalized<double, math::safe>(out->config.max_torque);

//        out->thrust = MIN_THRUST;
//    }

//    //compute torque range
////    {
////        auto target_normalized = math::normalized(target);
////        double max_torque = 0;
////        //calculate motor torques
////        for (auto& out: m_outputs)
////        {
////            math::vec3d torque = math::cross(out->config.position, THRUST_VECTOR * out->config.max_thrust);
////            //add z torque which is torque along the thrust axis
////            torque += THRUST_VECTOR * out->config.max_z_torque;
////            auto dot = math::dot(torque, target_normalized);
////            if (dot > 0)
////            {
////                max_torque += dot;
////            }
////        }
////        max_torque *= 0.75; //don't use the whole range to leave some room for mechanical issues with the motors and props
////        if (math::length_sq(target) > math::square(max_torque))
////        {
////            int a = 0;
////            //target.set_length(max_torque);
////        }
////    }


//    //compute thrusts for the target torque
//    double STEP = 0.9;
//    size_t iteration = 0;
//    math::vec3d old_crt;
//    while (true)
//    {
//        //calculate motor torques
//        math::vec3d crt;
//        for (auto& out: m_outputs)
//        {
//            double ratio = out->thrust / out->config.max_thrust;
//            out->torque = out->config.max_torque * ratio;
//            crt += out->torque;
//        }

//        if (iteration > 0)
//        {
//            if (math::equals(crt, old_crt, math::epsilon<double>()))
//            {
//                QLOGI("{}: Stabilized in {} iterations: {} - {}", STEP, iteration, crt, target);
//                break;
//            }
//        }
//        old_crt = crt;

//        //check if we're done
//        if (math::equals(crt, target, 0.01))
//        {
//            QLOGI("{}: Done in {} iterations", STEP, iteration);
//            break;
//        }

//        //how far are we for the target?
//        //divide by motor count because I want to distribute the difference to all motors
//        auto diff = (target - crt) / double(m_outputs.size());

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

//    double target_thrust = math::dot(THRUST_VECTOR, collective_thrust);

//    //calculate total thrust
//    double total_thrust = 0;
//    double max_inc = std::numeric_limits<double>::max(); //how much can we increase throttle before clipping
//    double max_dec = std::numeric_limits<double>::max(); //max decrease before clipping
//    for (auto& out: m_outputs)
//    {
//        total_thrust += out->thrust; //take into account only motors that produce useful thrust
//        max_dec = math::clamp(max_dec, 0.0, out->thrust - MIN_THRUST);
//        max_inc = math::min(max_inc, out->config.max_thrust - out->thrust);
//    }

//    //either increase or decrease if needed
//    double diff = (target_thrust - total_thrust) / double(m_outputs.size());
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
    m_accumulator.set_stream_path(idx, path, m_init_params->rate, m_hal);
}

auto Motor_Mixer::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("motor_mixer::set_config");

    sz::Motor_Mixer::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Motor_Mixer config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    return true;
}
auto Motor_Mixer::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Motor_Mixer::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}
auto Motor_Mixer::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}


}
}
