#pragma once

namespace silk
{
namespace node
{
namespace bus
{

class IBus : q::util::Noncopyable
{
public:
    virtual ~IBus() {}

    virtual auto get_name() const -> std::string const& = 0;
};


}
}
}
