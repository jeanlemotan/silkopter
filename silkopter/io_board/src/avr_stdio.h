#pragma once

#include <stdio.h>

int uart_putchar(char c, FILE *stream);
char uart_getchar(FILE *stream);

