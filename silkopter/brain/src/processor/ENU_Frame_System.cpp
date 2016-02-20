#include "BrainStdAfx.h"
#include "ENU_Frame_System.h"
#include "physics/constants.h"

#include "sz_math.hpp"
#include "sz_ENU_Frame_System.hpp"

namespace silk
{
namespace node
{

ENU_Frame_System::ENU_Frame_System(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::ENU_Frame_System::Init_Params())
    , m_config(new sz::ENU_Frame_System::Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

auto ENU_Frame_System::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("ENU_Frame_System::init");

    sz::ENU_Frame_System::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize ENU_Frame_System data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init();
}
auto ENU_Frame_System::init() -> bool
{
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    m_output_stream->set_rate(m_init_params->rate);
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
        { stream::IECEF_Position::TYPE, m_init_params->rate, "Position (ecef)", m_accumulator.get_stream_path(0) }
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
    m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_hal);
}

auto ENU_Frame_System::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("ENU_Frame_System::set_config");

    sz::ENU_Frame_System::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize ENU_Frame_System config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    return true;
}
auto ENU_Frame_System::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto ENU_Frame_System::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}

auto ENU_Frame_System::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}

}
}
