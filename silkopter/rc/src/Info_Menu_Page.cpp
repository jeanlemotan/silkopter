#include "Info_Menu_Page.h"
#include "Adafruit_GFX.h"
#include "Input.h"
#include "ISticks.h"
#include "Comms.h"
#include "Remote_Viewer_Server.h"

#include "Menu_System.h"

#include "utils/Clock.h"

namespace silk
{
extern int s_version_major;
extern int s_version_minor;

///////////////////////////////////////////////////////////////////////////////////////////////////

Info_Menu_Page::Info_Menu_Page(Comms& comms)
    : m_comms(comms)
{
    m_menu.push_submenu({"<-", "Sticks", "Comms", "Battery", "About"}, 0, 34);
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
        case 2: m_section = Section::COMMS; break;
        case 3: m_section = Section::BATTERY; break;
        case 4: m_section = Section::ABOUT; break;
        }
    }

    m_yaw.value = input.get_sticks().get_yaw();
    m_pitch.value = input.get_sticks().get_pitch();
    m_roll.value = input.get_sticks().get_roll();
    m_throttle.value = input.get_sticks().get_throttle();

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
    else if (m_section == Section::COMMS)
    {
        display.setCursor(0, 0);
        display.setTextWrap(true);
        display.printf("%d viewer(s)", static_cast<int>(m_comms.get_remote_viewer_server().get_remote_viewer_count()));
        display.printf("\n%ddBm RX", static_cast<int>(m_comms.get_rx_dBm()));
        display.printf("\n%ddBm TX", static_cast<int>(m_comms.get_rx_dBm()));
        display.printf("\nRX Lag: %dms", static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - m_comms.get_last_rx_tp()).count()));
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
