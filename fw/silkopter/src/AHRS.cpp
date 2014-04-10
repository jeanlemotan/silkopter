#include "AHRS.h"

using namespace silk;

void AHRS::process(board::IMU::Data const& data, chrono::secondsf dt)
{
	if (data.sample_idx != m_last_sample_idx)
	{
		m_last_sample_idx = data.sample_idx;
		
		auto const& gyro = data.angular_velocity;
		//gyro.z = 0; //TODO - use compass
		m_euler += math::vec3f(gyro.x, gyro.y, 0) * data.dt.count;
	}
	
	//only apply the complimentary filter when the accel pitch/roll are valid - that is when the Z is pointing UP
	auto const& accel = data.acceleration;
	if (accel.z > 0.1f)
 	{
		//calculate the pitch/roll from the accel. We'll use this to fix the gyro drift using a complimentary filter
		float accel_pitch_x = math::atan2<float, math::fast>(accel.y, accel.z);
		float accel_roll_y = -math::atan2<float, math::fast>(accel.x, math::sqrt<math::fast>(accel.y*accel.y + accel.z*accel.z));

		m_euler.x = math::lerp(m_euler.x, accel_pitch_x, data.dt.count);
		m_euler.y = math::lerp(m_euler.y, accel_roll_y, data.dt.count);
 	}
	
// 	m_local_quaternion.set_from_euler_xyz<math::standard>(m_euler);
// 	m_local_quaternion.get_as_mat3<math::standard>(m_local_rotation);
// 	m_world_quaternion = math::inverse<float, math::standard>(m_local_quaternion);
// 	m_world_quaternion.get_as_mat3<math::standard>(m_world_rotation);

	m_local_quaternion.set_from_euler_xyz<math::fast>(m_euler);
	m_world_quaternion = math::inverse<float, math::fast>(m_local_quaternion);
	m_local_quaternion.get_as_mat3_and_inv<math::fast>(m_local_rotation, m_world_rotation);
}

math::vec3f const& AHRS::get_euler() const
{
	return m_euler;
}
math::quatf const& AHRS::get_local_to_world_quaternion() const
{
	return m_local_quaternion;
}
math::mat3f const& AHRS::get_local_to_world_rotation() const
{
	return m_local_rotation;
}
math::quatf const& AHRS::get_world_to_local_quaternion() const
{
	return m_world_quaternion;
}
math::mat3f const& AHRS::get_world_to_local_rotation() const
{
	return m_world_rotation;
}
math::vec3f const& AHRS::get_front_vector() const
{
	return m_local_rotation.get_axis_y();
}
math::vec3f const& AHRS::get_right_vector() const
{
	return m_local_rotation.get_axis_x();
}
math::vec3f const& AHRS::get_up_vector() const
{
	return m_local_rotation.get_axis_z();
}


