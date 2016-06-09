#pragma once

#include "UAV.h"
#include "common/node/IGenerator.h"
#include "generator/Oscillator.h"

#include "Basic_Output_Stream.h"

//#include "sz_math.hpp"
//#include "sz_Scalar_Generator.hpp"

namespace silk
{
namespace node
{

template<class Stream_t>
class Scalar_Generator : public IGenerator
{
public:
    Scalar_Generator(UAV& uav);

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

    sz::Scalar_Generator::Init_Params m_descriptor;
    sz::Scalar_Generator::Config m_config;

    std::weak_ptr<stream::IFloat> m_modulation_stream;
    q::Path m_modulation_stream_path;

    typedef Basic_Output_Stream<Stream_t> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


template<class Stream_t>
Scalar_Generator<Stream_t>::Scalar_Generator(UAV& uav)
    : m_uav(uav)
{
    m_output_stream = std::make_shared<Output_Stream>();
}

template<class Stream_t>
auto Scalar_Generator<Stream_t>::init(std::shared_ptr<Node_Descriptor_Base> descriptor) -> bool
{
    QLOG_TOPIC("scalar_generator::init");

    auto specialized = std::dynamic_pointer_cast<Scalar_Generator_Descriptor>(descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }

    *m_descriptor = *specialized;

    return init();
}

template<class Stream_t>
auto Scalar_Generator<Stream_t>::init() -> bool
{
    if (m_descriptor.rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_descriptor.rate);
        return false;
    }
    m_output_stream->set_rate(m_descriptor.rate);
    return true;
}

template<class Stream_t>
auto Scalar_Generator<Stream_t>::get_descriptor() const -> std::shared_ptr<Node_Descriptor_Base>
{
    rapidjson::Document json;
    autojsoncxx::to_document(m_descriptor, json);
    return std::move(json);
}

template<class Stream_t>
auto Scalar_Generator<Stream_t>::start(q::Clock::time_point tp) -> bool
{
    m_output_stream->set_tp(tp);
    return true;
}

template<class Stream_t>
void Scalar_Generator<Stream_t>::set_input_stream_path(size_t idx, q::Path const& path)
{
    auto modulation_stream = m_uav.get_streams().template find_by_name<stream::IFloat>(path.get_as<std::string>());
    if (modulation_stream && modulation_stream->get_rate() != m_output_stream->get_rate())
    {
        QLOGW("Bad modulation stream '{}'. Expected rate {}Hz, got {}Hz", path, m_output_stream->get_rate(), modulation_stream->get_rate());
        m_modulation_stream.reset();
        m_modulation_stream_path.clear();
    }
    else
    {
        m_modulation_stream = modulation_stream;
        m_modulation_stream_path = path;
    }
}

template<class Stream_t>
auto Scalar_Generator<Stream_t>::set_config(std::shared_ptr<Node_Config_Base> config) -> bool
{
    QLOG_TOPIC("scalar_generator::set_config");

    auto specialized = std::dynamic_pointer_cast<Scalar_Generator_Config>(config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }

    *m_config = *specialized;

    return true;
}

template<class Stream_t>
auto Scalar_Generator<Stream_t>::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}
template<class Stream_t>
auto Scalar_Generator<Stream_t>::get_config() const -> std::shared_ptr<Node_Config_Base>
{
    rapidjson::Document json;
    autojsoncxx::to_document(m_config, json);
    return std::move(json);
}

template<class Stream_t>
auto Scalar_Generator<Stream_t>::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IFloat::TYPE, m_descriptor.rate, "Modulation", m_modulation_stream_path }
    }};
    return inputs;
}
template<class Stream_t>
auto Scalar_Generator<Stream_t>::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].name = "Output";
    outputs[0].stream = m_output_stream;
    return outputs;
}

template<class Stream_t>
void Scalar_Generator<Stream_t>::process()
{
    QLOG_TOPIC("scalar_generator::process");

    m_output_stream->clear();

    auto modulation_stream = m_modulation_stream.lock();
    if (modulation_stream)
    {
        auto const& samples = modulation_stream->get_samples();
        for (auto const& s: samples)
        {
            if (s.is_healthy)
            {
                m_output_stream->push_sample(m_config.value + s.value, true);
            }
            else
            {
                m_output_stream->push_last_sample(false);
            }
        }
    }
    else
    {
        auto samples_needed = m_output_stream->compute_samples_needed();
        while (samples_needed > 0)
        {
            m_output_stream->push_sample(m_config.value, true);
            samples_needed--;
        }
    }
}



}
}
