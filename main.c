// led.c
#include <avr/io.h>
#include <util/delay.h>

#include "uart_utils.h"
#include "spi_utils.h"
#include "magnetic_sensor.h"
#include "timer.h"
#include "frame_fifo.h"
#include <stdio.h>
#include <stdlib.h>

#define ATMEGA_60S (into_atmega_time(60000))

// volatile uint8_t hour_offset = 17;
// volatile uint8_t minute_offset = 47;

#define EPS 50

#define IS_AT(T,G) ((T >= G-EPS) && (T <= G+EPS))
// void display_clock()
// {
//     uint16_t seconds = 0;
//     uint16_t hours = 0;
//     uint16_t minutes = 0;

//     uint32_t current_time = get_round_time();
//     uint32_t global_time = get_timer();
//     uint32_t global_time_human = into_human_time(global_time);

//     uint32_t elapsed_minutes = global_time_human/60000;
//     uint32_t minute = (minute_offset+ elapsed_minutes) % 60;
//     uint32_t hour = (hour_offset+ (minute_offset+elapsed_minutes)/60) % 12;
    
//     uint16_t dt = get_round_dt();
//     uint32_t nb_elapsed_seconds = (global_time_human/1000) % 60;
//     uint32_t second_comp = dt - nb_elapsed_seconds * dt / 60;
//     uint16_t hour_comp = EPS + dt - hour * dt / 12;
//     uint16_t minute_comp = EPS + dt - minute * dt / 60 ;


//     // char buffer[32];
//     // sprintf(buffer, "%lu\n",hour_comp);
//     // uart_send_string(buffer);
//     if(current_time >= second_comp)
//     {
//         seconds = 0b0000000000000001;
//     } 

//     if(IS_AT(current_time, hour_comp))
//     // if(current_time >= hour_comp - EPS && current_time <= hour_comp +EPS)
//     {
//         hours = 0b1111111000000000;
//     }

//     if(IS_AT(current_time,minute_comp))
//     // if(current_time >= minute_comp - EPS && current_time <= minute_comp +EPS)
//     {
//         minutes = 0b1111111111111000;
//     }

//     spi_transmit_array((minutes | hours | seconds));
// }


int main(void)
{
    DDRD |= (1 << PD6); // turning microship led on
    PORTD |= (1 << PD6);

    uart_init();
    magnetic_sensor_init();
    spi_init();
    timer_init();
    sei();

    frame_fifo_init();

    uart_send_string("bonjour\n");
    uart_poll_transmit_cmds();

    // struct frame* f = frame_fifo_get();
    // spi_transmit_array(0b0000000);

    struct frame * f=NULL;
    while (1)
    {
        f = frame_fifo_get();
        if(f){
            uint32_t current_date = get_round_time();

            uint8_t amount = frame_fifo_get_amount();
            uint16_t lit = 0;
            (void)lit;
            for(uint8_t i = 0; i < amount; ++i)
            {
                if(IS_AT(current_date, f[i].date))
                {
                    lit = f[i].payload;
                    break;
                }
            }
            spi_transmit_array(lit);
        }
        // display_clock();
        uart_poll_received_cmds();
        uart_poll_transmit_cmds();
    }
}


