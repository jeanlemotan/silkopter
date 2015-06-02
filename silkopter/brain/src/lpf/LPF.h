#pragma once

#include "HAL.h"
#include "common/node/ILPF.h"
#include "utils/Butterworth.h"
#include "Basic_Output_Stream.h"

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
    LPF(HAL& hal);

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
    sz::LPF::Init_Params m_init_params;
    sz::LPF::Config m_config;

    std::weak_ptr<Stream_t> m_input_stream;

    util::Butterworth<typename Stream_t::Value> m_dsp;

    typedef Basic_Output_Stream<Stream_t> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


template<class Stream_t>
LPF<Stream_t>::LPF(HAL& hal)
    : m_hal(hal)
{
    autojsoncxx::to_document(m_init_params, m_init_paramsj);
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
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    m_init_params = sz;
    return init();
}

template<class Stream_t>
auto LPF<Stream_t>::init() -> bool
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

template<class Stream_t>
auto LPF<Stream_t>::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}

template<class Stream_t>
auto LPF<Stream_t>::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("lpf::set_config");
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(m_config, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize LPF config data: {}", ss.str());
        return false;
    }

    auto input_stream = m_hal.get_streams().template find_by_name<Stream_t>(m_config.input_streams.input);

    auto output_rate = m_output_stream->get_rate();

    auto rate = input_stream ? input_stream->get_rate() : 0u;
    if (rate != output_rate)
    {
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", m_config.input_streams.input, output_rate, rate);
        m_config.input_streams.input.clear();
        m_input_stream.reset();
    }
    else
    {
        m_input_stream = input_stream;
    }

    if (m_config.cutoff_frequency > output_rate / 2)
    {
        QLOGE("Cutoff frequency {}Hz is bigger than the nyquist frequency of {}Hz",
              m_config.cutoff_frequency, output_rate / 2);
        return false;
    }

    m_config.poles = math::max<uint32_t>(m_config.poles, 1);
    if (m_config.poles > 0 &&
        m_config.cutoff_frequency > 0 &&
        !m_dsp.setup(m_config.poles, output_rate, m_config.cutoff_frequency))
    {
        QLOGE("Cannot setup dsp filter.");
        return false;
    }
    m_dsp.reset();

    return true;
}
template<class Stream_t>
auto LPF<Stream_t>::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}
template<class Stream_t>
auto LPF<Stream_t>::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(m_config, json);
    return std::move(json);
}

template<class Stream_t>
auto LPF<Stream_t>::get_stream_inputs() const -> std::vector<Stream_Input>
{
    std::vector<Stream_Input> inputs =
    {{
        { Stream_t::TYPE, m_init_params.rate, "Input" }
    }};
    return inputs;
}
template<class Stream_t>
auto LPF<Stream_t>::get_stream_outputs() const -> std::vector<Stream_Output>
{
    std::vector<Stream_Output> outputs(1);
    outputs[0].type = Stream_t::TYPE;
    outputs[0].name = "Output";
    outputs[0].stream = m_output_stream;
    return outputs;
}

template<class Stream_t>
void LPF<Stream_t>::process()
{
    QLOG_TOPIC("lpf::process");

    m_output_stream->clear();

    auto input_stream = m_input_stream.lock();
    if (!input_stream || m_config.cutoff_frequency == 0)
    {
        return;
    }

    auto const& is = input_stream->get_samples();
    for (auto const& s: is)
    {
        auto value = s.value;
        m_dsp.process(value);
        m_output_stream->push_sample(value, s.is_healthy);
    };
}



}
}
