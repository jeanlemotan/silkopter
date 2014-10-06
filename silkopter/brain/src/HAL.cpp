#include "BrainStdAfx.h"
#include "HW_Interfaces.h"
#include "HAL_Motors_PiGPIO.h"
#include "HAL_Motors_Sim.h"
#include "HAL_Camera_MMAL.h"
//#include "HAL_Camera_Sim.h"
#include "Sensor_Interface_Pi.h"
#include "Sensor_Interface_Sim.h"


HW_Interfaces::HW_Interfaces()
{
#ifdef RASPBERRY_PI
    motors.reset(new HAL_Motors_PiGPIO);
    sensor_interface.reset(new Sensor_Interface_Rpi);
    camera.reset(new HAL_Camera_MMAL);
#else
    motors.reset(new HAL_Motors_Sim);
    sensor_interface.reset(new Sensor_Interface_Sim);
    //camera.reset(new HAL_Camera_Sim);
#endif
}

auto HW_Interfaces::init() -> Result
{
    if (motors && motors->init() != HAL_Motors::Result::OK)
    {
        return Result::FAILED;
    }
    if (camera && camera->init() != HAL_Camera::Result::OK)
    {
        return Result::FAILED;
    }
    if (sensor_interface && sensor_interface->init() != sensor_Interface::Result::OK)
    {
        return Result::FAILED;
    }
}
void HW_Interfaces::shutdown()
{
    if (motors)
    {
        motors->shutdown()
    }
    if (camera)
    {
        camera->shutdown()
    }
    if (sensor_interface)
    {
        sensor_interface->shutdown()
    }
}
