#include "FCStdAfx.h"
#include "Rate_Controller.h"

#include "hal.def.h"
//#include "sz_PID.hpp"
//#include "sz_Rate_Controller.hpp"

namespace silk
{
namespace node
{

Rate_Controller::Rate_Controller(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::Rate_Controller_Descriptor())
    , m_config(new hal::Rate_Controller_Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

ts::Result<void> Rate_Controller::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("rate_controller::init");

    auto specialized = dynamic_cast<hal::Rate_Controller_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}
ts::Result<void> Rate_Controller::init()
{
    m_output_stream->set_rate(m_descriptor->get_rate());
    return ts::success;
}

ts::Result<void> Rate_Controller::start(Clock::time_point tp)
{
    m_output_stream->set_tp(tp);
    return ts::success;
}

auto Rate_Controller::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IAngular_Velocity::TYPE, m_descriptor->get_rate(), "input", m_accumulator.get_stream_path(0) },
        { stream::IAngular_Velocity::TYPE, m_descriptor->get_rate(), "target", m_accumulator.get_stream_path(1) }
    }};
    return inputs;
}
auto Rate_Controller::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
         { "torque", m_output_stream }
    }};
    return outputs;
}

void Rate_Controller::process()
{
    QLOG_TOPIC("rate_controller::process");

    m_output_stream->clear();

    std::shared_ptr<const IMultirotor_Properties> multirotor_properties = m_hal.get_specialized_uav_properties<IMultirotor_Properties>();
    if (!multirotor_properties)
    {
        return;
    }

    m_accumulator.process([this, &multirotor_properties](stream::IAngular_Velocity::Sample const& i_sample,
                                                stream::IAngular_Velocity::Sample const& t_sample)
    {
        if (i_sample.is_healthy & t_sample.is_healthy)
        {
            math::vec3f ff = compute_feedforward(*multirotor_properties, i_sample.value, t_sample.value);
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


math::vec3f Rate_Controller::compute_feedforward(IMultirotor_Properties const& multirotor_properties, stream::IAngular_Velocity::Value const& input, stream::IAngular_Velocity::Value const& target)
{
    math::vec3f v = target - input;
    float vm = math::length(v) * multirotor_properties.get_moment_of_inertia();

    float max_T = m_config->get_max_torque();

    float A = multirotor_properties.get_motor_acceleration();
    float C = multirotor_properties.get_motor_deceleration();

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

ts::Result<void> Rate_Controller::set_input_stream_path(size_t idx, std::string const& path)
{
    return m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_hal);
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

ts::Result<void> Rate_Controller::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("rate_controller::set_config");

    auto specialized = dynamic_cast<hal::Rate_Controller_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    uint32_t output_rate = m_output_stream->get_rate();

    hal::Rate_Controller_Config::Feedback const& descriptor = m_config->get_feedback();
    PID::Params x_params, y_params, z_params;
    if (auto combined_pids = boost::get<hal::Rate_Controller_Config::Feedback::Combined_XY_PIDs>(&descriptor.get_xy_pids()))
    {
        fill_pid_params(x_params, *combined_pids, output_rate);
        fill_pid_params(y_params, *combined_pids, output_rate);
    }
    else
    {
        auto separate_pids = boost::get<hal::Rate_Controller_Config::Feedback::Separate_XY_PIDs>(&descriptor.get_xy_pids());
        fill_pid_params(x_params, separate_pids->get_x_pid(), output_rate);
        fill_pid_params(y_params, separate_pids->get_y_pid(), output_rate);
    }

    fill_pid_params(z_params, descriptor.get_z_pid(), output_rate);

    //scale factor to avoid using very small values in the UI
    const float pid_scale = 100.f;

    x_params.kp /= pid_scale;
    x_params.ki /= pid_scale;
    x_params.kd /= pid_scale;
    x_params.max_i /= pid_scale;

    y_params.kp /= pid_scale;
    y_params.ki /= pid_scale;
    y_params.kd /= pid_scale;
    y_params.max_i /= pid_scale;

    z_params.kp /= pid_scale;
    z_params.ki /= pid_scale;
    z_params.kd /= pid_scale;
    z_params.max_i /= pid_scale;

    //this should prevent integral windup when the PID is stressed a lot.
    //for example in sharpp changes of target
    float limit = m_config->get_max_torque();
    m_x_pid.set_output_limits(-limit, limit);
    m_y_pid.set_output_limits(-limit, limit);
    m_z_pid.set_output_limits(-limit, limit);

    if (!m_x_pid.set_params(x_params) ||
        !m_y_pid.set_params(y_params) ||
        !m_z_pid.set_params(z_params))
    {
        return make_error("Bad PID params");
    }

    return ts::success;
}
auto Rate_Controller::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto Rate_Controller::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}

ts::Result<std::shared_ptr<messages::INode_Message>> Rate_Controller::send_message(messages::INode_Message const& message)
{
    return make_error("Unknown message");
}

}
}
