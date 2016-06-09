#include "BrainStdAfx.h"
#include "ENU_Frame_System.h"
#include "physics/constants.h"

#include "uav.def.h"
//#include "sz_ENU_Frame_System.hpp"

namespace silk
{
namespace node
{

ENU_Frame_System::ENU_Frame_System(UAV& uav)
    : m_uav(uav)
    , m_descriptor(new ENU_Frame_System_Descriptor())
    , m_config(new ENU_Frame_System_Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

auto ENU_Frame_System::init(std::shared_ptr<Node_Descriptor_Base> descriptor) -> bool
{
    QLOG_TOPIC("ENU_Frame_System::init");

    auto specialized = std::dynamic_pointer_cast<ENU_Frame_System_Descriptor>(descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }

    *m_descriptor = *specialized;

    return init();
}
auto ENU_Frame_System::init() -> bool
{
    m_output_stream->set_rate(m_descriptor->get_rate());
    return true;
}

auto ENU_Frame_System::start(q::Clock::time_point tp) -> bool
{
    m_output_stream->set_tp(tp);
    return true;
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

void ENU_Frame_System::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_uav);
}

auto ENU_Frame_System::set_config(std::shared_ptr<Node_Config_Base> config) -> bool
{
    QLOG_TOPIC("ENU_Frame_System::set_config");

    auto specialized = std::dynamic_pointer_cast<ENU_Frame_System_Config>(config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }

    *m_config = *specialized;

    return true;
}
auto ENU_Frame_System::get_config() const -> std::shared_ptr<Node_Config_Base>
{
    return m_config;
}

auto ENU_Frame_System::get_descriptor() const -> std::shared_ptr<Node_Descriptor_Base>
{
    return m_descriptor;
}

auto ENU_Frame_System::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}

}
}
