#include "avr_stdio.h"
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

int uart_putchar(char c, FILE* stream)
{
    if (c == '\n')
        uart_putchar('\r', stream);
    loop_until_bit_is_set(UCSR1A, UDRE1);
    UDR1 = c;
    return 0;
}

char uart_getchar(FILE* stream)
{
    return 0;
}
