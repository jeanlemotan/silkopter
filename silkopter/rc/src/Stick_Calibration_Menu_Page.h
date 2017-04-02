#pragma once

#include "IMenu_Page.h"
#include "Menu.h"
#include "utils/Clock.h"

namespace silk
{

class HAL;

class Stick_Calibration_Menu_Page : public IMenu_Page
{
public:
    Stick_Calibration_Menu_Page(HAL& hal);

    bool process(Input& input, Menu_System& menu_system) override;
    void render(Adafruit_GFX& display);

private:
    HAL& m_hal;
    Menu m_menu;

    struct Axis_Data
    {
        float raw = 0.f;
        float min = 1.f;
        float max = 0.f;

        float center = 0.f;
        float deadband = 0.f; //this one is after min/max transformation - it's relative to the 0.5 center
    };

    Axis_Data m_yaw;
    Axis_Data m_pitch;
    Axis_Data m_roll;
    Axis_Data m_throttle;

    enum class Phase
    {
        CENTER_QUESTION,
        CENTER_MEASUREMENT,
        RANGE_QUESTION,
        RANGE_MEASUREMENT,
        DEADBAND_QUESTION,
        YAW_DEADBAND,
        PITCH_DEADBAND,
        ROLL_DEADBAND,
        THROTTLE_DEADBAND,
        DONE,
    } m_phase = Phase::CENTER_QUESTION;

    Clock::time_point m_phase_start_tp;
    size_t m_sample_count = 0;

    void process_deadband(Input& input, Axis_Data& axis);

    void next_phase();
    void draw_axis(Adafruit_GFX& display, int16_t y, const char* name, Axis_Data const& axis);
    void draw_axis_deadband(Adafruit_GFX& display, int16_t y, const char* name, Axis_Data const& axis);
};

}
