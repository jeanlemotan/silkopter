#include "BrainStdAfx.h"
#include "HAL.h"
#include "HAL_Motors_PiGPIO.h"
#include "HAL_Motors_Sim.h"
#include "HAL_Raspicam.h"
//#include "HAL_Camera_Sim.h"
#include "HAL_Sensors_Pi.h"
#include "HAL_Sensors_Sim.h"

#ifdef RASPBERRY_PI
#include "PiGPIO.h"
struct silk::HAL::Impl
{
    Impl(boost::asio::io_service& io_service)  {}

    silk::PiGPIO pigpio;
};
#else
#include "Sim_Comms.h"
struct silk::HAL::Impl
{
    Impl(boost::asio::io_service& io_service)
        : sim_comms(io_service) {}

    silk::Sim_Comms sim_comms;
};
#endif


using namespace silk;

auto HAL::init(boost::asio::io_service& io_service) -> Result
{
    m_impl.reset(new Impl(io_service));

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
    auto m = new HAL_Motors_Sim(m_impl->sim_comms);
    motors.reset(m);
//    if (m->init() != HAL_Motors_Sim::Result::OK)
//    {
//        return Result::FAILED;
//    }

//    auto c = new HAL_Raspicam;
//    camera.reset(c);
//    if (c->init() != HAL_Raspicam::Result::OK)
//    {
//        return Result::FAILED;
//    }

    auto s = new HAL_Sensors_Sim(m_impl->sim_comms);
    sensors.reset(s);
//    if (s->init() != HAL_Sensors_Sim::Result::OK)
//    {
//        return Result::FAILED;
//    }
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

