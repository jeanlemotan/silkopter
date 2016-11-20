#pragma once

#include "IMenu_Page.h"
#include "Menu.h"

namespace silk
{

class Main_Menu_Page : public IMenu_Page
{
public:
    Main_Menu_Page();

    bool process(Input& input, Menu_System& menu_system) override;
    void render(Adafruit_GFX& display) override;

private:
    Menu m_menu;

    enum class Submenu
    {
        MAIN_MENU,
        CONFIG,
        CALIBRATION,
    };

    void set_submenu(Submenu submenu);

    Submenu m_submenu = Submenu::MAIN_MENU;
};

}
