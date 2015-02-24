#include "BrainStdAfx.h"
#include "Inertial.h"
#include "physics/constants.h"

#include "sz_math.hpp"
#include "sz_hal_nodes.hpp"

namespace silk
{
namespace node
{
namespace processor
{

Inertial::Inertial(HAL& hal)
    : m_hal(hal)
{
}

auto Inertial::get_name() const -> std::string const&
{
    return m_params.name;
}

auto Inertial::init(rapidjson::Value const& json) -> bool
{
    sz::Inertial_Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Inertial data: {}", ss.str());
        return false;
    }
    Init_Params params;
    params.name = sz.name;
    params.reference_frame_stream = m_hal.get_streams().find_by_name<stream::IReference_Frame>(sz.reference_frame_stream);
    params.acceleration_stream = m_hal.get_streams().find_by_name<stream::IAcceleration>(sz.acceleration_stream);
    return init(params);
}
auto Inertial::init(Init_Params const& params) -> bool
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

auto Inertial::init() -> bool
{
    m_stream.params = &m_params;
    if (!m_params.reference_frame_stream)
    {
        QLOGE("No input reference frame stream specified");
        return false;
    }
    if (!m_params.acceleration_stream)
    {
        QLOGE("No input acceleration stream specified");
        return false;
    }
    if (m_params.acceleration_stream->get_rate() != m_params.reference_frame_stream->get_rate())
    {
        QLOGE("Reference Frame and Acceleration streams have different rates: {} != {}",
              m_params.reference_frame_stream->get_rate(),
              m_params.acceleration_stream->get_rate());
        return false;
    }

    m_dt = std::chrono::microseconds(1000000 / m_stream.get_rate());

    return true;
}

auto Inertial::get_input_stream_count() const -> size_t
{
    return 2;
}
auto Inertial::get_input_stream(size_t idx) -> stream::IStream&
{
    QASSERT(idx < get_input_stream_count());
    if (idx == 0)
    {
        return *m_params.reference_frame_stream;
    }
    else
    {
        return *m_params.acceleration_stream;
    }
}
auto Inertial::get_output_stream_count() const -> size_t
{
    return 1;
}
auto Inertial::get_output_stream(size_t idx) -> stream::ILinear_Acceleration&
{
    QASSERT(idx < get_output_stream_count());
    return m_stream;
}

void Inertial::process()
{
    m_stream.samples.clear();

    //accumulate the input streams
    {
        auto const& samples = m_params.reference_frame_stream->get_samples();
        m_reference_frame_samples.reserve(m_reference_frame_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_reference_frame_samples));
    }
    {
        auto const& samples = m_params.acceleration_stream->get_samples();
        m_acceleration_samples.reserve(m_acceleration_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_acceleration_samples));
    }

    //TODO add some protecton for severely out-of-sync streams

    size_t count = std::min(m_reference_frame_samples.size(), m_acceleration_samples.size());
    if (count == 0)
    {
        return;
    }

    m_stream.samples.resize(count);

    for (size_t i = 0; i < count; i++)
    {
        m_stream.last_sample.dt = m_dt;
        m_stream.last_sample.sample_idx++;

        auto w2l = math::inverse(m_reference_frame_samples[i].value.local_to_world);
        auto gravity_local = math::rotate(w2l, physics::constants::world_gravity);
        m_stream.last_sample.value = m_acceleration_samples[i].value - gravity_local;
        m_stream.samples[i] = m_stream.last_sample;
    }

    //consume processed samples
    m_reference_frame_samples.erase(m_reference_frame_samples.begin(), m_reference_frame_samples.begin() + count);
    m_acceleration_samples.erase(m_acceleration_samples.begin(), m_acceleration_samples.begin() + count);
}


}
}
}
