#pragma once

#include "ISonar.h"
#include "buses/II2C.h"

namespace silk
{
namespace sensors
{

class SRF02: public ISonar, q::util::Noncopyable
{
public:
    SRF02(q::String const& name);

    struct Params
    {
        size_t rate = 10;
        math::vec3f direction = math::vec3f(0, 0, -1);
        float min_distance = 0.2f;
        float max_distance = 5.f;
    };

    auto init(buses::II2C* bus, Params const& params) -> bool;

    auto get_sonar_name() const -> q::String const&;

    void process();

    auto get_sonar_samples() const -> std::vector<Sonar_Sample> const&;

private:
    buses::II2C* m_i2c = nullptr;
    q::String m_name;

    Params m_params;

    std::vector<Sonar_Sample> m_samples;
    q::Clock::duration m_dt;
    q::Clock::time_point m_last_time_point;
    uint32_t m_sample_idx = 0;
    int m_state = 0;
};

DECLARE_CLASS_PTR(SRF02);

}
}
