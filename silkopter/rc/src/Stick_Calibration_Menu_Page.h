#pragma once

#include "IMenu_Page.h"
#include "Menu.h"
#include "utils/Clock.h"

namespace silk
{

class Stick_Calibration_Menu_Page : public IMenu_Page
{
public:
    Stick_Calibration_Menu_Page();

    bool process(Input& input, Menu_System& menu_system) override;
    void render(Adafruit_GFX& display);

private:
    Menu m_menu;

    struct Axis_Data
    {
        float raw = 0.f;
        float min = 1.f;
        float max = 0.f;

        float center = 0.f;
    };

    Axis_Data m_yaw;
    Axis_Data m_pitch;
    Axis_Data m_roll;
    Axis_Data m_throttle;

    enum class Phase
    {
        CENTER_QUESTIION,
        CENTER_MEASUREMENT,
        RANGE_QUESTIION,
        RANGE_MEASUREMENT,
        DONE,
    } m_phase = Phase::CENTER_QUESTIION;

    Clock::time_point m_phase_start_tp;
    size_t m_sample_count = 0;

    void next_phase();
    void draw_axis(Adafruit_GFX& display, int16_t y, const char* name, Axis_Data const& axis);
};

}
