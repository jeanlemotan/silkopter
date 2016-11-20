#pragma once

#include "IInput_Device.h"

namespace silk
{

class IRotary_Encoder : public IInput_Device
{
public:
    virtual ~IRotary_Encoder() = default;

    virtual int32_t get_delta() const = 0;
    virtual int32_t get_clicks() const = 0;
};

}
