#pragma once

#include "common/node/stream/IStream.h"


namespace silk
{
namespace node
{

namespace source
{

class ISource : q::util::Noncopyable
{
    DEFINE_RTTI_BASE_CLASS(ISource);
public:
    virtual ~ISource() {}

    virtual auto get_type() const -> q::rtti::class_id { return q::rtti::get_class_id<ISource>(); }

    struct Output
    {
        q::rtti::class_id class_id = 0;
        std::string name;
        stream::IStream_ptr stream;
    };
    virtual auto get_outputs() const -> std::vector<Output> = 0;

    virtual auto init(rapidjson::Value const& init_params) -> bool = 0;
    virtual auto get_init_params() -> rapidjson::Document = 0;

    virtual auto set_config(rapidjson::Value const& json) -> bool = 0;
    virtual auto get_config() -> rapidjson::Document = 0;

    virtual void process() = 0;
};
DECLARE_CLASS_PTR(ISource);


}
}
}
