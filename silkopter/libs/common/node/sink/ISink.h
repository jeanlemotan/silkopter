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
    virtual auto get_type() const -> q::rtti::class_id { return q::rtti::get_class_id<ISink>(); }

    struct Input
    {
        q::rtti::class_id class_id = 0;
        std::string name;
        stream::IStream* stream = nullptr;
    };
    virtual auto get_inputs() const -> std::vector<Input> = 0;

    virtual auto init(rapidjson::Value const& init_params, rapidjson::Value const& config) -> bool = 0;
    virtual auto get_init_params() -> rapidjson::Document = 0;

    virtual auto set_config(rapidjson::Value const& json) -> bool = 0;
    virtual auto get_config() -> rapidjson::Document = 0;
};


}
}
}
