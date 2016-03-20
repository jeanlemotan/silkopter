#include "BrainStdAfx.h"
#include "AVRADC.h"
#include "physics/constants.h"
#include "utils/Timed_Scope.h"

#include "sz_math.hpp"
#include "sz_AVRADC.hpp"

namespace silk
{
namespace node
{


constexpr uint8_t AVRADC_ADDRESS            = 0x17;
constexpr uint8_t AVRADC_REG_ADC0           = 0x0;
constexpr uint8_t AVRADC_REG_ADC1           = 0x2;

constexpr std::chrono::milliseconds MIN_CONVERSION_DURATION(5);

AVRADC::AVRADC(UAV& uav)
    : m_uav(uav)
    , m_init_params(new sz::AVRADC::Init_Params())
    , m_config(new sz::AVRADC::Config())
{
    for (auto& adc: m_adcs)
    {
        adc = std::make_shared<Output_Stream>();
    }
}

auto AVRADC::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
        { "ADC0", m_adcs[0] },
        { "ADC1", m_adcs[1] }
     }};
    return outputs;
}
auto AVRADC::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("AVRADC::init");

    sz::AVRADC::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize AVRADC data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init();
}

auto AVRADC::init() -> bool
{
    m_i2c = m_uav.get_buses().find_by_name<bus::II2C>(m_init_params->bus);

    auto i2c = m_i2c.lock();
    if (!i2c)
    {
        QLOGE("No bus configured");
        return false;
    }

    i2c->lock();
    At_Exit at_exit([this, &i2c]()
    {
        i2c->unlock();
    });


    m_init_params->rate = math::clamp<size_t>(m_init_params->rate, 1, 500);

    m_adcs[0]->set_rate(m_init_params->rate);
    m_adcs[1]->set_rate(m_init_params->rate);

    return true;
}

auto AVRADC::start(q::Clock::time_point tp) -> bool
{
    m_last_process_tp = tp;
    for (auto& adc: m_adcs)
    {
        adc->set_tp(tp);
    }
    return true;
}


void AVRADC::process()
{
    QLOG_TOPIC("AVRADC::process");

    for (auto& adc: m_adcs)
    {
        adc->clear();
    }

    auto i2c = m_i2c.lock();
    if (!i2c)
    {
        return;
    }

    i2c->lock();
    At_Exit at_exit([this, &i2c]()
    {
        i2c->unlock();
    });

    auto now = q::Clock::now();
    if (now - m_last_process_tp < MIN_CONVERSION_DURATION)
    {
        return;
    }
    m_last_process_tp = now;

    //TODO - add healthy indication
    uint8_t data[4] = {0};
    if (i2c->read(AVRADC_ADDRESS, data, sizeof(data)))
    {
        for (size_t i = 0; i < m_adcs.size(); i++)
        {
            uint16_t idata = static_cast<uint16_t>((data[1 + i*2] << 8) | data[0 + i*2]);
            float value = math::clamp(idata / 32768.f, 0.f, 1.f);
            m_last_values[i] = value;
        }
        m_last_reading_tp = now;
    }
    else
    {
        m_stats.bus_failures++;
    }

    constexpr size_t k_max_sample_difference = 5;

    for (size_t i = 0; i < m_adcs.size(); i++)
    {
        //add samples up to the sample rate
        size_t samples_needed = m_adcs[i]->compute_samples_needed();
        bool is_healthy = q::Clock::now() - m_last_reading_tp <= m_adcs[0]->get_dt() * k_max_sample_difference;
        if (!is_healthy)
        {
            m_stats.added += samples_needed;
        }

        while (samples_needed > 0)
        {
            m_adcs[i]->push_sample(m_last_values[i], is_healthy);
            samples_needed--;
        }
    }

    if (m_stats.last_report_tp + std::chrono::seconds(1) < now)
    {
        if (m_stats != Stats())
        {
            QLOGW("Stats: a{}, bus{}",
                        m_stats.added,
                        m_stats.bus_failures);
        }
        m_stats = Stats();
        m_stats.last_report_tp = now;
    }
}

auto AVRADC::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("AVRADC::set_config");

    sz::AVRADC::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize AVRADC config data: {}", ss.str());
        return false;
    }

    *m_config = sz;
    return true;
}
auto AVRADC::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto AVRADC::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}

auto AVRADC::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}

}
}
