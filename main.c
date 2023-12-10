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

volatile uint32_t hour_offset = 11;
volatile uint32_t minute_offset = 1;

#define EPS 150


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
        uint32_t current_time = get_round_time();
        spi_transmit_array(payload_from_time(current_time));

        // uart_poll_received_cmds();
        // uart_poll_transmit_cmds();
    }
}


