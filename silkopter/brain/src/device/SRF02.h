#pragma once

#include "interface/ISonar.h"
#include "interface/II2C.h"

namespace silk
{
namespace device
{

class SRF02: public sensor::ISonar, q::util::Noncopyable
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

    auto init(bus::II2C* bus, Params const& params) -> bool;

    void process();

    auto get_name() const -> q::String const&;
    auto get_samples() const -> std::vector<sensor::Sonar_Sample> const&;

private:
    bus::II2C* m_i2c = nullptr;
    q::String m_name;

    Params m_params;

    std::vector<sensor::Sonar_Sample> m_samples;
    q::Clock::duration m_dt;
    q::Clock::time_point m_last_time_point;
    uint32_t m_sample_idx = 0;
    int m_state = 0;
};

DECLARE_CLASS_PTR(SRF02);

}
}
