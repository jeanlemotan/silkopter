#include "BrainStdAfx.h"
#include "EKF_ECEF.h"

#include "sz_math.hpp"
#include "sz_EKF_ECEF.hpp"

#include "Eigen/Dense"

namespace silk
{
namespace node
{

 EKF_ECEF::KF::KF()
{
    I.setIdentity();
}

void EKF_ECEF::KF::predict()
{
    Eigen::Vector3d x_new = A * x + B * u;
    Eigen::Matrix3d P_new = A * P * A.transpose() + Q;
    x = x_new;
    P = P_new;
}

void EKF_ECEF::KF::update()
{
    Eigen::Matrix3d KT = K.transpose();

    G = P * KT * (K * P * KT + R).inverse();
    Eigen::Vector3d x_new = x + G * (z - K * x);
    Eigen::Matrix3d P_new = (I - G * K) * P;

    x = x_new;
    P = P_new;
}

///////////////////////////////////////////////////////////////////////

EKF_ECEF::EKF_ECEF(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::EKF_ECEF::Init_Params())
    , m_config(new sz::EKF_ECEF::Config())
{
    m_position_output_stream = std::make_shared<Position_Output_Stream>();
    m_velocity_output_stream = std::make_shared<Velocity_Output_Stream>();
//    m_enu_frame_output_stream = std::make_shared<ENU_Frame_Stream>();
}

auto EKF_ECEF::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("EKF_ECEF::init");

    sz::EKF_ECEF::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize EKF_ECEF data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init();
}
auto EKF_ECEF::init() -> bool
{
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    m_position_output_stream->set_rate(m_init_params->rate);
    m_position_output_stream->set_tp(q::Clock::now());

    m_velocity_output_stream->set_rate(m_init_params->rate);
    m_velocity_output_stream->set_tp(q::Clock::now());


    double dt = std::chrono::duration<double>(m_position_output_stream->get_dt()).count();

    m_kf_x.A << 1, dt, 0.5*dt*dt,
                0,  1, dt,
                0,  0, 1;

    m_kf_x.B = Eigen::Matrix3d::Identity();
    m_kf_x.K = Eigen::Matrix3d::Identity();
    m_kf_x.P = Eigen::Matrix3d::Identity();
    m_kf_x.G = Eigen::Matrix3d::Identity();

    double pacc = math::square(2.0);
    double vacc = math::square(0.2);
    double aacc = math::square(3.0);
    m_kf_x.R << pacc,    0,       0,
                0,       vacc,    0,
                0,       0,       aacc;

    double pn = 0.01;
    double dt4 = dt*dt*dt*dt;
    double dt3 = dt*dt*dt;
    double dt2 = dt*dt;
    m_kf_x.Q << pn*0.25*dt4,    pn*0.5*dt3, pn*0.5*dt2,
                pn*0.5*dt3,     pn*dt2,     pn*dt,
                pn*0.5*dt2,     pn*dt,      pn*1.0;

    m_kf_y = m_kf_x;
    m_kf_z = m_kf_x;

    return true;
}

auto EKF_ECEF::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IGPS_Info::TYPE, m_init_params->rate, "GPS Info", m_accumulator.get_stream_path(0) },
        { stream::IECEF_Position::TYPE, m_init_params->rate, "GPS Position (ecef)", m_accumulator.get_stream_path(1) },
        { stream::IECEF_Velocity::TYPE, m_init_params->rate, "GPS Velocity (ecef)", m_accumulator.get_stream_path(2) },
        { stream::IENU_Linear_Acceleration::TYPE, m_init_params->rate, "Linear Acceleration (enu)", m_accumulator.get_stream_path(3) },
        { stream::IPressure::TYPE, m_init_params->rate, "Pressure", m_accumulator.get_stream_path(4) }
    }};
    return inputs;
}
auto EKF_ECEF::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
        { "Position (ecef)", m_position_output_stream },
        { "Velocity (ecef)", m_velocity_output_stream },
    }};
    return outputs;
}

void EKF_ECEF::process()
{
    QLOG_TOPIC("EKF_ECEF::process");


    m_position_output_stream->clear();
    m_velocity_output_stream->clear();

    double dts = q::Seconds(m_position_output_stream->get_dt()).count();

    m_accumulator.process([this, dts](
                          size_t,
                          stream::IGPS_Info::Sample const& gi_sample,
                          stream::IECEF_Position::Sample const& pos_sample,
                          stream::IECEF_Velocity::Sample const& vel_sample,
                          stream::IENU_Linear_Acceleration::Sample const& la_sample,
                          stream::IPressure::Sample const& p_sample)
    {
        if (gi_sample.is_healthy)
        {
            double pacc = gi_sample.value.pacc;
            double vacc = gi_sample.value.vacc;
            double aacc = math::square(3.0);
            m_kf_x.R << pacc,    0,       0,
                        0,       vacc,    0,
                        0,       0,       aacc;
        }

        auto last_pos_sample = m_position_output_stream->get_last_sample();

        if (pos_sample.is_healthy && vel_sample.is_healthy)
        {
            if (math::distance_sq(pos_sample.value, last_pos_sample.value) > math::square(20))
            {
                m_kf_x.x(0) = pos_sample.value.x;
                m_kf_y.x(0) = pos_sample.value.y;
                m_kf_z.x(0) = pos_sample.value.z;
            }

            m_kf_x.z(0) = pos_sample.value.x;
            m_kf_y.z(0) = pos_sample.value.y;
            m_kf_z.z(0) = pos_sample.value.z;

            m_kf_x.z(1) = vel_sample.value.x;
            m_kf_y.z(1) = vel_sample.value.y;
            m_kf_z.z(1) = vel_sample.value.z;

            auto lla_position = util::coordinates::ecef_to_lla(pos_sample.value);
            auto enu_to_ecef_rotation = util::coordinates::enu_to_ecef_rotation(lla_position);
            auto ecef_la = math::transform(enu_to_ecef_rotation, math::vec3d(la_sample.value));

            m_kf_x.z(2) = ecef_la.x;
            m_kf_y.z(2) = ecef_la.y;
            m_kf_z.z(2) = ecef_la.z;

            m_kf_x.predict();
            m_kf_x.update();

            m_kf_y.predict();
            m_kf_y.update();

            m_kf_z.predict();
            m_kf_z.update();
        }

        math::vec3d pos(m_kf_x.x(0), m_kf_y.x(0), m_kf_z.x(0));
        math::vec3f vel(m_kf_x.x(1), m_kf_y.x(1), m_kf_z.x(1));

        m_position_output_stream->push_sample(pos, true);
        m_velocity_output_stream->push_sample(vel, true);
    });
}

void EKF_ECEF::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_init_params->rate, m_hal);
}

auto EKF_ECEF::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("EKF_ECEF::set_config");

    sz::EKF_ECEF::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    return true;
}
auto EKF_ECEF::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto EKF_ECEF::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}

auto EKF_ECEF::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}

}
}
