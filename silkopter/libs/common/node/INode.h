#pragma once

#include "common/node/stream/IStream.h"

namespace silk
{
namespace node
{

class INode : q::util::Noncopyable
{
public:
    enum class Type : uint8_t
    {
        NODE,
        PROCESSOR,
        SINK,
        SOURCE,
        LPF,
        PILOT,
        RESAMPLER,
        TRANSFORMER
    };

    virtual ~INode() {}

    virtual auto get_type() const -> Type = 0;

    virtual auto init(rapidjson::Value const& init_params) -> bool = 0;
    virtual auto get_init_params() const -> rapidjson::Document const& = 0;

    virtual auto set_config(rapidjson::Value const& json) -> bool = 0;
    virtual auto get_config() const -> rapidjson::Document = 0;

    struct Input
    {
        IStream::Type type;
        uint32_t rate = 0;
        std::string name;
    };
    virtual auto get_inputs() const -> std::vector<Input> = 0;

    struct Output
    {
        IStream::Type type;
        std::string name;
        IStream_ptr stream;
    };
    virtual auto get_outputs() const -> std::vector<Output> = 0;

    virtual void process() = 0;
};
DECLARE_CLASS_PTR(INode);

template <INode::Type TYPE_VALUE>
class INode_Base : public INode
{
public:
    static constexpr Type TYPE = TYPE_VALUE;
    virtual auto get_type() const -> Type { return TYPE; }
};


}
}

