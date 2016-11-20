#pragma once

#include "IMenu_Page.h"

namespace silk
{

class Splash_Menu_Page : public IMenu_Page
{
public:

    Splash_Menu_Page();

    bool process(Input& input, Menu_System& menu_system) override;
    void render(Adafruit_GFX& display) override;

private:
//    struct Input_Data
//    {
//        float yaw = 0;
//        float pitch = 0;
//        float roll = 0;
//        float throttle = 0;

//        int32_t encoder1_clicks = 0;
//        int32_t encoder2_clicks = 0;
//    } m_input_data;
    q::Clock::time_point m_start_tp = q::Clock::now();
};

}
