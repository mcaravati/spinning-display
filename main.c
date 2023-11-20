// led.c
#include <avr/io.h>
#include <util/delay.h>

#include "uart_utils.h"
#include "spi_utils.h"
#include "magnetic_sensor.h"
#include "timer.h"
#include <stdio.h>

int main(void)
{
    DDRD |= (1 << PD6); // turning microship led on
    PORTD |= (1 << PD6);

    uart_init();
    magnetic_sensor_init();
    spi_init();
    timer_init();
    sei();
    uart_send_string("bonjour\n");

    //uint16_t message = 0;
    while (1)
    {
        uint32_t current_time =get_timer();
        if(current_time >= .99* dt/4 && current_time <= 1.01* dt/4)
        {
            spi_transmit_array(0b1111111111111111);
        } else
        {
            spi_transmit_array(0b0000000000000000);
        }

        uart_poll_received_cmds();
        uart_poll_transmit_cmds();
    }
}


