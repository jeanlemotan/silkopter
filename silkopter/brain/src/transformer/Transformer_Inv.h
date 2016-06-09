#pragma once

#include "UAV.h"
#include "common/node/ITransformer.h"
#include "common/stream/IFrame.h"

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
    Transformer_Inv(UAV& uav);

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

    std::shared_ptr<Transformer_Descriptor> m_descriptor;
    std::shared_ptr<Transformer_Config> m_config;

    Sample_Accumulator<In_Stream_t, Frame_Stream_t> m_accumulator;

    typedef Basic_Output_Stream<Out_Stream_t> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::Transformer_Inv(UAV& uav)
    : m_uav(uav)
{
    static_assert(In_Stream_t::SEMANTIC == Out_Stream_t::SEMANTIC, "Both streams need to have the same semantic");
    static_assert(In_Stream_t::SPACE == Frame_Stream_t::SPACE, "Bad Input stream or Frame");
    static_assert(Out_Stream_t::SPACE == Frame_Stream_t::PARENT_SPACE, "Bad Output stream or Frame");

    m_output_stream = std::make_shared<Output_Stream>();
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::init(std::shared_ptr<Node_Descriptor_Base> descriptor) -> bool
{
    QLOG_TOPIC("transformer_inv::init");

    auto specialized = std::dynamic_pointer_cast<Transformer_Descriptor>(descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }

    *m_descriptor = *specialized;

    return init();
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::init() -> bool
{
    m_output_stream->set_rate(m_descriptor->get_rate());
    return true;
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::get_descriptor() const -> std::shared_ptr<Node_Descriptor_Base>
{
    return m_descriptor;
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
void Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_uav);
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::set_config(std::shared_ptr<Node_Config_Base> config) -> bool
{
    QLOG_TOPIC("transformer_inv::set_config");

    auto specialized = std::dynamic_pointer_cast<Transformer_Config>(config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }

    *m_config = *specialized;

    return true;
}
template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}
template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::get_config() const -> std::shared_ptr<Node_Config_Base>
{
    return m_config;
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::start(q::Clock::time_point tp) -> bool
{
    m_output_stream->set_tp(tp);
    return true;
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { In_Stream_t::TYPE, m_descriptor->get_rate(), "Input", m_accumulator.get_stream_path(0) },
        { Frame_Stream_t::TYPE, m_descriptor->get_rate(), "Frame", m_accumulator.get_stream_path(1) }
    }};
    return inputs;
}
template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].name = "Output";
    outputs[0].stream = m_output_stream;
    return outputs;
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
void Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::process()
{
    QLOG_TOPIC("transformer_inv::process");

    m_output_stream->clear();

    m_accumulator.process([this](typename In_Stream_t::Sample const& in_sample,
                                  typename Frame_Stream_t::Sample const& f_sample)
    {
        if (in_sample.is_healthy & f_sample.is_healthy)
        {
            m_output_stream->push_sample(math::rotate(f_sample.value, in_sample.value), true);
        }
        else
        {
            m_output_stream->push_last_sample(false);
        }
    });
}



}
}
