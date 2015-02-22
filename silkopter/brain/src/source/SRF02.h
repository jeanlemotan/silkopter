#pragma once

#include "HAL.h"
#include "common/node/source/ISource.h"
#include "common/node/stream/IDistance.h"
#include "common/node/bus/II2C.h"

namespace silk
{
namespace node
{
namespace source
{

class SRF02 : public ISource
{
public:
    SRF02(HAL& hal);

    struct Init_Params
    {
        std::string name;
        bus::II2C* bus = nullptr;
        uint32_t rate = 10;
        math::vec3f direction = math::vec3f(0, 0, -1);
        float min_distance = 0.2f;
        float max_distance = 5.f;
    };

    auto init(rapidjson::Value const& json) -> bool;
    auto init(Init_Params const& params) -> bool;

    auto get_name() const -> std::string const&;
    auto get_output_stream_count() const -> size_t;
    auto get_output_stream(size_t idx) -> stream::IStream&;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    Init_Params m_params;

    struct Stream : public stream::IDistance
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }
        auto get_name() const -> std::string const& { return name; }

        uint32_t rate = 0;
        std::vector<Sample> samples;
        Sample last_sample;
        q::Clock::duration dt;
        q::Clock::time_point last_time_point;
        std::string name;
    } m_stream;

    int m_state = 0;
};

}
}
}