#pragma once

#include "common/node/ISonar.h"
#include "common/node/II2C.h"

namespace silk
{
namespace hw_device
{

class SRF02: public node::ISonar, q::util::Noncopyable
{
public:
    SRF02(q::String const& name);

    struct Init_Params
    {
        size_t rate = 10;
        math::vec3f direction = math::vec3f(0, 0, -1);
        float min_distance = 0.2f;
        float max_distance = 5.f;
    };

    auto init(node::II2C* bus, Init_Params const& params) -> bool;

    void process();

    auto get_name() const -> q::String const&;
    auto get_samples() const -> std::vector<node::ISonar::Sample> const&;

private:
    node::II2C* m_i2c = nullptr;
    q::String m_name;

    Init_Params m_params;

    std::vector<node::ISonar::Sample> m_samples;
    q::Clock::duration m_dt;
    q::Clock::time_point m_last_time_point;
    uint32_t m_sample_idx = 0;
    int m_state = 0;
};

DECLARE_CLASS_PTR(SRF02);

}
}
