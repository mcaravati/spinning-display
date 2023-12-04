// led.c
#include <avr/io.h>
#include <util/delay.h>

#include "uart_utils.h"
#include "spi_utils.h"
#include "magnetic_sensor.h"
#include "timer.h"
#include "maths_utils.h"
#include <stdio.h>

#define A_AMOUNT 18

int main(void)
{
    DDRD |= (1 << PD6); // turning microship led on
    PORTD |= (1 << PD6);

    uart_init();
    magnetic_sensor_init();
    spi_init();
    timer_init();
    sei();
    uart_send_string("bonjour\n\n");

    //uint16_t message = 0;
    while (1)
    {
        uint32_t current_time =get_timer();
        uint32_t dt = get_round_dt();

        if(current_time == dt || current_time == dt /4 || current_time == dt /2  || current_time == 3*dt /4)
        {
            spi_transmit_array(0b1111111111111111);
        } else
        {
            spi_transmit_array(0b0000000000000000);
        }

        uart_poll_received_cmds();
        uart_poll_transmit_cmds();
    }
    uart_poll_transmit_cmds();

}



// // char buffer2 [32];
// // sprintf(buffer2, "dt = %lu\n", dt);
// // uart_send_string(buffer2);