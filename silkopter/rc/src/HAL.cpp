#include "HAL.h"
#include "IBattery_Info.h"
#include "utils/hw/I2C_Dev.h"
#include "utils/hw/ADS1115.h"
#include "Battery_Info_ADS1115.h"
#include "Gimbal_Control_ADS1115.h"
#include "Sticks_ADS1115.h"
#include "Stick_Actuators_Throttle_DRV883x.h"

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
        : comms(hal)
        , i2c()
        , battery_gimbal_adc()
        , battery_info(hal, battery_gimbal_adc, 1) //battery is connected to channel 1
        , gimbal_control(battery_gimbal_adc, 0u, boost::none)
        , sticks(i2c)
        , stick_actuators(sticks)
        , settings()
    {}

    Comms comms;
    util::hw::I2C_Dev i2c;
    util::hw::ADS1115 battery_gimbal_adc;
    silk::Battery_Info_ADS1115 battery_info;
    silk::Gimbal_Control_ADS1115 gimbal_control;
    silk::Sticks_ADS1115 sticks;
    silk::Stick_Actuators_Throttle_DRV883x stick_actuators;

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

    {
        auto result = m_impl->sticks.init();
        if (result != ts::success)
        {
            return result.error();
        }
        silk::settings::Settings::Input::Sticks_Calibration const& sc = get_settings().get_input().get_sticks_calibration();
        m_impl->sticks.set_yaw_calibration(sc.get_yaw_min(), sc.get_yaw_center(), sc.get_yaw_max(), sc.get_yaw_deadband(), sc.get_yaw_curve());
        m_impl->sticks.set_pitch_calibration(sc.get_pitch_min(), sc.get_pitch_center(), sc.get_pitch_max(), sc.get_pitch_deadband(), sc.get_pitch_curve());
        m_impl->sticks.set_roll_calibration(sc.get_roll_min(), sc.get_roll_center(), sc.get_roll_max(), sc.get_roll_deadband(), sc.get_roll_curve());
        m_impl->sticks.set_throttle_calibration(sc.get_throttle_min(), sc.get_throttle_center(), sc.get_throttle_max(), sc.get_throttle_deadband(), sc.get_throttle_curve());
    }

    {
        auto result = m_impl->stick_actuators.init();
        if (result != ts::success)
        {
            return result.error();
        }
    }

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
ISticks& HAL::get_sticks()
{
    return m_impl->sticks;
}

IStick_Actuators& HAL::get_stick_actuators()
{
    return m_impl->stick_actuators;
}

IHaptic& HAL::get_haptic()
{
    return m_impl->stick_actuators;
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
    m_impl->comms.process();
    m_impl->sticks.process();
    m_impl->stick_actuators.process();
}


}
