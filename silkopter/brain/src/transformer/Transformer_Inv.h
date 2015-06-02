#pragma once

#include "HAL.h"
#include "common/node/ITransformer.h"
#include "common/node/stream/IFrame.h"

#include "sz_math.hpp"
#include "sz_Transformer.hpp"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"

namespace silk
{
namespace node
{

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
class Transformer_Inv : public ITransformer
{
public:
    Transformer_Inv(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document const&;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto get_stream_inputs() const -> std::vector<Stream_Input>;
    auto get_stream_outputs() const -> std::vector<Stream_Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    rapidjson::Document m_init_paramsj;
    sz::Transformer::Init_Params m_init_params;
    sz::Transformer::Config m_config;

    Sample_Accumulator<In_Stream_t, Frame_Stream_t> m_accumulator;

    typedef Basic_Output_Stream<Out_Stream_t> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::Transformer_Inv(HAL& hal)
    : m_hal(hal)
{
    static_assert(In_Stream_t::TYPE == Out_Stream_t::TYPE, "Both streams need to be of the same type");
    static_assert(In_Stream_t::SPACE == Frame_Stream_t::SPACE, "Bad Input stream or Frame");
    static_assert(Out_Stream_t::SPACE == Frame_Stream_t::PARENT_SPACE, "Bad Output stream or Frame");

    autojsoncxx::to_document(m_init_params, m_init_paramsj);
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("transformer_inv::init");

    sz::Transformer::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Transformer_Inv data: {}", ss.str());
        return false;
    }
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    m_init_params = sz;
    return init();
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::init() -> bool
{
    m_output_stream = std::make_shared<Output_Stream>();
    if (m_init_params.rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params.rate);
        return false;
    }
    m_output_stream->set_rate(m_init_params.rate);
    m_output_stream->set_tp(q::Clock::now());
    return true;
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("transformer_inv::set_config");
    sz::Transformer::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Transformer_Inv config data: {}", ss.str());
        return false;
    }

    m_config = sz;
    m_accumulator.clear_streams();

    auto output_rate = m_output_stream->get_rate();

    auto input_stream = m_hal.get_streams().template find_by_name<In_Stream_t>(sz.input_streams.input);
    auto frame_stream = m_hal.get_streams().template find_by_name<Frame_Stream_t>(sz.input_streams.frame);

    auto rate = input_stream ? input_stream->get_rate() : 0u;
    if (rate != output_rate)
    {
        m_config.input_streams.input.clear();
        QLOGE("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.input, output_rate, rate);
    }
    else
    {
        m_accumulator.template set_stream<0>(input_stream);
    }

    rate = frame_stream ? frame_stream->get_rate() : 0u;
    if (rate != output_rate)
    {
        m_config.input_streams.frame.clear();
        QLOGE("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.frame, output_rate, rate);
    }
    else
    {
        m_accumulator.template set_stream<1>(frame_stream);
    }

    return true;
}
template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}
template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(m_config, json);
    return std::move(json);
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::get_stream_inputs() const -> std::vector<Stream_Input>
{
    std::vector<Stream_Input> inputs =
    {{
        { In_Stream_t::TYPE, m_init_params.rate, "Input" },
        { Out_Stream_t::TYPE, m_init_params.rate, "Frame" }
    }};
    return inputs;
}
template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::get_stream_outputs() const -> std::vector<Stream_Output>
{
    std::vector<Stream_Output> outputs(1);
    outputs[0].type = Out_Stream_t::TYPE;
    outputs[0].name = "Output";
    outputs[0].stream = m_output_stream;
    return outputs;
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
void Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::process()
{
    QLOG_TOPIC("transformer_inv::process");

    m_output_stream->clear();

    m_accumulator.process([this](
                          size_t idx,
                          typename In_Stream_t::Sample const& in_sample,
                          typename Frame_Stream_t::Sample const& f_sample)
    {
        m_output_stream->push_sample(math::rotate(f_sample.value.rotation, in_sample.value), in_sample.is_healthy & f_sample.is_healthy);
    });
}



}
}
