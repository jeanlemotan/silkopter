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
public:
    virtual ~IProcessor() {}

    virtual auto init(rapidjson::Value const& json) -> bool = 0;

    virtual auto set_config(rapidjson::Value const&) -> bool { return false; }
    virtual auto get_config() -> boost::optional<rapidjson::Value const&> { return boost::none; }

    virtual auto get_name() const -> std::string const& = 0;

    virtual auto get_input_stream_count() const -> size_t = 0;
    virtual auto get_input_stream(size_t idx) -> stream::IStream& = 0;

    virtual auto get_output_stream_count() const -> size_t = 0;
    virtual auto get_output_stream(size_t idx) -> stream::IStream& = 0;

};


}
}
}
