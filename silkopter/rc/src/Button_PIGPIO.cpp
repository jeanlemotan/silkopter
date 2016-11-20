#include "Button_PIGPIO.h"

#include "utils/hw/pigpio.h"

namespace silk
{

Button_PIGPIO::Button_PIGPIO()
{

}
Button_PIGPIO::~Button_PIGPIO()
{

}

ts::Result<void> Button_PIGPIO::init(uint8_t gpio)
{
    m_gpio = gpio;

    if (gpioSetMode(m_gpio, PI_INPUT) != 0)
    {
        return ts::Error("Cannot set as input GPIO " + std::to_string(m_gpio));
    }
    gpioSetPullUpDown(m_gpio, PI_PUD_UP);
    gpioSetAlertFuncEx(m_gpio, &Button_PIGPIO::switch_pulse, this);

    return ts::success;
}

void Button_PIGPIO::switch_pulse(int gpio, int level, uint32_t tick, void* userdata)
{
   Button_PIGPIO* sw = reinterpret_cast<Button_PIGPIO*>(userdata);
   QASSERT(sw);
   if (!sw)
   {
       return;
   }

   if (gpio != sw->m_gpio)
   {
       return;
   }

   if (sw->m_is_pressed && level == 0)
   {
       sw->m_release_count++;
   }
   if (!sw->m_is_pressed && level != 0)
   {
       sw->m_press_count++;
   }

   sw->m_is_pressed = level != 0;
}

bool Button_PIGPIO::is_pressed() const
{
    return m_is_pressed;
}
bool Button_PIGPIO::was_pressed() const
{
    return m_was_pressed;
}
bool Button_PIGPIO::was_released() const
{
    return m_was_released;
}
uint32_t Button_PIGPIO::get_press_count() const
{
    return m_press_count;
}
uint32_t Button_PIGPIO::get_release_count() const
{
    return m_release_count;
}

void Button_PIGPIO::process()
{
    m_was_pressed = get_press_count() > m_last_press_count;
    m_last_press_count = get_press_count();

    m_was_released = get_release_count() > m_last_release_count;
    m_last_release_count = get_release_count();
}


}
