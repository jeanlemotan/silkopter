#pragma once

#include "Comms.h"
#include "IBattery_Info.h"
#include "IGimbal_Control.h"
#include "ISticks.h"
#include "IStick_Actuators.h"
#include "IHaptic.h"

#include "def_lang/Result.h"


namespace silk
{
namespace settings
{
class Settings;
}

class HAL
{
public:
    HAL();
    ~HAL();

    ts::Result<void> init();
    void shutdown();

    Comms& get_comms();
    IBattery_Info& get_battery_info();
    IGimbal_Control& get_gimbal_control();
    ISticks& get_sticks();
    IStick_Actuators& get_stick_actuators();
    IHaptic& get_haptic();

    settings::Settings& get_settings();
    void save_settings();

    void process();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};


}
