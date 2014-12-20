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
    auto get_average_current() const -> boost::optional<float>;
    auto get_average_voltage() const -> boost::optional<float>;

    auto get_cell_count() const -> boost::optional<size_t>;

    void reset();

private:
    auto compute_cell_count() -> boost::optional<size_t>;

    std::deque<std::pair<q::Clock::time_point, float>> m_current_samples;
    boost::optional<float> m_average_current;
    float m_last_current = 0.f;
    float m_capacity_used_mah = 0.f;

    std::deque<std::pair<q::Clock::time_point, float>> m_voltage_samples;
    boost::optional<float> m_average_voltage;

    boost::optional<size_t> m_cell_count;
};

}
