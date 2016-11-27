#include "Comms.h"
#include "Video_Decoder.h"
#include "Menu_System.h"
#include "Splash_Menu_Page.h"

#include "ISticks.h"
#include "IStick_Actuators.h"
#include "IRotary_Encoder.h"
#include "IButton.h"
#include "IButton_Matrix.h"

#include "Input.h"

#include "Adafruit_GFX.h"
#include "ArduiPi_OLED.h"

//#include "common/stream/IMultirotor_Commands.h"
//#include "common/stream/IMultirotor_State.h"

//boost::asio::io_service s_async_io_service(4);

int s_version_major = 1;
int s_version_minor = 0;

extern "C"
{
#include "utils/hw/pigpio.h"
#include "utils/hw/bcm2835.h"
}

///////////////////////////////////////////////////////////////////

std::chrono::microseconds PIGPIO_PERIOD(2);

static auto initialize_pigpio() -> bool
{
    static bool initialized = false;
    if (initialized)
    {
        return true;
    }

    QLOGI("Initializing pigpio");
    if (gpioCfgClock(PIGPIO_PERIOD.count(), 1, 0) < 0 ||
            gpioCfgPermissions(static_cast<uint64_t>(-1)))
    {
        QLOGE("Cannot configure pigpio");
        return false;
    }
    if (gpioInitialise() < 0)
    {
        QLOGE("Cannot initialize pigpio");
        return false;
    }

    initialized = true;
    return true;
}
static auto shutdown_pigpio() -> bool
{
    gpioTerminate();
    return true;
}

/* This prints an "Assertion failed" message and aborts.  */
void __assert_fail (const char *__assertion, const char *__file, unsigned int __line, const char *__function)
{
    QASSERT_MSG(false, "assert: {}:{}: {}: {}", __file, __line, __function, __assertion);
}


int main(int argc, char *argv[])
{
    q::logging::add_logger(q::logging::Logger_uptr(new q::logging::Console_Logger()));
    q::logging::set_decorations(q::logging::Decorations(q::logging::Decoration::TIMESTAMP, q::logging::Decoration::LEVEL, q::logging::Decoration::TOPIC));


    //    boost::shared_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(s_async_io_service));
    //    boost::thread_group worker_threads;
    //    for(int x = 0; x < 4; ++x)
    //    {
    //        worker_threads.create_thread(boost::bind(&boost::asio::io_service::run, &s_async_io_service));
    //    }

    if (!initialize_pigpio())
    {
        QLOGE("Cannot initialize pigpio");
        return false;
    }

    silk::Comms comms;
    silk::Menu_System menu_system;

    silk::Input input;
    input.init(comms);

    ArduiPi_OLED display;

    menu_system.push_page(std::unique_ptr<silk::IMenu_Page>(new silk::Splash_Menu_Page));

    size_t render_frames = 0;

    display.init(OLED_ADAFRUIT_I2C_128x64);

    q::Clock::time_point last_tp = q::Clock::now();
    size_t process_frames = 0;

    display.begin();

    // init done
    display.clearDisplay();   // clears the screen  buffer
    display.display();   		// display it (clear display)

    if (!comms.start())
    {
        display.print("Cannot start comms.");
        display.display();
        exit(1);
    }

    while (true)
    {
        input.process();
        comms.process();
        menu_system.process(input);

        process_frames++;
        if (q::Clock::now() - last_tp >= std::chrono::seconds(1))
        {
            last_tp = q::Clock::now();
            QLOGI("P FPS: {}, R FPS: {}", process_frames, render_frames);
            process_frames = 0;
            render_frames = 0;
        }

        if (display.displayIncremental(1000))
        {
            render_frames++;
            display.clearDisplay();

            menu_system.render(display);
        }

        //std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    shutdown_pigpio();

    return 0;
}
