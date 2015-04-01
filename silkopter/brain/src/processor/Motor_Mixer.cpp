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
    autojsoncxx::to_document(*m_init_params, m_init_paramsj);
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
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    *m_init_params = sz;
    return init();
}
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

    m_output_streams.resize(multi_config->motors.size());
    for (auto& os: m_output_streams)
    {
        os = std::make_shared<Stream>();
        os->rate = m_init_params->rate;
    }

    m_config->outputs.throttles.resize(multi_config->motors.size());
    m_outputs.resize(multi_config->motors.size());

    return true;
}

auto Motor_Mixer::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs(2);
    inputs[0].type = stream::ITorque::TYPE;
    inputs[0].rate = m_init_params->rate;
    inputs[0].name = "Torque";
    inputs[1].type = stream::IForce::TYPE;
    inputs[1].rate = m_init_params->rate;
    inputs[1].name = "Collective Force";
    return inputs;
}
auto Motor_Mixer::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(m_output_streams.size());
    for (size_t i = 0; i < m_output_streams.size(); i++)
    {
        outputs[i].type = stream::IThrottle::TYPE;
        outputs[i].name = q::util::format2<std::string>("Throttle {}", i);
        outputs[i].stream = m_output_streams[i];
    }
    return outputs;
}

void Motor_Mixer::process()
{
    QLOG_TOPIC("motor_mixer::process");

    for (auto& os: m_output_streams)
    {
        os->samples.clear();
    }

    auto force_stream = m_force_stream.lock();
    auto torque_stream = m_torque_stream.lock();
    if (!force_stream || !torque_stream)
    {
        return;
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

    //accumulate the input streams
    {
        auto const& samples = force_stream->get_samples();
        m_force_samples.reserve(m_force_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_force_samples));
    }
    {
        auto const& samples = torque_stream->get_samples();
        m_torque_samples.reserve(m_torque_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_torque_samples));
    }

    //TODO add some protecton for severely out-of-sync streams

    size_t count = std::min(m_force_samples.size(), m_torque_samples.size());
    if (count == 0)
    {
        return;
    }

    for (auto& os: m_output_streams)
    {
        os->samples.resize(count);
    }

    for (size_t i = 0; i < count; i++)
    {
        auto& sample = m_output_stream->last_sample;
        sample.dt = m_torque_samples[i].dt;
        sample.tp = m_torque_samples[i].tp;
        sample.sample_idx++;

//        sample.value = m_pid.process(m_input_samples[i].value, m_target_samples[i].value);
        compute_throttles(*multi_config, m_force_samples[i].value, m_torque_samples[i].value);

        m_output_stream->samples[i] = sample;
    }

    //consume processed samples
    m_force_samples.erase(m_force_samples.begin(), m_force_samples.begin() + count);
    m_torque_samples.erase(m_torque_samples.begin(), m_torque_samples.begin() + count);
}

static float compute_throttle_from_force(float max_force, float force)
{
    if (math::is_zero(max_force, math::epsilon<float>()))
    {
        return 0;
    }
    auto ratio = force / max_force;

    //thrust increases approximately with throttle^2
    auto throttle = math::sqrt<float, math::safe>(ratio);
    return throttle;
}

static float compute_moment_of_inertia(float mass, float radius, float height)
{
    //http://en.wikipedia.org/wiki/List_of_moments_of_inertia
    return (1.f / 12.f) * mass * (3.f * math::square(radius) + math::square(height));
}

void Motor_Mixer::compute_throttles(config::Multi const& multi_config, stream::IForce::Value const& collective_thrust, stream::ITorque::Value const& torque)
{
    math::vec3f speeds2d(-m_roll, m_pitch, 0.f);

    auto max_throttle = m_config->max_throttle;
    auto min_throttle = m_config->min_throttle;

    for (size_t i = 0; i < multi_config.motors.size(); i++)
    {
        auto const& config = multi_config.motors[i];

        //the dinamic range is limited by the distance from 0 to the collective thrust,
        //  or the half of the max thrust - which ever is smaller
        float half_thrust = config.max_thrust * 0.5f;
        float dynamic_range = math::min(collective_thrust, half_thrust);

        float max = -999.f;
        float output = math::dot(math::vec3f(config.position, 0.f), speeds2d);
        if (motor.info.clockwise)
        {
            output += m_yaw;
        }
        else
        {
            output -= m_yaw;
        }
        output = math::clamp(output, -dynamic_range, dynamic_range) + math::max(m_throttle, min_throttle);
        max = math::max(max, output);
        motor.output = output;
        QASSERT(output + std::numeric_limits<float>::epsilon() >= min_throttle);

        //if over the max, reduce throttle
        //This should never bring any motor under min because of how we calculated the dynamic range
        if (max > max_throttle)
        {
            float diff = max - max_throttle;
            for (auto& motor: m_output_streams)
            {
                motor.output -= diff;
                QASSERT(motor.output + std::numeric_limits<float>::epsilon() >= min_throttle);
                QASSERT(motor.output - std::numeric_limits<float>::epsilon() <= max_throttle);
            }
        }



        auto base_throttle = compute_throttle_from_force(config.max_thrust, force);

        //the dinamic range is limited by the distance from min to throttle,
        //  or the half total throttle range - which ever is smaller
        float min_v = math::max(base_throttle - min_throttle, 0.f);
        float half_range = (max_throttle - min_throttle) * 0.5f;
        float dynamic_range = math::min(min_v, half_range);

        float max = -999.f;
        for (auto& motor: m_motors)
        {
            float output = math::dot(math::vec3f(motor.info.position, 0.f), speeds2d);
            if (motor.info.clockwise)
            {
                output += m_yaw;
            }
            else
            {
                output -= m_yaw;
            }
            output = math::clamp(output, -dynamic_range, dynamic_range) + math::max(m_throttle, min_throttle);
            max = math::max(max, output);
            motor.output = output;
            QASSERT(output + std::numeric_limits<float>::epsilon() >= min_throttle);
        }

        //if over the max, reduce throttle
        //This should never bring any motor under min because of how we calculated the dynamic range
        if (max > max_throttle)
        {
            float diff = max - max_throttle;
            for (auto& motor: m_output_streams)
            {
                motor.output -= diff;
                QASSERT(motor.output + std::numeric_limits<float>::epsilon() >= min_throttle);
                QASSERT(motor.output - std::numeric_limits<float>::epsilon() <= max_throttle);
            }
        }
    }
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

    auto torque_stream = m_hal.get_streams().find_by_name<stream::ITorque>(sz.inputs.torque);
    auto force_stream = m_hal.get_streams().find_by_name<stream::IForce>(sz.inputs.force);

    auto rate = torque_stream ? torque_stream->get_rate() : 0u;
    if (rate != m_init_params->rate)
    {
        m_config->inputs.torque.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.torque, m_init_params->rate, rate);
        m_torque_stream.reset();
    }
    else
    {
        m_torque_stream = torque_stream;
    }

    rate = force_stream ? force_stream->get_rate() : 0u;
    if (rate != m_init_params->rate)
    {
        m_config->inputs.force.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.force, m_init_params->rate, rate);
        m_force_stream.reset();
    }
    else
    {
        m_force_stream = force_stream;
    }

    return true;
}
auto Motor_Mixer::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Motor_Mixer::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}
auto Motor_Mixer::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}


}
}
