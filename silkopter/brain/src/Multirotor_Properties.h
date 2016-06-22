#pragma once

#include "IUAV_Properties.h"

#include <stdint.h>
#include <vector>


namespace silk
{
namespace uav
{
struct Multirotor_Descriptor;
struct Tricopter_Descriptor;
struct Quadcopter_Descriptor;
struct Hexacopter_Descriptor;
struct Hexatri_Descriptor;
struct Octocopter_Descriptor;
struct Octaquad_Descriptor;
struct Custom_Multirotor_Descriptor;
}
}


namespace silk
{

class Multirotor_Properties : public IUAV_Properties
{
public:
    Multirotor_Properties() = default;

    bool init(uav::Multirotor_Descriptor const& descriptor);

    struct Motor
    {
        math::vec3f position;
        math::vec3f thrust_vector;
        bool clockwise = false;
    };

    float get_mass() const override;
    float get_moment_of_inertia() const override;

    float get_radius() const;
    float get_height() const;

    float get_motor_z_torque() const;
    float get_motor_thrust() const;
    float get_motor_acceleration() const;
    float get_motor_deceleration() const;

    std::vector<Motor> const& get_motors() const;

private:
    bool init_tricopter(uav::Tricopter_Descriptor const& descriptor);
    bool init_quadcopter(uav::Quadcopter_Descriptor const& descriptor);
    bool init_hexacopter(uav::Hexacopter_Descriptor const& descriptor);
    bool init_hexatri(uav::Hexatri_Descriptor const& descriptor);
    bool init_octocopter(uav::Octocopter_Descriptor const& descriptor);
    bool init_octaquad(uav::Octaquad_Descriptor const& descriptor);
    bool init_custom_multirotor(uav::Custom_Multirotor_Descriptor const& descriptor);


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
