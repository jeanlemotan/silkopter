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

#include "settings.def.h"

#include "def_lang/Serialization.h"
#include "def_lang/JSON_Serializer.h"
#include "QData.h"

//#include "common/stream/IMultirotor_Commands.h"
//#include "common/stream/IMultirotor_State.h"

//boost::asio::io_service s_async_io_service(4);

namespace silk
{

int s_version_major = 1;
int s_version_minor = 0;

const q::Path k_settings_path("settings.json");

settings::Settings s_settings;

}

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
    if (gpioCfgClock(PIGPIO_PERIOD.count(), silk::s_settings.get_hw().get_pigpio_period_us(), 0) < 0 ||
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

namespace silk
{

void save_settings();

void generate_settings_file()
{
    s_settings = settings::Settings();
    s_settings.get_comms().set_video_interfaces({"wlan2", "wlan3"});
    save_settings();
}

bool load_settings()
{
    //read the data
    q::data::File_Source fs(k_settings_path);
    if (!fs.is_open())
    {
        QLOGW("Failed to load '{}'", k_settings_path);
        generate_settings_file();
        return true;
    }

    std::string data = q::data::read_whole_source_as_string<std::string>(fs);

    ts::Result<ts::sz::Value> json_result = ts::sz::from_json(data);
    if (json_result != ts::success)
    {
        QLOGE("Failed to load '{}': {}", k_settings_path, json_result.error().what());
        return false;
    }

    settings::Settings settings;
    auto reserialize_result = settings::deserialize(settings, json_result.payload());
    if (reserialize_result != ts::success)
    {
        QLOGE("Failed to deserialize settings: {}", reserialize_result.error().what());
        return false;
    }

    s_settings = settings;

    return true;
}

void save_settings()
{
    ts::sz::Value sz_value = settings::serialize(s_settings);

    std::string json = ts::sz::to_json(sz_value, true);

    q::data::File_Sink fs(k_settings_path);
    if (fs.is_open())
    {
        fs.write(reinterpret_cast<uint8_t const*>(json.data()), json.size());
        fs.flush();
    }
    else
    {
        QLOGE("Cannot open '{}' to save settings.", k_settings_path);
    }
}
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

    ArduiPi_OLED display;
    display.init(OLED_ADAFRUIT_I2C_128x64);
    display.begin();

    if (!silk::load_settings())
    {
        QLOGE("Cannot load settings.");
        display.print("Cannot load settings.");
        display.display();
        exit(1);
    }

    if (!initialize_pigpio())
    {
        QLOGE("Cannot initialize pigpio");
        display.print("Cannot initialize pigpio.");
        display.display();
        exit(1);
    }

    silk::Comms comms;
    if (!comms.start())
    {
        QLOGE("Cannot start comms.");
        display.print("Cannot start comms.");
        display.display();
        exit(1);
    }

    silk::Menu_System menu_system;

    silk::Input input;
    input.init(comms);

    size_t render_frames = 0;

    q::Clock::time_point last_tp = q::Clock::now();
    size_t process_frames = 0;

    // init done
    display.clearDisplay();   // clears the screen  buffer
    display.display();   		// display it (clear display)

    menu_system.push_page(std::unique_ptr<silk::IMenu_Page>(new silk::Splash_Menu_Page));

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

        if (display.displayIncremental(silk::s_settings.get_hw().get_display_incremental_step_us()))
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
