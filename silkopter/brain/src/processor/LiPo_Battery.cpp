#include "BrainStdAfx.h"
#include "LiPo_Battery.h"

#include "sz_math.hpp"
#include "sz_LiPo_Battery.hpp"

namespace silk
{
namespace node
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
    autojsoncxx::to_document(*m_init_params, m_init_paramsj);
}

auto LiPo_Battery::init(rapidjson::Value const& init_params) -> bool
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
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    *m_init_params = sz;
    return init();
}
auto LiPo_Battery::init() -> bool
{
    m_output_stream = std::make_shared<Stream>();
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    m_output_stream->rate = m_init_params->rate;
    m_dt = std::chrono::microseconds(1000000 / m_output_stream->rate);
    return true;
}

auto LiPo_Battery::get_stream_inputs() const -> std::vector<Stream_Input>
{
    std::vector<Stream_Input> inputs =
    {{
        { stream::IVoltage::TYPE, m_init_params->rate, "Voltage" },
        { stream::ICurrent::TYPE, m_init_params->rate, "Current" }
    }};
    return inputs;
}
auto LiPo_Battery::get_stream_outputs() const -> std::vector<Stream_Output>
{
    std::vector<Stream_Output> outputs(1);
    outputs[0].type = stream::IBattery_State::TYPE;
    outputs[0].name = "Battery State";
    outputs[0].stream = m_output_stream;
    return outputs;
}

void LiPo_Battery::process()
{
    QLOG_TOPIC("lipo_battery::process");

    m_output_stream->samples.clear();

    auto current_stream = m_current_stream.lock();
    auto voltage_stream = m_voltage_stream.lock();
    if (!current_stream || !voltage_stream)
    {
        return;
    }

    //accumulate the input streams
    {
        auto const& samples = current_stream->get_samples();
        m_current_samples.reserve(m_current_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_current_samples));
    }
    {
        auto const& samples = voltage_stream->get_samples();
        m_voltage_samples.reserve(m_voltage_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_voltage_samples));
    }

    //TODO add some protecton for severely out-of-sync streams

    size_t count = std::min(m_current_samples.size(), m_voltage_samples.size());
    if (count == 0)
    {
        return;
    }

    m_output_stream->samples.resize(count);

    for (size_t i = 0; i < count; i++)
    {
        auto& sample = m_output_stream->last_sample;
        sample.dt = m_dt;
        sample.tp = m_current_samples[i].tp;
        sample.sample_idx++;

        {
            auto const& s = m_current_samples[i];
            sample.value.charge_used += s.value * q::Seconds(s.dt).count();
            stream::ICurrent::Value current = s.value;
            if (s.is_healthy)
            {
                m_current_filter.process(current);
            }
            sample.value.average_current = current;
        }
        {
            auto const& s = m_voltage_samples[i];
            stream::IVoltage::Value voltage = s.value;
            if (s.is_healthy)
            {
                m_voltage_filter.process(voltage);
            }
            sample.value.average_voltage = voltage;
        }
        sample.value.capacity_left =
                1.f - math::clamp(sample.value.charge_used / m_config->full_charge, 0.f, 1.f);

        m_output_stream->samples[i] = sample;
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
            QLOGI("Detected battery cell count: {} from voltage: {}V", m_cell_count, m_output_stream->last_sample.value.average_voltage);
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
    QLOG_TOPIC("lipo_battery::compute_cell_count");

    //wait to get a good voltage average
    if (m_output_stream->last_sample.sample_idx < CELL_COUNT_DETECTION_MIN_SAMPLES)
    {
        QLOGW("Skipping cell count detection: the voltage is not healthy: {}V from {} samples",
              m_output_stream->last_sample.value.average_voltage,
              m_output_stream->last_sample.sample_idx);
        return boost::none;
    }

    //detect the cell count only if the current consumption is not too big, otherwise the voltage drop will be significant
    if (m_output_stream->last_sample.value.average_current > CELL_COUNT_DETECTION_MAX_CURRENT)
    {
        QLOGW("Skipping cell count detection: the current is not healthy: {}", m_output_stream->last_sample.value.average_current);
        return boost::none;
    }

    float v = m_output_stream->last_sample.value.average_voltage;

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
    QLOG_TOPIC("lipo_battery::set_config");

    sz::LiPo_Battery::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize LiPo_Battery config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    auto voltage_stream = m_hal.get_streams().find_by_name<stream::IVoltage>(sz.input_streams.voltage);
    auto current_stream = m_hal.get_streams().find_by_name<stream::ICurrent>(sz.input_streams.current);

    auto rate = voltage_stream ? voltage_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config->input_streams.voltage.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.voltage, m_output_stream->rate, rate);
        m_voltage_stream.reset();
    }
    else
    {
        m_voltage_stream = voltage_stream;
    }

    rate = current_stream ? current_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config->input_streams.current.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.current, m_output_stream->rate, rate);
        m_current_stream.reset();
    }
    else
    {
        m_current_stream = current_stream;
    }

    return true;
}
auto LiPo_Battery::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto LiPo_Battery::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}
auto LiPo_Battery::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}


}
}
