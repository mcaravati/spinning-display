// led.c
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "uart_utils.h"
#include "spi_utils.h"
#include "ring_buffer.h"
#include "magnetic_sensor.h"

int8_t received_data;
struct ring_buffer ring_buffer;

ISR(USART_RX_vect)
{
    // Lire le byte reçu et le stocker
    received_data = UDR0;

    // tant que '\n' n'est pas reçu, on continue de lire
    if (received_data != '\n')
    {
        ring_buffer_put(&ring_buffer, received_data);

    }
    else
    {
        // on reconstitue le mot dans le buffer
        char word[ring_buffer_available_bytes(&ring_buffer)];
        uint8_t i = 0;
        while (ring_buffer_available_bytes(&ring_buffer))
        {
            word[i++] = ring_buffer_get(&ring_buffer);
        }
        word[i] = '\0';
        // Si word = help, on affiche les commandes disponibles
        uart_handle_command(word);
        uart_send_string(word);
    }
    

}

int main(void)
{
    ring_buffer_init(&ring_buffer);

    DDRD |= (1 << PD6); // turning microship led on
    PORTD |= (1 << PD6);

    uart_init();
    sei();

    magnetic_sensor_init();

    spi_init();
    //uint16_t message = 0;
    while (1)
    {
        uart_send_string("Hello world 2\n");
        _delay_ms(1000);

        // spi_transmit_array(1 << message++);
        // _delay_ms(50);
        // message %= 16;
    }
}


