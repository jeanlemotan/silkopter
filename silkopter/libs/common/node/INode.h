#pragma once

#include "common/stream/IStream.h"

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
    BRAIN,
    PILOT,
    RESAMPLER,
    TRANSFORMER,
    CONTROLLER,
    GENERATOR,
    SIMULATOR,
    CONFIG
};

class INode : q::util::Noncopyable
{
public:
    virtual ~INode() {}

    virtual auto get_type() const -> Type = 0;

    virtual auto init(rapidjson::Value const& init_params) -> bool = 0;
    virtual auto get_init_params() const -> rapidjson::Document = 0;

    virtual auto set_config(rapidjson::Value const& json) -> bool = 0;
    virtual auto get_config() const -> rapidjson::Document = 0;

    virtual auto start(q::Clock::time_point tp) -> bool = 0;

    virtual auto send_message(rapidjson::Value const& json) -> rapidjson::Document = 0;

    struct Input
    {
        stream::Type type;
        uint32_t rate;
        std::string name;
        q::Path stream_path; //the path of the stream connected to this input in the "node/output" format
    };
    virtual auto get_inputs() const -> std::vector<Input> = 0;
    virtual void set_input_stream_path(size_t idx, q::Path const& path) = 0;

    struct Output
    {
        std::string name;
        stream::IStream_ptr stream;
    };
    virtual auto get_outputs() const -> std::vector<Output> = 0;

    virtual void process() = 0;
};
DECLARE_CLASS_PTR(INode);

template <Type TYPE_VALUE>
class Node_Base : public INode
{
public:
    static constexpr Type TYPE = TYPE_VALUE;
    virtual auto get_type() const -> Type { return TYPE; }
};

template<Type T> Type constexpr Node_Base<T>::TYPE;



}
}

