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
    , m_descriptor(new uav::Rate_Controller_Descriptor())
    , m_config(new uav::Rate_Controller_Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

auto Rate_Controller::init(std::shared_ptr<uav::INode_Descriptor> descriptor) -> bool
{
    QLOG_TOPIC("rate_controller::init");

    auto specialized = std::dynamic_pointer_cast<uav::Rate_Controller_Descriptor>(descriptor);
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

    std::shared_ptr<const uav::Multirotor_Descriptor> multirotor_descriptor = m_uav.get_specialized_uav_descriptor<uav::Multirotor_Descriptor>();
    if (!multirotor_descriptor)
    {
        return;
    }

    m_accumulator.process([this, &multirotor_descriptor](stream::IAngular_Velocity::Sample const& i_sample,
                                                stream::IAngular_Velocity::Sample const& t_sample)
    {
        if (i_sample.is_healthy & t_sample.is_healthy)
        {
            math::vec3f ff = compute_feedforward(*multirotor_descriptor, i_sample.value, t_sample.value);
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


math::vec3f Rate_Controller::compute_feedforward(uav::Multirotor_Descriptor const& multirotor_descriptor, stream::IAngular_Velocity::Value const& input, stream::IAngular_Velocity::Value const& target)
{
    math::vec3f v = target - input;
    float vm = math::length(v) * multirotor_descriptor.get_moment_of_inertia();

    float max_T = m_config->get_feedforward().get_max_torque();

    float A = multirotor_descriptor.get_motor_acceleration();
    float C = multirotor_descriptor.get_motor_deceleration();

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

template<class T, class PID>
void fill_pid_params(T& dst, PID const& src, size_t rate)
{
    dst.kp = src.get_kp();
    dst.ki = src.get_ki();
    dst.kd = src.get_kd();
    dst.max_i = src.get_max_i();
    dst.d_filter = src.get_d_filter();
    dst.rate = rate;
}

auto Rate_Controller::set_config(std::shared_ptr<uav::INode_Config> config) -> bool
{
    QLOG_TOPIC("rate_controller::set_config");

    auto specialized = std::dynamic_pointer_cast<uav::Rate_Controller_Config>(config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }

    *m_config = *specialized;

    uint32_t output_rate = m_output_stream->get_rate();

    uav::Rate_Controller_Config::Feedback const& descriptor = m_config->get_feedback();
    PID::Params x_params, y_params, z_params;
    if (auto combined_pids = boost::get<uav::Rate_Controller_Config::Feedback::Combined_XY_PIDs>(&descriptor.get_xy_pids()))
    {
        fill_pid_params(x_params, *combined_pids, output_rate);
        fill_pid_params(y_params, *combined_pids, output_rate);
    }
    else
    {
        auto separate_pids = boost::get<uav::Rate_Controller_Config::Feedback::Separate_XY_PIDs>(&descriptor.get_xy_pids());
        fill_pid_params(x_params, separate_pids->get_x_pid(), output_rate);
        fill_pid_params(y_params, separate_pids->get_y_pid(), output_rate);
    }

    fill_pid_params(z_params, descriptor.get_z_pid(), output_rate);

    if (!m_x_pid.set_params(x_params) ||
        !m_y_pid.set_params(y_params) ||
        !m_z_pid.set_params(z_params))
    {
        QLOGE("Bad PID params");
        return false;
    }

    return true;
}
auto Rate_Controller::get_config() const -> std::shared_ptr<uav::INode_Config>
{
    return m_config;
}

auto Rate_Controller::get_descriptor() const -> std::shared_ptr<uav::INode_Descriptor>
{
    return m_descriptor;
}

//auto Rate_Controller::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}

}
}
