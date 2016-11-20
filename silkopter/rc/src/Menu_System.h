#pragma once

#include <vector>
#include <memory>
#include "IMenu_Page.h"

class Adafruit_GFX;

namespace silk
{

class Menu_System
{
public:
    void push_page(std::unique_ptr<IMenu_Page> page);

    void process(Input& input);
    void render(Adafruit_GFX& display);

private:
    std::vector<std::unique_ptr<IMenu_Page>> m_pages;
};

}
