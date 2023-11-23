#include "magnetic_sensor.h"
#include "spi_utils.h"
#include "timer.h"
#include "uart_utils.h"
#include <stdio.h>

volatile uint32_t dt = 0;
// static uint8_t detected_magnet = 0;
ISR(INT0_vect) { // Interrupt request handler
    //encode dt in ascii and send it
    if(EICRA & (1 << ISC00))  // Rising edge already met
    {
        uint32_t tmp = get_timer();
        dt += tmp/2;
        EICRA &= ~(1 << ISC00);
    } else
    {
        dt = get_timer();
        timer_reset();
        EICRA |= (1 << ISC00); // Enable PD2 high level interrupt
    }

}

uint32_t get_round_dt()
{
    return dt;
}

void magnetic_sensor_init() {
    DDRD &= ~(1 << PD2); // PD2 as input
        
    // EICRA &= ~(1 << ISC00);  
    EICRA |= (1 << ISC01) | (1 << ISC00); // Enable PD2 high level interrupt
    EIMSK |= (1 << INT0); // Enable interrupt request
    dt = 0;
}