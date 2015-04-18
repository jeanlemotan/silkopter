#pragma once

#include "HAL.h"
#include "common/node/ITransformer.h"
#include "common/node/stream/IFrame.h"

#include "sz_math.hpp"
#include "sz_Transformer.hpp"

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

    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    q::Clock::duration m_dt = q::Clock::duration(0);

    rapidjson::Document m_init_paramsj;
    sz::Transformer::Init_Params m_init_params;
    sz::Transformer::Config m_config;

    std::weak_ptr<In_Stream_t> m_input_stream;
    std::weak_ptr<Frame_Stream_t> m_frame_stream;

    std::vector<typename In_Stream_t::Sample> m_input_samples;
    std::vector<typename Frame_Stream_t::Sample> m_frame_samples;

    struct Stream : public Out_Stream_t
    {
        auto get_samples() const -> std::vector<typename Out_Stream_t::Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        uint32_t rate = 0;
        std::vector<typename Out_Stream_t::Sample> samples;
        typename Out_Stream_t::Sample last_sample;
    };
    mutable std::shared_ptr<Stream> m_output_stream;
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
    m_output_stream = std::make_shared<Stream>();
    if (m_init_params.rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params.rate);
        return false;
    }
    m_output_stream->rate = m_init_params.rate;
    m_dt = std::chrono::microseconds(1000000 / m_output_stream->rate);
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

    auto input_stream = m_hal.get_streams().template find_by_name<In_Stream_t>(sz.inputs.input);
    auto frame_stream = m_hal.get_streams().template find_by_name<Frame_Stream_t>(sz.inputs.frame);

    auto rate = input_stream ? input_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config.inputs.input.clear();
        QLOGE("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.input, m_output_stream->rate, rate);
        m_input_stream.reset();
    }
    else
    {
        m_input_stream = input_stream;
    }

    rate = frame_stream ? frame_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config.inputs.frame.clear();
        QLOGE("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.frame, m_output_stream->rate, rate);
        m_frame_stream.reset();
    }
    else
    {
        m_frame_stream = frame_stream;
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
auto Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { In_Stream_t::TYPE, m_init_params.rate, "Input" },
        { Out_Stream_t::TYPE, m_init_params.rate, "Frame" }
    }};
    return inputs;
}
template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].type = Out_Stream_t::TYPE;
    outputs[0].name = "Output";
    outputs[0].stream = m_output_stream;
    return outputs;
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
void Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::process()
{
    QLOG_TOPIC("transformer_inv::process");

    m_output_stream->samples.clear();

    auto input_stream = m_input_stream.lock();
    auto frame_stream = m_frame_stream.lock();
    if (!input_stream || !frame_stream)
    {
        return;
    }

    //accumulate the input streams
    {
        auto const& samples = input_stream->get_samples();
        m_input_samples.reserve(m_input_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_input_samples));
    }
    {
        auto const& samples = frame_stream->get_samples();
        m_frame_samples.reserve(m_frame_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_frame_samples));
    }

    //TODO add some protecton for severely out-of-sync streams

    size_t count = std::min(m_input_samples.size(), m_frame_samples.size());
    if (count == 0)
    {
        return;
    }

    m_output_stream->samples.resize(count);

    for (size_t i = 0; i < count; i++)
    {
        m_output_stream->last_sample.dt = m_dt;
        m_output_stream->last_sample.sample_idx++;
        m_output_stream->last_sample.value = math::rotate(m_frame_samples[i].value.rotation, m_input_samples[i].value);
        m_output_stream->samples[i] = m_output_stream->last_sample;
    }

    //consume processed samples
    m_input_samples.erase(m_input_samples.begin(), m_input_samples.begin() + count);
    m_frame_samples.erase(m_frame_samples.begin(), m_frame_samples.begin() + count);
}



}
}
