#include "Stick_Calibration_Menu_Page.h"
#include "Adafruit_GFX.h"
#include "Input.h"
#include "ISticks.h"

#include "Menu_System.h"

#include "settings.def.h"

namespace silk
{

extern settings::Settings s_settings;
extern void save_settings();

///////////////////////////////////////////////////////////////////////////////////////////////////

Stick_Calibration_Menu_Page::Stick_Calibration_Menu_Page()
{
    m_menu.push_submenu({"Next", "Cancel"}, 0, 34);
    m_phase_start_tp = Clock::now();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Stick_Calibration_Menu_Page::next_phase()
{
    m_phase_start_tp = Clock::now();
    m_phase = static_cast<Phase>(static_cast<int>(m_phase) + 1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool Stick_Calibration_Menu_Page::process(Input& input, Menu_System& menu_system)
{
    auto selected_entry = m_menu.process(input);
    if (selected_entry != boost::none)
    {
        switch (*selected_entry)
        {
        case 0:
        {
            if (m_phase != Phase::DONE)
            {
                if (m_phase == Phase::CENTER_MEASUREMENT)
                {
                    float div = m_sample_count > 0 ? static_cast<float>(m_sample_count) : 1.f;
                    m_yaw.center /= div;
                    m_pitch.center /= div;
                    m_roll.center /= div;
                    m_throttle.center /= div;
                }

                next_phase();
                if (m_phase == Phase::DONE)
                {
                    m_menu.push_submenu({"Save", "Discard"}, 0, 32);
                }
            }
            else
            {
                input.get_sticks().set_yaw_calibration(m_yaw.center, m_yaw.min, m_yaw.max);
                input.get_sticks().set_pitch_calibration(m_pitch.center, m_pitch.min, m_pitch.max);
                input.get_sticks().set_roll_calibration(m_roll.center, m_roll.min, m_roll.max);
                input.get_sticks().set_throttle_calibration(m_throttle.center, m_throttle.min, m_throttle.max);

                settings::Settings::Input::Sticks_Calibration& sc = s_settings.get_input().get_sticks_calibration();
                sc.set_yaw_center(m_yaw.center);
                sc.set_yaw_min(m_yaw.min);
                sc.set_yaw_max(m_yaw.max);
                sc.set_pitch_center(m_pitch.center);
                sc.set_pitch_min(m_pitch.min);
                sc.set_pitch_max(m_pitch.max);
                sc.set_roll_center(m_roll.center);
                sc.set_roll_min(m_roll.min);
                sc.set_roll_max(m_roll.max);
                sc.set_throttle_center(m_throttle.center);
                sc.set_throttle_min(m_throttle.min);
                sc.set_throttle_max(m_throttle.max);

                save_settings();

                return false;
            }
            break;
        }
        case 1: return false;
        }
    }

    m_yaw.raw = input.get_sticks().get_raw_yaw();
    m_pitch.raw = input.get_sticks().get_raw_pitch();
    m_roll.raw = input.get_sticks().get_raw_roll();
    m_throttle.raw = input.get_sticks().get_raw_throttle();

    if (m_phase == Phase::CENTER_MEASUREMENT)
    {
        m_yaw.center += m_yaw.raw;
        m_pitch.center += m_pitch.raw;
        m_roll.center += m_roll.raw;
        m_throttle.center += m_throttle.raw;

        m_sample_count++;
    }
    else if (m_phase == Phase::RANGE_MEASUREMENT)
    {
        m_yaw.min = math::min(m_yaw.min, m_yaw.raw);
        m_yaw.max = math::max(m_yaw.max, m_yaw.raw);
        m_pitch.min = math::min(m_pitch.min, m_pitch.raw);
        m_pitch.max = math::max(m_pitch.max, m_pitch.raw);
        m_roll.min = math::min(m_roll.min, m_roll.raw);
        m_roll.max = math::max(m_roll.max, m_roll.raw);
        m_throttle.min = math::min(m_throttle.min, m_throttle.raw);
        m_throttle.max = math::max(m_throttle.max, m_throttle.raw);

        m_sample_count++;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Stick_Calibration_Menu_Page::draw_axis(Adafruit_GFX& display, int16_t y, const char* name, Axis_Data const& axis)
{
    int16_t w = display.width();

    int16_t axis_x = 8;
    int16_t axis_w = w - axis_x - 1;
    int16_t axis_h = 5;
    int16_t axis_y = y + axis_h/2 + 1;
    int16_t center_x = axis_x + static_cast<int16_t>(axis.center * static_cast<float>(axis_w));
    int16_t min_x = axis_x + static_cast<int16_t>(axis.min * static_cast<float>(axis_w));
    int16_t max_x = axis_x + static_cast<int16_t>(axis.max * static_cast<float>(axis_w));
    int16_t value_x = axis_x + static_cast<int16_t>(axis.raw * static_cast<float>(axis_w));

    display.setCursor(0, y);
    display.print(name);
    display.drawLine(axis_x, axis_y, axis_x + axis_w, axis_y, 1);

    display.drawLine(center_x, axis_y - 1, center_x, axis_y + 1, 1);

    display.drawLine(min_x, axis_y - 2, min_x, axis_y + 2, 1);
    display.drawLine(axis_x, axis_y - 1, min_x, axis_y - 1, 1);
    display.drawLine(axis_x, axis_y + 1, min_x, axis_y + 1, 1);

    display.drawLine(max_x, axis_y - 2, max_x, axis_y + 2, 1);
    display.drawLine(max_x, axis_y - 1, axis_x + axis_w, axis_y - 1, 1);
    display.drawLine(max_x, axis_y + 1, axis_x + axis_w, axis_y + 1, 1);

    display.drawLine(value_x, axis_y - 2, value_x, axis_y + 2, 1);
    y += 8;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Stick_Calibration_Menu_Page::render(Adafruit_GFX& display)
{
    if (m_phase == Phase::CENTER_QUESTIION)
    {
        display.setCursor(0, 0);
        display.setTextWrap(true);
        display.print("Center all sticks and press ok");
    }
    else if (m_phase == Phase::CENTER_MEASUREMENT)
    {
        float div = m_sample_count > 0 ? static_cast<float>(m_sample_count) : 1.f;
        int32_t yaw_center = static_cast<int32_t>(m_yaw.center / div * 100.f);
        int32_t pitch_center = static_cast<int32_t>(m_pitch.center / div * 100.f);
        int32_t roll_center = static_cast<int32_t>(m_roll.center / div * 100.f);
        int32_t throttle_center = static_cast<int32_t>(m_throttle.center / div * 100.f);

        display.setCursor(0, 0);
        display.setTextWrap(true);
        display.printf("%d samples.\nY:%d P:%d R:%d T:%d", m_sample_count, yaw_center, pitch_center, roll_center, throttle_center);
    }
    else if (m_phase == Phase::RANGE_QUESTIION)
    {
        display.setCursor(0, 0);
        display.setTextWrap(true);
        display.print("Move all sticks to their extremes after pressing ok");
    }
    else if (m_phase == Phase::RANGE_MEASUREMENT)
    {
        draw_axis(display, 0, "Y", m_yaw);
        draw_axis(display, 8, "P", m_pitch);
        draw_axis(display, 16, "R", m_roll);
        draw_axis(display, 24, "T", m_throttle);
    }
    else if (m_phase == Phase::DONE)
    {
        display.setCursor(0, 0);
        display.setTextWrap(true);
        display.print("Calibration done. Do you want to save?");
    }

    m_menu.render(display, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

}
