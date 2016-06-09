#include "BrainStdAfx.h"
#include "Comp_ECEF.h"

#include "uav.def.h"
/*
namespace silk
{
namespace node
{

Comp_ECEF::Comp_ECEF(UAV& uav)
    : m_uav(uav)
    , m_descriptor(new Comp_ECEF_Descriptor())
    , m_config(new Comp_ECEF_Config())
{
    m_position_output_stream = std::make_shared<Position_Output_Stream>();
    m_velocity_output_stream = std::make_shared<Velocity_Output_Stream>();
//    m_enu_frame_output_stream = std::make_shared<ENU_Frame_Stream>();
}

auto Comp_ECEF::init(std::shared_ptr<Node_Descriptor_Base> descriptor) -> bool
{
    QLOG_TOPIC("Comp_ECEF::init");

    auto specialized = std::dynamic_pointer_cast<Comp_ECEF_Descriptor>(descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }

    *m_descriptor = *specialized;

    return init();
}

auto Comp_ECEF::init() -> bool
{
    if (m_descriptor->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_descriptor->rate);
        return false;
    }
    m_position_output_stream->set_rate(m_descriptor->rate);
    m_velocity_output_stream->set_rate(m_descriptor->rate);
    return true;
}

auto Comp_ECEF::start(q::Clock::time_point tp) -> bool
{
    m_position_output_stream->set_tp(tp);
    m_velocity_output_stream->set_tp(tp);
    return true;
}

auto Comp_ECEF::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IECEF_Position::TYPE, m_descriptor->rate, "Position (ecef)", m_accumulator.get_stream_path(0) },
        { stream::IECEF_Velocity::TYPE, m_descriptor->rate, "Velocity (ecef)", m_accumulator.get_stream_path(1) },
        { stream::IENU_Linear_Acceleration::TYPE, m_descriptor->rate, "Linear Acceleration (enu)", m_accumulator.get_stream_path(2) },
        { stream::IPressure::TYPE, m_descriptor->rate, "Pressure", m_accumulator.get_stream_path(3) }
    }};
    return inputs;
}
auto Comp_ECEF::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
        { "Position (ecef)", m_position_output_stream },
        { "Velocity (ecef)", m_velocity_output_stream },
    }};
    return outputs;
}

void Comp_ECEF::process()
{
    QLOG_TOPIC("Comp_ECEF::process");


    m_position_output_stream->clear();
    m_velocity_output_stream->clear();

    float dts = std::chrono::duration<float>(m_position_output_stream->get_dt()).count();

    m_accumulator.process([this, dts](stream::IECEF_Position::Sample const& pos_sample,
                                      stream::IECEF_Velocity::Sample const& vel_sample,
                                      stream::IENU_Linear_Acceleration::Sample const& la_sample,
                                      stream::IPressure::Sample const& p_sample)
    {
        auto last_pos_sample = m_position_output_stream->get_last_sample();

        if (pos_sample.is_healthy)
        {
            //if too far away, reset
            if (math::distance_sq(m_last_gps_position, pos_sample.value) > math::square(10.f))
            {
                last_pos_sample.value = pos_sample.value;
                m_last_gps_position = pos_sample.value;
                m_velocity.set(0, 0, 0);
            }
            else
            {
                auto lla_position = util::coordinates::ecef_to_lla(last_pos_sample.value);
                auto enu_to_ecef_rotation = util::coordinates::enu_to_ecef_rotation(lla_position);
                auto ecef_la = math::transform(enu_to_ecef_rotation, math::vec3d(la_sample.value));

                last_pos_sample.value = math::lerp(last_pos_sample.value, pos_sample.value, 3.f * dts) + ecef_la * dts * dts * 0.5f;
                //m_last_gps_position = last_pos_sample.value;

                //QLOGI("acc: {}", ecef_la * dts * dts * 0.5);
            }
        }

        m_position_output_stream->push_sample(last_pos_sample.value, last_pos_sample.is_healthy);
        m_velocity_output_stream->push_sample(m_velocity, last_pos_sample.is_healthy);
    });
}

void Comp_ECEF::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_descriptor->rate, m_uav);
}

auto Comp_ECEF::set_config(std::shared_ptr<Node_Config_Base> config) -> bool
{
    QLOG_TOPIC("Comp_ECEF::set_config");

    auto specialized = std::dynamic_pointer_cast<Comp_ECEF_Config>(config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }

    *m_config = *specialized;

    return true;
}
auto Comp_ECEF::get_config() const -> std::shared_ptr<Node_Config_Base>
{
    return m_config;
}

auto Comp_ECEF::get_descriptor() const -> std::shared_ptr<Node_Descriptor_Base>
{
    return m_descriptor;
}

auto Comp_ECEF::send_message(rapidjson::Value const& json) -> rapidjson::Document
{
    return rapidjson::Document();
}

}
}
*/
