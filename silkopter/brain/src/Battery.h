#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{

class Battery : q::util::Noncopyable
{
public:
    void process(std::vector<Current_Sample> const& current_samples,
                 std::vector<Voltage_Sample> const& voltage_samples);

    auto get_capacity_used() const -> float; //mAh
    auto get_average_current() const -> float;
    auto get_average_voltage() const -> float;

    void reset();

private:
    std::deque<Current_Sample> m_current_samples;
    q::Clock::duration m_current_duration = q::Clock::duration(0);
    float m_average_current = 0.f;
    float m_capacity_used_mah = 0.f;

    std::deque<Voltage_Sample> m_voltage_samples;
    q::Clock::duration m_voltage_duration = q::Clock::duration(0);
    float m_average_voltage = 0.f;
};

}
