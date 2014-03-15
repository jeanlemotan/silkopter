#include <avr/io.h>
#include <avr/interrupt.h>
#include "qmath.h"
#include "HAL/boards/Crius_AIOP2_RC_Outputs.h"

using namespace hal;

namespace hal
{
	RC_Outputs rc_outputs;
}

RC_Outputs::RC_Outputs()
{
  // --------------------- TIMER1: CH_10, CH_11 ---------------
  hal.gpio->pinMode(11,GPIO_OUTPUT); // CH_10 (PB5/OC1A)
  hal.gpio->pinMode(12,GPIO_OUTPUT); // CH_11 (PB6/OC1B)
  // WGM: 1 1 1 0. Clear Timer on Compare, TOP is ICR1.
  // CS11: prescale by 8 => 0.5us tick
  TCCR1A = (1<<WGM11);
  TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS11);
  OCR1A = 0xFFFF;
  OCR1B = 0xFFFF;
  ICR1 = 40000; //50hz freq...Datasheet says  (system_freq/prescaler)/target frequency. So (16000000hz/8)/50hz=40000,

  
  // --------------------- TIMER3: CH_1, CH_3, and CH_4 ---------------------
  hal.gpio->pinMode(5,GPIO_OUTPUT); // CH_1 (PE3/OC3A)
  hal.gpio->pinMode(2,GPIO_OUTPUT); // CH_3 (PE4/OC3B)
  hal.gpio->pinMode(3,GPIO_OUTPUT); // CH_4 (PE5/OC3C)

  // WGM: 1 1 1 0. Clear timer on Compare, TOP is ICR3
  // CS31: prescale by 8 => 0.5us tick
  TCCR3A =((1<<WGM31));
  TCCR3B = (1<<WGM33)|(1<<WGM32)|(1<<CS31);
  OCR3A = 0xFFFF; // Init OCR registers to nil output signal
  OCR3B = 0xFFFF;
  OCR3C = 0xFFFF;
  ICR3 = 40000; // 0.5us tick => 50hz freq

  // --------------------- TIMER4: CH_7, CH_8, and CH_11 ---------------------
  hal.gpio->pinMode(6,GPIO_OUTPUT); // CH_2 (PE3/OC4A)
  hal.gpio->pinMode(7,GPIO_OUTPUT); // CH_7 (PE4/OC4B)
  hal.gpio->pinMode(8,GPIO_OUTPUT); // CH_8 (PE5/OC4C)

  // WGM: 1 1 1 0. Clear timer on Compare, TOP is ICR3
  // CS31: prescale by 8 => 0.5us tick
  TCCR4A =((1<<WGM41));
  TCCR4B = (1<<WGM43)|(1<<WGM42)|(1<<CS41);
  OCR4A = 0xFFFF; // Init OCR registers to nil output signal
  OCR4B = 0xFFFF;
  OCR4C = 0xFFFF;
  ICR4 = 40000; // 0.5us tick => 50hz freq	
}
