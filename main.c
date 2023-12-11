// led.c
#include <avr/io.h>
#include <util/delay.h>

#include "uart_utils.h"
#include "spi_utils.h"
#include "magnetic_sensor.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define pi 31415//926535
#define pi_dec 10000
#define ATMEGA_60S (into_atmega_time(60000))

volatile uint32_t hour_offset = 0;
volatile uint32_t minute_offset = 0;


int main(void)
{
    DDRD |= (1 << PD6); // turning microship led on
    PORTD |= (1 << PD6);

    uart_init();
    magnetic_sensor_init();
    spi_init();
    timer_init();
    sei();
    // uart_send_string("bonjour\n");
    // uart_poll_transmit_cmds();

    // uint16_t goal_date = 0;
    // uint16_t goal_payload = 0;
    // uint8_t goal_reached = 0;
    // char buffer[50] = {0};


    // struct frame* f = 0;
    // while (!(f = frame_buffer_get()));

    // goal_date = f->date;
    // goal_payload = f->payload;

    // sprintf(buffer, "Initial date: %u\n", goal_date);
    // uart_send_string(buffer);

    while (1)
    {
        uint16_t seconds = 0;
        uint32_t current_time = get_round_time();

        uint32_t global_time = get_timer();
        uint32_t global_time_human = into_human_time(global_time);

        uint32_t elapsed_minutes = global_time_human/60000;
        uint32_t minute = (minute_offset+ elapsed_minutes) % 60;
        uint32_t hour = (hour_offset+ (minute_offset+elapsed_minutes)/60) % 12;

        uint32_t dt = get_round_dt();
        uint32_t nb_elapsed_seconds = (global_time_human/1000) % 60;
        uint32_t second_comp = dt - nb_elapsed_seconds * dt / 60;

        if(current_time >= second_comp)
        {
            seconds = 0b0000000000000001;
        } 

        spi_transmit_array(payload_from_time(current_time, (uint8_t) hour / 10, (uint8_t) hour % 10, (uint8_t) minute / 10, (uint8_t) minute % 10) | seconds);

        // uart_poll_received_cmds();
        // uart_poll_transmit_cmds();
    }
}


