#pragma once

#include "rapidjson/document.h"

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

    virtual auto set_config(rapidjson::Value const&) -> bool { return false; }
    virtual auto get_config() -> boost::optional<rapidjson::Value const&> { return boost::none; }
};


template<class T> struct Sample
{
    Sample() : value() {}
    T value;
    uint32_t sample_idx = 0; //different for each sample. NOTE - it's allowed to wrap so don't compare like this: if (my_sample_idx > sample_idx) !!!!!!!
    q::Clock::duration dt{0}; //the duration of this sample.
    bool is_healthy = true;
};


}
}
}
