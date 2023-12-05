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

volatile uint32_t hour_offset = 3;
volatile uint32_t minute_offset = 59;

#define EPS 52


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
        uint16_t seconds = 0;
        uint16_t hours = 0;
        uint16_t minutes = 0;

        uint32_t current_time = get_round_time();
        uint32_t global_time = get_timer();
        uint32_t global_time_human = into_human_time(global_time);

        uint32_t elapsed_minutes = global_time_human/60000;
        uint32_t minute = (minute_offset+ elapsed_minutes) % 60;
        uint32_t hour = (hour_offset+ (minute_offset+elapsed_minutes)/60) % 12;
        
        uint32_t dt = get_round_dt();
        uint32_t nb_elapsed_seconds = (global_time_human/1000) % 60;
        uint32_t hour_comp = hour * dt / 12;
        uint32_t minute_comp = minute * dt / 60 ;


        // char buffer[32];
        // sprintf(buffer, "%lu\n",hour_comp);
        // uart_send_string(buffer);
        if(current_time <=  nb_elapsed_seconds * dt / 60)
        {
            //spi_transmit_array(0b0000000000000001);
            seconds = 0b0000000000000001;
        } 

        if(current_time >= hour_comp - EPS && current_time <= hour_comp +EPS)
        {
            hours = 0b1111111000000000;
        }

        if(current_time >= minute_comp - EPS && current_time <= minute_comp +EPS)
        {
            minutes = 0b1111111111111000;
        }

        spi_transmit_array((minutes | hours | seconds));

        uart_poll_received_cmds();
        uart_poll_transmit_cmds();
    }
}


