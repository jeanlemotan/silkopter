#pragma once

#include "common/node/stream/IStream.h"

namespace silk
{
namespace node
{
namespace processor
{

class IProcessor : q::util::Noncopyable
{
    DEFINE_RTTI_BASE_CLASS(IProcessor);
public:
    virtual ~IProcessor() {}

    virtual auto get_type() const -> q::rtti::class_id { return q::rtti::get_class_id<IProcessor>(); }

    virtual auto init(rapidjson::Value const& init_params) -> bool = 0;
    virtual auto get_init_params() const -> rapidjson::Document = 0;

    virtual auto set_config(rapidjson::Value const& json) -> bool = 0;
    virtual auto get_config() const -> rapidjson::Document = 0;

    struct Input
    {
        q::rtti::class_id class_id = 0;
        std::string name;
        std::string stream_name;
    };
    virtual auto get_inputs() const -> std::vector<Input> = 0;

    struct Output
    {
        q::rtti::class_id class_id = 0;
        std::string name;
        stream::IStream_ptr stream;
    };
    virtual auto get_outputs() const -> std::vector<Output> = 0;

    virtual void process() = 0;
};
DECLARE_CLASS_PTR(IProcessor);


}
}
}
