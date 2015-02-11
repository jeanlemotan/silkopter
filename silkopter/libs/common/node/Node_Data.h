#pragma once

namespace silk
{
namespace node
{

template<class T> struct Sample
{
    Sample() : value() {}
    T value;
    uint32_t sample_idx = 0; //different for each sample. NOTE - it's allowed to wrap so don't compare like this: if (my_sample_idx > sample_idx) !!!!!!!
    q::Clock::duration dt{0}; //the duration of this sample.
};
template<class T> struct TP_Sample : public Sample<T>
{
    q::Clock::time_point time_point;
};

}
}
