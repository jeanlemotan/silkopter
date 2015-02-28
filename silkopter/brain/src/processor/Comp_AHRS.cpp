#include "BrainStdAfx.h"
#include "Comp_AHRS.h"

#include "sz_math.hpp"
#include "sz_Comp_AHRS.hpp"

namespace silk
{
namespace node
{
namespace processor
{

Comp_AHRS::Comp_AHRS(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Comp_AHRS::Init_Params())
    , m_config(new sz::Comp_AHRS::Config())
{
}

auto Comp_AHRS::get_name() const -> std::string const&
{
    return m_init_params->name;
}

auto Comp_AHRS::init(rapidjson::Value const& init_params, rapidjson::Value const& config) -> bool
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
    return init() && set_config(config);
}

auto Comp_AHRS::init() -> bool
{
    if (!m_init_params->name.empty())
    {
        m_stream.name = q::util::format2<std::string>("{}/stream", m_init_params->name);
        if (!m_hal.get_streams().add(m_stream))
        {
            return false;
        }
    }

    return true;
}

auto Comp_AHRS::get_input_stream_count() const -> size_t
{
    if (m_angular_velocity_stream && m_acceleration_stream && m_magnetic_field_stream)
    {
        return 3;
    }
    return 0;
}
auto Comp_AHRS::get_input_stream(size_t idx) -> stream::IStream&
{
    QASSERT(idx < get_input_stream_count());
    std::array<stream::IStream*, 3> streams =
    {{
        m_angular_velocity_stream, m_acceleration_stream, m_magnetic_field_stream
    }};
    QASSERT(streams.size() == get_input_stream_count());
    return *streams[idx];
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

    if (get_input_stream_count() == 0)
    {
        return;
    }

    //accumulate the input streams
    {
        auto const& samples = m_angular_velocity_stream->get_samples();
        m_angular_velocity_samples.reserve(m_angular_velocity_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_angular_velocity_samples));
    }
    {
        auto const& samples = m_acceleration_stream->get_samples();
        m_acceleration_samples.reserve(m_acceleration_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_acceleration_samples));
    }
    {
        auto const& samples = m_magnetic_field_stream->get_samples();
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

    auto* angular_velocity_stream = m_hal.get_streams().find_by_name<stream::IAngular_Velocity>(sz.inputs.angular_velocity);
    auto* acceleration_stream = m_hal.get_streams().find_by_name<stream::IAcceleration>(sz.inputs.acceleration);
    auto* magnetic_field_stream = m_hal.get_streams().find_by_name<stream::IMagnetic_Field>(sz.inputs.magnetic_field);
    if (!angular_velocity_stream || angular_velocity_stream->get_rate() == 0)
    {
        QLOGE("No input angular velocity stream specified");
        return false;
    }
    if (!acceleration_stream || acceleration_stream->get_rate() == 0)
    {
        QLOGE("No input acceleration stream specified");
        return false;
    }
    if (!magnetic_field_stream || magnetic_field_stream->get_rate() == 0)
    {
        QLOGE("No input magnetic field stream specified");
        return false;
    }
    if (acceleration_stream->get_rate() != magnetic_field_stream->get_rate() ||
        acceleration_stream->get_rate() != angular_velocity_stream->get_rate())
    {
        QLOGE("Angular velocity, Acceleration and Magnetic field streams have different rates: {} != {} != {}",
              angular_velocity_stream->get_rate(),
              acceleration_stream->get_rate(),
              magnetic_field_stream->get_rate());
        return false;
    }

    if (m_stream.rate != 0 && m_stream.rate != acceleration_stream->get_rate())
    {
        QLOGE("Input streams rate has changed: {} != {}",
              angular_velocity_stream->get_rate(),
              m_stream.rate);
        return false;
    }

    m_dt = std::chrono::microseconds(1000000 / m_stream.get_rate());

    m_angular_velocity_stream = angular_velocity_stream;
    m_acceleration_stream = acceleration_stream;
    m_magnetic_field_stream = magnetic_field_stream;
    m_stream.rate = m_angular_velocity_stream->get_rate();

    *m_config = sz;
    return true;
}
auto Comp_AHRS::get_config() -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Comp_AHRS::get_init_params() -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}

}
}
}
