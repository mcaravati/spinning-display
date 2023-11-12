#include "uart_utils.h"

void uart_init()
{
    // Set baud rate
    UBRR0H  = (unsigned char)(MYUBRR >> 8);
    UBRR0L  = (unsigned char)MYUBRR;
    UCSR0A = (1 << U2X0);
    // Enable transmission
    UCSR0B = (1 << TXEN0);

    // Set one stop bit
    UCSR0C = (0 << USBS0);

    // Set format for message (8 bits)
    UCSR0C |= (0 << UCSZ02) | (1 << UCSZ01) | (1 << UCSZ00);

    // Set parity to disabled
    UCSR0C |= (0 << UPM01) | (0 << UPM00);
}

void uart_send_byte(uint8_t byte){
    // attente de la fin de transmission
    while (!(UCSR0A & (1 << UDRE0)));
    // chargement du registre, déclenche la transmission
    UDR0 = byte;
}

void uart_send_string(char *string){
    while (*string != '\0'){
        uart_send_byte(*string);
        string++;
    }
}