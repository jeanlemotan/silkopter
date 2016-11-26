#include "Fly_Menu_Page.h"
#include "Adafruit_GFX.h"
#include "Input.h"
#include "IButton.h"

namespace silk
{

bool Fly_Menu_Page::process(Input& input, Menu_System& menu_system)
{
    if (input.get_menu_switch().was_released())
    {
        return false;
    }
    return true;
}

void Fly_Menu_Page::render(Adafruit_GFX& display)
{
    static int i = 0;
    display.setCursor(0, 0);
    display.print("  y   ");
    if (i & 1)
    {
        display.setCursor(0, 0);
        display.print("Flying");
    }
    i++;
}

}
