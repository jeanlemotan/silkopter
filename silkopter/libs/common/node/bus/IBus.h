#pragma once

namespace silk
{
namespace node
{
namespace bus
{

class IBus : q::util::Noncopyable
{
    DEFINE_RTTI_BASE_CLASS(IBus);
public:
    virtual ~IBus() {}

    virtual auto get_name() const -> std::string const& = 0;
    virtual auto get_type() const -> q::rtti::class_id = 0;

    virtual auto init(rapidjson::Value const& init_params, rapidjson::Value const& config) -> bool = 0;
    virtual auto get_init_params() -> rapidjson::Document = 0;

    virtual auto set_config(rapidjson::Value const& json) -> bool = 0;
    virtual auto get_config() -> rapidjson::Document = 0;
};


}
}
}
