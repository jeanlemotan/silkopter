#pragma once

class Adafruit_GFX;

namespace silk
{

class Input;
class Menu_System;

class IMenu_Page
{
public:
    virtual ~IMenu_Page() = default;

    virtual bool process(Input& input, Menu_System& menu_system) = 0;
    virtual void render(Adafruit_GFX& display) = 0;
};

}
