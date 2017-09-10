#pragma once

#include "Comms.h"
#include "IBattery_Info.h"
#include "Input.h"
#include "IGimbal_Control.h"

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
    Input& get_input();

    settings::Settings& get_settings();
    void save_settings();

    void process();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};


}
