#include "BrainStdAfx.h"
#include "AVRADC.h"
#include "physics/constants.h"
#include "utils/Timed_Scope.h"

#include "uav.def.h"
//#include "sz_AVRADC.hpp"

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
    , m_descriptor(new uav::AVRADC_Descriptor())
    , m_config(new uav::AVRADC_Config())
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
auto AVRADC::init(uav::INode_Descriptor const& descriptor) -> bool
{
    QLOG_TOPIC("AVRADC::init");

    auto specialized = dynamic_cast<uav::AVRADC_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }
    *m_descriptor = *specialized;

    return init();
}

auto AVRADC::init() -> bool
{
    m_i2c = m_uav.get_buses().find_by_name<bus::II2C>(m_descriptor->get_bus());

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


    m_adcs[0]->set_rate(m_descriptor->get_rate());
    m_adcs[1]->set_rate(m_descriptor->get_rate());

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

auto AVRADC::set_config(uav::INode_Config const& config) -> bool
{
    QLOG_TOPIC("AVRADC::set_config");

    auto specialized = dynamic_cast<uav::AVRADC_Config const*>(&config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }
    *m_config = *specialized;

    return true;
}
auto AVRADC::get_config() const -> std::shared_ptr<const uav::INode_Config>
{
    return m_config;
}

auto AVRADC::get_descriptor() const -> std::shared_ptr<const uav::INode_Descriptor>
{
    return m_descriptor;
}

//auto AVRADC::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}

}
}
