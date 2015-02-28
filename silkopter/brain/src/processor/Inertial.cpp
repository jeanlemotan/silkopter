#include "BrainStdAfx.h"
#include "Inertial.h"
#include "physics/constants.h"

#include "sz_math.hpp"
#include "sz_Inertial.hpp"

namespace silk
{
namespace node
{
namespace processor
{

Inertial::Inertial(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Inertial::Init_Params())
    , m_config(new sz::Inertial::Config())
{
}

auto Inertial::get_name() const -> std::string const&
{
    return m_init_params->name;
}

auto Inertial::init(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("inertial::init");

    sz::Inertial::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Inertial data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    autojsoncxx::to_document(sz, m_init_params_json);

    return init();
}
auto Inertial::init() -> bool
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

auto Inertial::get_input_stream_count() const -> size_t
{
    if (m_reference_frame_stream && m_acceleration_stream)
    {
        return 2;
    }
    return 0;
}
auto Inertial::get_input_stream(size_t idx) -> stream::IStream&
{
    QASSERT(idx < get_input_stream_count());
    std::array<stream::IStream*, 2> streams =
    {{
        m_reference_frame_stream, m_acceleration_stream
    }};
    QASSERT(streams.size() == get_input_stream_count());
    return *streams[idx];
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

    if (get_input_stream_count() == 0)
    {
        return;
    }

    //accumulate the input streams
    {
        auto const& samples = m_reference_frame_stream->get_samples();
        m_reference_frame_samples.reserve(m_reference_frame_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_reference_frame_samples));
    }
    {
        auto const& samples = m_acceleration_stream->get_samples();
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

auto Inertial::set_config(rapidjson::Value const& json) -> bool
{
    sz::Inertial::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Inertial config data: {}", ss.str());
        return false;
    }

    auto* reference_frame_stream = m_hal.get_streams().find_by_name<stream::IReference_Frame>(sz.inputs.reference_frame);
    auto* acceleration_stream = m_hal.get_streams().find_by_name<stream::IAcceleration>(sz.inputs.acceleration);
    if (!reference_frame_stream || reference_frame_stream->get_rate() == 0)
    {
        QLOGE("No input angular velocity stream specified");
        return false;
    }
    if (!acceleration_stream || acceleration_stream->get_rate() == 0)
    {
        QLOGE("No input acceleration stream specified");
        return false;
    }
    if (acceleration_stream->get_rate() != reference_frame_stream->get_rate())
    {
        QLOGE("Reference Frame and Acceleration streams have different rates: {} != {}",
              reference_frame_stream->get_rate(),
              acceleration_stream->get_rate());
        return false;
    }

    if (m_stream.rate != 0 && m_stream.rate != acceleration_stream->get_rate())
    {
        QLOGE("Input streams rate has changed: {} != {}",
              acceleration_stream->get_rate(),
              m_stream.rate);
        return false;
    }

    m_dt = std::chrono::microseconds(1000000 / m_stream.get_rate());

    m_reference_frame_stream = reference_frame_stream;
    m_acceleration_stream = acceleration_stream;
    m_stream.rate = m_reference_frame_stream->get_rate();

    *m_config = sz;
    autojsoncxx::to_document(*m_config, m_config_json);
    return true;
}
auto Inertial::get_config() -> boost::optional<rapidjson::Value const&>
{
    return m_config_json;
}
auto Inertial::get_init_params() -> boost::optional<rapidjson::Value const&>
{
    return m_init_params_json;
}


}
}
}
