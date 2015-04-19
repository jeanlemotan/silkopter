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

auto Rate_Controller::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IAngular_Velocity::TYPE, m_init_params->rate, "Input" },
        { stream::IAngular_Velocity::TYPE, m_init_params->rate, "Target" }
    }};
    return inputs;
}
auto Rate_Controller::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].type = stream::ITorque::TYPE;
    outputs[0].name = "Torque";
    outputs[0].stream = m_output_stream;
    return outputs;
}

void Rate_Controller::process()
{
    QLOG_TOPIC("rate_controller::process");

    m_output_stream->samples.clear();

    auto target_stream = m_target_stream.lock();
    auto input_stream = m_input_stream.lock();
    if (!target_stream || !input_stream)
    {
        return;
    }

    //accumulate the input streams
    {
        auto const& samples = target_stream->get_samples();
        m_target_samples.reserve(m_target_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_target_samples));
    }
    {
        auto const& samples = input_stream->get_samples();
        m_input_samples.reserve(m_input_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_input_samples));
    }

    //TODO add some protecton for severely out-of-sync streams

    size_t count = std::min(m_target_samples.size(), m_input_samples.size());
    if (count == 0)
    {
        return;
    }

    m_output_stream->samples.resize(count);

    for (size_t i = 0; i < count; i++)
    {
        auto& sample = m_output_stream->last_sample;
        sample.dt = m_input_samples[i].dt;
        sample.tp = m_input_samples[i].tp;
        sample.sample_idx++;

        float x = m_x_pid.process(m_input_samples[i].value.x, m_target_samples[i].value.x);
        float y = m_y_pid.process(m_input_samples[i].value.y, m_target_samples[i].value.y);
        float z = m_z_pid.process(m_input_samples[i].value.z, m_target_samples[i].value.z);

        sample.value.set(x, y, z);

        m_output_stream->samples[i] = sample;
    }

    //consume processed samples
    m_target_samples.erase(m_target_samples.begin(), m_target_samples.begin() + count);
    m_input_samples.erase(m_input_samples.begin(), m_input_samples.begin() + count);
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

    auto input_stream = m_hal.get_streams().find_by_name<stream::IAngular_Velocity>(sz.inputs.input);
    auto target_stream = m_hal.get_streams().find_by_name<stream::IAngular_Velocity>(sz.inputs.target);

    auto rate = input_stream ? input_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config->inputs.input.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.input, m_output_stream->rate, rate);
        m_input_stream.reset();
    }
    else
    {
        m_input_stream = input_stream;
    }

    rate = target_stream ? target_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config->inputs.target.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.target, m_output_stream->rate, rate);
        m_target_stream.reset();
    }
    else
    {
        m_target_stream = target_stream;
    }

    auto fill_params = [this](PID::Params& dst, sz::Rate_Controller::PID const& src)
    {
        dst.kp = src.kp;
        dst.ki = src.ki;
        dst.kd = src.kd;
        dst.max_i = src.max_i;
        dst.filter_poles = src.filter_poles;
        dst.filter_cutoff_frequency = src.filter_cutoff_frequency;
        dst.rate = m_output_stream->rate;
    };

    PID::Params x_params, y_params, z_params;
    if (m_config->combined_xy_pid)
    {
        fill_params(x_params, m_config->xy_pid);
        fill_params(y_params, m_config->xy_pid);
    }
    else
    {
        fill_params(x_params, m_config->x_pid);
        fill_params(y_params, m_config->y_pid);
    }
    fill_params(z_params, m_config->z_pid);

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
    if (m_config->combined_xy_pid)
    {
        json.RemoveMember("X PID");
        json.RemoveMember("Y PID");
    }
    else
    {
        json.RemoveMember("XY PID");
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
