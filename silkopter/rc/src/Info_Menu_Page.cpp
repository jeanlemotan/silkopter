#include "Info_Menu_Page.h"
#include "Adafruit_GFX.h"
#include "Input.h"
#include "ISticks.h"
#include "IStick_Actuators.h"
#include "IButton.h"
#include "IButton_Matrix.h"
#include "IBattery_Info.h"
#include "HAL.h"
#include "Remote_Viewer_Server.h"

#include "Menu_System.h"

#include "utils/Clock.h"

namespace silk
{
extern int s_version_major;
extern int s_version_minor;

///////////////////////////////////////////////////////////////////////////////////////////////////

Info_Menu_Page::Info_Menu_Page(HAL& hal)
    : m_hal(hal)
{
    m_menu.push_submenu({"<-", "Sticks", "Buttons", "Comms", "Battery", "About"}, 0, 34);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool Info_Menu_Page::process(Input& input, Menu_System& menu_system)
{
    auto selected_entry = m_menu.process(input);
    if (selected_entry != boost::none)
    {
        switch (*selected_entry)
        {
        case 0: return false;
        case 1: m_section = Section::STICKS; break;
        case 2: m_section = Section::BUTTONS; break;
        case 3: m_section = Section::COMMS; break;
        case 4: m_section = Section::BATTERY; break;
        case 5: m_section = Section::ABOUT; break;
        }
    }

    Comms& comms = m_hal.get_comms();

    if (m_section == Section::STICKS)
    {
        ISticks& sticks = input.get_sticks();
        m_yaw.value = sticks.get_yaw();
        m_pitch.value = sticks.get_pitch();
        m_roll.value = sticks.get_roll();
        m_throttle.value = sticks.get_throttle();

        if (!m_throttle_initialized)
        {
            m_throttle_initialized = true;
            m_throttle_mode_change_tp = Clock::now();
            sticks.set_throttle_deadband_position(ISticks::Deadband_Position::MIDDLE);
            input.get_stick_actuators().set_target_throttle(0.5f);
        }

        if (input.get_vertical_mode_switch().was_released())
        {
            m_throttle_mode_change_tp = Clock::now();

            if (sticks.get_throttle_deadband_position() == ISticks::Deadband_Position::MIDDLE)
            {
                sticks.set_throttle_deadband_position(ISticks::Deadband_Position::LOW);
                input.get_stick_actuators().set_target_throttle(0.f);
            }
            else if (sticks.get_throttle_deadband_position() == ISticks::Deadband_Position::LOW)
            {
                sticks.set_throttle_deadband_position(ISticks::Deadband_Position::MIDDLE);
                input.get_stick_actuators().set_target_throttle(0.5f);
            }
        }

        //deactivate the actuator
        if (Clock::now() - m_throttle_mode_change_tp > std::chrono::milliseconds(1000))
        {
            input.get_stick_actuators().set_target_throttle(boost::none);
        }
    }
    else if (m_section == Section::BUTTONS)
    {
        m_button_state.resize(input.get_button_matrix().get_row_count());
        for (size_t r = 0; r < m_button_state.size(); r++)
        {
            m_button_state[r].resize(input.get_button_matrix().get_column_count());
            for (size_t c = 0; c < m_button_state[r].size(); c++)
            {
                m_button_state[r][c] = input.get_button_matrix().get_button(r, c).is_pressed() ? 1 : 0;
            }
        }
    }
    else if (m_section == Section::COMMS)
    {
        Clock::time_point now = Clock::now();
        if (now - m_last_dbm_tp >= std::chrono::seconds(1))
        {
            m_last_dbm_tp = now;
            m_min_rx_dBm = comms.get_rx_dBm();
            m_min_tx_dBm = comms.get_tx_dBm();
        }
        m_min_rx_dBm = std::min<int>(m_min_rx_dBm, comms.get_rx_dBm());
        m_min_tx_dBm = std::min<int>(m_min_tx_dBm, comms.get_tx_dBm());
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Info_Menu_Page::draw_axis(Adafruit_GFX& display, int16_t y, const char* name, Axis_Data const& axis)
{
    int16_t w = display.width();

    int16_t axis_x = 8;
    int16_t axis_w = w - axis_x - 1;
    int16_t axis_h = 5;
    int16_t axis_y = y + axis_h/2 + 1;
    int16_t center_x = axis_x + axis_w / 2;
    int16_t value_x = axis_x + static_cast<int16_t>(axis.value * static_cast<float>(axis_w));

    display.setCursor(0, y);
    display.print(name);
    display.drawLine(axis_x, axis_y, axis_x + axis_w, axis_y, 1);

    display.drawLine(center_x, axis_y - 1, center_x, axis_y + 1, 1);

    display.drawLine(value_x, axis_y - 2, value_x, axis_y + 2, 1);
    y += 8;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Info_Menu_Page::render(Adafruit_GFX& display)
{
    if (m_section == Section::STICKS)
    {
        draw_axis(display, 0, "Y", m_yaw);
        draw_axis(display, 8, "P", m_pitch);
        draw_axis(display, 16, "R", m_roll);
        draw_axis(display, 24, "T", m_throttle);
    }
    else if (m_section == Section::BUTTONS)
    {
        int16_t btn_list_x = 106;
        int16_t btn_list_y = 2;

        int16_t btn_size = 8;
        for (size_t r = 0; r < m_button_state.size(); r++)
        {
            for (size_t c = 0; c < m_button_state[r].size(); c++)
            {
                int16_t btn_x = 10 + c*(btn_size - 1);
                int16_t btn_y = 2 + r*(btn_size - 1);
                if (m_button_state[r][c] == 1)
                {
                    display.fillRect(btn_x, btn_y, btn_size, btn_size, 1);
                    display.setCursor(btn_list_x, btn_list_y);
                    display.printf("%d%d", r, c);
                    btn_list_y += 8;
                }
                else
                {
                    display.drawRect(btn_x, btn_y, btn_size, btn_size, 1);
                }
            }
        }
    }
    else if (m_section == Section::COMMS)
    {
        display.setCursor(0, 0);
        display.setTextWrap(true);
        display.printf("i%d o%d v%d dBm\n", m_min_rx_dBm, m_min_tx_dBm, static_cast<int>(m_hal.get_comms().get_video_streamer().get_input_dBm()));
        display.printf("RC Lag: %dms\n", static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - m_hal.get_comms().get_last_rx_tp()).count()));
        //display.printf("%d viewer(s)\n", static_cast<int>(m_hal.get_comms().get_remote_viewer_server().get_remote_viewer_count()));
        display.printf("Viewer %s\n", m_hal.get_comms().get_remote_viewer_client().is_connected() ? "ON" : "OFF");
        display.printf("%.2fMbps video\n", static_cast<float>(m_hal.get_comms().get_video_streamer().get_video_rate() * 8) / (1024.f * 1024.f));
    }
    else if (m_section == Section::BATTERY)
    {
        display.setCursor(0, 0);
        display.setTextWrap(true);
        IBattery_Info const& bi = m_hal.get_battery_info();
        display.printf("Current: %.1fV\n", bi.get_voltage());
        display.printf("Low: %.1fV\n", bi.get_low_voltage());
        display.printf("Critical: %.1fV\n", bi.get_critical_voltage());
    }
    else if (m_section == Section::ABOUT)
    {
        display.setCursor(0, 0);
        display.setTextWrap(true);
        display.printf("Silkopter v%d.%d\nhttps://jeanleflambeur.wordpress.com/", s_version_major, s_version_minor);
    }

    m_menu.render(display, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

}
