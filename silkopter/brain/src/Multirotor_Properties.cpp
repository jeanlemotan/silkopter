#include "Multirotor_Properties.h"

#include "uav.def.h"

namespace silk
{


bool Multirotor_Properties::init(uav::Multirotor_Descriptor const& descriptor)
{
    QLOG_TOPIC("Multirotor::init");

    if (auto* d = dynamic_cast<uav::Tricopter_Descriptor const*>(&descriptor))
    {
        return init_tricopter(*d);
    }
    else if (auto* d = dynamic_cast<uav::Quadcopter_Descriptor const*>(&descriptor))
    {
        return init_quadcopter(*d);
    }
    else if (auto* d = dynamic_cast<uav::Hexacopter_Descriptor const*>(&descriptor))
    {
        return init_hexacopter(*d);
    }
    else if (auto* d = dynamic_cast<uav::Hexatri_Descriptor const*>(&descriptor))
    {
        return init_hexatri(*d);
    }
    else if (auto* d = dynamic_cast<uav::Octocopter_Descriptor const*>(&descriptor))
    {
        return init_octocopter(*d);
    }
    else if (auto* d = dynamic_cast<uav::Octaquad_Descriptor const*>(&descriptor))
    {
        return init_octaquad(*d);
    }
    else if (auto* d = dynamic_cast<uav::Custom_Multirotor_Descriptor const*>(&descriptor))
    {
        return init_custom_multirotor(*d);
    }
    else
    {
        QLOGE("Unknown multirotor descriptor type");
        return false;
    }

//    if (descriptor.get_motors().size() < 2)
//    {
//        QLOGE("Bad motor count: {}", descriptor.get_motors().size());
//        return false;
//    }
//    for (auto const& m: descriptor.get_motors())
//    {
//        if (math::is_zero(m.get_position(), math::epsilon<float>()))
//        {
//            QLOGE("Bad motor position: {}", m.get_position());
//            return false;
//        }
//    }

//    //http://en.wikipedia.org/wiki/List_of_moments_of_inertia
//    m_uav_descriptor.reset(new uav::Multirotor_Descriptor(descriptor)); //make a copy
//    if (math::is_zero(descriptor.get_moment_of_inertia(), math::epsilon<float>()))
//    {
//        m_uav_descriptor->set_moment_of_inertia((1.f / 12.f) * descriptor.get_mass() * (3.f * math::square(descriptor.get_radius()) + math::square(descriptor.get_height())));
//    }

//    descriptor_changed_signal.execute(*this);

}

float Multirotor_Properties::get_radius() const
{
    return m_radius;
}
float Multirotor_Properties::get_height() const
{
    return m_height;
}
float Multirotor_Properties::get_moment_of_inertia() const
{
    return m_moment_of_inertia;
}
float Multirotor_Properties::get_motor_z_torque() const
{
    return m_motor_z_torque;
}
float Multirotor_Properties::get_motor_thrust() const
{
    return m_motor_thrust;
}
float Multirotor_Properties::get_motor_acceleration() const
{
    return m_motor_acceleration;
}
float Multirotor_Properties::get_motor_deceleration() const
{
    return m_motor_deceleration;
}
std::vector<Multirotor_Properties::Motor> const& Multirotor_Properties::get_motors() const
{
    return m_motors;
}

bool Multirotor_Properties::init_tricopter(uav::Tricopter_Descriptor const& /*descriptor*/)
{
    QLOGE("Tricopters are not supported for now.");
    return false;
}

bool Multirotor_Properties::init_quadcopter(uav::Quadcopter_Descriptor const& descriptor)
{
    create_motors(4, 1, descriptor.get_plus_configuration());
}

bool Multirotor_Properties::init_hexacopter(uav::Hexacopter_Descriptor const& descriptor)
{
    create_motors(6, 1, descriptor.get_plus_configuration());
}

bool Multirotor_Properties::init_hexatri(uav::Hexatri_Descriptor const& descriptor)
{
    create_motors(3, 2, true);
}

bool Multirotor_Properties::init_octocopter(uav::Octocopter_Descriptor const& descriptor)
{
    create_motors(8, 1, descriptor.get_plus_configuration());
}

bool Multirotor_Properties::init_octaquad(uav::Octaquad_Descriptor const& descriptor)
{
    create_motors(4, 2, descriptor.get_plus_configuration());
}

bool Multirotor_Properties::init_custom_multirotor(uav::Custom_Multirotor_Descriptor const& /*descriptor*/)
{
    QLOGE("Custom multirotors are not supported for now.");
    return false;
}



}
