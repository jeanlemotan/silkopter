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
#include "Main_Menu_Page.h"

#include "settings.def.h"

#include "def_lang/Serialization.h"
#include "def_lang/JSON_Serializer.h"
#include "utils/Clock.h"

#include <fstream>

//#include "common/stream/IMultirotor_Commands.h"
//#include "common/stream/IMultirotor_State.h"

//boost::asio::io_service s_async_io_service(4);

namespace silk
{

int s_version_major = 1;
int s_version_minor = 0;

const std::string k_settings_path("settings.json");

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

///////////////////////////////////////////////////////////////////////////////////////////////////

static auto shutdown_pigpio() -> bool
{
    gpioTerminate();
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/* This prints an "Assertion failed" message and aborts.  */
void __assert_fail(const char *__assertion, const char *__file, unsigned int __line, const char *__function)
{
    QASSERT_MSG(false, "assert: {}:{}: {}: {}", __file, __line, __function, __assertion);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace silk
{

///////////////////////////////////////////////////////////////////////////////////////////////////

void save_settings();

///////////////////////////////////////////////////////////////////////////////////////////////////

void generate_settings_file()
{
    s_settings = settings::Settings();
    s_settings.get_comms().set_video_interfaces({"wlan2", "wlan3"});
    save_settings();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool load_settings()
{
    std::string data;

    {
        //read the data
        std::ifstream fs(k_settings_path, std::ifstream::in | std::ifstream::binary);
        if (!fs.is_open())
        {
            QLOGW("Failed to load '{}'", k_settings_path);
            generate_settings_file();
            return false;
        }

        fs.seekg (0, fs.end);
        size_t size = fs.tellg();
        fs.seekg (0, fs.beg);

        data.resize(size + 1);
        fs.read(&data[0], size);
    }

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

///////////////////////////////////////////////////////////////////////////////////////////////////

void save_settings()
{
    ts::sz::Value sz_value = settings::serialize(s_settings);

    std::string json = ts::sz::to_json(sz_value, true);

    std::ofstream fs(k_settings_path);
    if (fs.is_open())
    {
        fs.write(json.data(), json.size());
        fs.flush();
    }
    else
    {
        QLOGE("Cannot open '{}' to save settings.", k_settings_path);
    }
}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

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
    input.init();

    // init done
    display.clearDisplay();   // clears the screen  buffer
    display.display();   		// display it (clear display)

    std::unique_ptr<silk::IMenu_Page> mm(new silk::Main_Menu_Page(comms));

    menu_system.push_page(std::unique_ptr<silk::IMenu_Page>(new silk::Splash_Menu_Page(std::move(mm))));

    while (true)
    {
        input.process();
        comms.process();
        menu_system.process(input);

        if (display.displayIncremental(silk::s_settings.get_hw().get_display_incremental_step_us()))
        {
            display.clearDisplay();
            menu_system.render(display);
        }

        //std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    shutdown_pigpio();

    return 0;
}
