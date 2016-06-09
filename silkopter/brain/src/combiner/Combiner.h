#pragma once

#include "UAV.h"
#include "common/node/ICombiner.h"

#include "UAV.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"

#include "uav.def.h"
//#include "sz_Combiner.hpp"


namespace silk
{
struct Combiner_Descriptor;
struct Combiner_Config;
}


namespace silk
{
namespace node
{

template<class Stream_t>
class Combiner : public ICombiner
{
public:
    Combiner(UAV& uav);

    bool init(std::shared_ptr<Node_Descriptor_Base> descriptor) override;
    std::shared_ptr<Node_Descriptor_Base> get_descriptor() const override;

    bool set_config(std::shared_ptr<Node_Config_Base> config) override;
    std::shared_ptr<Node_Config_Base> get_config() const override;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    UAV& m_uav;

    std::shared_ptr<Combiner_Descriptor> m_descriptor;
    std::shared_ptr<Combiner_Config> m_config;

    Sample_Accumulator<Stream_t, Stream_t> m_accumulator;

    typedef typename Stream_t::Value Value;

    Value m_last_valid_value1;
    Value m_last_valid_value2;
    Value m_average;

    typedef Basic_Output_Stream<Stream_t> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;

};

template<class Stream_t>
Combiner<Stream_t>::Combiner(UAV& uav)
    : m_uav(uav)
    , m_descriptor(new Combiner_Descriptor())
    , m_config(new Combiner_Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

template<class Stream_t>
auto Combiner<Stream_t>::init(std::shared_ptr<Node_Descriptor_Base> descriptor) -> bool
{
    QLOG_TOPIC("Combiner::init");

    auto specialized = std::dynamic_pointer_cast<Combiner_Descriptor>(descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }

    *m_descriptor = *specialized;

    return init();
}

template<class Stream_t>
auto Combiner<Stream_t>::init() -> bool
{
    if (m_descriptor->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_descriptor->rate);
        return false;
    }

    m_output_stream->set_rate(m_descriptor->rate);

    return true;
}

template<class Stream_t>
auto Combiner<Stream_t>::start(q::Clock::time_point tp) -> bool
{
    m_output_stream->set_tp(tp);
    return true;
}

template<class Stream_t>
auto Combiner<Stream_t>::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { Stream_t::TYPE, m_descriptor->rate, "Input 1", m_accumulator.get_stream_path(0) },
        { Stream_t::TYPE, m_descriptor->rate, "Input 2", m_accumulator.get_stream_path(1) }
    }};
    return inputs;
}

template<class Stream_t>
auto Combiner<Stream_t>::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
         { "Output", m_output_stream }
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
void Combiner<Stream_t>::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_uav);
}

template<class Stream_t>
auto Combiner<Stream_t>::set_config(std::shared_ptr<Node_Config_Base> config) -> bool
{
    QLOG_TOPIC("Combiner::set_config");

    auto specialized = std::dynamic_pointer_cast<Combiner_Config>(config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }

    *m_config = *specialized;

    return true;
}

template<class Stream_t>
auto Combiner<Stream_t>::get_config() const -> std::shared_ptr<Node_Config_Base>
{
    return m_config;
}

template<class Stream_t>
auto Combiner<Stream_t>::get_descriptor() const -> std::shared_ptr<Node_Descriptor_Base>
{
    return m_descriptor;
}

template<class Stream_t>
auto Combiner<Stream_t>::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}


}
}
