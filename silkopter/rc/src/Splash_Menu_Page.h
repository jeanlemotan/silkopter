#pragma once

#include "IMenu_Page.h"
#include "utils/Clock.h"

namespace silk
{

class Splash_Menu_Page : public IMenu_Page
{
public:
    Splash_Menu_Page(std::unique_ptr<IMenu_Page> mm);

    bool process(Input& input, Menu_System& menu_system) override;
    void render(Adafruit_GFX& display) override;

private:
    std::unique_ptr<IMenu_Page> m_mm;

    Clock::time_point m_start_tp = Clock::now();
};

}
