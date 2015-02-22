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

    virtual auto get_input_stream_count() const -> size_t = 0;
    virtual auto get_input_stream(size_t idx) -> stream::IStream& = 0;

    virtual auto set_config(rapidjson::Value const&) -> bool { return false; }
    virtual auto get_config() -> boost::optional<rapidjson::Value const&> { return boost::none; }
};


}
}
}
