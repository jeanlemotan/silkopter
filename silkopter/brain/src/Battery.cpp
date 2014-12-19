#include "BrainStdAfx.h"
#include "Battery.h"

namespace silk
{


void Battery::process(const std::vector<Current_Sample>& current_samples, const std::vector<Voltage_Sample>& voltage_samples)
{
    constexpr std::chrono::milliseconds k_average_duration(500);

    {
        //process the new samples and store them
        for (auto& s: current_samples)
        {
            m_capacity_used_mah += s.value * q::Seconds(s.dt).count() / 3600.f;
            m_current_samples.push_back(s);
            m_current_duration += s.dt;
        }
        //remove from the average buffer the surplus so we have at most k_average_duration worth of samples
        while (m_current_duration > k_average_duration && !m_current_samples.empty())
        {
            m_current_duration -= m_current_samples.front().dt;
            m_current_samples.pop_front();
        }
        //compute the average
        if (!m_current_samples.empty())
        {
            float a = 0.f;
            for (auto& s: m_current_samples)
            {
                a += s.value;
            }
            a /= m_current_samples.size();
            m_average_current = a;
        }
    }

    {
        //process the new samples and store them
        for (auto& s: voltage_samples)
        {
            m_voltage_samples.push_back(s);
            m_voltage_duration += s.dt;
        }
        //remove from the average buffer the surplus so we have at most k_average_duration worth of samples
        while (m_voltage_duration > k_average_duration && !m_voltage_samples.empty())
        {
            m_voltage_duration -= m_voltage_samples.front().dt;
            m_voltage_samples.pop_front();
        }
        //compute the average
        if (!m_voltage_samples.empty())
        {
            float a = 0.f;
            for (auto& s: m_voltage_samples)
            {
                a += s.value;
            }
            a /= m_voltage_samples.size();
            m_average_voltage = a;
        }
    }

    SILK_INFO("{}V, {}A, {}Ah", m_average_voltage, m_average_current, m_capacity_used_mah);
}

auto Battery::get_capacity_used() const -> float
{
    return m_capacity_used_mah;
}
auto Battery::get_average_current() const -> float
{
    return m_average_current;
}
auto Battery::get_average_voltage() const -> float
{
    return m_average_voltage;
}

void Battery::reset()
{
    m_current_samples.clear();
    m_current_duration = std::chrono::milliseconds(0);
    m_average_current = 0.f;
    m_capacity_used_mah = 0.f;

    m_voltage_samples.clear();
    m_voltage_duration = std::chrono::milliseconds(0);
    m_average_voltage = 0.f;
}


}
