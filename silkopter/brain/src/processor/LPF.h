#pragma once

#include "HAL.h"
#include "common/node/processor/IFilter.h"
#include "utils/Butterworth.h"

#include "sz_math.hpp"
#include "sz_LPF.hpp"

namespace silk
{
namespace node
{
namespace processor
{


template<class Stream_t>
class LPF : public IFilter<Stream_t>
{
public:
    static const int MAX_POLES = 8;

    LPF(HAL& hal);

    auto init(rapidjson::Value const& json) -> bool;
    auto get_init_params() -> boost::optional<rapidjson::Value const&>;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() -> boost::optional<rapidjson::Value const&>;

    auto get_input_stream_count() const -> size_t;
    auto get_input_stream(size_t idx) -> Stream_t&;
    auto get_output_stream_count() const -> size_t;
    auto get_output_stream(size_t idx) -> Stream_t&;
    auto get_name() const -> std::string const&;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    sz::LPF::Init_Params m_init_params;
    rapidjson::Document m_init_params_json;

    sz::LPF::Config m_config;
    rapidjson::Document m_config_json;

    Stream_t* m_input_stream = nullptr;

    util::Butterworth<MAX_POLES, Stream_t::FILTER_CHANNELS> m_dsp;

    struct Stream : public Stream_t
    {
        auto get_samples() const -> std::vector<typename Stream_t::Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }
        auto get_name() const -> std::string const& { return name; }

        uint32_t rate = 0;
        std::vector<typename Stream_t::Sample> samples;
        uint32_t sample_idx = 0;
        std::string name;
    } m_stream;
};


template<class Stream_t>
LPF<Stream_t>::LPF(HAL& hal)
    : m_hal(hal)
{
}

template<class Stream_t>
auto LPF<Stream_t>::init(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("lpf::init");

    sz::LPF::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize LPF data: {}", ss.str());
        return false;
    }
    m_init_params = sz;
    autojsoncxx::to_document(sz, m_init_params_json);

    return init();
}
template<class Stream_t>
auto LPF<Stream_t>::get_init_params() -> boost::optional<rapidjson::Value const&>
{
    return m_init_params_json;
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

    auto* input_stream = m_hal.get_streams().template find_by_name<Stream_t>(sz.inputs.stream);
    if (!input_stream || input_stream->get_rate() == 0)
    {
        QLOGE("No input input stream specified");
        return false;
    }
    if (m_stream.rate != 0 && m_stream.rate != input_stream->get_rate())
    {
        QLOGE("Input streams rate has changed: {} != {}",
              input_stream->get_rate(),
              m_stream.rate);
        return false;
    }

    if (sz.cutoff_frequency > input_stream->get_rate() / 2)
    {
        QLOGE("{}: Cutoff frequency {}Hz is bigger than the nyquist frequency of {}Hz",
              get_name(),
              sz.cutoff_frequency, input_stream->get_rate() / 2);
        return false;
    }

    sz.poles = math::clamp<uint32_t>(sz.poles, 1, MAX_POLES);
    if (!m_dsp.setup(sz.poles, get_input_stream(0).get_rate(), sz.cutoff_frequency))
    {
        QLOGE("{}: Cannot setup dsp filter.", get_name());
        return false;
    }

    m_input_stream = input_stream;
    m_stream.rate = m_input_stream->get_rate();

    m_config = sz;
    autojsoncxx::to_document(m_config, m_config_json);
    return true;
}
template<class Stream_t>
auto LPF<Stream_t>::get_config() -> boost::optional<rapidjson::Value const&>
{
    return m_config_json;
}

template<class Stream_t>
auto LPF<Stream_t>::get_input_stream_count() const -> size_t
{
    return m_input_stream ? 1 : 0;
}
template<class Stream_t>
auto LPF<Stream_t>::get_input_stream(size_t idx) -> Stream_t&
{
    QASSERT(idx == 0);
    QASSERT(m_input_stream);
    return *m_input_stream;
}
template<class Stream_t>
auto LPF<Stream_t>::get_output_stream_count() const -> size_t
{
    return 1;
}
template<class Stream_t>
auto LPF<Stream_t>::get_output_stream(size_t idx) -> Stream_t&
{
    QASSERT(idx == 0);
    return m_stream;
}

template<class Stream_t>
auto LPF<Stream_t>::get_name() const -> std::string const&
{
    return m_init_params.name;
}

template<class Stream_t>
void LPF<Stream_t>::process()
{
    m_stream.samples.clear();

    if (get_input_stream_count() == 0)
    {
        return;
    }

    auto const& is = get_input_stream(0).get_samples();
    m_stream.samples.reserve(is.size());

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
           m_stream.samples.push_back(vs);
       }
       else
       {
           m_stream.samples.push_back(s);
       }
    };
}

template<class Stream_t>
auto LPF<Stream_t>::init() -> bool
{
    if (!m_init_params.name.empty())
    {
        m_stream.name = q::util::format2<std::string>("{}/stream", m_init_params.name);
        if (!m_hal.get_processors().add(*this) ||
            !m_hal.get_streams().add(m_stream))
        {
            return false;
        }
    }

    return true;
}



}
}
}
