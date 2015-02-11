//#include "BrainStdAfx.h"
//#include "IHAL.h"
//#include "HAL_Motors_PiGPIO.h"
//#include "HAL_Motors_Sim.h"
//#include "HAL_Raspicam.h"
//#include "HAL_Camera_Mount_PiGPIO.h"
//#include "HAL_PI.h"
//#include "HAL_Sensors_Sim.h"

//#ifdef RASPBERRY_PI
//extern "C"
//{
//    #include "bcm_host.h"
//    #include "pigpio.h"
//}
//#endif


//using namespace silk;

//auto HAL::init(Comms& comms) -> bool
//{
//    QLOG_TOPIC("hal::init");
//#ifdef RASPBERRY_PI

//    QLOGI("Initializing bcm_host");
//    bcm_host_init();

//    QLOGI("Initializing pigpio");
//    if (gpioCfgClock(2, 1, 0) < 0 ||
//        gpioCfgPermissions(static_cast<uint64_t>(-1)) ||
//        gpioCfgInterfaces(PI_DISABLE_SOCK_IF | PI_DISABLE_FIFO_IF))
//    {
//        QLOGE("Cannot configure pigpio");
//        return false;
//    }
//    if (gpioInitialise() < 0)
//    {
//        QLOGE("Cannot initialize pigpio");
//        return false;
//    }

//    //if ran with sudo, revert to normal user
//    if (geteuid() == 0 && getuid() != 0)
//    {
//        QLOGI("Giving up root for {}", getuid());
//        if (setuid(getuid()) < 0)
//        {
//            QLOGE("Cannot give up root priviledges");
//            return false;
//        }
//    }

//    QLOGI("Configuring i2c pin modes");
//    if (gpioSetMode(0, PI_INPUT) ||
//        gpioSetMode(1, PI_INPUT) ||
//        gpioSetMode(28, PI_ALT0) ||
//        gpioSetMode(29, PI_ALT0))
//    {
//        QLOGE("Cannot initialize pigpio i2c pin modes");
//        return false;
//    }

//    QLOGI("Creating HW devices");
//    motors.reset(new HAL_Motors_PiGPIO());
//    camera.reset(new HAL_Raspicam());
//    sensors.reset(new HAL_Sensors_HW());
//    camera_mount.reset(new HAL_Camera_Mount_PiGPIO());
//#else
//    motors.reset(new HAL_Motors_Sim(comms));
////    camera.reset(new HAL_Raspicam());
//    sensors.reset(new HAL_Sensors_Sim(comms));
//#endif

//    QLOGI("Initializing HW devices");
//    if (motors && !motors->init())
//    {
//        return false;
//    }
////    if (camera && !camera->init())
////    {
////        return false;
////    }
//    if (sensors && !sensors->init())
//    {
//        return false;
//    }
//    if (camera_mount && !camera_mount->init())
//    {
//        return false;
//    }

//    return true;
//}

//void HAL::shutdown()
//{
//    if (motors)
//    {
//        motors->shutdown();
//    }
//    if (camera)
//    {
//        camera->shutdown();
//    }
//    if (sensors)
//    {
//        sensors->shutdown();
//    }
//    if (camera_mount)
//    {
//        camera_mount->shutdown();
//    }

//#ifdef RASPBERRY_PI
//    gpioTerminate();
//#endif
//}

//void HAL::process()
//{
//    if (motors)
//    {
//        motors->process();
//    }
//    if (camera)
//    {
//        camera->process();
//    }
//    if (sensors)
//    {
//        sensors->process();
//    }
//    if (camera_mount)
//    {
//        camera_mount->process();
//    }
//}

