#include "BrainStdAfx.h"

#ifdef RASPBERRY_PI

#include "PiGPIO.h"
#include <pigpio.h>

using namespace silk;

///////////////////////////////////////////////////////////////

auto PiGPIO::init() -> bool
{
    if (gpioCfgClock(2, 1, 1) < 0)
    {
        SILK_WARNING("Cannot configure gpio clock");
        return false;
    }

    if (gpioInitialise() < 0)
    {
        SILK_WARNING("Cannot initialize gpio");
        return false;
    }
}

PiGPIO::~PiGPIO()
{
    gpioTerminate();
}

#endif
