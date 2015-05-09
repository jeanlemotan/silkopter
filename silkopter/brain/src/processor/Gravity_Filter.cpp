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

auto Gravity_Filter::get_stream_inputs() const -> std::vector<Stream_Input>
{
    std::vector<Stream_Input> inputs =
    {{
        { stream::IFrame::TYPE, m_init_params->rate, "Frame" },
        { stream::IAcceleration::TYPE, m_init_params->rate, "Acceleration" }
    }};
    return inputs;
}
auto Gravity_Filter::get_stream_outputs() const -> std::vector<Stream_Output>
{
    std::vector<Stream_Output> outputs =
    {{
         { stream::ILinear_Acceleration::TYPE, "Linear Acceleration", m_output_stream }
    }};
    return outputs;
}

void Gravity_Filter::process()
{
    QLOG_TOPIC("gravity_filter::process");

    m_output_stream->samples.clear();

    m_accumulator.process([this](
                          size_t idx,
                          stream::IFrame::Sample const& f_sample,
                          stream::IAcceleration::Sample const& a_sample)
    {
        auto& sample = m_output_stream->last_sample;
        sample.dt = m_dt;
        sample.tp = f_sample.tp;
        sample.sample_idx++;

        auto p2l = math::inverse<float, math::safe>(f_sample.value.rotation);
        auto gravity_local = math::rotate(p2l, physics::constants::world_gravity);
        sample.value = a_sample.value - gravity_local;
        m_output_stream->samples.push_back(sample);
    });
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
    m_accumulator.clear_streams();

    auto frame_stream = m_hal.get_streams().find_by_name<stream::IFrame>(sz.input_streams.frame);
    auto acceleration_stream = m_hal.get_streams().find_by_name<stream::IAcceleration>(sz.input_streams.acceleration);

    auto rate = frame_stream ? frame_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config->input_streams.frame.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.frame, m_output_stream->rate, rate);
    }
    else
    {
        m_accumulator.set_stream<0>(frame_stream);
    }

    rate = acceleration_stream ? acceleration_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config->input_streams.acceleration.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.acceleration, m_output_stream->rate, rate);
    }
    else
    {
        m_accumulator.set_stream<1>(acceleration_stream);
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

auto Gravity_Filter::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}

}
}
