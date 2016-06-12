#include "BrainStdAfx.h"
#include "Servo_Gimbal.h"

#include "uav.def.h"
//#include "sz_Servo_Gimbal.hpp"

namespace silk
{
namespace node
{

Servo_Gimbal::Servo_Gimbal(UAV& uav)
    : m_uav(uav)
    , m_descriptor(new uav::Servo_Gimbal_Descriptor())
    , m_config(new uav::Servo_Gimbal_Config())
{
    m_x_output_stream = std::make_shared<Output_Stream>();
    m_y_output_stream = std::make_shared<Output_Stream>();
    m_z_output_stream = std::make_shared<Output_Stream>();
}

auto Servo_Gimbal::init(uav::INode_Descriptor const& descriptor) -> bool
{
    QLOG_TOPIC("servo_gimbal::init");

    auto specialized = dynamic_cast<uav::Servo_Gimbal_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }
    *m_descriptor = *specialized;

    return init();
}

auto Servo_Gimbal::init() -> bool
{
    m_x_output_stream->set_rate(m_descriptor->get_rate());
    m_y_output_stream->set_rate(m_descriptor->get_rate());
    m_z_output_stream->set_rate(m_descriptor->get_rate());
    return true;
}

auto Servo_Gimbal::start(q::Clock::time_point tp) -> bool
{
    m_x_output_stream->set_tp(tp);
    m_y_output_stream->set_tp(tp);
    m_z_output_stream->set_tp(tp);
    return true;
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
         { "X PWM", m_x_output_stream },
         { "Y PWM", m_y_output_stream },
         { "Z PWM", m_z_output_stream }
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

void Servo_Gimbal::set_input_stream_path(size_t idx, q::Path const& path)
{
    if (idx == 0)
    {
        m_frame_accumulator.set_stream_path(0, path, m_descriptor->get_rate(), m_uav);
    }
    else if (idx == 1)
    {
        m_commands_accumulator.set_stream_path(0, path, m_descriptor->get_commands_rate(), m_uav);
    }
}

auto Servo_Gimbal::set_config(uav::INode_Config const& config) -> bool
{
    QLOG_TOPIC("servo_gimbal::set_config");

    auto specialized = dynamic_cast<uav::Servo_Gimbal_Config const*>(&config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }
    *m_config = *specialized;

    return true;
}
auto Servo_Gimbal::get_config() const -> std::shared_ptr<const uav::INode_Config>
{
    return m_config;
}

auto Servo_Gimbal::get_descriptor() const -> std::shared_ptr<const uav::INode_Descriptor>
{
    return m_descriptor;
}

//auto Servo_Gimbal::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}

}
}
