#include "BrainStdAfx.h"
#include "Comp_Location.h"

#include "sz_math.hpp"
#include "sz_hal_nodes.hpp"

namespace silk
{
namespace node
{
namespace processor
{

Comp_Location::Comp_Location(HAL& hal)
    : m_hal(hal)
{
}

auto Comp_Location::get_name() const -> std::string const&
{
    return m_params.name;
}

auto Comp_Location::init(rapidjson::Value const& json) -> bool
{
    sz::Comp_Location sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Comp_Location data: {}", ss.str());
        return false;
    }
    Init_Params params;
    params.name = sz.name;
    params.location_stream = m_hal.get_streams().find_by_name<stream::ILocation>(sz.location_stream);
    params.linear_acceleration_stream = m_hal.get_streams().find_by_name<stream::ILinear_Acceleration>(sz.linear_acceleration_stream);
    params.pressure_stream = m_hal.get_streams().find_by_name<stream::IPressure>(sz.pressure_stream);
    return init(params);
}
auto Comp_Location::init(Init_Params const& params) -> bool
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

auto Comp_Location::init() -> bool
{
    m_stream.params = &m_params;
    if (!m_params.location_stream)
    {
        QLOGE("No input location stream specified");
        return false;
    }
    if (!m_params.linear_acceleration_stream)
    {
        QLOGE("No input linear acceleration stream specified");
        return false;
    }
    if (!m_params.pressure_stream)
    {
        QLOGE("No input pressure stream specified");
        return false;
    }
    if (m_params.linear_acceleration_stream->get_rate() != m_params.location_stream->get_rate() ||
        m_params.location_stream->get_rate() != m_params.pressure_stream->get_rate())
    {
        QLOGE("Input streams have different rates:\n\tLocation: {}Hz\n\tLinear acceleration: {}Hz\n\tPressure: {}Hz",
              m_params.location_stream->get_rate(),
              m_params.linear_acceleration_stream->get_rate(),
              m_params.pressure_stream->get_rate());
        return false;
    }

    m_dt = std::chrono::microseconds(1000000 / m_stream.get_rate());

    return true;
}

auto Comp_Location::get_input_stream_count() const -> size_t
{
    return 3;
}
auto Comp_Location::get_input_stream(size_t idx) -> stream::IStream&
{
    QASSERT(idx < get_input_stream_count());
    std::array<stream::IStream*, 4> streams =
    {{
        m_params.location_stream, m_params.linear_acceleration_stream, m_params.pressure_stream
    }};
    return *streams[idx];
}
auto Comp_Location::get_output_stream_count() const -> size_t
{
    return 1;
}
auto Comp_Location::get_output_stream(size_t idx) -> stream::ILocation&
{
    QASSERT(idx < get_output_stream_count());
    return m_stream;
}

void Comp_Location::process()
{
    m_stream.samples.clear();

    //accumulate the input streams
    {
        auto const& samples = m_params.location_stream->get_samples();
        m_location_samples.reserve(m_location_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_location_samples));
    }
    {
        auto const& samples = m_params.linear_acceleration_stream->get_samples();
        m_linear_acceleration_samples.reserve(m_linear_acceleration_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_linear_acceleration_samples));
    }
    {
        auto const& samples = m_params.pressure_stream->get_samples();
        m_pressure_samples.reserve(m_pressure_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_pressure_samples));
    }

    //TODO add some protecton for severely out-of-sync streams

    size_t count = std::min(std::min(
                         m_location_samples.size(),
                         m_linear_acceleration_samples.size()),
                         m_pressure_samples.size());
    if (count == 0)
    {
        return;
    }

    m_stream.samples.resize(count);

    for (size_t i = 0; i < count; i++)
    {
        m_stream.last_sample.dt = m_dt;
        m_stream.last_sample.sample_idx++;

        m_stream.last_sample.value = m_location_samples[i].value;

        m_stream.samples[i] = m_stream.last_sample;
    }


    //consume processed samples
    m_location_samples.erase(m_location_samples.begin(), m_location_samples.begin() + count);
    m_linear_acceleration_samples.erase(m_linear_acceleration_samples.begin(), m_linear_acceleration_samples.begin() + count);
    m_pressure_samples.erase(m_pressure_samples.begin(), m_pressure_samples.begin() + count);
}


}
}
}
