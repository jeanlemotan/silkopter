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

    virtual auto init(rapidjson::Value const& json) -> bool = 0;
    virtual auto get_init_params() -> rapidjson::Document = 0;

    virtual auto set_config(rapidjson::Value const& json) -> bool = 0;
    virtual auto get_config() -> rapidjson::Document = 0;

    virtual auto get_name() const -> std::string const& = 0;

    virtual auto get_input_stream_count() const -> size_t = 0;
    virtual auto get_input_stream(size_t idx) -> stream::IStream& = 0;

    virtual auto get_output_stream_count() const -> size_t = 0;
    virtual auto get_output_stream(size_t idx) -> stream::IStream& = 0;

};


}
}
}
