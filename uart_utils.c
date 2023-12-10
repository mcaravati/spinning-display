#include "uart_utils.h"
#include "ring_buffer.h"
#include "spi_utils.h"
#include <string.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "frame_fifo.h"


struct ring_buffer receive_buffer;
struct ring_buffer transmit_buffer;
volatile int16_t received_cmd_count = 0;

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

    ring_buffer_init(&receive_buffer);    
    ring_buffer_init(&transmit_buffer);    
}

void uart_send_byte(uint8_t data)
{
    //stock in buffer
    ring_buffer_put(&transmit_buffer, data);
}

void uart_send_string(const char *str)
{
    if(strlen(str) >= (uint8_t)(RING_BUFFER_SIZE - ring_buffer_available_bytes(&transmit_buffer))) return;
    int c = 0;
    while(str[c] != '\0')
    {
        uart_send_byte(str[c++]);
    }
}

void uart_handle_command(const char* command){

    // Si command = help, on affiche les commandes disponibles
    // uart_send_string(command);
    if (strcmp(command, "help") == 0)
    {
        uart_send_string("Available commands:\n");
        uart_send_string("help: display this message\n");
    }

    if(strncmp(command, "img", 3) == 0)
    {
        uint16_t date = command[3] | (command[4] << 8);
        uint16_t payload = command[5] | (command[6] << 8);
        frame_fifo_put((struct frame){.date=date, .payload= payload});
    }
    if(strcmp(command, "new")==0) frame_fifo_reset();
    if(strcmp(command, "sz")==0)
    {
        char buf[8];
        sprintf(buf,"%u\n", frame_fifo_get_amount());
        uart_send_string(buf);
    }
}

void uart_poll_received_cmds()
{
    while(received_cmd_count)
    {
        char cmd [RING_BUFFER_SIZE+1] = {0};
        uart_get_command(cmd);
        uart_handle_command(cmd);
        --received_cmd_count;
    }
}

void uart_poll_transmit_cmds()
{
    while(ring_buffer_available_bytes(&transmit_buffer))
    {
        // chargement du registre, déclenche la transmission
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = ring_buffer_get(&transmit_buffer);
    }
}

void uart_get_command(char * cmd)
{
    int16_t count = 0;
    char c;
    while(ring_buffer_available_bytes(&receive_buffer) && (c = ring_buffer_get(&receive_buffer)) != '\n')
    {
        cmd[count++] = c;
    }
    cmd[count-1] = '\0';
}

ISR(USART_RX_vect)
{
    // Lire le byte reçu et le stocker
    int8_t received_data = UDR0;

    if (received_data == '\n')
    {
        received_cmd_count += 1;
        // We reach here
    }

    ring_buffer_put(&receive_buffer, received_data);
}