#pragma once


namespace silk
{
namespace node
{
namespace stream
{


template<class T> struct Sample
{
    Sample() : value() {}
    T value;
    uint32_t sample_idx = 0; //different for each sample. NOTE - it's allowed to wrap so don't compare like this: if (my_sample_idx > sample_idx) !!!!!!!
    q::Clock::duration dt{0}; //the duration of this sample.
    q::Clock::time_point time_point;
};


}
}
}
