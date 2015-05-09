#include "BrainStdAfx.h"
#include "Rate_Controller.h"

#include "sz_math.hpp"
#include "sz_Rate_Controller.hpp"

namespace silk
{
namespace node
{

Rate_Controller::Rate_Controller(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Rate_Controller::Init_Params())
    , m_config(new sz::Rate_Controller::Config())
{
    autojsoncxx::to_document(*m_init_params, m_init_paramsj);
}

auto Rate_Controller::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("rate_controller::init");

    sz::Rate_Controller::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Rate_Controller data: {}", ss.str());
        return false;
    }
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    *m_init_params = sz;
    return init();
}
auto Rate_Controller::init() -> bool
{
    m_output_stream = std::make_shared<Stream>();
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    m_output_stream->rate = m_init_params->rate;

    return true;
}

auto Rate_Controller::get_stream_inputs() const -> std::vector<Stream_Input>
{
    std::vector<Stream_Input> inputs =
    {{
        { stream::IAngular_Velocity::TYPE, m_init_params->rate, "Input" },
        { stream::IAngular_Velocity::TYPE, m_init_params->rate, "Target" }
    }};
    return inputs;
}
auto Rate_Controller::get_stream_outputs() const -> std::vector<Stream_Output>
{
    std::vector<Stream_Output> outputs =
    {{
         { stream::ITorque::TYPE, "Torque", m_output_stream }
    }};
    return outputs;
}

void Rate_Controller::process()
{
    QLOG_TOPIC("rate_controller::process");

    m_output_stream->samples.clear();

    auto multi_config = m_hal.get_multi_config();
    if (!multi_config)
    {
        return;
    }

    m_accumulator.process([this, &multi_config](
                          size_t idx,
                          stream::IAngular_Velocity::Sample const& i_sample,
                          stream::IAngular_Velocity::Sample const& t_sample)
    {
        auto& sample = m_output_stream->last_sample;
        sample.dt = i_sample.dt;
        sample.tp = i_sample.tp;
        sample.sample_idx++;

        math::vec3f ff = compute_feedforward(*multi_config, i_sample.value, t_sample.value);
        math::vec3f fb = compute_feedback(i_sample.value, t_sample.value);

        sample.value.set(ff * m_config->feedforward.weight + fb * m_config->feedback.weight);

        m_output_stream->samples.push_back(sample);
    });
}


math::vec3f Rate_Controller::compute_feedforward(config::Multi& config, stream::IAngular_Velocity::Value const& input, stream::IAngular_Velocity::Value const& target)
{
    //F = m*a
    //v = a * t

    math::vec3f v = target - input;
    float vm = math::length(v) * config.moment_of_inertia;

    float max_T = m_config->feedforward.max_torque;

    float A = config.motor_acceleration;
    float C = config.motor_deceleration;

    float x_sq = vm / ((A + C) * max_T / 2.f);
    float x = math::min(1.f, math::sqrt(x_sq));

    return math::normalized<float, math::safe>(v) * max_T * x;
}
math::vec3f Rate_Controller::compute_feedback(stream::IAngular_Velocity::Value const& input, stream::IAngular_Velocity::Value const& target)
{
    float x = m_x_pid.process(input.x, target.x);
    float y = m_y_pid.process(input.y, target.y);
    float z = m_z_pid.process(input.z, target.z);
    return math::vec3f(x, y, z);
}

auto Rate_Controller::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("rate_controller::set_config");

    sz::Rate_Controller::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Rate_Controller config data: {}", ss.str());
        return false;
    }

    *m_config = sz;
    m_accumulator.clear_streams();

    auto input_stream = m_hal.get_streams().find_by_name<stream::IAngular_Velocity>(sz.input_streams.input);
    auto target_stream = m_hal.get_streams().find_by_name<stream::IAngular_Velocity>(sz.input_streams.target);

    auto rate = input_stream ? input_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config->input_streams.input.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.input, m_output_stream->rate, rate);
    }
    else
    {
        m_accumulator.set_stream<0>(input_stream);
    }

    rate = target_stream ? target_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config->input_streams.target.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.target, m_output_stream->rate, rate);
    }
    else
    {
        m_accumulator.set_stream<1>(target_stream);
    }

    m_config->feedback.weight = math::clamp(m_config->feedback.weight, 0.f, 1.f);
    m_config->feedforward.weight = math::clamp(m_config->feedforward.weight, 0.f, 1.f);

    m_config->feedforward.max_torque = math::max(m_config->feedforward.max_torque, 0.f);



    auto fill_params = [this](PID::Params& dst, sz::Rate_Controller::PID const& src)
    {
        dst.kp = src.kp;
        dst.ki = src.ki;
        dst.kd = src.kd;
        dst.max_i = src.max_i;
        dst.d_filter = src.d_filter;
        dst.rate = m_output_stream->rate;
    };

    PID::Params x_params, y_params, z_params;
    if (m_config->feedback.combined_xy_pid)
    {
        fill_params(x_params, m_config->feedback.xy_pid);
        fill_params(y_params, m_config->feedback.xy_pid);
    }
    else
    {
        fill_params(x_params, m_config->feedback.x_pid);
        fill_params(y_params, m_config->feedback.y_pid);
    }
    fill_params(z_params, m_config->feedback.z_pid);

    if (!m_x_pid.set_params(x_params) ||
        !m_y_pid.set_params(y_params) ||
        !m_z_pid.set_params(z_params))
    {
        QLOGE("Bad PID params");
        return false;
    }

    return true;
}
auto Rate_Controller::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    if (m_config->feedback.combined_xy_pid)
    {
        jsonutil::remove_value(json, q::Path("Feedback/X PID"));
        jsonutil::remove_value(json, q::Path("Feedback/Y PID"));
    }
    else
    {
        jsonutil::remove_value(json, q::Path("Feedback/XY PID"));
    }
    return std::move(json);
}

auto Rate_Controller::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}
auto Rate_Controller::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}


}
}
