#include "BrainStdAfx.h"
#include "Servo_Gimbal.h"

#include "hal.def.h"
//#include "sz_Servo_Gimbal.hpp"

namespace silk
{
namespace node
{

Servo_Gimbal::Servo_Gimbal(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::Servo_Gimbal_Descriptor())
    , m_config(new hal::Servo_Gimbal_Config())
{
    m_x_output_stream = std::make_shared<Output_Stream>();
    m_y_output_stream = std::make_shared<Output_Stream>();
    m_z_output_stream = std::make_shared<Output_Stream>();
}

ts::Result<void> Servo_Gimbal::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("servo_gimbal::init");

    auto specialized = dynamic_cast<hal::Servo_Gimbal_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}

ts::Result<void> Servo_Gimbal::init()
{
    m_x_output_stream->set_rate(m_descriptor->get_rate());
    m_y_output_stream->set_rate(m_descriptor->get_rate());
    m_z_output_stream->set_rate(m_descriptor->get_rate());
    return ts::success;
}

ts::Result<void> Servo_Gimbal::start(q::Clock::time_point tp)
{
    m_x_output_stream->set_tp(tp);
    m_y_output_stream->set_tp(tp);
    m_z_output_stream->set_tp(tp);
    return ts::success;
}

auto Servo_Gimbal::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IUAV_Frame::TYPE, m_descriptor->get_rate(), "UAV Frame", m_frame_accumulator.get_stream_path(0) },
        { stream::IMultirotor_Commands::TYPE, m_descriptor->get_commands_rate(), "Commands", m_commands_accumulator.get_stream_path(0) }
    }};
    return inputs;
}
auto Servo_Gimbal::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
         { "x_pwm", m_x_output_stream },
         { "y_pwm", m_y_output_stream },
         { "z_pwm", m_z_output_stream }
    }};
    return outputs;
}

void Servo_Gimbal::process()
{
    QLOG_TOPIC("servo_gimbal::process");

    m_x_output_stream->clear();
    m_y_output_stream->clear();
    m_z_output_stream->clear();

    m_commands_accumulator.process([this](stream::IMultirotor_Commands::Sample const& i_commands)
    {
        m_commands_sample = i_commands;
    });

    m_frame_accumulator.process([this](stream::IUAV_Frame::Sample const& i_sample)
    {
        if (i_sample.is_healthy && m_commands_sample.is_healthy)
        {
            math::quatf rotation;


            if (m_commands_sample.value.gimbal.reference_frame.get() == stream::IMultirotor_Commands::Gimbal::Reference_Frame::GIMBAL)
            {
                rotation = m_commands_sample.value.gimbal.target_frame.get();
            }
            else
            {
                math::quatf const& target_rotation = m_commands_sample.value.gimbal.target_frame.get();
                rotation = math::inverse(i_sample.value) * target_rotation;
            }


            math::vec3f rotation_euler;
            rotation.get_as_euler_xyz(rotation_euler.x, rotation_euler.y, rotation_euler.z);

            {
                auto const& config = m_config->get_x_channel();

                math::anglef angle(rotation_euler.x);
                angle.normalize();
                float a = angle.radians;
                if (a > math::anglef::pi)
                {
                    a = a - math::anglef::_2pi;
                }
                auto min_a = math::radians(config.get_min_angle());
                auto max_a = math::radians(config.get_max_angle());
                auto mu = (a - min_a) / (max_a - min_a);
                mu = math::clamp(mu, 0.f, 1.f);

                m_x_output_stream->push_sample(mu * (config.get_max_pwm() - config.get_min_pwm()) + config.get_min_pwm(), true);
            }
            {
                auto const& config = m_config->get_y_channel();
                float mu = 0;
                m_y_output_stream->push_sample(mu * (config.get_max_pwm() - config.get_min_pwm()) + config.get_min_pwm(), true);
            }
            {
                auto const& config = m_config->get_z_channel();
                float mu = 0;
                m_z_output_stream->push_sample(mu * (config.get_max_pwm() - config.get_min_pwm()) + config.get_min_pwm(), true);
            }
        }
        else
        {
            m_x_output_stream->push_last_sample(false);
            m_y_output_stream->push_last_sample(false);
            m_z_output_stream->push_last_sample(false);
        }
    });

}

ts::Result<void> Servo_Gimbal::set_input_stream_path(size_t idx, std::string const& path)
{
    if (idx == 0)
    {
        return m_frame_accumulator.set_stream_path(0, path, m_descriptor->get_rate(), m_hal);
    }
    else if (idx == 1)
    {
        return m_commands_accumulator.set_stream_path(0, path, m_descriptor->get_commands_rate(), m_hal);
    }
    return make_error("Invalid stream index {}", idx);
}

ts::Result<void> Servo_Gimbal::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("servo_gimbal::set_config");

    auto specialized = dynamic_cast<hal::Servo_Gimbal_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    return ts::success;
}
auto Servo_Gimbal::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto Servo_Gimbal::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}

//auto Servo_Gimbal::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}

}
}
