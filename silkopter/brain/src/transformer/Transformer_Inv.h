#pragma once

#include "HAL.h"
#include "common/node/ITransformer.h"
#include "common/stream/IFrame.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"

#include "hal.def.h"

namespace silk
{
namespace node
{

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
class Transformer_Inv : public ITransformer
{
public:
    Transformer_Inv(HAL& hal);

    ts::Result<void> init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    ts::Result<void> set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    ts::Result<void> start(q::Clock::time_point tp) override;

    ts::Result<void> set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    ts::Result<void> init();

    HAL& m_hal;

    std::shared_ptr<hal::Transformer_Descriptor> m_descriptor;
    std::shared_ptr<hal::Transformer_Config> m_config;

    Sample_Accumulator<In_Stream_t, Frame_Stream_t> m_accumulator;

    typedef Basic_Output_Stream<Out_Stream_t> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::Transformer_Inv(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::Transformer_Descriptor)
    , m_config(new hal::Transformer_Config)
{
    static_assert(In_Stream_t::SEMANTIC == Out_Stream_t::SEMANTIC, "Both streams need to have the same semantic");
    static_assert(In_Stream_t::SPACE == Frame_Stream_t::SPACE, "Bad Input stream or Frame");
    static_assert(Out_Stream_t::SPACE == Frame_Stream_t::PARENT_SPACE, "Bad Output stream or Frame");

    m_output_stream = std::make_shared<Output_Stream>();
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
ts::Result<void> Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("transformer_inv::init");

    auto specialized = dynamic_cast<hal::Transformer_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
ts::Result<void> Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::init()
{
    m_output_stream->set_rate(m_descriptor->get_rate());
    return ts::success;
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
ts::Result<void> Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::set_input_stream_path(size_t idx, q::Path const& path)
{
    return m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_hal);
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
ts::Result<void> Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("transformer_inv::set_config");

    auto specialized = dynamic_cast<hal::Transformer_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    return ts::success;
}
//template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
//auto Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}
template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
auto Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

template<class In_Stream_t, class Out_Stream_t, class Frame_Stream_t>
ts::Result<void> Transformer_Inv<In_Stream_t, Out_Stream_t, Frame_Stream_t>::start(q::Clock::time_point tp)
{
    m_output_stream->set_tp(tp);
    return ts::success;
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
