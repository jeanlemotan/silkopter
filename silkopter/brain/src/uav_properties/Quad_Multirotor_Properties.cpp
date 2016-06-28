#include "Quad_Multirotor_Properties.h"

#include "hal.def.h"

namespace silk
{


bool Quad_Multirotor_Properties::init(hal::Quad_Multirotor_Descriptor const& descriptor)
{
    m_mass = descriptor.get_mass();
    m_radius = descriptor.get_radius();
    m_height = descriptor.get_height();
    m_motor_z_torque = descriptor.get_motor_z_torque();
    m_motor_thrust = descriptor.get_motor_thrust();
    m_motor_acceleration = descriptor.get_motor_acceleration();
    m_motor_deceleration = descriptor.get_motor_deceleration();
    m_moment_of_inertia = descriptor.get_moment_of_inertia();
    m_is_plus_configuration = descriptor.get_plus_configuration();

    if (math::is_zero(m_moment_of_inertia, math::epsilon<float>()))
    {
        m_moment_of_inertia = (1.f / 12.f) * m_mass * (3.f * math::square(m_radius) + math::square(m_height));
    }

    m_motors = create_motors(4, 1, descriptor.get_plus_configuration());

    return true;
}

float Quad_Multirotor_Properties::get_mass() const
{
    return m_mass;
}
float Quad_Multirotor_Properties::get_radius() const
{
    return m_radius;
}
float Quad_Multirotor_Properties::get_height() const
{
    return m_height;
}
float Quad_Multirotor_Properties::get_moment_of_inertia() const
{
    return m_moment_of_inertia;
}
float Quad_Multirotor_Properties::get_motor_z_torque() const
{
    return m_motor_z_torque;
}
float Quad_Multirotor_Properties::get_motor_thrust() const
{
    return m_motor_thrust;
}
float Quad_Multirotor_Properties::get_motor_acceleration() const
{
    return m_motor_acceleration;
}
float Quad_Multirotor_Properties::get_motor_deceleration() const
{
    return m_motor_deceleration;
}
bool Quad_Multirotor_Properties::is_plus_configuration() const
{
    return m_is_plus_configuration;
}
std::vector<Quad_Multirotor_Properties::Motor> const& Quad_Multirotor_Properties::get_motors() const
{
    return m_motors;
}

}
