#pragma once

#include "common/node/IProcessor.h"
#include "common/stream/ILinear_Acceleration.h"
#include "common/stream/IPosition.h"
#include "common/stream/IVelocity.h"
#include "common/stream/IPressure.h"
#include "common/stream/IFrame.h"

#include "HAL.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"

//#include "kalman/ekfilter.hpp"
#include "Eigen/Core"


namespace sz
{
namespace EKF_ECEF
{
struct Init_Params;
struct Config;
}
}


namespace silk
{
namespace node
{

class EKF_ECEF : public IProcessor
{
public:
    EKF_ECEF(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::shared_ptr<sz::EKF_ECEF::Init_Params> m_init_params;
    std::shared_ptr<sz::EKF_ECEF::Config> m_config;

    q::Clock::duration m_dt = q::Clock::duration(0);

    Sample_Accumulator<stream::IECEF_Position,
                       stream::IECEF_Velocity,
                       stream::IENU_Linear_Acceleration,
                       stream::IPressure> m_accumulator;

    typedef Basic_Output_Stream<stream::IECEF_Position> Position_Output_Stream;
    mutable std::shared_ptr<Position_Output_Stream> m_position_output_stream;

    typedef Basic_Output_Stream<stream::IECEF_Velocity> Velocity_Output_Stream;
    mutable std::shared_ptr<Velocity_Output_Stream> m_velocity_output_stream;

    boost::optional<float> m_last_baro_altitude;

    template<size_t St, size_t Me>
    class KF
    {
    public:
        KF();

        //state
        Eigen::Matrix<double, St, St> A; //State transition matrix.
        typedef Eigen::Matrix<double, St, 1> State_Vector;
        State_Vector x; //State estimate

        //input
        Eigen::Matrix<double, St, St> B; //Control matrix. This is used to define linear equations for any control factors.
        typedef Eigen::Matrix<double, St, 1> Input_Vector;
        Input_Vector u;

        Eigen::Matrix<double, Me, St> H; //Observation matrix. Multiply a state vector by H to translate it to a measurement vector.

        typedef Eigen::Matrix<double, Me, 1> Measurement_Vector;
        Measurement_Vector z; //measurement data

        //error
        Eigen::Matrix<double, St, St> P; //state error
        Eigen::Matrix<double, St, St> Q; //estimated process error covariance

        Eigen::Matrix<double, St, Me> K; //kalman gain
        Eigen::Matrix<double, Me, Me> R; //measurement error covariance

        Eigen::Matrix<double, St, St> I; //identity

        void predict();
        void update();

        void process();
    };
    KF<3, 3> m_kf_x;
    KF<3, 3> m_kf_y;
    KF<3, 4> m_kf_z;
    //KF<3, 3> m_kf_z;

    float m_dts = 0;

    template<class Value>
    struct Delayer
    {
        void init(float dt, float lag);
        void push_back(Value const& value);
        auto get_value() -> Value const&;

        std::deque<Value> values;
        size_t min_value_count = 0;
    };

    Delayer<stream::IECEF_Velocity::Value> m_velocity_delayer;
    Delayer<stream::IENU_Linear_Acceleration::Value> m_linear_acceleration_delayer;
    Delayer<stream::IPressure::Value> m_pressure_alt_delayer;
};



}
}

