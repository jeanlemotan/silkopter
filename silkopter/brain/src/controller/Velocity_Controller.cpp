#include "BrainStdAfx.h"
#include "Velocity_Controller.h"

#include "sz_math.hpp"
#include "sz_Velocity_Controller.hpp"

namespace silk
{
namespace node
{

Velocity_Controller::Velocity_Controller(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Velocity_Controller::Init_Params())
    , m_config(new sz::Velocity_Controller::Config())
{
    autojsoncxx::to_document(*m_init_params, m_init_paramsj);
}

auto Velocity_Controller::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("velocity_controller::init");

    sz::Velocity_Controller::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Velocity_Controller data: {}", ss.str());
        return false;
    }
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    *m_init_params = sz;
    return init();
}
auto Velocity_Controller::init() -> bool
{
    m_output_frame_stream = std::make_shared<Output_Frame_Stream>();
    m_output_force_stream = std::make_shared<Output_Force_Stream>();
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    m_output_frame_stream->set_rate(m_init_params->rate);
    m_output_frame_stream->set_tp(q::Clock::now());
    m_output_force_stream->set_rate(m_init_params->rate);
    m_output_force_stream->set_tp(q::Clock::now());
    return true;
}

auto Velocity_Controller::get_stream_inputs() const -> std::vector<Stream_Input>
{
    std::vector<Stream_Input> inputs =
    {{
        { stream::IVelocity::TYPE, m_init_params->rate, "Input" },
        { stream::IVelocity::TYPE, m_init_params->rate, "Target" }
    }};
    return inputs;
}
auto Velocity_Controller::get_stream_outputs() const -> std::vector<Stream_Output>
{
    std::vector<Stream_Output> outputs(2);
    outputs[0].type = stream::IFrame::TYPE;
    outputs[0].name = "Frame";
    outputs[0].stream = m_output_frame_stream;
    outputs[1].type = stream::IForce::TYPE;
    outputs[1].name = "Collective Frame";
    outputs[1].stream = m_output_force_stream;
    return outputs;
}

void Velocity_Controller::process()
{
    QLOG_TOPIC("velocity_controller::process");

    m_output_frame_stream->clear();
    m_output_force_stream->clear();

    m_accumulator.process([this](
                          size_t idx,
                          stream::IVelocity::Sample const& i_sample,
                          stream::IVelocity::Sample const& t_sample)
    {
//        math::vec3f ff = compute_feedforward(*multi_config, i_sample.value, t_sample.value);
//        math::vec3f fb = compute_feedback(i_sample.value, t_sample.value);

//        Output_Stream::Value value(ff * m_config->feedforward.weight + fb * m_config->feedback.weight);

//        m_output_stream->push_sample(value, i_sample.is_healthy & t_sample.is_healthy);
    });
}

auto Velocity_Controller::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("velocity_controller::set_config");

    sz::Velocity_Controller::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Velocity_Controller config data: {}", ss.str());
        return false;
    }

    *m_config = sz;
    m_accumulator.clear_streams();

    auto input_stream = m_hal.get_streams().find_by_name<stream::IVelocity>(sz.input_streams.input);
    auto target_stream = m_hal.get_streams().find_by_name<stream::IVelocity>(sz.input_streams.target);

    auto rate = input_stream ? input_stream->get_rate() : 0u;
    if (rate != m_init_params->rate)
    {
        m_config->input_streams.input.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.input, m_init_params->rate, rate);
    }
    else
    {
        m_accumulator.set_stream<0>(input_stream);
    }

    rate = target_stream ? target_stream->get_rate() : 0u;
    if (rate != m_init_params->rate)
    {
        m_config->input_streams.target.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.target, m_init_params->rate, rate);
    }
    else
    {
        m_accumulator.set_stream<1>(target_stream);
    }

    return true;
}
auto Velocity_Controller::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Velocity_Controller::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}
auto Velocity_Controller::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}


}
}
