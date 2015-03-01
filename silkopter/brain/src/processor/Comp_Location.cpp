#include "BrainStdAfx.h"
#include "Comp_Location.h"

#include "sz_math.hpp"
#include "sz_Comp_Location.hpp"

namespace silk
{
namespace node
{
namespace processor
{

Comp_Location::Comp_Location(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Comp_Location::Init_Params())
    , m_config(new sz::Comp_Location::Config())
{
}

auto Comp_Location::get_name() const -> std::string const&
{
    return m_init_params->name;
}

auto Comp_Location::init(rapidjson::Value const& init_params, rapidjson::Value const& config) -> bool
{
    QLOG_TOPIC("comp_location::init");

    sz::Comp_Location::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Comp_Location data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init() && set_config(config);
}
auto Comp_Location::init() -> bool
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

auto Comp_Location::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs(3);
    inputs[0].class_id = q::rtti::get_class_id<stream::ILocation>();
    inputs[0].name = "location";
    inputs[0].stream = m_location_stream;
    inputs[1].class_id = q::rtti::get_class_id<stream::ILinear_Acceleration>();
    inputs[1].name = "linear_acceleration";
    inputs[1].stream = m_linear_acceleration_stream;
    inputs[2].class_id = q::rtti::get_class_id<stream::IPressure>();
    inputs[2].name = "pressure";
    inputs[2].stream = m_pressure_stream;
    return inputs;
}
auto Comp_Location::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].class_id = q::rtti::get_class_id<stream::ILocation>();
    outputs[0].name = "location";
    outputs[0].stream = &m_stream;
    return outputs;
}

void Comp_Location::process()
{
    m_stream.samples.clear();

    if (!m_location_stream ||
        !m_linear_acceleration_stream ||
        !m_pressure_stream)
    {
        return;
    }

    //accumulate the input streams
    {
        auto const& samples = m_location_stream->get_samples();
        m_location_samples.reserve(m_location_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_location_samples));
    }
    {
        auto const& samples = m_linear_acceleration_stream->get_samples();
        m_linear_acceleration_samples.reserve(m_linear_acceleration_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_linear_acceleration_samples));
    }
    {
        auto const& samples = m_pressure_stream->get_samples();
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


auto Comp_Location::set_config(rapidjson::Value const& json) -> bool
{
    sz::Comp_Location::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Comp_Location config data: {}", ss.str());
        return false;
    }

    auto* location_stream = m_hal.get_streams().find_by_name<stream::ILocation>(sz.inputs.location);
    auto* linear_acceleration_stream = m_hal.get_streams().find_by_name<stream::ILinear_Acceleration>(sz.inputs.linear_acceleration);
    auto* pressure_stream = m_hal.get_streams().find_by_name<stream::IPressure>(sz.inputs.pressure);
    if (!location_stream || location_stream->get_rate() == 0)
    {
        QLOGE("No input angular velocity stream specified");
        return false;
    }
    if (!linear_acceleration_stream || linear_acceleration_stream->get_rate() == 0)
    {
        QLOGE("No input acceleration stream specified");
        return false;
    }
    if (!pressure_stream || pressure_stream->get_rate() == 0)
    {
        QLOGE("No input magnetic field stream specified");
        return false;
    }
    if (linear_acceleration_stream->get_rate() != pressure_stream->get_rate() ||
        linear_acceleration_stream->get_rate() != location_stream->get_rate())
    {
        QLOGE("Angular velocity, Acceleration and Magnetic field streams have different rates: {} != {} != {}",
              location_stream->get_rate(),
              linear_acceleration_stream->get_rate(),
              pressure_stream->get_rate());
        return false;
    }

    if (m_stream.rate != 0 && m_stream.rate != linear_acceleration_stream->get_rate())
    {
        QLOGE("Input streams rate has changed: {} != {}",
              location_stream->get_rate(),
              m_stream.rate);
        return false;
    }

    m_dt = std::chrono::microseconds(1000000 / m_stream.get_rate());

    m_location_stream = location_stream;
    m_linear_acceleration_stream = linear_acceleration_stream;
    m_pressure_stream = pressure_stream;
    m_stream.rate = m_location_stream->get_rate();

    *m_config = sz;
    return true;
}
auto Comp_Location::get_config() -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Comp_Location::get_init_params() -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}

}
}
}
