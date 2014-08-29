#pragma  once

#include "Curve.h"

class World;

class Motor
{
    friend class World;
    friend class UAV;
public:
	typedef Curve<float> Thrust_Curve; //rpm to thrust
	typedef Curve<float> Amps_Curve; //rpm to amps

	//acceleration is in rpm/s
    Motor(math::vec2f const& position,
          bool clockwise,
          float max_rpm,
          float acceleration,
          float decceleration,
          float air_drag);

    auto get_thrust_curve() -> Thrust_Curve&;
    auto get_amps_curve() -> Amps_Curve&;

    auto get_air_drag() const -> float;

	void set_throttle(float throttle);
    auto get_throttle() const -> float;

    auto get_position() const -> math::vec2f const&;
    auto get_thrust() const -> float; //in kilograms
    auto get_force() const -> float; //in newtons
    auto get_amps() const -> float; //in amps/hour
    auto get_rpm() const -> float;//in rot/min
    auto get_max_rpm() const -> float;//in rot/min

	void reset();

    void process(q::Clock::duration dt);

protected:
    math::vec2f m_position;
    bool m_is_clockwise = true;
	float m_acceleration = 0;
	float m_decceleration = 0;
	float m_max_rpm = 0;
	float m_throttle = 0;
	float m_rpm = 0;
	float m_air_drag = 0;
	Thrust_Curve m_thrust_curve;
	Amps_Curve m_amps_curve;
};
