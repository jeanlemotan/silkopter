#pragma once

#include "HAL.h"
#include "common/node/processor/ITransformer.h"
#include "common/node/stream/IFrame_Stream.h"

#include "sz_math.hpp"
#include "sz_Transformer.hpp"

namespace silk
{
namespace node
{

template<class In_Stream_t, class Out_Stream_t>
class Transformer : public ITransformer
{
public:
    Transformer(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document const&;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    rapidjson::Document m_init_paramsj;
    sz::Transformer::Init_Params m_init_params;
    sz::Transformer::Config m_config;

    std::weak_ptr<In_Stream_t> m_input_stream;
    std::weak_ptr<IENU_Frame_Stream> m_input_frame_stream;

    struct Stream : public Out_Stream_t
    {
        auto get_samples() const -> std::vector<typename Out_Stream_t::Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        uint32_t rate = 0;
        std::vector<typename Out_Stream_t::Sample> samples;
    };
    mutable std::shared_ptr<Stream> m_output_stream;
};


template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
Transformer<In_Stream_t, Out_Stream_t, Frame_Stream_t>::Transformer(HAL& hal)
    : m_hal(hal)
{
    autojsoncxx::to_document(m_init_params, m_init_paramsj);
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
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    m_init_params = sz;
    return init();
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer<In_Stream_t, Out_Stream_t, Frame_Stream_t>::init() -> bool
{
    m_output_stream = std::make_shared<Stream>();
    if (m_init_params.rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params.rate);
        return false;
    }
    m_output_stream->rate = m_init_params.rate;
    return true;
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer<In_Stream_t, Out_Stream_t, Frame_Stream_t>::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
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

    auto input_stream = m_hal.get_streams().template find_by_name<In_Stream_t>(sz.inputs.input);

    auto rate = input_stream ? input_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        QLOGE("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.input, m_output_stream->rate, rate);
        m_input_stream.reset();
    }
    else
    {
        m_input_stream = input_stream;
    }

    if (sz.cutoff_frequency > m_output_stream->rate / 2)
    {
        QLOGE("Cutoff frequency {}Hz is bigger than the nyquist frequency of {}Hz",
              sz.cutoff_frequency, m_output_stream->rate / 2);
        return false;
    }

    sz.poles = math::max<uint32_t>(sz.poles, 2);
    if (!m_dsp.setup(sz.poles, m_output_stream->rate, sz.cutoff_frequency))
    {
        QLOGE("Cannot setup dsp filter.");
        return false;
    }
    m_dsp.reset();

    m_config = sz;

    return true;
}
template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer<In_Stream_t, Out_Stream_t, Frame_Stream_t>::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(m_config, json);
    return std::move(json);
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer<In_Stream_t, Out_Stream_t, Frame_Stream_t>::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs(1);
    inputs[0].type = In_Stream_t::TYPE;
    inputs[0].rate = m_output_stream ? m_output_stream->rate : 0;
    inputs[0].name = "Input";
    return inputs;
}
template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer<In_Stream_t, Out_Stream_t, Frame_Stream_t>::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].type = In_Stream_t::TYPE;
    outputs[0].name = "Output";
    outputs[0].stream = m_output_stream;
    return outputs;
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
void Transformer<In_Stream_t, Out_Stream_t, Frame_Stream_t>::process()
{
    QLOG_TOPIC("transformer::process");

    m_output_stream->samples.clear();

    auto input_stream = m_input_stream.lock();
    if (!input_stream)
    {
        return;
    }

    auto const& is = input_stream->get_samples();
    m_output_stream->samples.reserve(is.size());

    for (auto const& s: is)
    {
       if (s.is_healthy)
       {
           m_output_stream->samples.push_back(s);
           m_dsp.process(m_output_stream->samples.back().value);
       }
       else
       {
           m_output_stream->samples.push_back(s);
       }
    };
}



}
}
