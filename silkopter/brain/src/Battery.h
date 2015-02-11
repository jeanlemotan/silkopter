#pragma once

#include "common/node/IVoltmeter.h"
#include "common/node/IAmmeter.h"

namespace silk
{

class Battery : q::util::Noncopyable
{
public:
    Battery();

    void process(std::vector<node::IAmmeter::Sample> const& ammeter_samples,
                 std::vector<node::IVoltmeter::Sample> const& voltmeter_samples);

    auto get_capacity_used() const -> float; //mAh
    auto get_average_current() const -> boost::optional<float>;
    auto get_average_voltage() const -> boost::optional<float>;

    auto get_cell_count() const -> boost::optional<size_t>;

    void reset();

    struct Persistent_State
    {
        float capacity_used = 0;
        size_t cell_count = 0;
    };

private:
    auto compute_cell_count() -> boost::optional<size_t>;

    std::deque<std::pair<q::Clock::time_point, float>> m_ammeter_samples;
    boost::optional<float> m_average_current;
    float m_last_current = 0.f;
    float m_capacity_used_mah = 0.f;

    void load_state();
    void save_state();
    Persistent_State m_loaded_state;
    Persistent_State m_saved_state;
    q::Clock::time_point m_last_save_time_point = q::Clock::now();

    std::deque<std::pair<q::Clock::time_point, float>> m_voltmeter_samples;
    boost::optional<float> m_average_voltage;

    boost::optional<size_t> m_cell_count;
};

}
