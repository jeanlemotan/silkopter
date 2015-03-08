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
    *m_init_params = sz;
    return init();
}

auto Comp_AHRS::init() -> bool
{
    m_output_stream = std::make_shared<Stream>();
    return true;
}

auto Comp_AHRS::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs(3);
    inputs[0].class_id = q::rtti::get_class_id<stream::IAngular_Velocity>();
    inputs[0].name = "Angular Velocity";
    inputs[1].class_id = q::rtti::get_class_id<stream::IAcceleration>();
    inputs[1].name = "Acceleration";
    inputs[2].class_id = q::rtti::get_class_id<stream::IMagnetic_Field>();
    inputs[2].name = "Magnetic Field";
    return inputs;
}
auto Comp_AHRS::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].class_id = q::rtti::get_class_id<stream::IReference_Frame>();
    outputs[0].name = "Reference Frame";
    outputs[0].stream = m_output_stream;
    return outputs;
}

void Comp_AHRS::process()
{
    m_output_stream->samples.clear();

    auto angular_velocity_stream = m_angular_velocity_stream.lock();
    auto acceleration_stream     = m_acceleration_stream.lock();
    auto magnetic_field_stream   = m_magnetic_field_stream.lock();
    if (!angular_velocity_stream ||
        !acceleration_stream ||
        !magnetic_field_stream)
    {
        return;
    }

    //accumulate the input streams
    {
        auto const& samples = angular_velocity_stream->get_samples();
        m_angular_velocity_samples.reserve(m_angular_velocity_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_angular_velocity_samples));
    }
    {
        auto const& samples = acceleration_stream->get_samples();
        m_acceleration_samples.reserve(m_acceleration_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_acceleration_samples));
    }
    {
        auto const& samples = magnetic_field_stream->get_samples();
        m_magnetic_field_samples.reserve(m_magnetic_field_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_magnetic_field_samples));
    }

    //TODO add some protecton for severely out-of-sync streams

    size_t count = std::min(std::min(m_angular_velocity_samples.size(), m_acceleration_samples.size()), m_magnetic_field_samples.size());
    if (count == 0)
    {
        return;
    }

    m_output_stream->samples.resize(count);

    float av_length = 0;

    for (size_t i = 0; i < count; i++)
    {
        m_output_stream->last_sample.dt = m_dt;
        m_output_stream->last_sample.sample_idx++;

        {
            auto const& s = m_angular_velocity_samples[i];
            auto omega = s.value;
            auto dts = q::Seconds(s.dt).count();

            auto theta = omega * dts;
            auto theta_magnitude = math::length(theta);
            if (theta_magnitude > std::numeric_limits<float>::epsilon())
            {
                auto av = theta*0.5f;
                av_length = theta_magnitude;
                auto& a = m_output_stream->last_sample.value.local_to_world;
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
            auto const& as = m_acceleration_samples[i];
            auto const& ms = m_magnetic_field_samples[i];

            m_noisy_up_w = math::normalized<float, math::safe>(as.value); //acceleration points opposite of gravity - so up
            m_noisy_front_w = math::normalized<float, math::safe>(ms.value); //this is always good
            m_noisy_right_w = math::normalized<float, math::safe>(math::cross(m_noisy_front_w, m_noisy_up_w));
            m_noisy_front_w = math::cross(m_noisy_right_w, m_noisy_up_w);
            //m_earth_frame_up = math::cross(m_earth_frame_right, m_earth_frame_front);

            math::mat3f mat(m_noisy_front_w, m_noisy_right_w, m_noisy_up_w);
            math::quatf noisy_quat;
            noisy_quat.set_from_mat3(mat);
            noisy_quat.invert();

            auto& rot = m_output_stream->last_sample.value.local_to_world;

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
                float mu = q::Seconds(m_dt).count() * 0.5f + av_length;
                rot = math::nlerp<float, math::safe>(rot, noisy_quat, mu);
            }
            rot = math::normalized<float, math::safe>(rot);
        }

        m_output_stream->samples[i] = m_output_stream->last_sample;
    }


    //consume processed samples
    m_angular_velocity_samples.erase(m_angular_velocity_samples.begin(), m_angular_velocity_samples.begin() + count);
    m_acceleration_samples.erase(m_acceleration_samples.begin(), m_acceleration_samples.begin() + count);
    m_magnetic_field_samples.erase(m_magnetic_field_samples.begin(), m_magnetic_field_samples.begin() + count);
}

auto Comp_AHRS::set_config(rapidjson::Value const& json) -> bool
{
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
    m_output_stream->rate = 0;

    auto angular_velocity_stream = m_hal.get_streams().find_by_name<stream::IAngular_Velocity>(sz.inputs.angular_velocity);
    auto acceleration_stream = m_hal.get_streams().find_by_name<stream::IAcceleration>(sz.inputs.acceleration);
    auto magnetic_field_stream = m_hal.get_streams().find_by_name<stream::IMagnetic_Field>(sz.inputs.magnetic_field);

    m_angular_velocity_stream = angular_velocity_stream;
    m_acceleration_stream = acceleration_stream;
    m_magnetic_field_stream = magnetic_field_stream;

    uint32_t output_stream_rate = 0;

    auto rate = angular_velocity_stream ? angular_velocity_stream->get_rate() : 0u;
    if (rate == 0 || (output_stream_rate > 0 && rate != output_stream_rate))
    {
        QLOGE("Bad input stream '{}'. Rate {}Hz", sz.inputs.angular_velocity, rate);
        return false;
    }
    output_stream_rate = rate;

    rate = acceleration_stream ? acceleration_stream->get_rate() : 0u;
    if (rate == 0 || (output_stream_rate > 0 && rate != output_stream_rate))
    {
        QLOGE("Bad input stream '{}'. Rate {}Hz", sz.inputs.acceleration, rate);
        return false;
    }
    output_stream_rate = rate;

    rate = magnetic_field_stream ? magnetic_field_stream->get_rate() : 0u;
    if (rate == 0 || (output_stream_rate > 0 && rate != output_stream_rate))
    {
        QLOGE("Bad input stream '{}'. Rate {}Hz", sz.inputs.magnetic_field, rate);
        return false;
    }
    output_stream_rate = rate;

    m_output_stream->rate = rate;
    m_dt = std::chrono::microseconds(1000000 / m_output_stream->rate);

    return true;
}
auto Comp_AHRS::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Comp_AHRS::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}

}
}
