#include "stdafx.h"
#include "Motor.h"
#include "physics/constants.h"

Motor::Motor(math::vec2f const& position,
             bool clockwise,
             float max_rpm,
             float acceleration,
             float decceleration,
             float air_drag)
    : m_position(position)
    , m_is_clockwise(clockwise)
    , m_max_rpm(max_rpm)
	, m_acceleration(acceleration)
	, m_decceleration(decceleration)
	, m_air_drag(air_drag)
{

}

Motor::Thrust_Curve& Motor::get_thrust_curve()
{
	return m_thrust_curve;
}
Motor::Amps_Curve& Motor::get_amps_curve()
{
	return m_amps_curve;
}

void Motor::set_throttle(float throttle)
{
	m_throttle = throttle;
}
float Motor::get_throttle() const
{
	return m_throttle;
}

float Motor::get_thrust() const
{
	return m_thrust_curve.get_value(m_rpm / m_max_rpm);
}
float Motor::get_force() const
{
	return get_thrust() * physics::constants::g;
}
float Motor::get_amps() const
{
	return m_amps_curve.get_value(m_rpm / m_max_rpm);
}
float Motor::get_rpm() const
{
	return m_rpm;
}
float Motor::get_max_rpm() const
{
	return m_max_rpm;
}

void Motor::process(q::Clock::duration dt)
{
	auto target_rpm = m_throttle * m_max_rpm;
	if (!math::equals(m_rpm, target_rpm))
	{
		auto delta = target_rpm - m_rpm;
		float acc = delta > 0 ? m_acceleration : m_decceleration;
		float d = math::min(math::abs(delta), acc * q::Seconds(dt).count());
		m_rpm += math::sgn(delta) * d;
		m_rpm = math::clamp(m_rpm, 0.f, m_max_rpm);
	}
}

void Motor::reset()
{
	m_throttle = 0;
	m_rpm = 0;
}

float Motor::get_air_drag() const
{
	return m_air_drag;
}
