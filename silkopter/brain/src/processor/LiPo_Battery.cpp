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

constexpr uint32_t MIN_RATE = 50;

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
    if (m_params.full_charge < 0.01f)
    {
        QLOGE("Full charge is too small - {}", m_params.full_charge);
        return false;
    }
    if (m_params.voltage_stream->get_rate() != m_params.current_stream->get_rate())
    {
        QLOGE("Voltage stream and current stream have different rates: {} != {}", m_params.voltage_stream->get_rate(), m_params.current_stream->get_rate());
        return false;
    }
    if (m_params.voltage_stream->get_rate() < MIN_RATE)
    {
        QLOGE("Input streams has to be at least {}Hz. Now it's {}Hz", MIN_RATE, m_params.voltage_stream->get_rate());
        return false;
    }

    m_dt = std::chrono::microseconds(1000000 / m_stream.get_rate());

    m_stream.last_sample.sample_idx = 0;
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

    //accumulate the input streams
    {
        auto const& samples = get_input_current_stream().get_samples();
        m_current_samples.reserve(m_current_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_current_samples));
    }
    {
        auto const& samples = get_input_voltage_stream().get_samples();
        m_voltage_samples.reserve(m_voltage_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_voltage_samples));
    }

    //TODO add some protecton for severely out-of-sync streams

    size_t count = std::min(m_current_samples.size(), m_voltage_samples.size());
    if (count == 0)
    {
        return;
    }

    m_stream.samples.resize(count);

    stream::ICurrent::FILTER_CHANNEL_TYPE* c_channels[stream::ICurrent::FILTER_CHANNELS];
    stream::IVoltage::FILTER_CHANNEL_TYPE* v_channels[stream::IVoltage::FILTER_CHANNELS];

    for (size_t i = 0; i < count; i++)
    {
        m_stream.last_sample.dt = m_dt;
        m_stream.last_sample.sample_idx++;

        {
            auto const& s = m_current_samples[i];
            m_stream.last_sample.value.charge_used += s.value * q::Seconds(s.dt).count();
            stream::ICurrent::Value current = s.value;
            stream::ICurrent::setup_channels(c_channels, current);
            m_current_filter.process(1, c_channels);
            m_stream.last_sample.value.average_current = current;
        }
        {
            auto const& s = m_voltage_samples[i];
            stream::IVoltage::Value voltage = s.value;
            stream::IVoltage::setup_channels(v_channels, voltage);
            m_voltage_filter.process(1, v_channels);
            m_stream.last_sample.value.average_voltage = voltage;
        }
        m_stream.last_sample.value.capacity_left = 1.f - math::clamp(m_stream.last_sample.value.charge_used / m_params.full_charge, 0.f, 1.f);
        m_stream.samples[i] = m_stream.last_sample;
    }

    //consume processed samples
    m_current_samples.erase(m_current_samples.begin(), m_current_samples.begin() + count);
    m_voltage_samples.erase(m_voltage_samples.begin(), m_voltage_samples.begin() + count);

    //compute cell count
    if (!m_cell_count)
    {
        m_cell_count = compute_cell_count();
        if (m_cell_count)
        {
            QLOGI("Detected battery cell count: {} from voltage: {}V", m_cell_count, m_stream.last_sample.value.average_voltage);
//            if (m_loaded_state.cell_count > 0 && m_loaded_state.cell_count != *m_cell_count)
//            {
//                m_capacity_used_mah = 0;
//                QLOGI("Battery probably changed so ignoring saved state.");
//            }
        }
    }
}

auto LiPo_Battery::compute_cell_count() -> boost::optional<uint8_t>
{
    //wait to get a good voltage average
    if (m_stream.last_sample.sample_idx < CELL_COUNT_DETECTION_MIN_SAMPLES)
    {
        QLOGW("Skipping cell count detection: the voltage is not healthy: {}V from {} samples",
              m_stream.last_sample.value.average_voltage,
              m_stream.last_sample.sample_idx);
        return boost::none;
    }

    //detect the cell count only if the current consumption is not too big, otherwise the voltage drop will be significant
    if (!m_stream.last_sample.value.average_current > CELL_COUNT_DETECTION_MAX_CURRENT)
    {
        QLOGW("Skipping cell count detection: the current is not healthy: {}", m_stream.last_sample.value.average_current);
        return boost::none;
    }

    float v = m_stream.last_sample.value.average_voltage;

    //probably the is a faster, analytical way to find this without counting, but i'm not a mathematician!
    for (uint8_t i = 1; i < 30; i++)
    {
        float min_v = MIN_CELL_VOLTAGE * i;
        float max_v = MAX_CELL_VOLTAGE * i;
        if (v >= min_v && v <= max_v)
        {
            return i;
        }
    }
    return boost::none;
}


}
}
}
