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
    if (m_init_params->motor_count == 0)
    {
        QLOGE("Bad motor count: {}", m_init_params->motor_count);
        return false;
    }

    m_output_streams.resize(m_init_params->motor_count);
    for (auto& os: m_output_streams)
    {
        os = std::make_shared<Stream>();
        os->rate = m_init_params->rate;
    }

    m_config->outputs.throttles.resize(m_init_params->motor_count);
    m_config->motors.resize(m_init_params->motor_count);

    m_dt = std::chrono::microseconds(1000000 / m_init_params->rate);
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

//    m_output_stream->samples.resize(count);

//    for (size_t i = 0; i < count; i++)
//    {
//        m_output_stream->last_sample.dt = m_dt;
//        m_output_stream->last_sample.sample_idx++;
//        m_output_stream->samples[i] = m_output_stream->last_sample;
//    }

    //consume processed samples
    m_force_samples.erase(m_force_samples.begin(), m_force_samples.begin() + count);
    m_torque_samples.erase(m_torque_samples.begin(), m_torque_samples.begin() + count);
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
    m_config->outputs.throttles.resize(m_init_params->motor_count);
    m_config->motors.resize(m_init_params->motor_count);

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


}
}
