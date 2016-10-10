#pragma once

namespace silk
{

class IInput_Device
{
public:
    virtual ~IInput_Device() = default;

    virtual void process() = 0;
};

}
