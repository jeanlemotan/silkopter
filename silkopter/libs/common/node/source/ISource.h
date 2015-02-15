#pragma once

namespace silk
{
namespace node
{
namespace stream
{
    class IStream;
}

namespace source
{

class ISource : q::util::Noncopyable
{
public:
    virtual ~ISource() {}

    virtual auto get_name() const -> std::string const& = 0;
    virtual auto get_stream() -> stream::IStream& = 0;
};


}
}
}
