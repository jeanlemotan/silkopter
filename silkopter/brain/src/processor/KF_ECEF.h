#pragma once

#include "common/node/IProcessor.h"
#include "common/stream/ILinear_Acceleration.h"
#include "common/stream/IPosition.h"
#include "common/stream/IVelocity.h"
#include "common/stream/IAcceleration.h"
#include "common/stream/IFrame.h"

#include "HAL.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"

#include "Eigen/Core"


namespace silk
{
namespace hal
{
struct KF_ECEF_Descriptor;
struct KF_ECEF_Config;
}
}


namespace silk
{
namespace node
{

class KF_ECEF : public IProcessor
{
public:
    KF_ECEF(HAL& hal);

    ts::Result<void> init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    ts::Result<void> set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    ts::Result<void> start(q::Clock::time_point tp) override;

    ts::Result<void> set_input_stream_path(size_t idx, std::string const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    ts::Result<void> init();

    HAL& m_hal;

    std::shared_ptr<hal::KF_ECEF_Descriptor> m_descriptor;
    std::shared_ptr<hal::KF_ECEF_Config> m_config;

    q::Clock::duration m_dt = q::Clock::duration(0);

    Sample_Accumulator<stream::IECEF_Position,
                       stream::IECEF_Velocity,
                       stream::IENU_Linear_Acceleration> m_accumulator;

    typedef Basic_Output_Stream<stream::IECEF_Position> Position_Output_Stream;
    mutable std::shared_ptr<Position_Output_Stream> m_position_output_stream;

    typedef Basic_Output_Stream<stream::IECEF_Velocity> Velocity_Output_Stream;
    mutable std::shared_ptr<Velocity_Output_Stream> m_velocity_output_stream;

    typedef Basic_Output_Stream<stream::IECEF_Linear_Acceleration> Linear_Acceleration_Output_Stream;
    mutable std::shared_ptr<Linear_Acceleration_Output_Stream> m_linear_acceleration_output_stream;

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
    KF<3, 3> m_kf_z;

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

    Delayer<stream::IECEF_Position::Value> m_gps_position_delayer;
    Delayer<stream::IECEF_Velocity::Value> m_gps_velocity_delayer;
    Delayer<stream::IECEF_Linear_Acceleration::Value> m_linear_acceleration_delayer;
};



}
}

