#include "BrainStdAfx.h"
#include "Gravity_Filter.h"
#include "physics/constants.h"

#include "sz_math.hpp"
#include "sz_Gravity_Filter.hpp"

namespace silk
{
namespace node
{

Gravity_Filter::Gravity_Filter(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Gravity_Filter::Init_Params())
    , m_config(new sz::Gravity_Filter::Config())
{
    autojsoncxx::to_document(*m_init_params, m_init_paramsj);
}

auto Gravity_Filter::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("gravity_filter::init");

    sz::Gravity_Filter::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Gravity_Filter data: {}", ss.str());
        return false;
    }
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    *m_init_params = sz;
    return init();
}
auto Gravity_Filter::init() -> bool
{
    m_output_stream = std::make_shared<Stream>();
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    m_output_stream->rate = m_init_params->rate;
    m_dt = std::chrono::microseconds(1000000 / m_output_stream->rate);
    return true;
}

auto Gravity_Filter::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs(2);
    inputs[0].type = stream::IFrame::TYPE;
    inputs[0].rate = m_init_params->rate;
    inputs[0].name = "Frame";
    inputs[1].type = stream::IAcceleration::TYPE;
    inputs[1].rate = m_init_params->rate;
    inputs[1].name = "Acceleration";
    return inputs;
}
auto Gravity_Filter::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].type = stream::ILinear_Acceleration::TYPE;
    outputs[0].name = "Linear Acceleration";
    outputs[0].stream = m_output_stream;
    return outputs;
}

void Gravity_Filter::process()
{
    QLOG_TOPIC("gravity_filter::process");

    m_output_stream->samples.clear();

    auto frame_stream = m_frame_stream.lock();
    auto acceleration_stream = m_acceleration_stream.lock();
    if (!frame_stream || !acceleration_stream)
    {
        return;
    }

    //accumulate the input streams
    {
        auto const& samples = frame_stream->get_samples();
        m_frame_samples.reserve(m_frame_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_frame_samples));
    }
    {
        auto const& samples = acceleration_stream->get_samples();
        m_acceleration_samples.reserve(m_acceleration_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_acceleration_samples));
    }

    //TODO add some protecton for severely out-of-sync streams

    size_t count = std::min(m_frame_samples.size(), m_acceleration_samples.size());
    if (count == 0)
    {
        return;
    }

    m_output_stream->samples.resize(count);

    for (size_t i = 0; i < count; i++)
    {
        auto& sample = m_output_stream->last_sample;
        sample.dt = m_dt;
        sample.tp = m_frame_samples[i].tp;
        sample.sample_idx++;

        auto const& p2l = m_frame_samples[i].value.parent_to_this;
        auto gravity_local = math::rotate(p2l, physics::constants::world_gravity);
        sample.value = m_acceleration_samples[i].value - gravity_local;
        m_output_stream->samples[i] = sample;
    }

    //consume processed samples
    m_frame_samples.erase(m_frame_samples.begin(), m_frame_samples.begin() + count);
    m_acceleration_samples.erase(m_acceleration_samples.begin(), m_acceleration_samples.begin() + count);
}

auto Gravity_Filter::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("gravity_filter::set_config");

    sz::Gravity_Filter::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Gravity_Filter config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    auto frame_stream = m_hal.get_streams().find_by_name<stream::IFrame>(sz.inputs.frame);
    auto acceleration_stream = m_hal.get_streams().find_by_name<stream::IAcceleration>(sz.inputs.acceleration);

    auto rate = frame_stream ? frame_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config->inputs.frame.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.frame, m_output_stream->rate, rate);
        m_frame_stream.reset();
    }
    else
    {
        m_frame_stream = frame_stream;
    }

    rate = acceleration_stream ? acceleration_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config->inputs.acceleration.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.acceleration, m_output_stream->rate, rate);
        m_acceleration_stream.reset();
    }
    else
    {
        m_acceleration_stream = acceleration_stream;
    }

    return true;
}
auto Gravity_Filter::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Gravity_Filter::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}


}
}
