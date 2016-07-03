#pragma once

#include "HAL.h"
#include "common/node/ICombiner.h"

#include "HAL.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"

#include "hal.def.h"
//#include "sz_Combiner.hpp"


namespace silk
{
namespace hal
{
struct Combiner_Descriptor;
struct Combiner_Config;
}
}


namespace silk
{
namespace node
{

template<class Stream_t>
class Combiner : public ICombiner
{
public:
    Combiner(HAL& hal);

    ts::Result<void> init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    ts::Result<void> set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    ts::Result<void> start(q::Clock::time_point tp) override;

    ts::Result<void> set_input_stream_path(size_t idx, std::string const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    ts::Result<void> init();

    HAL& m_hal;

    std::shared_ptr<hal::Combiner_Descriptor> m_descriptor;
    std::shared_ptr<hal::Combiner_Config> m_config;

    Sample_Accumulator<Stream_t, Stream_t> m_accumulator;

    typedef typename Stream_t::Value Value;

    Value m_last_valid_value1;
    Value m_last_valid_value2;
    Value m_average;

    typedef Basic_Output_Stream<Stream_t> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;

};

template<class Stream_t>
Combiner<Stream_t>::Combiner(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::Combiner_Descriptor())
    , m_config(new hal::Combiner_Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

template<class Stream_t>
ts::Result<void> Combiner<Stream_t>::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("Combiner::init");

    auto specialized = dynamic_cast<hal::Combiner_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}

template<class Stream_t>
ts::Result<void> Combiner<Stream_t>::init()
{
    m_output_stream->set_rate(m_descriptor->get_rate());
    return ts::success;
}

template<class Stream_t>
ts::Result<void> Combiner<Stream_t>::start(q::Clock::time_point tp)
{
    m_output_stream->set_tp(tp);
    return ts::success;
}

template<class Stream_t>
auto Combiner<Stream_t>::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { Stream_t::TYPE, m_descriptor->get_rate(), "Input 1", m_accumulator.get_stream_path(0) },
        { Stream_t::TYPE, m_descriptor->get_rate(), "Input 2", m_accumulator.get_stream_path(1) }
    }};
    return inputs;
}

template<class Stream_t>
auto Combiner<Stream_t>::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
         { "output", m_output_stream }
    }};
    return outputs;
}

template<class Stream_t>
void Combiner<Stream_t>::process()
{
    QLOG_TOPIC("Combiner::process");

    m_output_stream->clear();

    m_accumulator.process([this](typename Stream_t::Sample const& sample1, typename Stream_t::Sample const& sample2)
    {
        if (sample1.is_healthy & sample2.is_healthy)
        {
            m_last_valid_value1 = sample1.value;
            m_last_valid_value2 = sample2.value;
            m_average = (sample1.value + sample2.value) * 0.5f;
            m_output_stream->push_sample(m_average, true);
        }
        else if (sample1.is_healthy)
        {
            m_last_valid_value1 = sample1.value;
            m_output_stream->push_sample(sample1.value, true);
        }
        else if (sample2.is_healthy)
        {
            m_last_valid_value2 = sample2.value;
            m_output_stream->push_sample(sample2.value, true);
        }
        else
        {
            m_output_stream->push_last_sample(false);
        }
    });
}

template<class Stream_t>
ts::Result<void> Combiner<Stream_t>::set_input_stream_path(size_t idx, std::string const& path)
{
    return m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_hal);
}

template<class Stream_t>
ts::Result<void> Combiner<Stream_t>::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("Combiner::set_config");

    auto specialized = dynamic_cast<hal::Combiner_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    return ts::success;
}

template<class Stream_t>
auto Combiner<Stream_t>::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

template<class Stream_t>
auto Combiner<Stream_t>::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}

//template<class Stream_t>
//auto Combiner<Stream_t>::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}


}
}
