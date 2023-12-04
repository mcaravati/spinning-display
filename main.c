// led.c
#include <avr/io.h>
#include <util/delay.h>

#include "uart_utils.h"
#include "spi_utils.h"
#include "magnetic_sensor.h"
#include "timer.h"
#include <stdio.h>

#define pi 31415//926535
#define pi_dec 10000
#define ATMEGA_60S (into_atmega_time(60000))

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
        uint32_t current_time = get_round_time();
        uint32_t global_time = get_timer();
        uint32_t dt = get_round_dt();
        uint32_t nb_elapsed_seconds = (into_human_time(global_time)/1000) % 60;


        // char buffer[32];
        // sprintf(buffer, "%lu\n",nb_elapsed_seconds);
        // uart_send_string(buffer);
        if(current_time <=  nb_elapsed_seconds * dt / 60)
        {
            spi_transmit_array(0b0000000000000001);
        } else
        {
            spi_transmit_array(0b0000000000000000);
        }

        // if(current_time == dt || current_time == dt /4 || current_time == dt /2  || current_time == 3*dt /4)
        // {

        uart_poll_received_cmds();
        uart_poll_transmit_cmds();
    }
}


