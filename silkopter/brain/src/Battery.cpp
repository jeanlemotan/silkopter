#include "BrainStdAfx.h"
#include "Battery.h"

namespace silk
{

constexpr std::chrono::milliseconds CURRENT_AVERAGE_LENGTH(500);
constexpr std::chrono::seconds VOLTAGE_AVERAGE_LENGTH(5);

constexpr size_t CELL_COUNT_DETECTION_MIN_SAMPLES = 5;
constexpr float CELL_COUNT_DETECTION_MAX_CURRENT = 0.8f;

constexpr size_t CAPACITY_DETECTION_MIN_SAMPLES = 5;
constexpr float CAPACITY_DETECTION_MAX_CURRENT = 0.4f;

constexpr float MIN_CELL_VOLTAGE = 2.9f;
constexpr float MAX_CELL_VOLTAGE = 4.1f;


template<class S>
auto get_samples_duration(std::vector<S> const& samples) -> q::Clock::duration
{
    q::Clock::duration d(0);
    for (auto& s: samples)
    {
        d += s.dt;
    }
    return d;
}
void remove_old_samples(std::deque<std::pair<q::Clock::time_point, float>>& samples, q::Clock::duration max_duration)
{
    auto min_tp = q::Clock::now() - max_duration;
    while (!samples.empty() && samples.front().first < min_tp)
    {
        samples.pop_front();
    }
}
auto compute_average(std::deque<std::pair<q::Clock::time_point, float>> const& samples) -> boost::optional<float>
{
    //compute the average
    if (samples.empty())
    {
        return boost::none;
    }

    float a = 0.f;
    for (auto& s: samples)
    {
        a += s.second;
    }
    a /= samples.size();
    return a;
}


void Battery::process(const std::vector<sensors::Current_Sample>& current_samples, const std::vector<sensors::Voltage_Sample>& voltage_samples)
{
    {
        //samples started at this time point
        auto start = q::Clock::now() - get_samples_duration(current_samples);

        //process the new samples and store them
        float h = 1.f / 3600.f;
        for (auto& s: current_samples)
        {
            m_capacity_used_mah += (s.value * m_last_current) * 0.5f * q::Seconds(s.dt).count() * h;
            m_last_current = s.value;

            m_current_samples.emplace_back(start, s.value);
            start += s.dt;
        }
        remove_old_samples(m_current_samples, std::chrono::milliseconds(500));
        m_average_current = compute_average(m_current_samples);
    }

    {
        //samples started at this time point
        auto start = q::Clock::now() - get_samples_duration(voltage_samples);

        //process the new samples and store them
        for (auto& s: voltage_samples)
        {
            //voltages lower than one cell are just measurement errors
            if (s.value > MIN_CELL_VOLTAGE)
            {
                m_voltage_samples.emplace_back(start, s.value);
            }
            start += s.dt;
        }
        remove_old_samples(m_voltage_samples, std::chrono::seconds(5));
        m_average_voltage = compute_average(m_voltage_samples);
    }

    //new voltage data? compute cell count
    if (!m_cell_count && !voltage_samples.empty())
    {
        m_cell_count = compute_cell_count();
        if (m_cell_count)
        {
            SILK_INFO("Detected battery cell count: {} from voltage: {}V", m_cell_count, m_average_voltage);
        }
    }

    static int xxx = 0;
    if (!current_samples.empty())
    {
        xxx++;
    }

    if (xxx > 20)
    {
        xxx = 0;
        //SILK_INFO("{}V, {}A, {}Ah", m_average_voltage, m_average_current, m_capacity_used_mah);
    }
}

auto Battery::compute_cell_count() -> boost::optional<size_t>
{
    //wait to get a good voltage average
    if (!m_average_voltage || m_voltage_samples.size() < CELL_COUNT_DETECTION_MIN_SAMPLES)
    {
        SILK_WARNING("Skipping cell count detection: the voltage is not healthy: {}V from {} samples", m_average_voltage, m_voltage_samples.size());
        return boost::none;
    }

    //detect the cell count only if the current consumption is not too big, otherwise the voltage drop will be significant
    if (!m_average_current || *m_average_current > CELL_COUNT_DETECTION_MAX_CURRENT)
    {
        SILK_WARNING("Skipping cell count detection: the current is not healthy: {}", m_average_current);
        return boost::none;
    }

    float v = *m_average_voltage;

    //probably the is a faster, analytical way to find this without counting, but i'm not a mathematician!
    for (size_t i = 1; i < 30; i++)
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


auto Battery::get_capacity_used() const -> float
{
    return m_capacity_used_mah;
}
auto Battery::get_average_current() const -> boost::optional<float>
{
    return m_average_current;
}
auto Battery::get_average_voltage() const -> boost::optional<float>
{
    return m_average_voltage;
}

auto Battery::get_cell_count() const -> boost::optional<size_t>
{
    return m_cell_count;
}

void Battery::reset()
{
    m_current_samples.clear();
    m_average_current.reset();
    m_last_current = 0.f;
    m_capacity_used_mah = 0.f;

    m_voltage_samples.clear();
    m_average_voltage.reset();

    m_cell_count.reset();
}


}
