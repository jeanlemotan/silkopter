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

    util::coordinates::ECEF m_last_gps_position;
    math::vec3f m_velocity;

    class KF
    {
    public:
        KF();

        //state
        Eigen::Matrix3d A;
        Eigen::Vector3d x;

        //input
        Eigen::Matrix3d B;
        Eigen::Vector3d u;

        //measurement
        Eigen::Matrix3d K;
        Eigen::Vector3d z;

        //error
        Eigen::Matrix3d P;
        Eigen::Matrix3d G;
        Eigen::Matrix3d R;
        Eigen::Matrix3d Q;

        Eigen::Matrix3d I;

        void predict();
        void update();
    };
    KF m_kf_x;
    KF m_kf_y;
    KF m_kf_z;
};



}
}

