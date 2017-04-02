#pragma once

#include "Comms.h"
#include "IBattery_Info.h"
#include "Input.h"
#include "ArduiPi_OLED.h"

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
    Input& get_input();

    ArduiPi_OLED& get_display();

    settings::Settings& get_settings();
    void save_settings();

    void process();
    bool render();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};


}
