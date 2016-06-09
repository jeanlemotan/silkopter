#include "BrainStdAfx.h"
#include "KF_ECEF.h"

#include "uav.def.h"
//#include "sz_KF_ECEF.hpp"

#include "Eigen/Dense"

namespace silk
{
namespace node
{

template<size_t St, size_t Me>
KF_ECEF::KF<St, Me>::KF()
{
    A.setIdentity();
    x.setZero();

    B.setIdentity();
    u.setZero();

    H.setIdentity();
    z.setZero();

    //error
    P.setIdentity();
    Q.setIdentity();

    K.setIdentity();
    R.setIdentity();

    I.setIdentity();

    I.setIdentity();
    B.setIdentity();
    H.setIdentity();
    P.setIdentity();
    K.setIdentity();
}

template<size_t St, size_t Me>
void KF_ECEF::KF<St, Me>::predict()
{
    x = A * x + B * u; //state prediction
    P = A * P * A.transpose() + Q; //covariance prediction
}

template<size_t St, size_t Me>
void KF_ECEF::KF<St, Me>::update()
{
    auto HT = H.transpose();

    Measurement_Vector y = z - H * x; //innovation

    auto S = H * P * HT + R; //innovation covariance

    K = P * HT * S.inverse(); //gain
    x = x + K * y; // state
    P = (I - K * H) * P; //covariance
}

template<size_t St, size_t Me>
void KF_ECEF::KF<St, Me>::process()
{
    predict();
    update();
}

///////////////////////////////////////////////////////////////////////

template<class Value>
void KF_ECEF::Delayer<Value>::init(float dt, float lag)
{
    QASSERT(dt > 0.f && lag >= 0.f);
    min_value_count = math::max(static_cast<size_t>(math::round(lag / dt)), size_t(1));
    values.clear();
}

template<class Value>
auto KF_ECEF::Delayer<Value>::get_value() -> Value const&
{
    QASSERT(!values.empty());
    return values.front();
}

template<class Value>
void KF_ECEF::Delayer<Value>::push_back(Value const& value)
{
    values.push_back(value);
    while (values.size() > min_value_count)
    {
        values.pop_front();
    }
}

///////////////////////////////////////////////////////////////////////

KF_ECEF::KF_ECEF(UAV& uav)
    : m_uav(uav)
    , m_descriptor(new KF_ECEF_Descriptor())
    , m_config(new KF_ECEF_Config())
{
    m_position_output_stream = std::make_shared<Position_Output_Stream>();
    m_velocity_output_stream = std::make_shared<Velocity_Output_Stream>();
    m_linear_acceleration_output_stream = std::make_shared<Linear_Acceleration_Output_Stream>();
}

auto KF_ECEF::init(std::shared_ptr<Node_Descriptor_Base> descriptor) -> bool
{
    QLOG_TOPIC("KF_ECEF::init");

    auto specialized = std::dynamic_pointer_cast<KF_ECEF_Descriptor>(descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }

    *m_descriptor = *specialized;

    return init();
}
auto KF_ECEF::init() -> bool
{
    m_position_output_stream->set_rate(m_descriptor->get_rate());
    m_velocity_output_stream->set_rate(m_descriptor->get_rate());
    m_linear_acceleration_output_stream->set_rate(m_descriptor->get_rate());

    m_dts = std::chrono::duration<double>(m_position_output_stream->get_dt()).count();
    double dt = m_dts;

    {
        m_kf_x.A << 1,      dt,     0.5*dt*dt,
                    0,      1,      dt,
                    0,      0,      1;

        m_kf_x.H << 1,      0,      0,
                    0,      1,      0,
                    0,      0,      1;

        double pn = 0.01;
        double dt4 = dt*dt*dt*dt;
        double dt3 = dt*dt*dt;
        double dt2 = dt*dt;
        m_kf_x.Q << pn*0.25*dt4,    pn*0.5*dt3, pn*0.5*dt2,
                    pn*0.5*dt3,     pn*dt2,     pn*dt,
                    pn*0.5*dt2,     pn*dt,      pn*1.0;
    }

    m_kf_y = m_kf_x;
    m_kf_z = m_kf_x;
//    m_kf_z = m_kf_x;

    return true;
}

auto KF_ECEF::start(q::Clock::time_point tp) -> bool
{
    m_position_output_stream->set_tp(tp);
    m_velocity_output_stream->set_tp(tp);
    m_linear_acceleration_output_stream->set_tp(tp);
    return true;
}

auto KF_ECEF::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IECEF_Position::TYPE, m_descriptor->get_rate(), "GPS Position (ecef)", m_accumulator.get_stream_path(0) },
        { stream::IECEF_Velocity::TYPE, m_descriptor->get_rate(), "GPS Velocity (ecef)", m_accumulator.get_stream_path(1) },
        { stream::IENU_Linear_Acceleration::TYPE, m_descriptor->get_rate(), "Linear Acceleration (enu)", m_accumulator.get_stream_path(2) },
    }};
    return inputs;
}
auto KF_ECEF::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
        { "Position (ecef)", m_position_output_stream },
        { "Velocity (ecef)", m_velocity_output_stream },
        { "Linear Acceleration (ecef)", m_linear_acceleration_output_stream },
    }};
    return outputs;
}

void KF_ECEF::process()
{
    QLOG_TOPIC("KF_ECEF::process");


    m_position_output_stream->clear();
    m_velocity_output_stream->clear();
    m_linear_acceleration_output_stream->clear();

    m_accumulator.process([this](stream::IECEF_Position::Sample const& gps_pos_sample,
                                  stream::IECEF_Velocity::Sample const& gps_vel_sample,
                                  stream::IENU_Linear_Acceleration::Sample const& la_sample)
    {
        if (gps_pos_sample.is_healthy & gps_vel_sample.is_healthy & la_sample.is_healthy)
        {
            util::coordinates::LLA lla_position = util::coordinates::ecef_to_lla(gps_pos_sample.value);
            math::mat3d enu_to_ecef_rotation = util::coordinates::enu_to_ecef_rotation(lla_position);
            math::vec3f ecef_la = math::vec3f(math::transform(enu_to_ecef_rotation, math::vec3d(la_sample.value)));

            stream::IECEF_Position::Sample const& last_gps_pos_sample = m_position_output_stream->get_last_sample();
            if (math::distance_sq(gps_pos_sample.value, last_gps_pos_sample.value) > math::square(20))
            {
                m_kf_x.x.setZero();
                m_kf_y.x.setZero();
                m_kf_z.x.setZero();
                m_kf_x.x(0) = gps_pos_sample.value.x;
                m_kf_y.x(0) = gps_pos_sample.value.y;
                m_kf_z.x(0) = gps_pos_sample.value.z;
            }

            {
                m_gps_position_delayer.push_back(gps_pos_sample.value);
                stream::IECEF_Position::Value const& pos = m_gps_position_delayer.get_value();
                m_kf_x.z(0) = pos.x;
                m_kf_y.z(0) = pos.y;
                m_kf_z.z(0) = pos.z;
            }


            {
                m_gps_velocity_delayer.push_back(gps_vel_sample.value);
                stream::IECEF_Velocity::Value const& vel = m_gps_velocity_delayer.get_value();
                m_kf_x.z(1) = vel.x;
                m_kf_y.z(1) = vel.y;
                m_kf_z.z(1) = vel.z;
            }

            {
                m_linear_acceleration_delayer.push_back(ecef_la);
                stream::IECEF_Linear_Acceleration::Value const& acc = m_linear_acceleration_delayer.get_value();
                m_kf_x.z(2) = acc.x;
                m_kf_y.z(2) = acc.y;
                m_kf_z.z(2) = acc.z;
            }

            m_kf_x.process();
            m_kf_y.process();
            m_kf_z.process();

            m_position_output_stream->push_sample(math::vec3d(m_kf_x.x(0), m_kf_y.x(0), m_kf_z.x(0)), true);
            m_velocity_output_stream->push_sample(math::vec3f(m_kf_x.x(1), m_kf_y.x(1), m_kf_z.x(1)), true);
            m_linear_acceleration_output_stream->push_sample(math::vec3f(m_kf_x.x(2), m_kf_y.x(2), m_kf_z.x(2)), true);
        }
        else
        {
            m_position_output_stream->push_last_sample(false);
            m_velocity_output_stream->push_last_sample(false);
            m_linear_acceleration_output_stream->push_last_sample(false);
        }
    });
}

void KF_ECEF::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_descriptor->get_rate(), m_uav);
}

auto KF_ECEF::set_config(std::shared_ptr<Node_Config_Base> config) -> bool
{
    QLOG_TOPIC("KF_ECEF::set_config");

    auto specialized = std::dynamic_pointer_cast<KF_ECEF_Config>(config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }

    *m_config = *specialized;

    double gps_pos_acu = math::square(m_config->get_gps_position_accuracy());
    double gps_vel_acu = math::square(m_config->get_gps_velocity_accuracy());
    double acc_acu = math::square(m_config->get_acceleration_accuracy());

    m_kf_x.R << gps_pos_acu,    0,              0,
                0,              gps_vel_acu,    0,
                0,              0,              acc_acu;

    m_kf_y.R = m_kf_x.R;
    m_kf_z.R = m_kf_x.R;

    m_gps_position_delayer.init(m_dts, m_config->get_gps_position_lag());
    m_gps_velocity_delayer.init(m_dts, m_config->get_gps_velocity_lag());
    m_linear_acceleration_delayer.init(m_dts, m_config->get_acceleration_lag());

    return true;
}
auto KF_ECEF::get_config() const -> std::shared_ptr<Node_Config_Base>
{
    return m_config;
}

auto KF_ECEF::get_descriptor() const -> std::shared_ptr<Node_Descriptor_Base>
{
    return m_descriptor;
}

auto KF_ECEF::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}

}
}
