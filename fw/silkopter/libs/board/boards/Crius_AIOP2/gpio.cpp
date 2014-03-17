#include "Config.h"

#if BOARD_TYPE == CRIUS_AIOP2

#include <avr/io.h>
#include <avr/interrupt.h>
#include "debug/debug.h"
#include "board/boards/Crius_AIOP2/gpio.h"
#include "board/boards/pins_arduino_mega.h"

namespace board
{
namespace gpio
{

typedef void (*ISR_Func_Ptr)();
static volatile ISR_Func_Ptr s_isr_function = nullptr;
static bool s_is_initialized = false;

SIGNAL(INT6_vect) 
{
    if (s_isr_function) 
	{
        s_isr_function();
    }
}   

// Get the bit location within the hardware port of the given virtual pin.
// This comes from the pins_*.c file for the active board configuration.
// 
// These perform slightly better as macros compared to inline functions
//
#define digital_pin_to_port(P) ( pgm_read_byte( p_digital_pin_to_port + (P) ) )
#define digital_pin_to_bit_mask(P) ( pgm_read_byte( p_digital_pin_to_bit_mask + (P) ) )
#define digital_pin_to_timer(P) ( pgm_read_byte( p_digital_pin_to_timer + (P) ) )
#define port_output_register(P) ( (volatile uint8_t *)( pgm_read_word( p_port_to_output + (P))) )
#define port_input_register(P) ( (volatile uint8_t *)( pgm_read_word( p_port_to_input + (P))) )
#define port_mode_register(P) ( (volatile uint8_t *)( pgm_read_word( p_port_to_mode + (P))) )

void init()
{
	if (s_is_initialized)
	{
		return;
	}	
	s_is_initialized = true;

}

void set_pin_mode(uint8_t pin, Mode mode) 
{
	ASSERT(s_is_initialized);

    uint8_t port = digital_pin_to_port(pin);
    if (port == NOT_A_PIN) 
	{
		return;
	}

    uint8_t bit = digital_pin_to_bit_mask(pin);

    // JWS: can I let the optimizer do this?
    volatile uint8_t* reg = port_mode_register(port);

    if (mode == Mode::INPUT) 
	{
        uint8_t oldSREG = SREG;
		cli();
        *reg &= ~bit;
        SREG = oldSREG;
    } 
	else 
	{
        uint8_t oldSREG = SREG;
		cli();
        *reg |= bit;
        SREG = oldSREG;
    }
}

bool read(uint8_t pin) 
{
	ASSERT(s_is_initialized);

    uint8_t port = digital_pin_to_port(pin);
    if (port == NOT_A_PIN) 
	{
		return 0;
	}

    uint8_t bit = digital_pin_to_bit_mask(pin);
    return (*port_input_register(port) & bit) ? true : false;
}

void write(uint8_t pin, bool value) 
{
	ASSERT(s_is_initialized);

    uint8_t port = digital_pin_to_port(pin);
    if (port == NOT_A_PIN) 
	{
		return;
	}

    uint8_t bit = digital_pin_to_bit_mask(pin);
    volatile uint8_t* out = port_output_register(port);

    uint8_t oldSREG = SREG;
    cli();

    if (value) 
	{
        *out &= ~bit;
    } 
	else 
	{
        *out |= bit;
    }

    SREG = oldSREG;
}

void toggle(uint8_t pin) 
{
	ASSERT(s_is_initialized);

    uint8_t port = digital_pin_to_port(pin);
    if (port == NOT_A_PIN) 
	{
		return;
	}

    uint8_t bit = digital_pin_to_bit_mask(pin);
    volatile uint8_t* out = port_output_register(port);

    uint8_t oldSREG = SREG;
    cli();

    *out ^= bit;

    SREG = oldSREG;
}

// /* Implement GPIO Interrupt 6, used for MPU6000 data ready on APM2. */
// bool AVRGPIO::attach_interrupt(
//         uint8_t interrupt_num, AP_HAL::Proc proc, uint8_t mode) {
//     /* Mode is to set the ISCn0 and ISCn1 bits.
//      * These correspond to the GPIO_INTERRUPT_ defs in AP_HAL.h */
//     if (!((mode == 0)||(mode == 1)||(mode == 2)||(mode==3))) return false;
//     if (interrupt_num == 6) {
// 	uint8_t oldSREG = SREG;
// 	cli();	
//         s_isr_function = proc;
//         /* Set the ISC60 and ICS61 bits in EICRB according to the value
//          * of mode. */
//         EICRB = (EICRB & ~((1 << ISC60) | (1 << ISC61))) | (mode << ISC60);
//         EIMSK |= (1 << INT6);
// 	SREG = oldSREG;
//         return true;
//     } else {
//         return false;
//     }
// }


// AP_HAL::DigitalSource* AVRGPIO::channel(uint16_t pin) {
//     uint8_t bit = digital_pin_to_bit_mask(pin);
//     uint8_t port = digital_pin_to_port(pin);
//     if (port == NOT_A_PIN) return NULL;
//     return new AVRDigitalSource(bit, port);
// }
// 
// void AVRDigitalSource::mode(uint8_t output) {
//     const uint8_t bit = _bit;
//     const uint8_t port = _port;
// 
//     volatile uint8_t* reg;
//     reg = port_mode_register(port);
// 
//     if (output == GPIO_INPUT) {
//         uint8_t oldSREG = SREG;
//                 cli();
//         *reg &= ~bit;
//         SREG = oldSREG;
//     } else {
//         uint8_t oldSREG = SREG;
//                 cli();
//         *reg |= bit;
//         SREG = oldSREG;
//     }
// }
// 
// uint8_t AVRDigitalSource::read() {
//     const uint8_t bit = _bit;
//     const uint8_t port = _port;
//     if (*port_input_register(port) & bit) return 1;
//     return 0;
// }
// 
// void AVRDigitalSource::write(uint8_t value) {
//     const uint8_t bit = _bit;
//     const uint8_t port = _port;
//     volatile uint8_t* out;
//     out = port_output_register(port);
// 
//     uint8_t oldSREG = SREG;
//     cli();
// 
//     if (value == 0) {
//         *out &= ~bit;
//     } else {
//         *out |= bit;
//     }
// 
//     SREG = oldSREG;
// }
// 
// void AVRDigitalSource::toggle() {
//     const uint8_t bit = _bit;
//     const uint8_t port = _port;
//     volatile uint8_t* out;
//     out = port_output_register(port);
// 
//     uint8_t oldSREG = SREG;
//     cli();
// 
//     *out ^= bit;
// 
//     SREG = oldSREG;
// }
// 
// /*
//   return true when USB is connected
//  */
// bool AVRGPIO::usb_connected(void)
// {
// #if HAL_GPIO_USB_MUX_PIN != -1
//     pinMode(HAL_GPIO_USB_MUX_PIN, GPIO_INPUT);
//     return !read(HAL_GPIO_USB_MUX_PIN);
// #else
//     return false;
// #endif
// }

}
}

#endif

