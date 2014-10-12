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

    auto get_angular_velocity_l() const -> math::vec3f const&;

    auto get_front_vector_l() const -> math::vec3f const&;
    auto get_right_vector_l() const -> math::vec3f const&;
    auto get_up_vector_l() const -> math::vec3f const&;

    auto get_front_vector_w() const -> math::vec3f const&;
    auto get_right_vector_w() const -> math::vec3f const&;
    auto get_up_vector_w() const -> math::vec3f const&;

    //body to earth
    auto get_quat_l2w() const -> math::quatf const&;
    auto get_mat_l2w() const -> math::mat3f const&;

    //earth to body
    auto get_quat_w2l() const -> math::quatf const&;
    auto get_mat_w2l() const -> math::mat3f const&;

    void reset();

private:
    q::Clock::time_point m_last_correction_timestamp;

    uint32_t m_last_gyroscope_sample_idx = 0;
    uint32_t m_last_accelerometer_sample_idx = 0;
    uint32_t m_last_compass_sample_idx = 0;

    math::vec3f m_angular_velocity;
    math::mat3f m_mat_l2w;
    math::quatf m_quat_l2w;
    math::mat3f m_mat_w2l;
    math::quatf m_quat_w2l;

    math::vec3f	m_noisy_front_w;
    math::vec3f	m_noisy_right_w;
    math::vec3f	m_noisy_up_w;
    math::quatf m_noisy_quat_l2w;
};

}
