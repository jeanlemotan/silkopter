#pragma once

namespace silk
{
namespace node
{
namespace source
{

class ISource : q::util::Noncopyable
{
public:
    virtual ~ISource() {}

    virtual auto get_name() const -> std::string const& = 0;

};


}
}
}
