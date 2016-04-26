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
    COMBINER,
    CONTROLLER,
    GENERATOR,
    SIMULATOR,
    CONFIG
};

inline auto get_as_string(Type s) -> std::string
{
    switch (s)
    {
    case Type::NODE: return "Node";
    case Type::PROCESSOR: return "Processor";
    case Type::SINK: return "Sink";
    case Type::SOURCE: return "Source";
    case Type::LPF: return "LPF";
    case Type::BRAIN: return "Brain";
    case Type::PILOT: return "Pilot";
    case Type::RESAMPLER: return "Resampler";
    case Type::TRANSFORMER: return "Transformer";
    case Type::COMBINER: return "Combiner";
    case Type::CONTROLLER: return "Controller";
    case Type::GENERATOR: return "Generator";
    case Type::SIMULATOR: return "Simulator";
    case Type::CONFIG: return "Config";
    default: QASSERT(false); return "Unknown";
    }
}


class INode : q::util::Noncopyable
{
public:
    virtual ~INode() = default;

    virtual Type get_type() const = 0;

    virtual bool init(std::shared_ptr<ts::IValue> descriptor) { return false; }
    virtual std::shared_ptr<const ts::IValue> get_descriptor() const { return nullptr; }

    virtual bool set_config(std::shared_ptr<ts::IValue> config) { return false; }
    virtual std::shared_ptr<const ts::IValue> get_config2() const  { return nullptr; }


    virtual bool init(rapidjson::Value const& init_params) = 0;
    virtual auto get_init_params() const -> rapidjson::Document = 0;

    virtual auto set_config(rapidjson::Value const& json) -> bool = 0;
    virtual auto get_config() const -> rapidjson::Document = 0;

    virtual bool start(q::Clock::time_point tp) = 0;

    virtual auto send_message(rapidjson::Value const& json) -> rapidjson::Document = 0;

    struct Input
    {
        stream::Type type;
        uint32_t rate;
        std::string name;
        q::Path stream_path; //the path of the stream connected to this input in the "node/output" format
    };
    virtual std::vector<Input> get_inputs() const = 0;
    virtual void set_input_stream_path(size_t idx, q::Path const& path) = 0;

    struct Output
    {
        std::string name;
        std::shared_ptr<stream::IStream> stream;
    };
    virtual std::vector<Output> get_outputs() const  = 0;

    virtual void process() = 0;
};

template <Type TYPE_VALUE>
class Node_Base : public INode
{
public:
    static constexpr Type TYPE = TYPE_VALUE;
    virtual Type get_type() const { return TYPE; }
};

template<Type T> Type constexpr Node_Base<T>::TYPE;



}
}

