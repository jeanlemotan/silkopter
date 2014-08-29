#pragma once

#include "IO_Board.h"

namespace silk
{

class AHRS : q::util::Noncopyable
{
public:
    void process(IO_Board::Sensor_Sample const& sensors);

    auto get_angular_velocity() const -> math::vec3f const&;

    auto get_front_vector() const -> math::vec3f const&;
    auto get_right_vector() const -> math::vec3f const&;
    auto get_up_vector() const -> math::vec3f const&;

    auto get_local_to_world_quat() const -> math::quatf const&;
    auto get_local_to_world_mat() const -> math::mat3f const&;

    auto get_world_to_local_quat() const -> math::quatf const&;
    auto get_world_to_local_mat() const -> math::mat3f const&;

    void reset();

private:
    q::Clock::time_point m_last_correction_timestamp;

    uint32_t m_last_gyroscope_sample_idx = 0;
    uint32_t m_last_accelerometer_sample_idx = 0;
    uint32_t m_last_compass_sample_idx = 0;

    math::vec3f m_angular_velocity;
    math::mat3f m_local_to_world_mat;
    math::quatf m_local_to_world_quat;
    math::mat3f m_world_to_local_mat;
    math::quatf m_world_to_local_quat;

    math::vec3f	m_earth_frame_front;
    math::vec3f	m_earth_frame_right;
    math::vec3f	m_earth_frame_up;
    math::quatf m_local_to_world_quat_noisy;
};

}
