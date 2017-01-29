#include "BrainStdAfx.h"
#include "Comp_AHRS.h"

#include "hal.def.h"
//#include "sz_Comp_AHRS.hpp"

namespace silk
{
namespace node
{

Comp_AHRS::Comp_AHRS(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::Comp_AHRS_Descriptor())
    , m_config(new hal::Comp_AHRS_Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

ts::Result<void> Comp_AHRS::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("comp_ahrs::init");

    auto specialized = dynamic_cast<hal::Comp_AHRS_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}

ts::Result<void> Comp_AHRS::init()
{
    m_output_stream->set_rate(m_descriptor->get_rate());
    return ts::success;
}

ts::Result<void> Comp_AHRS::start(Clock::time_point tp)
{
    m_output_stream->set_tp(tp);
    return ts::success;
}

auto Comp_AHRS::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IAngular_Velocity::TYPE, m_descriptor->get_rate(), "angular_velocity", m_accumulator.get_stream_path(0) },
        { stream::IAcceleration::TYPE, m_descriptor->get_rate(), "acceleration", m_accumulator.get_stream_path(1) },
        { stream::IMagnetic_Field::TYPE, m_descriptor->get_rate(), "magnetic_field", m_accumulator.get_stream_path(2) }
    }};
    return inputs;
}
auto Comp_AHRS::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].name = "frame";
    outputs[0].stream = m_output_stream;
    return outputs;
}

void Comp_AHRS::process()
{
    QLOG_TOPIC("comp_ahrs::process");

    m_output_stream->clear();

    auto dts = std::chrono::duration<float>(m_output_stream->get_dt()).count();

    m_accumulator.process([this, dts](stream::IAngular_Velocity::Sample const& av_sample,
                                      stream::IAcceleration::Sample const& a_sample,
                                      stream::IMagnetic_Field::Sample const& mf_sample)
    {
        float av_length = 0;

        math::quatf rotation(m_output_stream->get_last_sample().value);

        if (av_sample.is_healthy)
        {
            math::vec3f omega(av_sample.value);

            math::vec3f theta = omega * dts;
            float theta_magnitude = math::length(theta);
            if (theta_magnitude > std::numeric_limits<float>::epsilon())
            {
                auto av = theta*0.5f;
                av_length = theta_magnitude;
                auto& a = rotation;
                float w = /*(av.w * a.w)*/ - (av.x * a.x) - (av.y * a.y) - (av.z * a.z);
                float x = (av.x * a.w) /*+ (av.w * a.x)*/ + (av.z * a.y) - (av.y * a.z);
                float y = (av.y * a.w) /*+ (av.w * a.y)*/ + (av.x * a.z) - (av.z * a.x);
                float z = (av.z * a.w) /*+ (av.w * a.z)*/ + (av.y * a.x) - (av.x * a.y);

                a.x += x;
                a.y += y;
                a.z += z;
                a.w += w;
                a.normalize<math::safe>();
                //m_angular_velocity = gyroscope.value;
            }
        }

        if (a_sample.is_healthy && mf_sample.is_healthy)
        {
            m_noisy_up_w = math::normalized<float, math::safe>(a_sample.value); //acceleration points opposite of gravity - so up
            math::vec3f front = mf_sample.value;
            //sanity checks - zero magnetic field, colinear mf and acc vectors
            if (math::is_zero(math::length_sq(front), math::epsilon<float>()))
            {
                QLOGW("zero front vector - invalid magnetic field!!!!!");
                front = math::vec3f(0, 1, 0);
            }
            front = math::normalized<float, math::safe>(front);
            if (math::abs(math::dot(m_noisy_up_w, front)) < 0.99f)
            {
                m_noisy_front_w = front;
            }
            else
            {
                QLOGW("colinear front and up vectors - using the previous front vector!!!!!");
            }

            m_noisy_right_w = math::normalized<float, math::safe>(math::cross(m_noisy_front_w, m_noisy_up_w));
            m_noisy_front_w = math::cross(m_noisy_up_w, m_noisy_right_w);

            math::mat3f mat(m_noisy_right_w /* X */, m_noisy_front_w /* Y */, m_noisy_up_w /* Z */);
            math::quatf noisy_quat;
            noisy_quat.set_from_mat3(mat);
            noisy_quat.invert();

            math::quatf& rot = rotation;

            //cancel drift
            static int xxx = 50;
            if (xxx > 0)
            {
                xxx--;
                rot = noisy_quat;
            }
            else
            {
                //take the rate of rotation into account here - the quicker the rotation the bigger the mu
                //like this we compensate for gyro saturation errors
                float mu = (dts + av_length) * m_config->get_drift_correction_factor();
                rot = math::nlerp<float, math::safe>(rot, noisy_quat, mu);
            }
            rot = math::normalized<float, math::safe>(rot);
        }

        m_output_stream->push_sample(rotation, av_sample.is_healthy & a_sample.is_healthy & mf_sample.is_healthy);
    });
}

ts::Result<void> Comp_AHRS::set_input_stream_path(size_t idx, std::string const& path)
{
    return m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_hal);
}

ts::Result<void> Comp_AHRS::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("comp_ahrs::set_config");

    auto specialized = dynamic_cast<hal::Comp_AHRS_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    //m_config->drift_correction_factor = math::clamp(m_config->drift_correction_factor, 0.f, 1.f);

    return ts::success;
}
auto Comp_AHRS::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto Comp_AHRS::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}

ts::Result<std::shared_ptr<messages::INode_Message>> Comp_AHRS::send_message(messages::INode_Message const& message)
{
    return make_error("Unknown message");
}

}
}
