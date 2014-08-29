#pragma once

#include <stdint.h>
#include <stddef.h>
#include "_qmath.h"

namespace silk
{
	class Motor_Mixer
	{
	public:
		struct Motor_Info
		{
			Motor_Info() = default;
            Motor_Info(math::vec2f const& position, bool clockwise)
                : position(position), clockwise(clockwise) {}

			math::vec2f position; //	position represents a point in the 2D XY plane with Y going forward and X to the right.
			bool clockwise = true;
		};

        void remove_all_motors();
		void add_motor(Motor_Info const& motor);

        //output never exceeds this range.
        void set_output_range(float min, float max);

		//throttle is collective - it applies to all motors
		//	0 means motors off
		//	1 means full throttle
		//	range is 0 - 1
		// yaw pitch and roll are the throttles for each of the 3 axis.
		//	zero means no change
		//	positive means the motors will spin as to rotate that axis in the positive direction
		//	negative means the motors will spin as to rotate that axis in the negative direction
		//	the range is -1 .. 1
		void set_data(float collective_throttle, float yaw, float pitch, float roll);

        size_t get_motor_count() const;
        Motor_Info const& get_motor_info(size_t idx) const;
        float get_motor_output(size_t idx) const;

	private:
		void update_mixing();

		struct Motor_Data
		{
			Motor_Info info;
			float output = 0.f;
		};
        std::vector<Motor_Data> m_motors;

        float m_min_output = 0.f;
        float m_max_output = 1.f;
		
		float m_throttle = 0.f;
		float m_yaw = 0;
		float m_pitch = 0;
		float m_roll = 0;
	};
}
