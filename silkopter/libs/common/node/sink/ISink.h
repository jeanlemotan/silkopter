#pragma once

namespace silk
{
namespace node
{
namespace stream
{
    class IStream;
}

namespace sink
{

class ISink : q::util::Noncopyable
{
public:
    virtual ~ISink() {}

    virtual auto get_name() const -> std::string const& = 0;
    virtual auto get_stream() -> stream::IStream& = 0;
};


}
}
}
