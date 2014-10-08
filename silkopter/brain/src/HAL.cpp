#include "BrainStdAfx.h"
#include "HAL.h"
#include "HAL_Motors_PiGPIO.h"
#include "HAL_Motors_Sim.h"
#include "HAL_Raspicam.h"
//#include "HAL_Camera_Sim.h"
#include "HAL_Sensors_Pi.h"
#include "HAL_Sensors_Sim.h"

using namespace silk;

#ifdef RASPBERRY_PI
struct HAL::Impl
{
    PiGPIO pigpio;
};
#else
struct HAL::Impl
{
    Sim_Comms sim_comms;
};
#endif
auto HAL::init() -> Result
{
    m_impl.reset(new Impl);

#ifdef RASPBERRY_PI
    auto m = new HAL_Motors_PiGPIO(m_impl->pigpio);
    motors.reset(m);
    if (m->init() != HAL_Motors_PiGPIO::Result::OK)
    {
        return Result::FAILED;
    }

    auto c = new HAL_Raspicam;
    camera.reset(c);
    if (c->init() != HAL_Raspicam::Result::OK)
    {
        return Result::FAILED;
    }

    auto s = new HAL_Sensors_Pi;
    sensors.reset(s);
    if (s->init() != HAL_Sensors_Pi::Result::OK)
    {
        return Result::FAILED;
    }
#else
    auto m = new HAL_Motors_Sim;
    motors.reset(m);
    if (m->init() != HAL_Motors_Sim::Result::OK)
    {
        return Result::FAILED;
    }

//    auto c = new HAL_Raspicam;
//    camera.reset(c);
//    if (c->init() != HAL_Raspicam::Result::OK)
//    {
//        return Result::FAILED;
//    }

    auto s = new HAL_Sensors_Sim;
    sensors.reset(s);
    if (s->init() != HAL_Sensors_Sim::Result::OK)
    {
        return Result::FAILED;
    }
#endif

    return Result::OK;
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
}

