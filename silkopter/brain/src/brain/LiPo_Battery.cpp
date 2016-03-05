#include "BrainStdAfx.h"
#include "LiPo_Battery.h"

namespace silk
{

constexpr uint32_t MIN_RATE = 50;

constexpr size_t CELL_COUNT_DETECTION_MIN_SAMPLES = 5;
constexpr float CELL_COUNT_DETECTION_MAX_CURRENT = 6.f;

constexpr size_t CAPACITY_DETECTION_MIN_SAMPLES = 5;
constexpr float CAPACITY_DETECTION_MAX_CURRENT = 0.4f;

constexpr float MIN_CELL_VOLTAGE = 2.9f;
constexpr float MAX_CELL_VOLTAGE = 4.1f;

LiPo_Battery::LiPo_Battery()
{
}

auto LiPo_Battery::init(size_t rate) -> bool
{
    if (rate == 0)
    {
        QLOGE("Bad rate: {}Hz", rate);
        return false;
    }

    m_rate = rate;

    if (!m_current_filter.setup(2, rate, 10.f) ||
        !m_voltage_filter.setup(2, rate, 2.f))
    {
        QLOGE("Cannot setup dsp filters");
        return false;
    }

    return true;
}

auto LiPo_Battery::process(stream::IVoltage::Sample const& v_sample, stream::ICurrent::Sample const& c_sample) -> stream::IBattery_State::Sample const&
{
    QLOG_TOPIC("lipo_battery::process");

    float dts = 1.f / m_rate;
    float current_factor = dts / 3600.f;

    if (c_sample.is_healthy & v_sample.is_healthy)
    {
        m_battery_state.is_healthy = true;
        m_battery_state.value.charge_used += c_sample.value * current_factor;
        stream::ICurrent::Value current = c_sample.value;
        m_current_filter.process(current);
        m_battery_state.value.average_current = current;

        stream::IVoltage::Value voltage = v_sample.value;
        m_voltage_filter.process(voltage);
        m_battery_state.value.average_voltage = voltage;

        m_battery_state.value.capacity_left = 1.f - math::clamp(m_battery_state.value.charge_used / m_config.full_charge, 0.f, 1.f);
    }
    else
    {
        m_battery_state.is_healthy = false;
    }

    //compute cell count
    if (!m_cell_count)
    {
        m_cell_count = compute_cell_count();
        if (m_cell_count)
        {
            QLOGI("Detected battery cell count: {} from voltage: {}V", m_cell_count, m_battery_state.value.average_voltage);
//            if (m_loaded_state.cell_count > 0 && m_loaded_state.cell_count != *m_cell_count)
//            {
//                m_capacity_used_mah = 0;
//                QLOGI("Battery probably changed so ignoring saved state.");
//            }
        }
    }

    return m_battery_state;
}

auto LiPo_Battery::compute_cell_count() -> boost::optional<uint8_t>
{
    QLOG_TOPIC("lipo_battery::compute_cell_count");

    auto const& value = m_battery_state.value;

    //wait to get a good voltage average
//    if (last_sample.sample_idx < CELL_COUNT_DETECTION_MIN_SAMPLES)
//    {
//        QLOGW("Skipping cell count detection: the voltage is not healthy: {}V from {} samples",
//              last_sample.value.average_voltage,
//              last_sample.sample_idx);
//        return boost::none;
//    }

    //detect the cell count only if the current consumption is not too big, otherwise the voltage drop will be significant
    if (value.average_current > CELL_COUNT_DETECTION_MAX_CURRENT)
    {
        QLOGW("Skipping cell count detection: the current is not healthy: {}", value.average_current);
        return boost::none;
    }

    float v = value.average_voltage;

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

auto LiPo_Battery::set_config(Config const& config) -> bool
{
    QLOG_TOPIC("lipo_battery::set_config");

    m_config = config;

    return true;
}

void LiPo_Battery::reset()
{
    m_battery_state.value.charge_used = m_config.full_charge;
    m_battery_state.value.capacity_left = 1.f;
}


}
