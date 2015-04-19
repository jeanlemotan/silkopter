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

constexpr math::vec3f THRUST_VECTOR(0, 0, 1);

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
    math::vec3f center;
    math::vec3f torque;
    for (auto& mc: multi_config->motors)
    {
        center += mc.position;
        torque += math::cross(mc.position, THRUST_VECTOR) + mc.max_z_torque;
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

    m_outputs.resize(multi_config->motors.size());
    for (auto& os: m_outputs)
    {
        os = std::make_shared<Stream>();
        os->rate = m_init_params->rate;
    }

    m_config->outputs.throttles.resize(multi_config->motors.size());

    return true;
}

auto Motor_Mixer::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::ITorque::TYPE, m_init_params->rate, "Torque" },
        { stream::IForce::TYPE, m_init_params->rate, "Collective Force" }
    }};
    return inputs;
}
auto Motor_Mixer::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(m_outputs.size());
    for (size_t i = 0; i < m_outputs.size(); i++)
    {
        outputs[i].type = stream::IThrottle::TYPE;
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


    if (math::abs((int)m_force_samples.size() - (int)m_torque_samples.size()) > 30)
    {
        m_force_samples.resize(count);
        m_torque_samples.resize(count);
    }


    if (count == 0)
    {
        return;
    }

    for (auto& os: m_outputs)
    {
        os->samples.resize(count);
    }

    for (size_t i = 0; i < count; i++)
    {
        compute_throttles(*multi_config, m_force_samples[i].value, m_torque_samples[i].value);

        auto dt = m_torque_samples[i].dt;
        auto tp = m_torque_samples[i].tp;

        for (size_t mi = 0; mi < m_outputs.size(); mi++)
        {
            auto& sample = m_outputs[mi]->last_sample;
            sample.dt = dt;
            sample.tp = tp;
            sample.value = m_outputs[mi]->throttle;
            sample.sample_idx++;
            m_outputs[mi]->samples[i] = sample;
        }
    }

    //consume processed samples
    m_force_samples.erase(m_force_samples.begin(), m_force_samples.begin() + count);
    m_torque_samples.erase(m_torque_samples.begin(), m_torque_samples.begin() + count);
}

static float compute_throttle_from_thrust(float max_thrust, float thrust)
{
    if (math::is_zero(max_thrust, math::epsilon<float>()))
    {
        return 0;
    }
    auto ratio = thrust / max_thrust;

    //thrust increases approximately with throttle^2
    auto throttle = math::sqrt<float, math::safe>(ratio);
    return throttle;
}
static float compute_thrust_from_throttle(float max_thrust, float throttle)
{
    auto thrust = math::square(throttle * max_thrust);
    return thrust;
}

static float compute_moment_of_inertia(float mass, float radius, float height)
{
    //http://en.wikipedia.org/wiki/List_of_moments_of_inertia
    return (1.f / 12.f) * mass * (3.f * math::square(radius) + math::square(height));
}

constexpr float MIN_THRUST = 0.01f;

void Motor_Mixer::compute_throttles(config::Multi const& multi_config, stream::IForce::Value const& collective_thrust, stream::ITorque::Value const& target)
{
//    auto max_throttle = m_config->max_throttle;
//    auto min_throttle = m_config->min_throttle;

    //reset thrusts to mid range
    for (size_t i = 0; i < m_outputs.size(); i++)
    {
        auto const& mc = multi_config.motors[i];
        m_outputs[i]->config.position = mc.position;
        m_outputs[i]->config.max_thrust = mc.max_thrust;
        m_outputs[i]->config.max_thrust_inv = math::inverse<float, math::safe>(mc.max_thrust);
        m_outputs[i]->config.max_z_torque = mc.max_z_torque;

        m_outputs[i]->thrust = MIN_THRUST;
    }

    constexpr float STEP = 0.4f;
    size_t iteration = 0;
    while (true)
    {
        //calculate motor torques
        for (auto& out: m_outputs)
        {
            float ratio = out->thrust * out->config.max_thrust_inv;
            out->torque = math::cross(out->config.position, THRUST_VECTOR * out->thrust);
            //add z torque which is torque along the thrust axis
            out->torque += THRUST_VECTOR * (out->config.max_z_torque * ratio);
        }

        //calculate the crt torque
        math::vec3f crt;
        for (auto& out: m_outputs)
        {
            crt += out->torque;
        }

        //check if we're done
        if (math::equals(crt, target, 0.0001f))
        {
            QLOGI("{}: Done in {} iterations", STEP, iteration);
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
            auto tq = math::normalized<float, math::safe>(out->torque);
            auto f = math::dot(tq, diff) * STEP; //go toqards the difference in small steps so we can converge
            out->thrust = math::clamp(out->thrust + f, MIN_THRUST, out->config.max_thrust);
        }

        iteration++;
        if (iteration > 5000)
        {
            QLOGW("Too many iterations!!! {}", iteration);
            break;
        }
    }

    float target_thrust = math::dot(THRUST_VECTOR, collective_thrust);

    //calculate total thrust
    float total_thrust = 0;
    float max_inc = std::numeric_limits<float>::max(); //how much can we increase throttle before clipping
    float max_dec = std::numeric_limits<float>::max(); //max decrease before clipping
    for (auto& out: m_outputs)
    {
        total_thrust += out->thrust; //take into account only motors that produce useful thrust
        max_dec = math::clamp(max_dec, 0.f, out->thrust - MIN_THRUST);
        max_inc = math::min(max_inc, out->config.max_thrust - out->thrust);
    }

    //either increase or decrease if needed
    float diff = (target_thrust - total_thrust) / float(m_outputs.size());
    if (diff > 0)
    {
        diff = math::min(diff, max_inc);
        for (auto& out: m_outputs)
        {
            out->thrust += diff;
            QASSERT(out->thrust >= MIN_THRUST && out->thrust <= out->config.max_thrust);
        }
    }
    else if (diff < 0)
    {
        diff = math::min(-diff, max_dec);
        for (auto& out: m_outputs)
        {
            out->thrust -= diff;
            QASSERT(out->thrust >= MIN_THRUST && out->thrust <= out->config.max_thrust);
        }
    }

    //convert thrust to throttle and clip
    for (auto& out: m_outputs)
    {
        out->throttle = compute_throttle_from_thrust(out->config.max_thrust, out->thrust);
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
