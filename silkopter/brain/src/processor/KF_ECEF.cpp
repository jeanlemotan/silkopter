#include "BrainStdAfx.h"
#include "KF_ECEF.h"

#include "sz_math.hpp"
#include "sz_KF_ECEF.hpp"

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
void KF_ECEF::Delayer<Value>::init(double dt, double lag)
{
    QASSERT(dt > 0.0 && lag >= 0.0);
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

KF_ECEF::KF_ECEF(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::KF_ECEF::Init_Params())
    , m_config(new sz::KF_ECEF::Config())
{
    m_position_output_stream = std::make_shared<Position_Output_Stream>();
    m_velocity_output_stream = std::make_shared<Velocity_Output_Stream>();
    m_acceleration_output_stream = std::make_shared<Acceleration_Output_Stream>();
}

auto KF_ECEF::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("KF_ECEF::init");

    sz::KF_ECEF::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize KF_ECEF data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init();
}
auto KF_ECEF::init() -> bool
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

    m_acceleration_output_stream->set_rate(m_init_params->rate);
    m_acceleration_output_stream->set_tp(q::Clock::now());

    m_dts = std::chrono::duration<double>(m_position_output_stream->get_dt()).count();
    double dt = m_dts;

    {
        m_kf_x.A << 1, dt, 0.5*dt*dt,
                    0,  1, dt,
                    0,  0, 1;
        double pn = 0.01;
        double dt4 = dt*dt*dt*dt;
        double dt3 = dt*dt*dt;
        double dt2 = dt*dt;
        m_kf_x.Q << pn*0.25*dt4,    pn*0.5*dt3, pn*0.5*dt2,
                pn*0.5*dt3,     pn*dt2,     pn*dt,
                pn*0.5*dt2,     pn*dt,      pn*1.0;
    }
    {
        double vc = 1; //how many velocity sensors we have
        m_kf_z.A << 1,  dt/vc,  0.5*dt*dt,
                    0,  1,      dt*vc,
                    0,  0,      1;
        m_kf_z.H << 1,  0,  0,
                    0,  1,  0,
                    0,  0,  1,
                    0,  0,  0;

        double pn = 0.01;
        double dt4 = dt*dt*dt*dt;
        double dt3 = dt*dt*dt;
        double dt2 = dt*dt;
        m_kf_z.Q << pn*0.25*dt4,    pn*0.5*dt3, pn*0.5*dt2,
                    pn*0.5*dt3,     pn*dt2,     pn*dt,
                    pn*0.5*dt2,     pn*dt,      pn*1.0;
    }

    m_kf_y = m_kf_x;
//    m_kf_z = m_kf_x;

    return true;
}

auto KF_ECEF::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IECEF_Position::TYPE, m_init_params->rate, "GPS Position (ecef)", m_accumulator.get_stream_path(0) },
        { stream::IECEF_Velocity::TYPE, m_init_params->rate, "GPS Velocity (ecef)", m_accumulator.get_stream_path(1) },
        { stream::IENU_Linear_Acceleration::TYPE, m_init_params->rate, "Linear Acceleration (enu)", m_accumulator.get_stream_path(2) },
        { stream::IPressure::TYPE, m_init_params->rate, "Pressure", m_accumulator.get_stream_path(3) }
    }};
    return inputs;
}
auto KF_ECEF::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
        { "Position (ecef)", m_position_output_stream },
        { "Velocity (ecef)", m_velocity_output_stream },
        { "Acceleration (ecef)", m_acceleration_output_stream },
    }};
    return outputs;
}

void KF_ECEF::process()
{
    QLOG_TOPIC("KF_ECEF::process");


    m_position_output_stream->clear();
    m_velocity_output_stream->clear();
    m_acceleration_output_stream->clear();

    m_accumulator.process([this](
                          size_t,
                          stream::IECEF_Position::Sample const& pos_sample,
                          stream::IECEF_Velocity::Sample const& vel_sample,
                          stream::IENU_Linear_Acceleration::Sample const& la_sample,
                          stream::IPressure::Sample const& p_sample)
    {
        if (pos_sample.is_healthy && vel_sample.is_healthy)
        {
            auto lla_position = util::coordinates::ecef_to_lla(pos_sample.value);
            auto enu_to_ecef_rotation = util::coordinates::enu_to_ecef_rotation(lla_position);
            auto ecef_la = math::transform(enu_to_ecef_rotation, la_sample.value);

            auto last_pos_sample = m_position_output_stream->get_last_sample();
            if (math::distance_sq(pos_sample.value, last_pos_sample.value) > math::square(20))
            {
                m_kf_x.x.setZero();
                m_kf_y.x.setZero();
                m_kf_z.x.setZero();
                m_kf_x.x(0) = pos_sample.value.x;
                m_kf_y.x(0) = pos_sample.value.y;
                m_kf_z.x(0) = pos_sample.value.z;
            }

            m_kf_x.z(0) = pos_sample.value.x;
            m_kf_y.z(0) = pos_sample.value.y;
            m_kf_z.z(0) = pos_sample.value.z;

            m_velocity_delayer.push_back(vel_sample.value);
            m_linear_acceleration_delayer.push_back(ecef_la);

            auto const& vel = m_velocity_delayer.get_value();
            m_kf_x.z(1) = vel.x;
            m_kf_y.z(1) = vel.y;
            m_kf_z.z(1) = vel.z;

            //baro velocity. in case the baro is not healthy
            m_kf_z.z(3) = vel.z;

            auto const& acc = m_linear_acceleration_delayer.get_value();
            m_kf_x.z(2) = acc.x;
            m_kf_y.z(2) = acc.y;
            m_kf_z.z(2) = acc.z;
        }

        if (p_sample.is_healthy)
        {
            double alt = (1.0 - std::pow((p_sample.value / 1013.25), 0.190284)) * 44307.69396;
            if (m_last_baro_altitude)
            {
                m_pressure_alt_delayer.push_back((alt - *m_last_baro_altitude) / m_dts);

                auto const& alt = m_pressure_alt_delayer.get_value();
                m_kf_z.z(3) = alt;

//                QLOGI("{}", (crt_alt - last_alt) / dts);
            }
            m_last_baro_altitude = alt;
        }

        m_kf_x.process();
        m_kf_y.process();
        m_kf_z.process();

        math::vec3d pos(m_kf_x.x(0), m_kf_y.x(0), m_kf_z.x(0));
        math::vec3d vel(m_kf_x.x(1), m_kf_y.x(1), m_kf_z.x(1));
        math::vec3d acc(m_kf_x.x(2), m_kf_y.x(2), m_kf_z.x(2));

        m_position_output_stream->push_sample(pos, true);
        m_velocity_output_stream->push_sample(vel, true);
        m_acceleration_output_stream->push_sample(acc, true);
    });
}

void KF_ECEF::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_init_params->rate, m_hal);
}

auto KF_ECEF::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("KF_ECEF::set_config");

    sz::KF_ECEF::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    m_config->position_accuracy = math::max(m_config->position_accuracy, 0.0);
    m_config->velocity_lag = math::max(m_config->velocity_lag, 0.0);
    m_config->velocity_accuracy = math::max(m_config->velocity_accuracy, 0.0);
    m_config->acceleration_lag = math::max(m_config->acceleration_lag, 0.0);
    m_config->acceleration_accuracy = math::max(m_config->acceleration_accuracy, 0.0);
    m_config->pressure_alt_lag = math::max(m_config->pressure_alt_lag, 0.0);
    m_config->pressure_alt_accuracy = math::max(m_config->pressure_alt_accuracy, 0.0);


    double pacc = math::square(m_config->position_accuracy);
    double vacc = math::square(m_config->velocity_accuracy);
    double aacc = math::square(m_config->acceleration_accuracy);
    double bacc = math::square(m_config->pressure_alt_accuracy);

    m_kf_x.R << pacc,    0,       0,
                0,       vacc,    0,
                0,       0,       aacc;
    m_kf_y.R = m_kf_x.R;
    m_kf_z.R << pacc,   0,      0,      0,
                0,      vacc,   0,      0,
                0,      0,      aacc,   0,
                0,      0,      0,      bacc;

    m_velocity_delayer.init(m_dts, m_config->velocity_lag);
    m_linear_acceleration_delayer.init(m_dts, m_config->acceleration_lag);
    m_pressure_alt_delayer.init(m_dts, m_config->pressure_alt_lag);

    return true;
}
auto KF_ECEF::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto KF_ECEF::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}

auto KF_ECEF::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}

}
}
