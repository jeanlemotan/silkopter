#include "IMultirotor_Properties.h"
#include "qmath.h"

namespace silk
{

std::vector<IMultirotor_Properties::Motor> IMultirotor_Properties::create_motors(size_t arm_count, float radius, bool plus_config, bool double_config)
{
    std::vector<IMultirotor_Properties::Motor> res;

    float angle_between_motors = math::anglef::_2pi / (float)arm_count;
    float starting_angle = plus_config ? 0 : (angle_between_motors / 2.f);

    size_t rows = double_config ? 2 : 1;
    for (size_t r = 0; r < rows; r++)
    {
        float angle = starting_angle;
        for (size_t c = 0; c < arm_count; c++)
        {
            float x = 0.f;
            float y = 0.f;
            math::sin_cos(angle, y, x);

            x *= radius;
            y *= radius;

            Motor motor;
            motor.position = math::vec3f(x, y, 0.f);
            motor.thrust_vector = math::vec3f(0, 0, 1.f);
            motor.clockwise = (c & 1) == 0;
            res.push_back(motor);

            angle += angle_between_motors;
        }
    }


    return res;
}




}
