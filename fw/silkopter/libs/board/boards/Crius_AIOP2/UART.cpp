#include "Config.h"

#if BOARD_TYPE == CRIUS_AIOP2

#include <avr/interrupt.h>
#include <qmath.h>
#include "debug/debug.h"
#include "board/boards/avr_UART.h"

using namespace board;

#define UART_COUNT 4

static volatile uint8_t* s_ubrrh[UART_COUNT] = { &UBRR0H, &UBRR1H, &UBRR2H, &UBRR3H };
static volatile uint8_t* s_ubrrl[UART_COUNT] = { &UBRR0L, &UBRR1L, &UBRR2L, &UBRR3L };
static volatile uint8_t* s_ucsra[UART_COUNT] = { &UCSR0A, &UCSR1A, &UCSR2A, &UCSR3A };
static volatile uint8_t* s_ucsrb[UART_COUNT] = { &UCSR0B, &UCSR1B, &UCSR2B, &UCSR3B };
static volatile uint8_t* s_ucsrc[UART_COUNT] = { &UCSR0C, &UCSR1C, &UCSR2C, &UCSR3C };

UART_ISR(0);
UART_ISR(1);
UART_ISR(2);
UART_ISR(3);

namespace board
{
	UART uart0(0, s_ubrrh[0], s_ubrrl[0], s_ucsra[0], s_ucsrb[0], s_ucsrc[0]);
	UART uart1(1, s_ubrrh[1], s_ubrrl[1], s_ucsra[1], s_ucsrb[1], s_ucsrc[1]);
	UART uart2(2, s_ubrrh[2], s_ubrrl[2], s_ucsra[2], s_ucsrb[2], s_ucsrc[2]);
	UART uart3(3, s_ubrrh[3], s_ubrrl[3], s_ucsra[3], s_ucsrb[3], s_ucsrc[3]);
}

#endif
