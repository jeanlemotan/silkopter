#pragma once

#include "IMenu_Page.h"
#include "Menu.h"
#include "utils/Clock.h"

namespace silk
{
class HAL;

class Info_Menu_Page : public IMenu_Page
{
public:
    Info_Menu_Page(HAL& hal);

    bool process(Input& input, Menu_System& menu_system) override;
    void render(Adafruit_GFX& display);

private:
    Menu m_menu;
    HAL& m_hal;

    struct Axis_Data
    {
        float value = 0.f;
    };

    Axis_Data m_yaw;
    Axis_Data m_pitch;
    Axis_Data m_roll;
    Axis_Data m_throttle;

    std::vector<std::vector<uint8_t>> m_button_state;

    enum class Section
    {
        STICKS,
        BUTTONS,
        COMMS,
        BATTERY,
        ABOUT,
    } m_section = Section::STICKS;

    void draw_axis(Adafruit_GFX& display, int16_t y, const char* name, Axis_Data const& axis);

    Clock::time_point m_throttle_mode_change_tp = Clock::now();
    bool m_throttle_initialized = false;

    Clock::time_point m_last_dbm_tp = Clock::now();
    int m_min_rx_dBm = 999;
    int m_min_tx_dBm = 999;
};

}
