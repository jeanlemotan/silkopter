#include "BrainStdAfx.h"
#include "HAL.h"
#include "HAL_Motors_PiGPIO.h"
#include "HAL_Motors_Sim.h"
#include "HAL_Raspicam.h"
//#include "HAL_Camera_Sim.h"
#include "HAL_Sensors_Pi.h"
#include "HAL_Sensors_Sim.h"

#ifdef RASPBERRY_PI
    extern "C"
    {
        #include "bcm_host.h"
    }
    #include "PiGPIO.h"
    struct silk::HAL::Impl
    {
        Impl(boost::asio::io_service& io_service)  {}

        void process()
        {
        }

        silk::PiGPIO pigpio;
    };
#else
    #include "Sim_Comms.h"
    struct silk::HAL::Impl
    {
        Impl(boost::asio::io_service& io_service)
            : sim_comms(io_service) {}

        void process()
        {
            if (!sim_comms.is_connected())
            {
                sim_comms.connect();
            }
            else
            {
                sim_comms.process();
            }
        }

        silk::Sim_Comms sim_comms;
    };
#endif


using namespace silk;

auto HAL::init(boost::asio::io_service& io_service) -> bool
{
    m_impl.reset(new Impl(io_service));

#ifdef RASPBERRY_PI
    SILK_INFO("initializing bcm_host");
    bcm_host_init();

    auto m = new HAL_Motors_PiGPIO(m_impl->pigpio);
    motors.reset(m);
    if (!m->init())
    {
        return false;
    }

    auto c = new HAL_Raspicam;
    camera.reset(c);
    if (!c->init())
    {
        return false;
    }

    auto s = new HAL_Sensors_Pi;
    sensors.reset(s);
    if (!s->init())
    {
        return false;
    }
#else
    auto m = new HAL_Motors_Sim(m_impl->sim_comms);
    motors.reset(m);
//    if (m->init() != HAL_Motors_Sim::Result::OK)
//    {
//        return false;
//    }

//    auto c = new HAL_Raspicam;
//    camera.reset(c);
//    if (c->init() != HAL_Raspicam::Result::OK)
//    {
//        return false;
//    }

    auto s = new HAL_Sensors_Sim(m_impl->sim_comms);
    sensors.reset(s);
//    if (s->init() != HAL_Sensors_Sim::Result::OK)
//    {
//        return false;
//    }
#endif

    return true;
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
    m_impl->process();
}

