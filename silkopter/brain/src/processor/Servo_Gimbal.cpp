#include "BrainStdAfx.h"
#include "Servo_Gimbal.h"

#include "sz_math.hpp"
#include "sz_Servo_Gimbal.hpp"

namespace silk
{
namespace node
{

Servo_Gimbal::Servo_Gimbal(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Servo_Gimbal::Init_Params())
    , m_config(new sz::Servo_Gimbal::Config())
{
    autojsoncxx::to_document(*m_init_params, m_init_paramsj);
}

auto Servo_Gimbal::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("servo_gimbal::init");

    sz::Servo_Gimbal::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Servo_Gimbal data: {}", ss.str());
        return false;
    }
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    *m_init_params = sz;
    return init();
}

auto Servo_Gimbal::init() -> bool
{
    m_x_output_stream = std::make_shared<Stream>();
    m_y_output_stream = std::make_shared<Stream>();
    m_z_output_stream = std::make_shared<Stream>();
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    m_x_output_stream->rate = m_init_params->rate;
    m_y_output_stream->rate = m_init_params->rate;
    m_z_output_stream->rate = m_init_params->rate;
    m_dt = std::chrono::microseconds(1000000 / m_init_params->rate);
    return true;
}

auto Servo_Gimbal::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IFrame::TYPE, m_init_params->rate, "Frame" },
        { stream::IFrame::TYPE, m_init_params->rate, "Target" }
    }};
    return inputs;
}
auto Servo_Gimbal::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(3);
    outputs[0].type = stream::IPWM::TYPE;
    outputs[0].name = "X PWM";
    outputs[0].stream = m_x_output_stream;
    outputs[1].type = stream::IPWM::TYPE;
    outputs[1].name = "Y PWM";
    outputs[1].stream = m_x_output_stream;
    outputs[2].type = stream::IPWM::TYPE;
    outputs[2].name = "Z PWM";
    outputs[2].stream = m_x_output_stream;
    return outputs;
}

void Servo_Gimbal::process()
{
    QLOG_TOPIC("servo_gimbal::process");

    m_x_output_stream->samples.clear();
    m_y_output_stream->samples.clear();
    m_z_output_stream->samples.clear();

    auto frame_stream = m_frame_stream.lock();
//    auto target_frame_stream = m_target_stream.lock();
    if (!frame_stream/* || !target_frame_stream*/)
    {
        return;
    }

    //accumulate the input streams
    {
        auto const& samples = frame_stream->get_samples();
        m_frame_samples.reserve(m_frame_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_frame_samples));
    }
//    {
//        auto const& samples = target_frame_stream->get_samples();
//        m_target_frame_samples.reserve(m_target_frame_samples.size() + samples.size());
//        std::copy(samples.begin(), samples.end(), std::back_inserter(m_target_frame_samples));
//    }

    //TODO add some protecton for severely out-of-sync streams

//    size_t count = std::min(m_frame_samples.size(), m_target_frame_samples.size());
    size_t count = m_frame_samples.size();
    if (count == 0)
    {
        return;
    }

    m_x_output_stream->samples.resize(count);
    m_y_output_stream->samples.resize(count);
    m_z_output_stream->samples.resize(count);

    math::vec3f rotation_euler;

    for (size_t i = 0; i < count; i++)
    {
        auto rotation = m_frame_samples[i].value.rotation;
        rotation.get_as_euler_xyz(rotation_euler.x, rotation_euler.y, rotation_euler.z);

        {
            auto& sample = m_x_output_stream->last_sample;
            sample.dt = m_dt;
            sample.tp = m_frame_samples[i].tp;
            sample.sample_idx++;

            auto const& config = m_config->outputs.x_pwm;

            math::anglef angle(rotation_euler.x);
            angle.normalize();
            float a = angle.radians;
            if (a > math::anglef::pi)
            {
                a = a - math::anglef::_2pi;
            }
            auto min_a = math::radians(config.min_angle);
            auto max_a = math::radians(config.max_angle);
            auto mu = (a - min_a) / (max_a - min_a);
            mu = math::clamp(mu, 0.f, 1.f);
            sample.value = mu * (config.max_pwm - config.min_pwm) + config.min_pwm;

            m_x_output_stream->samples[i] = sample;
        }
        {
            auto& sample = m_y_output_stream->last_sample;
            sample.dt = m_dt;
            sample.tp = m_frame_samples[i].tp;
            sample.sample_idx++;

            m_y_output_stream->samples[i] = sample;
        }
        {
            auto& sample = m_z_output_stream->last_sample;
            sample.dt = m_dt;
            sample.tp = m_frame_samples[i].tp;
            sample.sample_idx++;

            m_z_output_stream->samples[i] = sample;
        }
    }


    //consume processed samples
    m_frame_samples.erase(m_frame_samples.begin(), m_frame_samples.begin() + count);
//    m_target_frame_samples.erase(m_target_frame_samples.begin(), m_target_frame_samples.begin() + count);
}

auto Servo_Gimbal::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("servo_gimbal::set_config");

    sz::Servo_Gimbal::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Servo_Gimbal config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    auto frame_stream = m_hal.get_streams().find_by_name<stream::IFrame>(sz.inputs.frame);
    auto target_stream = m_hal.get_streams().find_by_name<stream::IFrame>(sz.inputs.target);

    auto rate = frame_stream ? frame_stream->get_rate() : 0u;
    if (rate != m_init_params->rate)
    {
        m_config->inputs.frame.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.frame, m_init_params->rate, rate);
        m_frame_stream.reset();
    }
    else
    {
        m_frame_stream = frame_stream;
    }

    rate = target_stream ? target_stream->get_rate() : 0u;
    if (rate != m_init_params->rate)
    {
        m_config->inputs.target.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.target, m_init_params->rate, rate);
        m_target_stream.reset();
    }
    else
    {
        m_target_stream = target_stream;
    }

    return true;
}
auto Servo_Gimbal::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Servo_Gimbal::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}

auto Servo_Gimbal::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}

}
}
