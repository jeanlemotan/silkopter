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
    m_output_stream = std::make_shared<Output_Stream>();
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
    *m_init_params = sz;
    return init();
}
auto Gravity_Filter::init() -> bool
{
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    m_output_stream->set_rate(m_init_params->rate);
    return true;
}

auto Gravity_Filter::start(q::Clock::time_point tp) -> bool
{
    m_output_stream->set_tp(tp);
    return true;
}

auto Gravity_Filter::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IFrame::TYPE, m_init_params->rate, "Frame", m_accumulator.get_stream_path(0) },
        { stream::IAcceleration::TYPE, m_init_params->rate, "Acceleration", m_accumulator.get_stream_path(1) }
    }};
    return inputs;
}
auto Gravity_Filter::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
         { "Linear Acceleration", m_output_stream }
    }};
    return outputs;
}

void Gravity_Filter::process()
{
    QLOG_TOPIC("gravity_filter::process");

    m_output_stream->clear();

    m_accumulator.process([this](stream::IFrame::Sample const& f_sample,
                                stream::IAcceleration::Sample const& a_sample)
    {
        if (f_sample.is_healthy & a_sample.is_healthy)
        {
            auto p2l = math::inverse<float, math::safe>(f_sample.value);
            auto gravity_local = math::rotate(p2l, physics::constants::world_gravity);
            m_output_stream->push_sample(a_sample.value - gravity_local, true);
        }
        else
        {
            m_output_stream->push_last_sample(false);
        }
    });
}

void Gravity_Filter::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_hal);
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

    return true;
}
auto Gravity_Filter::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Gravity_Filter::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}

auto Gravity_Filter::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}

}
}
