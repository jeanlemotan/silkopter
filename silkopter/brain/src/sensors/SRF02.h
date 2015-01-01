#pragma once

#include "i2c.h"

namespace silk
{

class SRF02: public q::util::Noncopyable
{
public:
    auto init(q::Clock::duration sample_time) -> bool;

    void process();

    struct Data
    {
        float value;
        q::Clock::duration dt;
    };

    auto get_distance_data() -> boost::optional<Data>;

private:
    i2c m_i2c;

    boost::optional<Data> m_data;
    q::Clock::duration m_sample_time;
    q::Clock::time_point m_last_time_point;
    int m_state = 0;
};

}




