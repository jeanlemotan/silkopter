#include "Main_Menu_Page.h"
#include "Adafruit_GFX.h"
#include "Input.h"
#include "IRotary_Encoder.h"
#include "IButton.h"

#include "Menu_System.h"
#include "Fly_Menu_Page.h"
#include "Info_Menu_Page.h"
#include "Stick_Calibration_Menu_Page.h"

namespace silk
{

extern int s_version_major;
extern int s_version_minor;


Main_Menu_Page::Main_Menu_Page()
{
    set_submenu(Submenu::MAIN_MENU);
}

void Main_Menu_Page::set_submenu(Submenu submenu)
{
    if (submenu == Submenu::MAIN_MENU)
    {
        m_menu.push_submenu(
                    {"Fly", "Info", "Configuration"},
                    0, 12);
    }
    else if (submenu == Submenu::CONFIG)
    {
        m_menu.push_submenu(
                    {"<-",
                     "Stick Calibration",
                     "Encoder Calibration",
                     "Feedback Calibration",
                     "Vibration Calibration",
                     "Battery Calibration"},
                    0, 12);
    }

    m_submenu = submenu;
}

bool Main_Menu_Page::process(Input& input, Menu_System& menu_system)
{
    auto selected_entry = m_menu.process(input);
    if (selected_entry != boost::none)
    {
        if (m_submenu == Submenu::MAIN_MENU)
        {
            switch (*selected_entry)
            {
            case 0: menu_system.push_page(std::unique_ptr<IMenu_Page>(new Fly_Menu_Page)); break;
            case 1: menu_system.push_page(std::unique_ptr<IMenu_Page>(new Info_Menu_Page)); break;
            case 2: set_submenu(Submenu::CONFIG);
            }
        }
        else if (m_submenu == Submenu::CONFIG)
        {
            switch (*selected_entry)
            {
            case 0: m_menu.pop_submenu(); m_submenu = Submenu::MAIN_MENU; break;
            case 1: menu_system.push_page(std::unique_ptr<IMenu_Page>(new Stick_Calibration_Menu_Page)); break;
//            case 2: menu_system.push_page(std::unique_ptr<IMenu_Page>(new Encoder_Calibration_Menu_Page)); break;
//            case 3: menu_system.push_page(std::unique_ptr<IMenu_Page>(new Feedback_Calibration_Menu_Page)); break;
//            case 4: menu_system.push_page(std::unique_ptr<IMenu_Page>(new Vibration_Calibration_Menu_Page)); break;
//            case 5: menu_system.push_page(std::unique_ptr<IMenu_Page>(new Battery_Calibration_Menu_Page)); break;
            }
        }
    }

    return true;
}

void Main_Menu_Page::render(Adafruit_GFX& display)
{
    display.setCursor(0, 0);
    if (m_submenu == Submenu::MAIN_MENU)
    {
        display.printf("Silkopter v%d.%d", s_version_major, s_version_minor);
    }
    else if (m_submenu == Submenu::CONFIG)
    {
        display.printf("Config");
    }

    m_menu.render(display, 0);
}

}
