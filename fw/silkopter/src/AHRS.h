#pragma once

#include "util/Noncopyable.h"
#include "board/board.h"

namespace silk
{

class AHRS : util::Noncopyable
{
	public:

	void process(board::IMU::Gyroscope_Data const& gdata,
				board::IMU::Accelerometer_Data const& adata);
		
	//returns the euler along all three axis
	math::vec3f const& get_euler() const;
	math::vec3f const& get_front_vector() const;
	math::vec3f const& get_right_vector() const;
	math::vec3f const& get_up_vector() const;
		
	math::quatf const& get_local_to_world_quaternion() const;
	math::mat3f const& get_local_to_world_rotation() const;

	math::quatf const& get_world_to_local_quaternion() const;
	math::mat3f const& get_world_to_local_rotation() const;
		
	private:
	math::vec3f m_euler;
	math::mat3f m_local_rotation;
	math::quatf m_local_quaternion;
	math::mat3f m_world_rotation;
	math::quatf m_world_quaternion;
};

}
