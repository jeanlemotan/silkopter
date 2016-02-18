#pragma once

#include "HAL.h"
#include "common/node/ICombiner.h"

#include "HAL.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"

#include "sz_math.hpp"
#include "sz_Combiner.hpp"


namespace sz
{
namespace Combiner
{
struct Init_Params;
struct Config;
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

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::shared_ptr<sz::Combiner::Init_Params> m_init_params;
    std::shared_ptr<sz::Combiner::Config> m_config;

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
    , m_init_params(new sz::Combiner::Init_Params())
    , m_config(new sz::Combiner::Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

template<class Stream_t>
auto Combiner<Stream_t>::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("Combiner::init");

    sz::Combiner::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Combiner data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init();
}

template<class Stream_t>
auto Combiner<Stream_t>::init() -> bool
{
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }

    m_output_stream->set_rate(m_init_params->rate);

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
        { Stream_t::TYPE, m_init_params->rate, "Input 1", m_accumulator.get_stream_path(0) },
        { Stream_t::TYPE, m_init_params->rate, "Input 2", m_accumulator.get_stream_path(1) }
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
    m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_hal);
}

template<class Stream_t>
auto Combiner<Stream_t>::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("Combiner::set_config");

    sz::Combiner::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Combiner config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    return true;
}

template<class Stream_t>
auto Combiner<Stream_t>::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

template<class Stream_t>
auto Combiner<Stream_t>::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}

template<class Stream_t>
auto Combiner<Stream_t>::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}


}
}
