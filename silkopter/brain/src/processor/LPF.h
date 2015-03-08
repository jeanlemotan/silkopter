#pragma once

#include "HAL.h"
#include "common/node/processor/ILPF.h"
#include "utils/Butterworth.h"

#include "sz_math.hpp"
#include "sz_LPF.hpp"

namespace silk
{
namespace node
{

template<class Stream_t>
class LPF : public ILPF
{
public:
    static const int MAX_POLES = 8;

    LPF(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    sz::LPF::Init_Params m_init_params;
    sz::LPF::Config m_config;

    std::weak_ptr<Stream_t> m_input_stream;

    util::Butterworth<MAX_POLES, Stream_t::FILTER_CHANNELS> m_dsp;

    struct Stream : public Stream_t
    {
        auto get_samples() const -> std::vector<typename Stream_t::Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        uint32_t rate = 0;
        std::vector<typename Stream_t::Sample> samples;
        uint32_t sample_idx = 0;
    };
    mutable std::shared_ptr<Stream> m_output_stream;
};


template<class Stream_t>
LPF<Stream_t>::LPF(HAL& hal)
    : m_hal(hal)
{
}

template<class Stream_t>
auto LPF<Stream_t>::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("lpf::init");

    sz::LPF::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize LPF data: {}", ss.str());
        return false;
    }
    m_init_params = sz;
    return init();
}

template<class Stream_t>
auto LPF<Stream_t>::init() -> bool
{
    m_output_stream = std::make_shared<Stream>();
    return true;
}

template<class Stream_t>
auto LPF<Stream_t>::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(m_init_params, json);
    return std::move(json);
}

template<class Stream_t>
auto LPF<Stream_t>::set_config(rapidjson::Value const& json) -> bool
{
    sz::LPF::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize LPF config data: {}", ss.str());
        return false;
    }

    m_config = sz;
    m_output_stream->rate = 0;

    auto input_stream = m_hal.get_streams().template find_by_name<Stream_t>(sz.inputs.input);
    m_input_stream = input_stream;

    auto rate = input_stream ? input_stream->get_rate() : 0u;
    if (rate == 0)
    {
        QLOGE("Bad input stream '{}'. Rate {}Hz", sz.inputs.input, rate);
        return false;
    }

    if (sz.cutoff_frequency > input_stream->get_rate() / 2)
    {
        QLOGE("Cutoff frequency {}Hz is bigger than the nyquist frequency of {}Hz",
              sz.cutoff_frequency, input_stream->get_rate() / 2);
        return false;
    }

    sz.poles = math::clamp<uint32_t>(sz.poles, 1, MAX_POLES);
    if (!m_dsp.setup(sz.poles, input_stream->get_rate(), sz.cutoff_frequency))
    {
        QLOGE("Cannot setup dsp filter.");
        return false;
    }

    m_output_stream->rate = rate;

    return true;
}
template<class Stream_t>
auto LPF<Stream_t>::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(m_config, json);
    return std::move(json);
}

template<class Stream_t>
auto LPF<Stream_t>::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs(1);
    inputs[0].class_id = q::rtti::get_class_id<Stream_t>();
    inputs[0].name = "Input";
    return inputs;
}
template<class Stream_t>
auto LPF<Stream_t>::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].class_id = q::rtti::get_class_id<Stream_t>();
    outputs[0].name = "Output";
    return outputs;
}

template<class Stream_t>
void LPF<Stream_t>::process()
{
    m_output_stream->samples.clear();

    auto input_stream = m_input_stream.lock();
    if (!input_stream)
    {
        return;
    }

    auto const& is = input_stream->get_samples();
    m_output_stream->samples.reserve(is.size());

    std::array<double, Stream_t::FILTER_CHANNELS> channels;
    for (auto& s: is)
    {
       if (Stream_t::get_channels_from_value(channels, s.value))
       {
           m_dsp.process(channels.data());

           typename Stream_t::Sample vs;
           vs.dt = s.dt;
           vs.sample_idx = s.sample_idx;
           vs.value = s.value;
           Stream_t::get_value_from_channels(vs.value, channels);
           m_output_stream->samples.push_back(vs);
       }
       else
       {
           m_output_stream->samples.push_back(s);
       }
    };
}



}
}
