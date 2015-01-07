#pragma once

#include "common/sensors/Sensor_Samples.h"


namespace silk
{

class GPS_Protocol : q::util::Noncopyable
{
public:
    virtual ~GPS_Protocol() {}


    //new data available. Process it and return true if the data is recognized
    virtual auto detect(uint8_t const* data, size_t size) -> bool = 0;

    //once detected, initialize and take ownership of the device
    virtual auto init(int fd) -> bool = 0;

    //call regularly to update the sample
    virtual void process() = 0;

    //return the current sample
    virtual auto get_sample() const -> sensors::GPS_Sample const& = 0;
};





}
