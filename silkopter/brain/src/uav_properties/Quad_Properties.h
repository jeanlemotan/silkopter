#pragma once

#include "uav_properties/IMultirotor_Properties.h"

#include <stdint.h>
#include <vector>


namespace silk
{
namespace uav
{
struct Quad_Descriptor;
}
}


namespace silk
{

class Quad_Properties : public IMultirotor_Properties
{
public:
    virtual ~Quad_Properties() = default;

    bool init(uav::Quad_Descriptor const& descriptor);

    float get_mass() const override;
    float get_moment_of_inertia() const override;

    float get_radius() const override;
    float get_height() const override;

    float get_motor_z_torque() const override;
    float get_motor_thrust() const override;
    float get_motor_acceleration() const override;
    float get_motor_deceleration() const override;

    std::vector<Motor> const& get_motors() const override;

private:
    float m_mass = 1.f;
    float m_radius = 1.f;
    float m_height = 1.f;
    float m_motor_z_torque = 1.f;
    float m_motor_thrust = 1.f;
    float m_motor_acceleration = 10.f;
    float m_motor_deceleration = 10.f;
    float m_moment_of_inertia = 0.f;

    std::vector<Motor> m_motors;
};


}
