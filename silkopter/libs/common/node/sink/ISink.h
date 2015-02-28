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
    DEFINE_RTTI_BASE_CLASS(ISink);
public:
    virtual ~ISink() {}

    virtual auto get_name() const -> std::string const& = 0;
    virtual auto get_type() const -> std::string const&
    {
        return q::rtti::get_class_name<ISink>();
    }

    virtual auto get_input_stream_count() const -> size_t = 0;
    virtual auto get_input_stream(size_t idx) -> stream::IStream& = 0;

    virtual auto init(rapidjson::Value const& json) -> bool = 0;
    virtual auto get_init_params() -> rapidjson::Document = 0;

    virtual auto set_config(rapidjson::Value const& json) -> bool = 0;
    virtual auto get_config() -> rapidjson::Document = 0;
};


}
}
}
