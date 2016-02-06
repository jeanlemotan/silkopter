#pragma once

#include "QBase.h"
#include <stdint.h>
#include <stddef.h>
#include <_qmath.h>

namespace board
{
    class Gyroscope : q::util::Noncopyable
	{
	public:
        struct Data_Config
        {
            math::vec3f bias;
            math::vec3f scale = math::vec3f::one;
        };
        virtual Data_Config get_data_config() const = 0;

        struct Data
        {
            uint8_t id; //this is incremented every time there is a new sample
            uint8_t dt_ms; //for how long is this measurement
            math::vec3s16 angular_velocity;
        };

        //returns true if the data was written to
        virtual bool get_data(Data& data) const = 0;
    };

}
