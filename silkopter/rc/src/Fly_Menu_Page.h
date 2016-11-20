#pragma once

#include "IMenu_Page.h"

namespace silk
{

class Fly_Menu_Page : public IMenu_Page
{
public:
    bool process(Input& input, Menu_System& menu_system);
    void render(Adafruit_GFX& display);
};

}
