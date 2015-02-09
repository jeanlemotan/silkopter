#pragma once

#include "i2c.h"
#include "ISonar.h"

namespace silk
{
namespace sensors
{

class SRF02: public ISonar, q::util::Noncopyable
{
public:
    auto init(q::Clock::duration sample_time) -> bool;

    void process();

    auto get_sonar_samples() const -> std::vector<Sonar_Sample> const&;

private:
    i2c m_i2c;

    std::vector<Sonar_Sample> m_samples;
    q::Clock::duration m_sample_time;
    q::Clock::time_point m_last_time_point;
    uint32_t m_sample_idx = 0;
    int m_state = 0;
};

}
}
