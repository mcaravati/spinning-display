// led.c
#include <avr/io.h>
#include <util/delay.h>

#include "uart_utils.h"
#include "spi_utils.h"

int main(void)
{

    DDRD |= (1 << PD6); // turning microship led on
    PORTD |= (1 << PD6); 

    // uart_init(BAUD);

    spi_init();
    PORTC &= ~(1 << LE); // LE // FIXME : not really a fix but maybe useless??
    spi_transmit(0b00001000);
    
    // spi_transmit(0x01);
    // spi_transmit(0xe);
    /* while (1)
    {
        uart_send_byte('H');
        uart_send_string("ello world\n");
        _delay_ms(1000);
    } */
}


