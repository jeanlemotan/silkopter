#pragma once

#include "qmath.h"
#include "uav_properties/IUAV_Properties.h"

#include <stdint.h>
#include <vector>

namespace silk
{

class IMultirotor_Properties : public IUAV_Properties
{
public:
    virtual ~IMultirotor_Properties() = default;

    struct Motor
    {
        math::vec3f position;
        math::vec3f thrust_vector;
        bool clockwise = false;
    };

    virtual float get_radius() const = 0;
    virtual float get_height() const = 0;

    virtual float get_motor_z_torque() const = 0;
    virtual float get_motor_thrust() const = 0;
    virtual float get_motor_acceleration() const = 0;
    virtual float get_motor_deceleration() const = 0;

    virtual std::vector<Motor> const& get_motors() const = 0;

protected:
    static std::vector<Motor> create_motors(size_t arm_count, float radius, bool plus_config, bool double_config);
};


}
