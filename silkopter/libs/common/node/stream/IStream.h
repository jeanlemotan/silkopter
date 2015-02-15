#pragma once

namespace silk
{
namespace node
{
namespace stream
{

class IStream : q::util::Noncopyable
{
public:
    virtual ~IStream() {}

    virtual auto get_rate() const -> uint32_t = 0;
    virtual auto get_name() const -> std::string const& = 0;
};


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
