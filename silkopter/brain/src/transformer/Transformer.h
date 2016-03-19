#pragma once

#include "HAL.h"
#include "common/node/ITransformer.h"
#include "common/stream/IFrame.h"

#include "sz_math.hpp"
#include "sz_Transformer.hpp"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"

namespace silk
{
namespace node
{

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
class Transformer : public ITransformer
{
public:
    Transformer(HAL& hal);

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

    sz::Transformer::Init_Params m_init_params;
    sz::Transformer::Config m_config;

    Sample_Accumulator<In_Stream_t, Frame_Stream_t> m_accumulator;

    typedef Basic_Output_Stream<Out_Stream_t> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
Transformer<In_Stream_t, Out_Stream_t, Frame_Stream_t>::Transformer(HAL& hal)
    : m_hal(hal)
{
    static_assert(In_Stream_t::SEMANTIC == Out_Stream_t::SEMANTIC, "Both streams need to have the same type");
    static_assert(In_Stream_t::SPACE == Frame_Stream_t::PARENT_SPACE, "Bad Input stream or Frame");
    static_assert(Out_Stream_t::SPACE == Frame_Stream_t::SPACE, "Bad Output stream or Frame");

    m_output_stream = std::make_shared<Output_Stream>();
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer<In_Stream_t, Out_Stream_t, Frame_Stream_t>::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("transformer::init");

    sz::Transformer::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Transformer data: {}", ss.str());
        return false;
    }
    m_init_params = sz;
    return init();
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer<In_Stream_t, Out_Stream_t, Frame_Stream_t>::init() -> bool
{
    if (m_init_params.rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params.rate);
        return false;
    }
    m_output_stream->set_rate(m_init_params.rate);
    return true;
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer<In_Stream_t, Out_Stream_t, Frame_Stream_t>::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(m_init_params, json);
    return std::move(json);
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
void Transformer<In_Stream_t, Out_Stream_t, Frame_Stream_t>::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_hal);
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer<In_Stream_t, Out_Stream_t, Frame_Stream_t>::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("transformer::set_config");
    sz::Transformer::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Transformer config data: {}", ss.str());
        return false;
    }

    m_config = sz;

    return true;
}
template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer<In_Stream_t, Out_Stream_t, Frame_Stream_t>::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}
template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer<In_Stream_t, Out_Stream_t, Frame_Stream_t>::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(m_config, json);
    return std::move(json);
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer<In_Stream_t, Out_Stream_t, Frame_Stream_t>::start(q::Clock::time_point tp) -> bool
{
    m_output_stream->set_tp(tp);
    return true;
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer<In_Stream_t, Out_Stream_t, Frame_Stream_t>::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { In_Stream_t::TYPE, m_init_params.rate, "Input", m_accumulator.get_stream_path(0) },
        { Frame_Stream_t::TYPE, m_init_params.rate, "Frame", m_accumulator.get_stream_path(1) }
    }};
    return inputs;
}
template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer<In_Stream_t, Out_Stream_t, Frame_Stream_t>::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
        { "Output", m_output_stream }
    }};
    return outputs;
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
void Transformer<In_Stream_t, Out_Stream_t, Frame_Stream_t>::process()
{
    QLOG_TOPIC("transformer::process");

    m_output_stream->clear();

    m_accumulator.process([this](typename In_Stream_t::Sample const& in_sample,
                                typename Frame_Stream_t::Sample const& f_sample)
    {
        if (in_sample.is_healthy & f_sample.is_healthy)
        {
            m_output_stream->push_sample(math::rotate(math::inverse(f_sample.value), in_sample.value), true);
        }
        else
        {
            m_output_stream->push_last_sample(false);
        }
    });
}



}
}
