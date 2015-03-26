#pragma once

#include "common/node/stream/IStream.h"

namespace silk
{
namespace node
{

enum class Type : uint8_t
{
    NODE,
    PROCESSOR,
    SINK,
    SOURCE,
    LPF,
    PILOT,
    RESAMPLER,
    TRANSFORMER,
    CONTROLLER,
    GENERATOR,
    SIMULATOR,
};

class INode : q::util::Noncopyable
{
public:
    virtual ~INode() {}

    virtual auto get_type() const -> Type = 0;

    virtual auto init(rapidjson::Value const& init_params) -> bool = 0;
    virtual auto get_init_params() const -> rapidjson::Document const& = 0;

    virtual auto set_config(rapidjson::Value const& json) -> bool = 0;
    virtual auto get_config() const -> rapidjson::Document = 0;

    virtual auto send_message(rapidjson::Value const& json) -> rapidjson::Document = 0;

    struct Input
    {
        stream::Type type;
        uint32_t rate = 0;
        std::string name;
    };
    virtual auto get_inputs() const -> std::vector<Input> = 0;

    struct Output
    {
        stream::Type type;
        std::string name;
        stream::IStream_ptr stream;
    };
    virtual auto get_outputs() const -> std::vector<Output> = 0;

    virtual void process() = 0;
};
DECLARE_CLASS_PTR(INode);

template <Type TYPE_VALUE>
class INode_Base : public INode
{
public:
    static constexpr Type TYPE = TYPE_VALUE;
    virtual auto get_type() const -> Type { return TYPE; }
};

template<Type T> Type constexpr INode_Base<T>::TYPE;



}
}

