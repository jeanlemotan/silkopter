#include "BrainStdAfx.h"
#include "LiPo_Battery.h"

#include "sz_math.hpp"
#include "sz_hal_nodes.hpp"

namespace silk
{
namespace node
{
namespace processor
{

constexpr std::chrono::milliseconds CURRENT_AVERAGE_LENGTH(500);
constexpr std::chrono::seconds VOLTAGE_AVERAGE_LENGTH(5);

constexpr size_t CELL_COUNT_DETECTION_MIN_SAMPLES = 5;
constexpr float CELL_COUNT_DETECTION_MAX_CURRENT = 0.8f;

constexpr size_t CAPACITY_DETECTION_MIN_SAMPLES = 5;
constexpr float CAPACITY_DETECTION_MAX_CURRENT = 0.4f;

constexpr float MIN_CELL_VOLTAGE = 2.9f;
constexpr float MAX_CELL_VOLTAGE = 4.1f;

LiPo_Battery::LiPo_Battery(HAL& hal)
    : m_hal(hal)
{
}

auto LiPo_Battery::get_name() const -> std::string const&
{
    return m_params.name;
}

auto LiPo_Battery::init(rapidjson::Value const& json) -> bool
{
    sz::LiPo_Battery sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize LiPo_Battery data: {}", ss.str());
        return false;
    }
    Init_Params params;
    params.name = sz.name;
    params.voltage_stream = m_hal.get_streams().find_by_name<stream::IVoltage>(sz.voltage_stream);
    params.current_stream = m_hal.get_streams().find_by_name<stream::ICurrent>(sz.current_stream);
    params.full_charge = sz.full_charge;
    return init(params);
}
auto LiPo_Battery::init(Init_Params const& params) -> bool
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

auto LiPo_Battery::init() -> bool
{
    m_stream.params = &m_params;
    if (!m_params.voltage_stream)
    {
        QLOGE("No input voltage specified");
        return false;
    }
    if (!m_params.current_stream)
    {
        QLOGE("No input current specified");
        return false;
    }
    if (m_params.voltage_stream->get_rate() > m_params.current_stream->get_rate())
    {
        QLOGE("Current stream has a bigger rate than the voltage stream: {} > {}", m_params.voltage_stream->get_rate(), m_params.current_stream->get_rate());
        return false;
    }
}

auto LiPo_Battery::get_input_stream_count() const -> size_t
{
    return 2;
}
auto LiPo_Battery::get_input_stream(size_t idx) -> stream::IStream&
{
    QASSERT(idx < get_input_stream_count());
    if (idx == 0)
    {
        return get_input_current_stream();
    }
    else
    {
        return get_input_voltage_stream();
    }
}
auto LiPo_Battery::get_output_stream_count() const -> size_t
{
    return 1;
}
auto LiPo_Battery::get_output_stream(size_t idx) -> stream::IStream&
{
    QASSERT(idx < get_output_stream_count());
    return get_output_battery_state_stream();
}
auto LiPo_Battery::get_input_voltage_stream() -> stream::IVoltage&
{
    QASSERT(m_params.voltage_stream);
    return *m_params.voltage_stream;
}
auto LiPo_Battery::get_input_current_stream() -> stream::ICurrent&
{
    QASSERT(m_params.current_stream);
    return *m_params.current_stream;
}
auto LiPo_Battery::get_output_battery_state_stream() -> stream::IBattery_State&
{
    return m_stream;
}


void LiPo_Battery::process()
{
    m_stream.samples.clear();
    auto const& s = get_input_current_stream().get_samples();
    m_stream.samples.resize(s.size());

//    std::transform(s.begin(), s.end(), m_stream.samples.begin(), [](stream::IADC_Value::Sample const& sample)
//    {
//       Stream::Sample vs;
//       vs.dt = sample.dt;
//       vs.sample_idx = sample.sample_idx;
//       vs.value = sample.value;
//       return vs;
//    });

    //TODO - apply scale - bias
}


}
}
}
