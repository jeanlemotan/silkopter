#pragma once

#include "i2c.h"

namespace silk
{

class SRF02: public q::util::Noncopyable
{
public:
    auto init(q::Clock::duration sample_time) -> bool;

    void process();

    auto read_distance() -> boost::optional<float>;
    auto get_sample_time() const -> q::Clock::duration;

private:
    i2c m_i2c;

    auto read_distance(size_t idx) -> boost::optional<float>;

    boost::optional<float> m_data;
    q::Clock::duration m_sample_time;
    q::Clock::time_point m_last_time_point;
    q::Clock::time_point m_measurement_start;
};

}




