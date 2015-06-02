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
    m_output_stream = std::make_shared<Output_Stream>();
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    m_output_stream->set_rate(m_init_params->rate);
    m_output_stream->set_tp(q::Clock::now());

    m_current_filter.setup(2, m_init_params->rate, 10.0);
    m_voltage_filter.setup(2, m_init_params->rate, 2.0);

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
    std::vector<Stream_Output> outputs =
    {{
         { stream::IBattery_State::TYPE, "Battery State", m_output_stream }
    }};
    return outputs;
}

void LiPo_Battery::process()
{
    QLOG_TOPIC("lipo_battery::process");

    m_output_stream->clear();

    m_accumulator.process([this](size_t idx, stream::ICurrent::Sample const& current_sample, stream::IVoltage::Sample const& voltage_sample)
    {
        Output_Stream::Value value;

        {
            value.charge_used += current_sample.value * (q::Seconds(current_sample.dt).count() / 3600.f);
            stream::ICurrent::Value current = current_sample.value;
            m_current_filter.process(current);
            value.average_current = current;
        }
        {
            stream::IVoltage::Value voltage = voltage_sample.value;
            m_voltage_filter.process(voltage);
            value.average_voltage = voltage;
        }
        value.capacity_left = 1.f - math::clamp(value.charge_used / m_config->full_charge, 0.f, 1.f);

        m_output_stream->push_sample(value, current_sample.is_healthy & voltage_sample.is_healthy);
    });

    //compute cell count
    if (!m_cell_count)
    {
        m_cell_count = compute_cell_count();
        if (m_cell_count)
        {
            QLOGI("Detected battery cell count: {} from voltage: {}V", m_cell_count, m_output_stream->get_last_sample().value.average_voltage);
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

    auto const& last_sample = m_output_stream->get_last_sample();

    //wait to get a good voltage average
    if (last_sample.sample_idx < CELL_COUNT_DETECTION_MIN_SAMPLES)
    {
        QLOGW("Skipping cell count detection: the voltage is not healthy: {}V from {} samples",
              last_sample.value.average_voltage,
              last_sample.sample_idx);
        return boost::none;
    }

    //detect the cell count only if the current consumption is not too big, otherwise the voltage drop will be significant
    if (last_sample.value.average_current > CELL_COUNT_DETECTION_MAX_CURRENT)
    {
        QLOGW("Skipping cell count detection: the current is not healthy: {}", last_sample.value.average_current);
        return boost::none;
    }

    float v = last_sample.value.average_voltage;

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
    m_accumulator.clear_streams();

    auto output_rate = m_output_stream->get_rate();

    auto voltage_stream = m_hal.get_streams().find_by_name<stream::IVoltage>(sz.input_streams.voltage);
    auto current_stream = m_hal.get_streams().find_by_name<stream::ICurrent>(sz.input_streams.current);

    auto rate = voltage_stream ? voltage_stream->get_rate() : 0u;
    if (rate != output_rate)
    {
        m_config->input_streams.voltage.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.voltage, output_rate, rate);
    }
    else
    {
        m_accumulator.set_stream<1>(voltage_stream);
    }

    rate = current_stream ? current_stream->get_rate() : 0u;
    if (rate != output_rate)
    {
        m_config->input_streams.current.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.current, output_rate, rate);
    }
    else
    {
        m_accumulator.set_stream<0>(current_stream);
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
