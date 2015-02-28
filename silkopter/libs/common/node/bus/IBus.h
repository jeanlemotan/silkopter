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
    virtual auto get_type() const -> std::string const&
    {
        return q::rtti::get_class_name<IBus>();
    }

    virtual auto init(rapidjson::Value const& json) -> bool = 0;
    virtual auto get_init_params() -> rapidjson::Document = 0;

    virtual auto set_config(rapidjson::Value const& json) -> bool = 0;
    virtual auto get_config() -> rapidjson::Document = 0;
};


}
}
}
