#pragma once

#include "IMenu_Page.h"
#include "Menu.h"
#include "utils/Clock.h"

namespace silk
{

class HAL;

class Battery_Calibration_Menu_Page : public IMenu_Page
{
public:
    Battery_Calibration_Menu_Page(HAL& hal);

    bool process(Input& input, Menu_System& menu_system) override;
    void render(Adafruit_GFX& display);

private:
    void refresh_menu();

    HAL& m_hal;
    Menu m_menu;
    boost::optional<size_t> m_selected_entry;
};

}
