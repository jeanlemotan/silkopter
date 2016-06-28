#include "BrainStdAfx.h"
#include "ENU_Frame_System.h"
#include "physics/constants.h"

#include "hal.def.h"
//#include "sz_ENU_Frame_System.hpp"

namespace silk
{
namespace node
{

ENU_Frame_System::ENU_Frame_System(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::ENU_Frame_System_Descriptor())
    , m_config(new hal::ENU_Frame_System_Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

ts::Result<void> ENU_Frame_System::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("ENU_Frame_System::init");

    auto specialized = dynamic_cast<hal::ENU_Frame_System_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}
ts::Result<void> ENU_Frame_System::init()
{
    m_output_stream->set_rate(m_descriptor->get_rate());
    return ts::success;
}

ts::Result<void> ENU_Frame_System::start(q::Clock::time_point tp)
{
    m_output_stream->set_tp(tp);
    return ts::success;
}

auto ENU_Frame_System::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IECEF_Position::TYPE, m_descriptor->get_rate(), "Position (ecef)", m_accumulator.get_stream_path(0) }
    }};
    return inputs;
}
auto ENU_Frame_System::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
         { "Frame (enu->ecef)", m_output_stream }
    }};
    return outputs;
}

void ENU_Frame_System::process()
{
    QLOG_TOPIC("ENU_Frame_System::process");

    m_output_stream->clear();

    m_accumulator.process([this](stream::IECEF_Position::Sample const& sample)
    {
        if (sample.is_healthy)
        {
            util::coordinates::LLA lla_position = util::coordinates::ecef_to_lla(sample.value);
            math::mat3d enu_to_ecef_rotation = util::coordinates::enu_to_ecef_rotation(lla_position);
            math::quatd rot;
            rot.set_from_mat3(enu_to_ecef_rotation);

            m_output_stream->push_sample(math::quatf(rot), true);
        }
        else
        {
            m_output_stream->push_last_sample(false);
        }
    });
}

ts::Result<void> ENU_Frame_System::set_input_stream_path(size_t idx, q::Path const& path)
{
    return m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_hal);
}

ts::Result<void> ENU_Frame_System::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("ENU_Frame_System::set_config");

    auto specialized = dynamic_cast<hal::ENU_Frame_System_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    return ts::success;
}
auto ENU_Frame_System::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto ENU_Frame_System::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}

//auto ENU_Frame_System::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}

}
}
