#pragma once

#include "IInput_Device.h"
#include "utils/Clock.h"

namespace silk
{

class IHaptic : public virtual IInput_Device
{
public:
    virtual ~IHaptic() = default;

    struct Note
    {
        uint32_t frequency; //zero means no vibration
        Clock::duration duration;
    };

    virtual void vibrate(std::vector<Note> const& notes) = 0;
};

}
