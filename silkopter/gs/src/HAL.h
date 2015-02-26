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

namespace silk
{

class HAL;

template<class Base>
class Registry : q::util::Noncopyable
{
    friend class HAL;
public:
    auto get_all() const -> std::vector<std::shared_ptr<Base>> const&;
    template<class T> auto find_by_name(std::string const& name) const -> std::shared_ptr<T>;

    template<class T> auto add(std::shared_ptr<T> node) -> bool;
    void remove_all();

private:
    std::vector<std::shared_ptr<Base>> m_nodes;
};

namespace node
{
class Common
{
public:
    std::string name;
    rapidjson::Document config;
};

namespace source
{

class Source : public ISource, public Common
{
public:
    auto get_name() const -> std::string const& { return name; }
    auto get_output_stream_count() const -> size_t { return output_streams.size(); }
    auto get_output_stream(size_t idx) -> node::stream::IStream& { return *output_streams[idx]; }
    auto init(rapidjson::Value const& json) -> bool { return true; }

    std::vector<std::shared_ptr<stream::IStream>> output_streams;
};
}

namespace sink
{
class Sink : public ISink, public Common
{
public:
    auto get_name() const -> std::string const& { return name; }
    auto get_input_stream_count() const -> size_t { return input_streams.size(); }
    auto get_input_stream(size_t idx) -> node::stream::IStream& { return *input_streams[idx]; }
    auto init(rapidjson::Value const& json) -> bool { return true; }

    std::vector<std::shared_ptr<stream::IStream>> input_streams;
};
}

namespace processor
{
class Processor : public IProcessor, public Common
{
public:
    auto get_name() const -> std::string const& { return name; }
    auto get_input_stream_count() const -> size_t { return input_streams.size(); }
    auto get_input_stream(size_t idx) -> node::stream::IStream& { return *input_streams[idx]; }
    auto get_output_stream_count() const -> size_t { return output_streams.size(); }
    auto get_output_stream(size_t idx) -> node::stream::IStream& { return *output_streams[idx]; }
    auto init(rapidjson::Value const& json) -> bool { return true; }

    std::vector<std::shared_ptr<stream::IStream>> input_streams;
    std::vector<std::shared_ptr<stream::IStream>> output_streams;
};
}

namespace stream
{
class Stream_Common : public Common
{
public:
    uint32_t rate = 0;
};

class Acceleration : public IAcceleration, public Stream_Common
{
public:
    auto get_name() const -> std::string const& { return name; }
    auto get_rate() const -> uint32_t { return rate; }
    auto get_samples() const -> std::vector<Sample> const& { return samples; }

    std::vector<Sample> samples;
};
class Angular_Velocity : public IAngular_Velocity, public Stream_Common
{
public:
    auto get_name() const -> std::string const& { return name; }
    auto get_rate() const -> uint32_t { return rate; }
    auto get_samples() const -> std::vector<Sample> const& { return samples; }

    std::vector<Sample> samples;
};
class ADC_Value : public IADC_Value, public Stream_Common
{
public:
    auto get_name() const -> std::string const& { return name; }
    auto get_rate() const -> uint32_t { return rate; }
    auto get_samples() const -> std::vector<Sample> const& { return samples; }

    std::vector<Sample> samples;
};
class Battery_State : public IBattery_State, public Stream_Common
{
public:
    auto get_name() const -> std::string const& { return name; }
    auto get_rate() const -> uint32_t { return rate; }
    auto get_samples() const -> std::vector<Sample> const& { return samples; }

    std::vector<Sample> samples;
};
class Cardinal_Points : public ICardinal_Points, public Stream_Common
{
public:
    auto get_name() const -> std::string const& { return name; }
    auto get_rate() const -> uint32_t { return rate; }
    auto get_samples() const -> std::vector<Sample> const& { return samples; }

    std::vector<Sample> samples;
};
class Current : public ICurrent, public Stream_Common
{
public:
    auto get_name() const -> std::string const& { return name; }
    auto get_rate() const -> uint32_t { return rate; }
    auto get_samples() const -> std::vector<Sample> const& { return samples; }

    std::vector<Sample> samples;
};
class Distance : public IDistance, public Stream_Common
{
public:
    auto get_name() const -> std::string const& { return name; }
    auto get_rate() const -> uint32_t { return rate; }
    auto get_samples() const -> std::vector<Sample> const& { return samples; }

    std::vector<Sample> samples;
};
class Linear_Acceleration : public ILinear_Acceleration, public Stream_Common
{
public:
    auto get_name() const -> std::string const& { return name; }
    auto get_rate() const -> uint32_t { return rate; }
    auto get_samples() const -> std::vector<Sample> const& { return samples; }

    std::vector<Sample> samples;
};
class Location : public ILocation, public Stream_Common
{
public:
    auto get_name() const -> std::string const& { return name; }
    auto get_rate() const -> uint32_t { return rate; }
    auto get_samples() const -> std::vector<Sample> const& { return samples; }

    std::vector<Sample> samples;
};
class Magnetic_Field : public IMagnetic_Field, public Stream_Common
{
public:
    auto get_name() const -> std::string const& { return name; }
    auto get_rate() const -> uint32_t { return rate; }
    auto get_samples() const -> std::vector<Sample> const& { return samples; }

    std::vector<Sample> samples;
};
class Pressure : public IPressure, public Stream_Common
{
public:
    auto get_name() const -> std::string const& { return name; }
    auto get_rate() const -> uint32_t { return rate; }
    auto get_samples() const -> std::vector<Sample> const& { return samples; }

    std::vector<Sample> samples;
};
class PWM_Value : public IPWM_Value, public Stream_Common
{
public:
    auto get_name() const -> std::string const& { return name; }
    auto get_rate() const -> uint32_t { return rate; }
    auto get_samples() const -> std::vector<Sample> const& { return samples; }

    std::vector<Sample> samples;
};
class Reference_Frame : public IReference_Frame, public Stream_Common
{
public:
    auto get_name() const -> std::string const& { return name; }
    auto get_rate() const -> uint32_t { return rate; }
    auto get_samples() const -> std::vector<Sample> const& { return samples; }

    std::vector<Sample> samples;
};
class Temperature : public ITemperature, public Stream_Common
{
public:
    auto get_name() const -> std::string const& { return name; }
    auto get_rate() const -> uint32_t { return rate; }
    auto get_samples() const -> std::vector<Sample> const& { return samples; }

    std::vector<Sample> samples;
};
class Voltage : public IVoltage, public Stream_Common
{
public:
    auto get_name() const -> std::string const& { return name; }
    auto get_rate() const -> uint32_t { return rate; }
    auto get_samples() const -> std::vector<Sample> const& { return samples; }

    std::vector<Sample> samples;
};

}
}


class HAL : q::util::Noncopyable
{
public:
    HAL();
    ~HAL();

    auto get_sources()      -> Registry<node::source::ISource>&;
    auto get_sinks()        -> Registry<node::sink::ISink>&;
    auto get_processors()   -> Registry<node::processor::IProcessor>&;
    auto get_streams()      -> Registry<node::stream::IStream>&;

private:
    Registry<node::source::ISource> m_sources;
    Registry<node::sink::ISink> m_sinks;
    Registry<node::processor::IProcessor> m_processors;
    Registry<node::stream::IStream> m_streams;
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
    auto it = std::find_if(m_nodes.begin(), m_nodes.end(), [&](std::shared_ptr<Base> const& s) { return s->get_name() == name; });
    return it != m_nodes.end() ? std::dynamic_pointer_cast<T>(*it) : std::shared_ptr<T>();
}
template<class Base>
template<class T>
auto Registry<Base>::add(std::shared_ptr<T> node) -> bool
{
    QASSERT(node);
    if (find_by_name<Base>(node->get_name()))
    {
        QLOGE("Duplicated name in node {}", node->get_name());
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
