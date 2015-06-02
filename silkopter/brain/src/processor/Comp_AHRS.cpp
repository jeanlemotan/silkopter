#include "BrainStdAfx.h"
#include "Comp_AHRS.h"

#include "sz_math.hpp"
#include "sz_Comp_AHRS.hpp"

namespace silk
{
namespace node
{

Comp_AHRS::Comp_AHRS(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Comp_AHRS::Init_Params())
    , m_config(new sz::Comp_AHRS::Config())
{
    autojsoncxx::to_document(*m_init_params, m_init_paramsj);
}

auto Comp_AHRS::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("comp_ahrs::init");

    sz::Comp_AHRS::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Comp_AHRS data: {}", ss.str());
        return false;
    }
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    *m_init_params = sz;
    return init();
}

auto Comp_AHRS::init() -> bool
{
    m_output_stream = std::make_shared<Output_Stream>();
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    m_output_stream->set_rate(m_init_params->rate);
    m_output_stream->set_tp(q::Clock::now());
    return true;
}

auto Comp_AHRS::get_stream_inputs() const -> std::vector<Stream_Input>
{
    std::vector<Stream_Input> inputs =
    {{
        { stream::IAngular_Velocity::TYPE, m_init_params->rate, "Angular Velocity" },
        { stream::IAcceleration::TYPE, m_init_params->rate, "Acceleration" },
        { stream::IMagnetic_Field::TYPE, m_init_params->rate, "Magnetic Field" }
    }};
    return inputs;
}
auto Comp_AHRS::get_stream_outputs() const -> std::vector<Stream_Output>
{
    std::vector<Stream_Output> outputs(1);
    outputs[0].type = stream::IFrame::TYPE;
    outputs[0].name = "Frame";
    outputs[0].stream = m_output_stream;
    return outputs;
}

void Comp_AHRS::process()
{
    QLOG_TOPIC("comp_ahrs::process");

    m_output_stream->clear();

    m_accumulator.process([this](
                          size_t idx,
                          stream::IAngular_Velocity::Sample const& av_sample,
                          stream::IAcceleration::Sample const& a_sample,
                          stream::IMagnetic_Field::Sample const& m_sample)
    {
        float av_length = 0;

        Output_Stream::Value value;

        {
            auto omega = av_sample.value;
            auto dts = q::Seconds(av_sample.dt).count();

            auto theta = omega * dts;
            auto theta_magnitude = math::length(theta);
            if (theta_magnitude > std::numeric_limits<float>::epsilon())
            {
                auto av = theta*0.5f;
                av_length = theta_magnitude;
                auto& a = value.rotation;
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

        {
            m_noisy_up_w = math::normalized<float, math::safe>(a_sample.value); //acceleration points opposite of gravity - so up
            m_noisy_front_w = math::normalized<float, math::safe>(m_sample.value); //this is always good
            m_noisy_right_w = math::normalized<float, math::safe>(math::cross(m_noisy_front_w, m_noisy_up_w));
            m_noisy_front_w = math::cross(m_noisy_right_w, m_noisy_up_w);
            //m_earth_frame_up = math::cross(m_earth_frame_right, m_earth_frame_front);

            math::mat3f mat(m_noisy_front_w, m_noisy_right_w, m_noisy_up_w);
            math::quatf noisy_quat;
            noisy_quat.set_from_mat3(mat);
            noisy_quat.invert();

            auto& rot = value.rotation;

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
                float mu = q::Seconds(m_output_stream->get_dt()).count() * 0.5f + av_length;
                rot = math::nlerp<float, math::safe>(rot, noisy_quat, mu);
            }
            rot = math::normalized<float, math::safe>(rot);
        }

        m_output_stream->push_sample(value, av_sample.is_healthy & a_sample.is_healthy & m_sample.is_healthy);
    });
}

auto Comp_AHRS::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("comp_ahrs::set_config");

    sz::Comp_AHRS::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Comp_AHRS config data: {}", ss.str());
        return false;
    }

    *m_config = sz;
    m_accumulator.clear_streams();

    auto output_rate = m_output_stream->get_rate();

    auto angular_velocity_stream = m_hal.get_streams().find_by_name<stream::IAngular_Velocity>(sz.input_streams.angular_velocity);
    auto acceleration_stream = m_hal.get_streams().find_by_name<stream::IAcceleration>(sz.input_streams.acceleration);
    auto magnetic_field_stream = m_hal.get_streams().find_by_name<stream::IMagnetic_Field>(sz.input_streams.magnetic_field);

    auto rate = angular_velocity_stream ? angular_velocity_stream->get_rate() : 0u;
    if (rate != output_rate)
    {
        m_config->input_streams.angular_velocity.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.angular_velocity, output_rate, rate);
    }
    else
    {
        m_accumulator.set_stream<0>(angular_velocity_stream);
    }

    rate = acceleration_stream ? acceleration_stream->get_rate() : 0u;
    if (rate != output_rate)
    {
        m_config->input_streams.acceleration.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.acceleration, output_rate, rate);
    }
    else
    {
        m_accumulator.set_stream<1>(acceleration_stream);
    }

    rate = magnetic_field_stream ? magnetic_field_stream->get_rate() : 0u;
    if (rate != output_rate)
    {
        m_config->input_streams.magnetic_field.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.magnetic_field, output_rate, rate);
    }
    else
    {
        m_accumulator.set_stream<2>(magnetic_field_stream);
    }

    return true;
}
auto Comp_AHRS::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Comp_AHRS::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}

auto Comp_AHRS::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}

}
}
