#include "BrainStdAfx.h"
#include "LiPo_Battery.h"

#include "sz_math.hpp"
#include "sz_LiPo_Battery.hpp"

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
    , m_init_params(new sz::LiPo_Battery::Init_Params())
    , m_config(new sz::LiPo_Battery::Config())
{
}

auto LiPo_Battery::get_name() const -> std::string const&
{
    return m_init_params->name;
}

auto LiPo_Battery::init(rapidjson::Value const& init_params, rapidjson::Value const& config) -> bool
{
    QLOG_TOPIC("lipo_battery::init");

    sz::LiPo_Battery::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize LiPo_Battery data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init() && set_config(config);
}
auto LiPo_Battery::init() -> bool
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

auto LiPo_Battery::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs(2);
    inputs[0].class_id = q::rtti::get_class_id<stream::IVoltage>();
    inputs[0].name = "voltage";
    inputs[0].stream = m_voltage_stream;
    inputs[1].class_id = q::rtti::get_class_id<stream::ICurrent>();
    inputs[1].name = "current";
    inputs[1].stream = m_current_stream;
    return inputs;
}
auto LiPo_Battery::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].class_id = q::rtti::get_class_id<stream::IBattery_State>();
    outputs[0].name = "battery_state";
    outputs[0].stream = &m_stream;
    return outputs;
}

void LiPo_Battery::process()
{
    m_stream.samples.clear();

    if (!m_current_stream ||
        !m_voltage_stream)
    {
        return;
    }

    //accumulate the input streams
    {
        auto const& samples = m_current_stream->get_samples();
        m_current_samples.reserve(m_current_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_current_samples));
    }
    {
        auto const& samples = m_voltage_stream->get_samples();
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

    std::array<double, stream::ICurrent::FILTER_CHANNELS> c_channels;
    std::array<double, stream::IVoltage::FILTER_CHANNELS> v_channels;

    for (size_t i = 0; i < count; i++)
    {
        m_stream.last_sample.dt = m_dt;
        m_stream.last_sample.sample_idx++;

        {
            auto const& s = m_current_samples[i];
            m_stream.last_sample.value.charge_used += s.value * q::Seconds(s.dt).count();
            stream::ICurrent::Value current = s.value;
            if (stream::ICurrent::get_channels_from_value(c_channels, current))
            {
                m_current_filter.process(c_channels.data());
                stream::ICurrent::get_value_from_channels(current, c_channels);
            }
            m_stream.last_sample.value.average_current = current;
        }
        {
            auto const& s = m_voltage_samples[i];
            stream::IVoltage::Value voltage = s.value;
            if (stream::IVoltage::get_channels_from_value(v_channels, voltage))
            {
                m_voltage_filter.process(v_channels.data());
                stream::IVoltage::get_value_from_channels(voltage, v_channels);
            }
            m_stream.last_sample.value.average_voltage = voltage;
        }
        m_stream.last_sample.value.capacity_left = 1.f - math::clamp(m_stream.last_sample.value.charge_used / m_config->full_charge, 0.f, 1.f);
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
    if (m_stream.last_sample.value.average_current > CELL_COUNT_DETECTION_MAX_CURRENT)
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

auto LiPo_Battery::set_config(rapidjson::Value const& json) -> bool
{
    sz::LiPo_Battery::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize LiPo_Battery config data: {}", ss.str());
        return false;
    }

    auto* voltage_stream = m_hal.get_streams().find_by_name<stream::IVoltage>(sz.inputs.voltage);
    auto* current_stream = m_hal.get_streams().find_by_name<stream::ICurrent>(sz.inputs.current);
    if (!voltage_stream || voltage_stream->get_rate() == 0)
    {
        QLOGE("No input angular velocity stream specified");
        return false;
    }
    if (!current_stream || current_stream->get_rate() == 0)
    {
        QLOGE("No input current stream specified");
        return false;
    }
    if (current_stream->get_rate() != voltage_stream->get_rate())
    {
        QLOGE("Voltage and current streams have different rates: {} != {}",
              voltage_stream->get_rate(),
              current_stream->get_rate());
        return false;
    }

    if (m_stream.rate != 0 && m_stream.rate != current_stream->get_rate())
    {
        QLOGE("Input streams rate has changed: {} != {}",
              current_stream->get_rate(),
              m_stream.rate);
        return false;
    }
    if (sz.full_charge < 0.1f)
    {
        QLOGE("Full charge is too small: {}", sz.full_charge);
        return false;
    }

    m_dt = std::chrono::microseconds(1000000 / m_stream.get_rate());

    m_voltage_stream = voltage_stream;
    m_current_stream = current_stream;
    m_stream.rate = m_voltage_stream->get_rate();

    *m_config = sz;
    return true;
}
auto LiPo_Battery::get_config() -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto LiPo_Battery::get_init_params() -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}


}
}
}
