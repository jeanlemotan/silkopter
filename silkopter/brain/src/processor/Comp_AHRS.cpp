#include "BrainStdAfx.h"
#include "Comp_AHRS.h"

#include "sz_math.hpp"
#include "sz_hal_nodes.hpp"

namespace silk
{
namespace node
{
namespace processor
{

Comp_AHRS::Comp_AHRS(HAL& hal)
    : m_hal(hal)
{
}

auto Comp_AHRS::get_name() const -> std::string const&
{
    return m_params.name;
}

auto Comp_AHRS::init(rapidjson::Value const& json) -> bool
{
    sz::Comp_AHRS_Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Comp_AHRS data: {}", ss.str());
        return false;
    }
    Init_Params params;
    params.name = sz.name;
    params.angular_velocity_stream = m_hal.get_streams().find_by_name<stream::IAngular_Velocity>(sz.angular_velocity_stream);
    params.acceleration_stream = m_hal.get_streams().find_by_name<stream::IAcceleration>(sz.acceleration_stream);
    params.magnetic_field_stream = m_hal.get_streams().find_by_name<stream::IMagnetic_Field>(sz.magnetic_field_stream);
    return init(params);
}
auto Comp_AHRS::init(Init_Params const& params) -> bool
{
    m_params = params;

    if (!init())
    {
        return false;
    }

    if (!m_params.name.empty())
    {
        m_stream.name = q::util::format2<std::string>("{}/stream", m_params.name);
        if (!m_hal.get_streams().add(m_stream))
        {
            return false;
        }
    }
    return true;
}

auto Comp_AHRS::init() -> bool
{
    m_stream.params = &m_params;
    if (!m_params.angular_velocity_stream)
    {
        QLOGE("No input angular velocity stream specified");
        return false;
    }
    if (!m_params.acceleration_stream)
    {
        QLOGE("No input acceleration stream specified");
        return false;
    }
    if (!m_params.magnetic_field_stream)
    {
        QLOGE("No input magnetic field stream specified");
        return false;
    }
    if (m_params.acceleration_stream->get_rate() != m_params.magnetic_field_stream->get_rate() ||
        m_params.acceleration_stream->get_rate() != m_params.angular_velocity_stream->get_rate())
    {
        QLOGE("Angular velocity, Acceleration and Magnetic field streams have different rates: {} != {} != {}",
              m_params.angular_velocity_stream->get_rate(),
              m_params.acceleration_stream->get_rate(),
              m_params.magnetic_field_stream->get_rate());
        return false;
    }

    m_dt = std::chrono::microseconds(1000000 / m_stream.get_rate());

    return true;
}

auto Comp_AHRS::get_input_stream_count() const -> size_t
{
    return 3;
}
auto Comp_AHRS::get_input_stream(size_t idx) -> stream::IStream&
{
    QASSERT(idx < get_input_stream_count());
    if (idx == 0)
    {
        return *m_params.angular_velocity_stream;
    }
    else if (idx == 1)
    {
        return *m_params.acceleration_stream;
    }
    else
    {
        return *m_params.magnetic_field_stream;
    }
}
auto Comp_AHRS::get_output_stream_count() const -> size_t
{
    return 1;
}
auto Comp_AHRS::get_output_stream(size_t idx) -> stream::IReference_Frame&
{
    QASSERT(idx < get_output_stream_count());
    return m_stream;
}

void Comp_AHRS::process()
{
    m_stream.samples.clear();

    //accumulate the input streams
    {
        auto const& samples = m_params.angular_velocity_stream->get_samples();
        m_angular_velocity_samples.reserve(m_angular_velocity_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_angular_velocity_samples));
    }
    {
        auto const& samples = m_params.acceleration_stream->get_samples();
        m_acceleration_samples.reserve(m_acceleration_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_acceleration_samples));
    }
    {
        auto const& samples = m_params.magnetic_field_stream->get_samples();
        m_magnetic_field_samples.reserve(m_magnetic_field_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_magnetic_field_samples));
    }

    //TODO add some protecton for severely out-of-sync streams

    size_t count = std::min(std::min(m_angular_velocity_samples.size(), m_acceleration_samples.size()), m_magnetic_field_samples.size());
    if (count == 0)
    {
        return;
    }

    m_stream.samples.resize(count);

    float av_length = 0;

    for (size_t i = 0; i < count; i++)
    {
        m_stream.last_sample.dt = m_dt;
        m_stream.last_sample.sample_idx++;

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
                auto& a = m_stream.last_sample.value.local_to_world;
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

            auto& rot = m_stream.last_sample.value.local_to_world;

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

        m_stream.samples[i] = m_stream.last_sample;
    }


    //consume processed samples
    m_angular_velocity_samples.erase(m_angular_velocity_samples.begin(), m_angular_velocity_samples.begin() + count);
    m_acceleration_samples.erase(m_acceleration_samples.begin(), m_acceleration_samples.begin() + count);
    m_magnetic_field_samples.erase(m_magnetic_field_samples.begin(), m_magnetic_field_samples.begin() + count);
}


}
}
}
