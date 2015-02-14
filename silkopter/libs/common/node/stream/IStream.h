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

};


}
}
}
