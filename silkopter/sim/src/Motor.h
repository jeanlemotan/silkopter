#pragma  once

#include "Config.h"

class World;

class Motor
{
    friend class World;
    friend class UAV;
public:

	//acceleration is in rpm/s
    Motor(Config::Motor const& config);

    void set_air_speed(float air_speed);
    void set_volts(float volts);

    auto get_config() const -> Config::Motor const&;

    auto compute_thrust() const -> float; //in kilograms
    auto compute_force() const -> float; //in newtons
    auto compute_power() const -> float; //in watts
    auto compute_rpm() const -> float;//in rot/min

	void reset();

    void process(q::Clock::duration dt);

protected:
    Config::Motor m_config;
    float m_rpm = 0;
    float m_volts = 0;
    float m_air_speed = 0;
    Config::Propeller m_propeller;
};
