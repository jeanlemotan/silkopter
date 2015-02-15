#pragma once

namespace silk
{
namespace node
{
namespace sink
{

class ISink : q::util::Noncopyable
{
public:
    virtual ~ISink() {}

    virtual auto get_name() const -> std::string const& = 0;

};


}
}
}
