#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{

class AHRS : q::util::Noncopyable
{
public:
    void process(Gyroscope_Sample const& gyroscope,
                 Accelerometer_Sample const& accelerometer,
                 Compass_Sample const& compass);

    auto get_angular_velocity() const -> math::vec3f const&;

    auto get_front_vector() const -> math::vec3f const&;
    auto get_right_vector() const -> math::vec3f const&;
    auto get_up_vector() const -> math::vec3f const&;

    //body to earth
    auto get_b2e_quat() const -> math::quatf const&;
    auto get_b2e_mat() const -> math::mat3f const&;

    //earth to body
    auto get_e2b_quat() const -> math::quatf const&;
    auto get_e2b_mat() const -> math::mat3f const&;

    void reset();

private:
    q::Clock::time_point m_last_correction_timestamp;

    uint32_t m_last_gyroscope_sample_idx = 0;
    uint32_t m_last_accelerometer_sample_idx = 0;
    uint32_t m_last_compass_sample_idx = 0;

    math::vec3f m_angular_velocity;
    math::mat3f m_b2e_mat;
    math::quatf m_b2e_quat;
    math::mat3f m_e2b_mat;
    math::quatf m_e2b_quat;

    math::vec3f	m_earth_frame_front;
    math::vec3f	m_earth_frame_right;
    math::vec3f	m_earth_frame_up;
    math::quatf m_b2e_quat_noisy;
};

}
