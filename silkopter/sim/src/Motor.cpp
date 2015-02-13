#include "stdafx.h"
#include "Motor.h"
#include "physics/constants.h"

Motor::Motor(Config::Motor const& config)
    : m_config(config)
{
}

void Motor::set_volts(float volts)
{
    m_volts = volts;
}
void Motor::set_air_speed(float air_speed)
{
    m_air_speed = air_speed;
}
float Motor::compute_thrust() const
{
    float force_Kg = compute_force() / physics::constants::g;
    return force_Kg;
}
float Motor::compute_force() const
{
    //http://flitetest.com/articles/propeller-static-dynamic-thrust-calculation

    auto rpm = compute_rpm();
    float force_N = 4.392399e-8f * rpm
                * (math::pow(m_config.propeller.diameter, 3.5f) / math::sqrt(m_config.propeller.pitch))
                * (4.23333e-4f * rpm * m_config.propeller.pitch - m_air_speed);
    return force_N;
}
float Motor::compute_power() const
{
    //Abbott formula: Power (WATTS)=P(in.) X D(in.)^4 X RPM^3 X 5.33 X 10^-15
    double rpm = static_cast<double>(compute_rpm());

    double power = static_cast<double>(m_config.propeller.factor)
                    * static_cast<double>(m_config.propeller.pitch)
                    * math::pow(static_cast<double>(m_config.propeller.diameter), 4.0)
                    * math::pow(rpm, 3.0) * 5.33 * 10e-15;

    return power;
}
float Motor::compute_rpm() const
{
    /*
    RPM = Kv * (V - Vloss)
    Vloss = I * Rm

    P = F(RPM)
    I = P / V

    ~~ RPM = Kv * (V - ((F(RPM) / V) * Rm))

    I = F(Kv * (V - (I * Rm))) / V



    I = (sqrt(Io^2 * Rm^2 - 2*Io*Rm*V - 4*P*Rm + V^2) + Io*Rm + V) / (2 * Rm)

    Pin = Vin * Iin
    Iin = Pin / Vin

    Pout = (Vin - Iin * Rm) * (Iin - Io)
    (v - x * r) * (x - o) - p = 0

    Pout = (Vin - Pin * Rm / Vin) * (Pin / Vin - Io)

    p = (v - ((n * m) / v)) * ((n / v) - o)
    (v - ((x * m) / v)) * ((x / v) - o) - p = 0

    Ploss = Rm * Iin^2 + Vin*Io



    */




    return m_rpm;
}

auto Motor::get_config() const -> Config::Motor const&
{
    return m_config;
}

void Motor::process(q::Clock::duration dt)
{
    auto target_rpm = m_volts * m_config.Kv;
	if (!math::equals(m_rpm, target_rpm))
	{
		auto delta = target_rpm - m_rpm;
        float acc = delta > 0 ? m_config.acceleration : m_config.decceleration;
		float d = math::min(math::abs(delta), acc * q::Seconds(dt).count());
		m_rpm += math::sgn(delta) * d;
        m_rpm = math::max(m_rpm, 0.f);
	}
}

void Motor::reset()
{
	m_rpm = 0;
    m_volts = 0;
}
