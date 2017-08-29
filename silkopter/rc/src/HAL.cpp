#include "HAL.h"
#include "IBattery_Info.h"
#include "Input.h"
#include "ArduiPi_OLED.h"
#include "utils/hw/I2C_Dev.h"
#include "utils/hw/ADS1115.h"
#include "Battery_Info_ADS1115.h"
#include "Gimbal_Control_ADS1115.h"

#include "def_lang/Serialization.h"
#include "def_lang/JSON_Serializer.h"

#include "settings.def.h"
#include <fstream>

extern "C"
{
#include "utils/hw/pigpio.h"
#include "utils/hw/bcm2835.h"
}

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace silk
{

static const std::string k_settings_filename("settings.json");
extern std::string s_program_path;


struct HAL::Impl
{
    Impl(HAL& hal)
        : display()
        , comms(hal)
        , i2c()
        , battery_gimbal_adc()
        , battery_info(hal, battery_gimbal_adc, 1) //battery is connected to channel 1
        , gimbal_control(battery_gimbal_adc, 0u, boost::none)
        , input(hal, i2c)
        , settings()
    {}

    ArduiPi_OLED display;
    Comms comms;
    util::hw::I2C_Dev i2c;
    util::hw::ADS1115 battery_gimbal_adc;
    silk::Battery_Info_ADS1115 battery_info;
    silk::Gimbal_Control_ADS1115 gimbal_control;
    silk::Input input;

    settings::Settings settings;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

static bool pigpio_is_isitialized = false;

static bool initialize_pigpio(HAL& hal)
{
    if (pigpio_is_isitialized)
    {
        return true;
    }

    QLOGI("Initializing pigpio");
    if (gpioCfgClock(hal.get_settings().get_hw().get_pigpio_period_us(), PI_CLOCK_PCM, 0) < 0 ||
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

    pigpio_is_isitialized = true;
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

static void shutdown_pigpio()
{
    if (pigpio_is_isitialized)
    {
        gpioTerminate();
    }
    pigpio_is_isitialized = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

static void generate_settings_file(HAL& hal)
{
    hal.get_settings() = settings::Settings();
    hal.get_settings().get_comms().set_video_wlan_interfaces({"wlan1", "wlan2"});
    hal.save_settings();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

static ts::Result<void> load_settings(HAL& hal)
{
    std::string filename = s_program_path + "/" + k_settings_filename;
    QLOGI("Loading settings from '{}'", filename);

    std::string data;

    {
        //read the data
        std::ifstream fs(filename, std::ifstream::in | std::ifstream::binary);
        if (!fs.is_open())
        {
            QLOGW("Failed to load '{}'", filename);
            generate_settings_file(hal);
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
        return ts::Error("Failed to load '" + filename + "': " + json_result.error().what());
    }

    settings::Settings settings;
    auto reserialize_result = settings::deserialize(settings, json_result.payload());
    if (reserialize_result != ts::success)
    {
        return ts::Error("Failed to deserialize settings: " + reserialize_result.error().what());
    }

    hal.get_settings() = settings;

    return ts::success;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void HAL::save_settings()
{
    std::string filename = s_program_path + "/" + k_settings_filename;
    QLOGI("Saving settings to '{}'", filename);

    ts::sz::Value sz_value = settings::serialize(m_impl->settings);

    std::string json = ts::sz::to_json(sz_value, true);

    std::ofstream fs(filename);
    if (fs.is_open())
    {
        fs.write(json.data(), json.size());
        fs.flush();
    }
    else
    {
        QLOGE("Cannot open '{}' to save settings.", filename);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

HAL::HAL()
{
    m_impl.reset(new Impl(*this));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

HAL::~HAL()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////

ts::Result<void> HAL::init()
{
    m_impl->display.init(OLED_ADAFRUIT_I2C_128x64);
    m_impl->display.begin();

    // init done
    m_impl->display.clearDisplay();   // clears the screen  buffer
    m_impl->display.display();   		// display it (clear display)

    ts::Result<void> result = load_settings(*this);
    if (result != ts::success)
    {
        return result.error();
    }

    if (!initialize_pigpio(*this))
    {
        return ts::Error("Cannot initialize pigpio");
    }

    if (!m_impl->comms.start())
    {
        return ts::Error("Cannot start comms.");
    }

    result = m_impl->i2c.init("/dev/i2c-1");
    if (result != ts::success)
    {
        return result.error();
    }

    {
        util::hw::ADS1115::Descriptor descriptor;
        descriptor.i2c_address = 0x49;
        descriptor.adcs[0].is_enabled = true;
        descriptor.adcs[0].rate = 200;
        descriptor.adcs[1].is_enabled = true;
        descriptor.adcs[1].rate = 200;

        result = m_impl->battery_gimbal_adc.init(m_impl->i2c, descriptor);
        if (result != ts::success)
        {
            return result.error();
        }
    }

    m_impl->battery_info.init();
    m_impl->gimbal_control.init();

    m_impl->input.init();

    return ts::success;
}

void HAL::shutdown()
{
    shutdown_pigpio();
}

Comms& HAL::get_comms()
{
    return m_impl->comms;
}

IBattery_Info& HAL::get_battery_info()
{
    return m_impl->battery_info;
}

IGimbal_Control& HAL::get_gimbal_control()
{
    return m_impl->gimbal_control;
}

Input& HAL::get_input()
{
    return m_impl->input;
}

ArduiPi_OLED& HAL::get_display()
{
    return m_impl->display;
}

settings::Settings& HAL::get_settings()
{
    return m_impl->settings;
}

void HAL::process()
{
    m_impl->battery_gimbal_adc.process(m_impl->i2c);
    m_impl->battery_info.process();
    m_impl->gimbal_control.process();

    m_impl->input.process();
    m_impl->comms.process();
}

bool HAL::render()
{
    if (m_impl->display.displayIncremental(m_impl->settings.get_hw().get_display_incremental_step_us()))
    {
        m_impl->display.clearDisplay();
        return true;
    }
    return false;
}

}
