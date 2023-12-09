#include "magnetic_sensor.h"
#include "spi_utils.h"
#include "timer.h"
#include "uart_utils.h"
#include <stdio.h>

#define NUM_ROUND 10

volatile uint32_t last_date = 0;
volatile uint32_t dt = 0;
volatile uint32_t dt_buffer[NUM_ROUND] = {0};
volatile uint32_t buffer_index = 0;
ISR(INT0_vect) { // Interrupt request handler
    //encode dt in ascii and send it
    if(EICRA & (1 << ISC00))  // If we leave the magnet
    {
        
        EICRA &= ~(1 << ISC00);
    } else                  // If we meet the magnet
    {
        uint32_t tmp = get_timer();
        dt = dt - dt_buffer[buffer_index]/NUM_ROUND;
        dt_buffer[buffer_index] = tmp - last_date;
        dt = dt + dt_buffer[buffer_index]/NUM_ROUND;

        buffer_index = (buffer_index+1) % NUM_ROUND;
        dt = tmp-last_date;
        last_date = tmp;

        EICRA |= (1 << ISC00); // Enable PD2 high level interrupt
    }

    frame_buffer_reset();
}

uint32_t get_round_dt()
{
    return dt;
}

uint32_t get_round_time()
{
    return get_timer() - last_date;
}

void magnetic_sensor_init() {
    DDRD &= ~(1 << PD2); // PD2 as input
        
    // EICRA &= ~(1 << ISC00);  
    EICRA |= (1 << ISC01) | (1 << ISC00); // Enable PD2 high level interrupt
    EIMSK |= (1 << INT0); // Enable interrupt request
    dt = 0;
}