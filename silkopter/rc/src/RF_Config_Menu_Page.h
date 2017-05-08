#pragma once

#include "IMenu_Page.h"
#include "Menu.h"
#include "utils/Clock.h"

namespace silk
{

class HAL;

class RF_Config_Menu_Page : public IMenu_Page
{
public:
    RF_Config_Menu_Page(HAL& hal);

    bool process(Input& input, Menu_System& menu_system) override;
    void render(Adafruit_GFX& display);

private:
    void refresh_menu();

    HAL& m_hal;
    Menu m_menu;
    boost::optional<size_t> m_selected_entry;

    float m_initial_center_frequency = 0.f;
    float m_initial_xtal_adjustment = 0.f;

    float m_center_frequency = 0.f;
    float m_xtal_adjustment = 0.f;

    Clock::time_point m_last_dbm_tp = Clock::now();
    int m_min_rx_dBm = 999;
    int m_min_tx_dBm = 999;
};

}
