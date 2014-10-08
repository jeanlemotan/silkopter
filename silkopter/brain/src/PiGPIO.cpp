#include "BrainStdAfx.h"

#ifdef RASPBERRY_PI

#include "PiGPIO.h"
#include <pigpio.h>

using namespace silk;

///////////////////////////////////////////////////////////////

auto PiGPIO::init() -> Result
{
    if (gpioCfgClock(2, 1, 1) < 0)
    {
        SILK_WARNING("Cannot configure gpio clock");
        return Result::FAILED;
    }

    if (gpioInitialise() < 0)
    {
        SILK_WARNING("Cannot initialize gpio");
        return Result::FAILED;
    }
}

PiGPIO::~PiGPIO()
{
    gpioTerminate();
}

#endif
