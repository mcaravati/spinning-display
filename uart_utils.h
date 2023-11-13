#ifndef _UART_UTILS_H_
#define _UART_UTILS_H_

#include <avr/io.h>

// define baudrate
#define FOSC 13000000// Clock Speed
#define MYUBRR ((FOSC/(8*BAUD))-1)


void uart_init();
void uart_send_byte(uint8_t byte);
void uart_send_string(char* string);

#endif // !_UART_UTILS_H_