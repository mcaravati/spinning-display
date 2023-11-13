#include "uart_utils.h"

void uart_init()
{
    // Set baud rate
    UBRR0H = (unsigned char)(MYUBRR >> 8);
    UBRR0L = (unsigned char)MYUBRR;
    UCSR0A = (1 << U2X0);
    // Enable transmission and reception and reception interrupt
    UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);

    // Set one stop bit
    UCSR0C = (0 << USBS0);

    // Set format for message (8 bits)
    UCSR0C |= (0 << UCSZ02) | (1 << UCSZ01) | (1 << UCSZ00);

    // Set parity to disabled
    UCSR0C |= (0 << UPM01) | (0 << UPM00);
}

void uart_send_byte(uint8_t byte) {
    // attente de la fin de transmission
    while (!(UCSR0A & (1 << UDRE0)));
    // chargement du registre, déclenche la transmission
    UDR0 = byte;
}

void uart_send_string(char* string) {
    while (*string != '\0') {
        uart_send_byte(*string);
        string++;
    }
}

void uart_handle_command(char* command){
        // Si command = help, on affiche les commandes disponibles
        if (command[0] == 'h' && command[1] == 'e' && command[2] == 'l' && command[3] == 'p')
        {
            uart_send_string("Available commands:\n");
            uart_send_string("help: display this message\n");
            uart_send_string("led on: turn on the microchip led\n");
            uart_send_string("led off: turn off the microchip led\n");
        }
}