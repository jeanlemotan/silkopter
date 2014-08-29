#include "BrainStdAfx.h"
#include "Motor_Mixer.h"

using namespace silk;


void Motor_Mixer::remove_all_motors()
{
    m_motors.clear();
}

void Motor_Mixer::add_motor(Motor_Info const& motor)
{
    Motor_Data d;
    d.info = motor;
    m_motors.push_back(d);
}

void Motor_Mixer::set_output_range(float min, float max)
{
    QASSERT(min < max);
    if (min >= max)
    {
        return;
    }
    m_min_output = min;
    m_max_output = max;
}

void Motor_Mixer::set_data(float collective_throttle, float yaw, float pitch, float roll)
{
	m_throttle = collective_throttle;
	m_yaw = yaw;
	m_pitch = pitch;
	m_roll = roll;
	
	update_mixing();
}

void Motor_Mixer::update_mixing()
{
    math::vec3f speeds2d(-m_roll, m_pitch, 0.f);

    //the dinamic range is limited by the distance from min to throttle,
    //  or the half total throttle range - which ever is smaller
    float min_v = math::max(m_throttle - m_min_output, 0.f);
    float half_range = (m_max_output - m_min_output) * 0.5f;
    float dynamic_range = math::min(min_v, half_range);

    float max = -999.f;
    for (auto& motor: m_motors)
    {
        float output = math::dot(math::vec3f(motor.info.position, 0.f), speeds2d);
        if (motor.info.clockwise)
        {
            output += m_yaw;
        }
        else
        {
            output -= m_yaw;
        }
        output = math::clamp(output, -dynamic_range, dynamic_range) + math::max(m_throttle, m_min_output);
        max = math::max(max, output);
        motor.output = output;
        QASSERT(output + std::numeric_limits<float>::epsilon() >= m_min_output);
    }

    //if over the max, reduce throttle
    //This should never bring any motor under min because of how we calculated the dynamic range
    if (max > m_max_output)
    {
        float diff = max - m_max_output;
        for (auto& motor: m_motors)
        {
            motor.output -= diff;
            QASSERT(motor.output + std::numeric_limits<float>::epsilon() >= m_min_output);
            QASSERT(motor.output - std::numeric_limits<float>::epsilon() <= m_max_output);
        }
    }
}

size_t Motor_Mixer::get_motor_count() const
{
    return m_motors.size();
}
Motor_Mixer::Motor_Info const& Motor_Mixer::get_motor_info(size_t idx) const
{
	return m_motors[idx].info;
}
float Motor_Mixer::get_motor_output(size_t idx) const
{
	return m_motors[idx].output;
}


