#pragma once

#include "common/node/source/ISonar.h"
#include "common/node/bus/II2C.h"

namespace silk
{
namespace node
{
namespace source
{

class SRF02: public source::ISonar
{
public:
    SRF02();

    struct Init_Params
    {
        size_t rate = 10;
        math::vec3f direction = math::vec3f(0, 0, -1);
        float min_distance = 0.2f;
        float max_distance = 5.f;
    };

    auto init(bus::II2C* bus, Init_Params const& params) -> bool;

    void process();

    auto get_stream() const -> stream::IDistance const&;

private:
    bus::II2C* m_i2c = nullptr;

    Init_Params m_params;

    struct Stream : public stream::IDistance
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        std::vector<Sample> samples;
        q::Clock::duration dt;
        q::Clock::time_point last_time_point;
        uint32_t sample_idx = 0;
    } m_stream;

    int m_state = 0;
};

DECLARE_CLASS_PTR(SRF02);

}
}
}
