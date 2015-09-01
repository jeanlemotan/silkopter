#include "BrainStdAfx.h"
#include "Rate_Controller.h"

#include "sz_math.hpp"
#include "sz_PID.hpp"
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
    m_output_stream = std::make_shared<Output_Stream>();
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
    *m_init_params = sz;
    return init();
}
auto Rate_Controller::init() -> bool
{
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    m_output_stream->set_rate(m_init_params->rate);
    m_output_stream->set_tp(q::Clock::now());

    return true;
}

auto Rate_Controller::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IAngular_Velocity::TYPE, m_init_params->rate, "Input", m_accumulator.get_stream_path(0) },
        { stream::IAngular_Velocity::TYPE, m_init_params->rate, "Target", m_accumulator.get_stream_path(1) }
    }};
    return inputs;
}
auto Rate_Controller::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
         { "Torque", m_output_stream }
    }};
    return outputs;
}

void Rate_Controller::process()
{
    QLOG_TOPIC("rate_controller::process");

    m_output_stream->clear();

    auto multi_config = m_hal.get_multi_config();
    if (!multi_config)
    {
        return;
    }

    m_accumulator.process([this, &multi_config](stream::IAngular_Velocity::Sample const& i_sample,
                                                stream::IAngular_Velocity::Sample const& t_sample)
    {
        math::vec3f ff = compute_feedforward(*multi_config, i_sample.value, t_sample.value);
        math::vec3f fb = compute_feedback(i_sample.value, t_sample.value);

        Output_Stream::Value value(ff * m_config->feedforward.weight + fb * m_config->feedback.weight);

        m_output_stream->push_sample(value, i_sample.is_healthy & t_sample.is_healthy);
    });
}


math::vec3f Rate_Controller::compute_feedforward(config::Multi& config, stream::IAngular_Velocity::Value const& input, stream::IAngular_Velocity::Value const& target)
{
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

void Rate_Controller::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_hal);
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

    m_config->feedback.weight = math::clamp(m_config->feedback.weight, 0.f, 1.f);
    m_config->feedforward.weight = math::clamp(m_config->feedforward.weight, 0.f, 1.f);

    m_config->feedforward.max_torque = math::max(m_config->feedforward.max_torque, 0.f);

    auto fill_params = [this](PID::Params& dst, sz::PID const& src)
    {
        dst.kp = src.kp;
        dst.ki = src.ki;
        dst.kd = src.kd;
        dst.max_i = src.max_i;
        dst.d_filter = src.d_filter;
        dst.rate = m_output_stream->get_rate();
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

auto Rate_Controller::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}

auto Rate_Controller::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}

}
}
