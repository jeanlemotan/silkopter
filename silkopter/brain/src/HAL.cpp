#include "BrainStdAfx.h"
#include "HAL.h"
#include "HAL_Motors_PiGPIO.h"
#include "HAL_Motors_Sim.h"
#include "HAL_Raspicam.h"
#include "HAL_Camera_Mount_PiGPIO.h"
#include "HAL_Sensors_HW.h"
#include "HAL_Sensors_Sim.h"

#ifdef RASPBERRY_PI
extern "C"
{
    #include "bcm_host.h"
    #include "pigpiod_if.h"
}
#endif


using namespace silk;

auto HAL::init() -> bool
{
#ifdef RASPBERRY_PI

    SILK_INFO("initializing bcm_host");
    bcm_host_init();

    SILK_INFO("Connecting to pigpiod daemon");
    if (pigpio_start(nullptr, nullptr) < 0)
    {
        SILK_ERR("Cannot connect to pigpiod. Make sure it's started.");
        return false;
    }

    set_mode(0, PI_INPUT);
    set_mode(1, PI_INPUT);
    set_mode(28, PI_ALT0);
    set_mode(29, PI_ALT0);

    motors.reset(new HAL_Motors_PiGPIO());
    camera.reset(new HAL_Raspicam());
    sensors.reset(new HAL_Sensors_HW());
    camera_mount.reset(new HAL_Camera_Mount_PiGPIO());
#else
//    motors.reset(new HAL_Motors_Sim());
////    camera.reset(new HAL_Raspicam());
//    sensors.reset(new HAL_Sensors_Sim());
#endif

    if (motors && !motors->init())
    {
        return false;
    }
    if (camera && !camera->init())
    {
        return false;
    }
    if (sensors && !sensors->init())
    {
        return false;
    }
    if (camera_mount && !camera_mount->init())
    {
        return false;
    }

    return true;
}

void HAL::shutdown()
{
    if (motors)
    {
        motors->shutdown();
    }
    if (camera)
    {
        camera->shutdown();
    }
    if (sensors)
    {
        sensors->shutdown();
    }

#ifdef RASPBERRY_PI
    pigpio_stop();
#endif
}

void HAL::process()
{
    if (motors)
    {
        motors->process();
    }
    if (camera)
    {
        camera->process();
    }
    if (sensors)
    {
        sensors->process();
    }

    //NOTE!!! this HAS to be here, at the end. The SIM comms depends on it
    //m_impl->process();
}

