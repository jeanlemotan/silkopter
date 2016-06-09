#include "BrainStdAfx.h"
#include "Rate_Controller.h"

#include "uav.def.h"
//#include "sz_PID.hpp"
//#include "sz_Rate_Controller.hpp"

namespace silk
{
namespace node
{

Rate_Controller::Rate_Controller(UAV& uav)
    : m_uav(uav)
    , m_descriptor(new Rate_Controller_Descriptor())
    , m_config(new Rate_Controller_Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

auto Rate_Controller::init(std::shared_ptr<Node_Descriptor_Base> descriptor) -> bool
{
    QLOG_TOPIC("rate_controller::init");

    auto specialized = std::dynamic_pointer_cast<Rate_Controller_Descriptor>(descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }

    *m_descriptor = *specialized;

    return init();
}
auto Rate_Controller::init() -> bool
{
    m_output_stream->set_rate(m_descriptor->get_rate());
    return true;
}

auto Rate_Controller::start(q::Clock::time_point tp) -> bool
{
    m_output_stream->set_tp(tp);
    return true;
}

auto Rate_Controller::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IAngular_Velocity::TYPE, m_descriptor->get_rate(), "Input", m_accumulator.get_stream_path(0) },
        { stream::IAngular_Velocity::TYPE, m_descriptor->get_rate(), "Target", m_accumulator.get_stream_path(1) }
    }};
    return inputs;
}
auto Rate_Controller::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
         { "Torque", m_output_stream }
    }};
    return outputs;
}

void Rate_Controller::process()
{
    QLOG_TOPIC("rate_controller::process");

    m_output_stream->clear();

    std::shared_ptr<const Multirotor_Config> multirotor_config = m_uav.get_specialized_uav_config<Multirotor_Config>();
    if (!multirotor_config)
    {
        return;
    }

    m_accumulator.process([this, &multirotor_config](stream::IAngular_Velocity::Sample const& i_sample,
                                                stream::IAngular_Velocity::Sample const& t_sample)
    {
        if (i_sample.is_healthy & t_sample.is_healthy)
        {
            math::vec3f ff = compute_feedforward(*multirotor_config, i_sample.value, t_sample.value);
            math::vec3f fb = compute_feedback(i_sample.value, t_sample.value);

            Output_Stream::Value value(ff * m_config->get_feedforward().get_weight() + fb * m_config->get_feedback().get_weight());

            m_output_stream->push_sample(value, true);
        }
        else
        {
            m_output_stream->push_last_sample(false);
        }
    });
}


math::vec3f Rate_Controller::compute_feedforward(Multirotor_Config const& config, stream::IAngular_Velocity::Value const& input, stream::IAngular_Velocity::Value const& target)
{
    math::vec3f v = target - input;
    float vm = math::length(v) * config.get_moment_of_inertia();

    float max_T = m_config->get_feedforward().get_max_torque();

    float A = config.get_motor_acceleration();
    float C = config.get_motor_deceleration();

    float x_sq = vm / ((A + C) * max_T / 2.f);
    float x = math::min(1.f, math::sqrt(x_sq));

    return math::normalized<float, math::safe>(v) * max_T * x;
}
math::vec3f Rate_Controller::compute_feedback(stream::IAngular_Velocity::Value const& input, stream::IAngular_Velocity::Value const& target)
{
    float x = m_x_pid.process(input.x, target.x);
    float y = m_y_pid.process(input.y, target.y);
    float z = m_z_pid.process(input.z, target.z);
    return math::vec3f(x, y, z);
}

void Rate_Controller::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_uav);
}

auto Rate_Controller::set_config(std::shared_ptr<Node_Config_Base> config) -> bool
{
    QLOG_TOPIC("rate_controller::set_config");

    auto specialized = std::dynamic_pointer_cast<Rate_Controller_Config>(config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }

    *m_config = *specialized;

    //todo - fix this
//    m_config->feedback.weight = math::clamp(m_config->feedback.weight, 0.f, 1.f);
//    m_config->feedforward.weight = math::clamp(m_config->feedforward.weight, 0.f, 1.f);

//    m_config->feedforward.max_torque = math::max(m_config->feedforward.max_torque, 0.01f);

//    auto fill_params = [this](PID::Params& dst, sz::PID const& src)
//    {
//        dst.kp = src.kp;
//        dst.ki = src.ki;
//        dst.kd = src.kd;
//        dst.max_i = src.max_i;
//        dst.d_filter = src.d_filter;
//        dst.rate = m_output_stream->get_rate();
//    };

//    PID::Params x_params, y_params, z_params;
//    if (m_config->feedback.combined_xy_pid)
//    {
//        fill_params(x_params, m_config->feedback.xy_pid);
//        fill_params(y_params, m_config->feedback.xy_pid);
//    }
//    else
//    {
//        fill_params(x_params, m_config->feedback.x_pid);
//        fill_params(y_params, m_config->feedback.y_pid);
//    }
//    fill_params(z_params, m_config->feedback.z_pid);

//    if (!m_x_pid.set_params(x_params) ||
//        !m_y_pid.set_params(y_params) ||
//        !m_z_pid.set_params(z_params))
//    {
//        QLOGE("Bad PID params");
//        return false;
//    }

    return true;
}
auto Rate_Controller::get_config() const -> std::shared_ptr<Node_Config_Base>
{
    //todo - fix this
//    rapidjson::Document json;
//    autojsoncxx::to_document(*m_config, json);
//    if (m_config->feedback.combined_xy_pid)
//    {
//        jsonutil::remove_value(json, q::Path("Feedback/X PID"));
//        jsonutil::remove_value(json, q::Path("Feedback/Y PID"));
//    }
//    else
//    {
//        jsonutil::remove_value(json, q::Path("Feedback/XY PID"));
//    }
//    return std::move(json);
    return m_config;
}

auto Rate_Controller::get_descriptor() const -> std::shared_ptr<Node_Descriptor_Base>
{
    return m_descriptor;
}

auto Rate_Controller::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}

}
}
