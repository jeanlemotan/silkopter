#pragma once

#include "common/node/stream/IAcceleration.h"
#include "common/node/stream/IAngular_Velocity.h"
#include "common/node/stream/IADC_Value.h"
#include "common/node/stream/IBattery_State.h"
#include "common/node/stream/ICardinal_Points.h"
#include "common/node/stream/ICurrent.h"
#include "common/node/stream/IDistance.h"
#include "common/node/stream/ILinear_Acceleration.h"
#include "common/node/stream/ILocation.h"
#include "common/node/stream/IMagnetic_Field.h"
#include "common/node/stream/IPressure.h"
#include "common/node/stream/IPWM_Value.h"
#include "common/node/stream/IReference_Frame.h"
#include "common/node/stream/ITemperature.h"
#include "common/node/stream/IVideo.h"
#include "common/node/stream/IVoltage.h"

#include "common/node/source/ISource.h"
#include "common/node/sink/ISink.h"
#include "common/node/processor/IMultirotor.h"

#include "utils/Json_Helpers.h"

namespace silk
{

class HAL;




namespace node
{


namespace stream
{

struct GS_IStream
{
    virtual ~GS_IStream() {}

    std::string name;
    q::rtti::class_id class_id;
    uint32_t rate = 0;
};
DECLARE_CLASS_PTR(GS_IStream);

struct Acceleration : public GS_IStream
{
    typedef IAcceleration::Sample Sample;
    std::vector<Sample> samples;
};
DECLARE_CLASS_PTR(Acceleration);
struct Angular_Velocity : public GS_IStream
{
    typedef IAngular_Velocity::Sample Sample;
    std::vector<Sample> samples;
};
DECLARE_CLASS_PTR(Angular_Velocity);
struct ADC_Value : public GS_IStream
{
    typedef IADC_Value::Sample Sample;
    std::vector<Sample> samples;
};
DECLARE_CLASS_PTR(ADC_Value);
struct Battery_State : public GS_IStream
{
    typedef IBattery_State::Sample Sample;
    std::vector<Sample> samples;
};
DECLARE_CLASS_PTR(Battery_State);
struct Cardinal_Points : public GS_IStream
{
    typedef ICardinal_Points::Sample Sample;
    std::vector<Sample> samples;
};
DECLARE_CLASS_PTR(Cardinal_Points);
struct Current : public GS_IStream
{
    typedef ICurrent::Sample Sample;
    std::vector<Sample> samples;
};
DECLARE_CLASS_PTR(Current);
struct Distance : public GS_IStream
{
    typedef IDistance::Sample Sample;
    std::vector<Sample> samples;
};
DECLARE_CLASS_PTR(Distance);
struct Linear_Acceleration : public GS_IStream
{
    typedef ILinear_Acceleration::Sample Sample;
    std::vector<Sample> samples;
};
DECLARE_CLASS_PTR(Linear_Acceleration);
struct Location : public GS_IStream
{
    typedef ILocation::Sample Sample;
    std::vector<Sample> samples;
};
DECLARE_CLASS_PTR(Location);
struct Magnetic_Field : public GS_IStream
{
    typedef IMagnetic_Field::Sample Sample;
    std::vector<Sample> samples;
};
DECLARE_CLASS_PTR(Magnetic_Field);
struct Pressure : public GS_IStream
{
    typedef IPressure::Sample Sample;
    std::vector<Sample> samples;
};
DECLARE_CLASS_PTR(Pressure);
struct PWM_Value : public GS_IStream
{
    typedef IPWM_Value::Sample Sample;
    std::vector<Sample> samples;
};
DECLARE_CLASS_PTR(PWM_Value);
struct Reference_Frame : public GS_IStream
{
    typedef IReference_Frame::Sample Sample;
    std::vector<Sample> samples;
};
DECLARE_CLASS_PTR(Reference_Frame);
struct Temperature : public GS_IStream
{
    typedef IADC_Value::Sample Sample;
    std::vector<Sample> samples;
};
DECLARE_CLASS_PTR(Temperature);
struct Voltage : public GS_IStream
{
    typedef IVoltage::Sample Sample;
    std::vector<Sample> samples;
};
DECLARE_CLASS_PTR(Voltage);

}


namespace source
{
struct Source
{
    virtual ~Source() {}

    Source() = default;
    Source(Source&& other) = default;
    Source(Source const& other)
        : name(other.name)
        , class_id(other.class_id)
        , init_params(jsonutil::clone_value(other.init_params))
        , config(jsonutil::clone_value(other.config))
        , outputs(other.outputs)
    {
    }

    std::string name;
    q::rtti::class_id class_id;
    rapidjson::Document init_params;
    rapidjson::Document config;
    struct Output
    {
        q::rtti::class_id class_id;
        std::string name;
        stream::GS_IStream_ptr stream;
    };
    std::vector<Output> outputs;
};
DECLARE_CLASS_PTR(Source);

}

namespace sink
{

struct Sink
{
    virtual ~Sink() {}
    Sink() = default;
    Sink(Sink&& other) = default;
    Sink(Sink const& other)
        : name(other.name)
        , class_id(other.class_id)
        , init_params(jsonutil::clone_value(other.init_params))
        , config(jsonutil::clone_value(other.config))
        , inputs(other.inputs)
    {
    }

    std::string name;
    q::rtti::class_id class_id;
    rapidjson::Document init_params;
    rapidjson::Document config;
    struct Input
    {
        q::rtti::class_id class_id;
        std::string name;
        stream::GS_IStream_ptr stream;
    };
    std::vector<Input> inputs;
};
DECLARE_CLASS_PTR(Sink);

}

namespace processor
{

struct Processor
{
    virtual ~Processor() {}
    Processor() = default;
    Processor(Processor&& other) = default;
    Processor(Processor const& other)
        : name(other.name)
        , class_id(other.class_id)
        , init_params(jsonutil::clone_value(other.init_params))
        , config(jsonutil::clone_value(other.config))
        , inputs(other.inputs)
        , outputs(other.outputs)
    {
    }

    std::string name;
    q::rtti::class_id class_id;
    rapidjson::Document init_params;
    rapidjson::Document config;
    struct Input
    {
        q::rtti::class_id class_id;
        std::string name;
        stream::GS_IStream_ptr stream;
    };
    std::vector<Input> inputs;

    struct Output
    {
        q::rtti::class_id class_id;
        std::string name;
        stream::GS_IStream_ptr stream;
    };
    std::vector<Output> outputs;
};
DECLARE_CLASS_PTR(Processor);

}


}







template<class Base>
class Registry : q::util::Noncopyable
{
public:
    auto get_all() const -> std::vector<std::shared_ptr<Base>> const&;
    template<class T> auto find_by_name(std::string const& name) const -> std::shared_ptr<T>;
    template<class T> auto add(std::shared_ptr<T> node) -> bool;
    void remove_all();
private:
    std::vector<std::shared_ptr<Base>> m_nodes;
};


class HAL : q::util::Noncopyable
{
    friend class Comms;
public:
    HAL();
    ~HAL();

    auto get_source_defs() const    -> Registry<node::source::Source> const&;
    auto get_sink_defs() const      -> Registry<node::sink::Sink> const&;
    auto get_processor_defs() const -> Registry<node::processor::Processor> const&;

    auto get_sources() const        -> Registry<node::source::Source> const&;
    auto get_sinks() const          -> Registry<node::sink::Sink> const&;
    auto get_processors() const     -> Registry<node::processor::Processor> const&;
    auto get_streams() const        -> Registry<node::stream::GS_IStream> const&;

    enum class Result
    {
        OK,
        FAILED,
        TIMEOUT
    };

    typedef std::function<void(Result, node::source::Source_ptr)> Add_Source_Callback;
    void add_source(node::source::Source_ptr def, node::source::Source_ptr source, Add_Source_Callback callback);

    q::util::Signal<void()> node_defs_refreshed_signal;
    q::util::Signal<void()> nodes_refreshed_signal;

protected:
    Registry<node::source::Source> m_source_defs;
    Registry<node::sink::Sink> m_sink_defs;
    Registry<node::processor::Processor> m_processor_defs;

    Registry<node::source::Source> m_sources;
    Registry<node::sink::Sink> m_sinks;
    Registry<node::processor::Processor> m_processors;
    Registry<node::stream::GS_IStream> m_streams;

    struct Add_Source_Queue_Item
    {
        bool was_sent = false;
        q::Clock::time_point sent_time_point;
        uint32_t req_id = 0;
        node::source::Source_ptr def;
        node::source::Source_ptr source;
        Add_Source_Callback callback;
    };
    std::vector<Add_Source_Queue_Item> m_add_source_queue;

private:

};


/////////////////////////////////////////////////////////////////////////////////////////////////


template<class Base>
auto Registry<Base>::get_all() const -> std::vector<std::shared_ptr<Base>> const&
{
    return m_nodes;
}
template<class Base>
template<class T>
auto Registry<Base>::find_by_name(std::string const& name) const -> std::shared_ptr<T>
{
    auto it = std::find_if(m_nodes.begin(), m_nodes.end(), [&](std::shared_ptr<Base> const& s) { return s->name == name; });
    return it != m_nodes.end() ? std::dynamic_pointer_cast<T>(*it) : std::shared_ptr<T>();
}
template<class Base>
template<class T>
auto Registry<Base>::add(std::shared_ptr<T> node) -> bool
{
    QASSERT(node);
    if (find_by_name<Base>(node->name))
    {
        QLOGE("Duplicated name in node {}", node->name);
        return false;
    }
    m_nodes.push_back(std::move(node));
    return true;
}
template<class Base>
void Registry<Base>::remove_all()
{
    m_nodes.clear();
}




}
