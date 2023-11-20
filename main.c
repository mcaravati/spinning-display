// led.c
#include <avr/io.h>
#include <util/delay.h>

#include "uart_utils.h"
#include "spi_utils.h"
#include "magnetic_sensor.h"

int main(void)
{
    DDRD |= (1 << PD6); // turning microship led on
    PORTD |= (1 << PD6);

    uart_init();

    magnetic_sensor_init();

    spi_init();
    //uint16_t message = 0;
    while (1)
    {
        uart_poll_received_cmds();
        uart_poll_transmit_cmds();
    }
}


