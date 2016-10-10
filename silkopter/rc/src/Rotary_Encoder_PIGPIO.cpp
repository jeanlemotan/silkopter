#include "Rotary_Encoder_PIGPIO.h"

#include "utils/hw/pigpio.h"

namespace silk
{

Rotary_Encoder_PIGPIO::Rotary_Encoder_PIGPIO()
{

}
Rotary_Encoder_PIGPIO::~Rotary_Encoder_PIGPIO()
{

}

ts::Result<void> Rotary_Encoder_PIGPIO::init(uint8_t gpio1, uint8_t gpio2)
{
    m_gpio1 = gpio1;
    m_gpio2 = gpio2;

    if (gpioSetMode(m_gpio1, PI_INPUT) != 0)
    {
        return ts::Error("Cannot set as input GPIO " + std::to_string(m_gpio1));
    }
    if (gpioSetMode(m_gpio2, PI_INPUT) != 0)
    {
        return ts::Error("Cannot set as input GPIO " + std::to_string(m_gpio2));
    }
    gpioSetPullUpDown(m_gpio1, PI_PUD_UP);
    gpioSetPullUpDown(m_gpio2, PI_PUD_UP);
    gpioSetAlertFuncEx(m_gpio1, &Rotary_Encoder_PIGPIO::encoder_pulse, this);
    gpioSetAlertFuncEx(m_gpio2, &Rotary_Encoder_PIGPIO::encoder_pulse, this);

    return ts::success;
}

void Rotary_Encoder_PIGPIO::encoder_pulse(int gpio, int level, uint32_t tick, void* userdata)
{
   /*

             +---------+         +---------+      0
             |         |         |         |
   A         |         |         |         |
             |         |         |         |
   +---------+         +---------+         +----- 1

       +---------+         +---------+            0
       |         |         |         |
   B   |         |         |         |
       |         |         |         |
   ----+         +---------+         +---------+  1

   */

   Rotary_Encoder_PIGPIO* encoder = reinterpret_cast<Rotary_Encoder_PIGPIO*>(userdata);
   QASSERT(encoder);
   if (!encoder)
   {
       return;
   }

   if (gpio == encoder->m_gpio1)
   {
       encoder->m_lev_a = level;
   }
   else
   {
       encoder->m_lev_b = level;
   }

   if (gpio != encoder->m_last_gpio) /* debounce */
   {
      encoder->m_last_gpio = gpio;

      if (gpio == encoder->m_gpio1 && level == 0)
      {
         if (!encoder->m_lev_b)
         {
             ++encoder->m_clicks;
         }
      }
      else if (gpio == encoder->m_gpio2 && level == 1)
      {
         if (encoder->m_lev_a)
         {
             --encoder->m_clicks;
         }
      }
   }
}

int32_t Rotary_Encoder_PIGPIO::get_clicks() const
{
    return m_clicks;
}

void Rotary_Encoder_PIGPIO::process()
{

}


}
