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

#include "common/node/bus/IBus.h"
#include "common/node/source/ISource.h"
#include "common/node/sink/ISink.h"
#include "common/node/processor/IMultirotor.h"

#include "utils/Json_Helpers.h"

namespace silk
{

class HAL;




namespace node
{

namespace bus
{

struct GS_IBus
{
    virtual ~GS_IBus() {}
    GS_IBus() = default;
    GS_IBus(GS_IBus&&) = default;
    auto operator=(GS_IBus&&) -> GS_IBus& = default;

    std::string name;
    q::rtti::class_id class_id;
    rapidjson::Document init_params;
    rapidjson::Document config;
};

struct I2C : public GS_IBus
{
};

struct UART : public GS_IBus
{
};

struct SPI : public GS_IBus
{
};

}



namespace stream
{

struct GS_IStream
{
    virtual ~GS_IStream() {}
    GS_IStream() = default;
    GS_IStream(GS_IStream&&) = default;
    auto operator=(GS_IStream&&) -> GS_IStream& = default;

    std::string name;
    q::rtti::class_id class_id;
    uint32_t rate = 0;
};

struct Acceleration : public GS_IStream
{
    typedef IAcceleration::Sample Sample;
    std::vector<Sample> samples;
};
struct Angular_Velocity : public GS_IStream
{
    typedef IAngular_Velocity::Sample Sample;
    std::vector<Sample> samples;
};
struct ADC_Value : public GS_IStream
{
    typedef IADC_Value::Sample Sample;
    std::vector<Sample> samples;
};
struct Battery_State : public GS_IStream
{
    typedef IBattery_State::Sample Sample;
    std::vector<Sample> samples;
};
struct Cardinal_Points : public GS_IStream
{
    typedef ICardinal_Points::Sample Sample;
    std::vector<Sample> samples;
};
struct Current : public GS_IStream
{
    typedef ICurrent::Sample Sample;
    std::vector<Sample> samples;
};
struct Distance : public GS_IStream
{
    typedef IDistance::Sample Sample;
    std::vector<Sample> samples;
};
struct Linear_Acceleration : public GS_IStream
{
    typedef ILinear_Acceleration::Sample Sample;
    std::vector<Sample> samples;
};
struct Location : public GS_IStream
{
    typedef ILocation::Sample Sample;
    std::vector<Sample> samples;
};
struct Magnetic_Field : public GS_IStream
{
    typedef IMagnetic_Field::Sample Sample;
    std::vector<Sample> samples;
};
struct Pressure : public GS_IStream
{
    typedef IPressure::Sample Sample;
    std::vector<Sample> samples;
};
struct PWM_Value : public GS_IStream
{
    typedef IPWM_Value::Sample Sample;
    std::vector<Sample> samples;
};
struct Reference_Frame : public GS_IStream
{
    typedef IReference_Frame::Sample Sample;
    std::vector<Sample> samples;
};
struct Temperature : public GS_IStream
{
    typedef IADC_Value::Sample Sample;
    std::vector<Sample> samples;
};
struct Voltage : public GS_IStream
{
    typedef IVoltage::Sample Sample;
    std::vector<Sample> samples;
};
}


namespace source
{
struct GS_ISource
{
    virtual ~GS_ISource() {}
    GS_ISource() = default;
    GS_ISource(GS_ISource&&) = default;
    auto operator=(GS_ISource&&) -> GS_ISource& = default;

    std::string name;
    q::rtti::class_id class_id;
    rapidjson::Document init_params;
    rapidjson::Document config;
    struct Output
    {
        q::rtti::class_id class_id;
        std::string name;
        std::shared_ptr<stream::GS_IStream> stream;
    };
    std::vector<Output> outputs;
};

struct Source : public GS_ISource
{
};
}

namespace sink
{

struct GS_ISink
{
    virtual ~GS_ISink() {}
    GS_ISink() = default;
    GS_ISink(GS_ISink&&) = default;
    auto operator=(GS_ISink&&) -> GS_ISink& = default;

    std::string name;
    q::rtti::class_id class_id;
    rapidjson::Document init_params;
    rapidjson::Document config;
    struct Input
    {
        q::rtti::class_id class_id;
        std::string name;
        std::shared_ptr<stream::GS_IStream> stream;
    };
    std::vector<Input> inputs;
};

struct Sink : public GS_ISink
{
};
}

namespace processor
{

struct GS_IProcessor
{
    virtual ~GS_IProcessor() {}
    GS_IProcessor() = default;
    GS_IProcessor(GS_IProcessor&&) = default;
    auto operator=(GS_IProcessor&&) -> GS_IProcessor& = default;

    std::string name;
    q::rtti::class_id class_id;
    rapidjson::Document init_params;
    rapidjson::Document config;
    struct Input
    {
        q::rtti::class_id class_id;
        std::string name;
        std::shared_ptr<stream::GS_IStream> stream;
    };
    std::vector<Input> inputs;

    struct Output
    {
        q::rtti::class_id class_id;
        std::string name;
        std::shared_ptr<stream::GS_IStream> stream;
    };
    std::vector<Output> outputs;
};

struct Processor : public GS_IProcessor
{
};
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
public:
    HAL();
    ~HAL();

    auto get_bus_factory()          -> Registry<node::bus::GS_IBus>&;
    auto get_source_factory()       -> Registry<node::source::GS_ISource>&;
    auto get_sink_factory()         -> Registry<node::sink::GS_ISink>&;
    auto get_processor_factory()    -> Registry<node::processor::GS_IProcessor>&;

    auto get_buses()        -> Registry<node::bus::GS_IBus>&;
    auto get_sources()      -> Registry<node::source::GS_ISource>&;
    auto get_sinks()        -> Registry<node::sink::GS_ISink>&;
    auto get_processors()   -> Registry<node::processor::GS_IProcessor>&;
    auto get_streams()      -> Registry<node::stream::GS_IStream>&;

    q::util::Signal<void()> node_factories_refreshed_signal;
    q::util::Signal<void()> nodes_refreshed_signal;

private:
    Registry<node::bus::GS_IBus> m_bus_factory;
    Registry<node::source::GS_ISource> m_source_factory;
    Registry<node::sink::GS_ISink> m_sink_factory;
    Registry<node::processor::GS_IProcessor> m_processor_factory;

    Registry<node::bus::GS_IBus> m_buses;
    Registry<node::source::GS_ISource> m_sources;
    Registry<node::sink::GS_ISink> m_sinks;
    Registry<node::processor::GS_IProcessor> m_processors;
    Registry<node::stream::GS_IStream> m_streams;
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
