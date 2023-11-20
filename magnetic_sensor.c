#include "magnetic_sensor.h"
#include "spi_utils.h"
#include "timer.h"
#include "uart_utils.h"
#include <stdio.h>

ISR(INT0_vect) { // Interrupt request handler
    //encode dt in ascii and send it
    uint32_t tmp = get_timer();
    if (tmp > 10 && tmp < 150) {
        dt = tmp;
    }
    timer_reset();
}

void magnetic_sensor_init() {
    DDRD &= ~(1 << PD2); // PD2 as input
        
    EICRA &= ~(1 << ISC00);  // Enable PD2 low level interrupt
    EIMSK |= (1 << INT0); // Enable interrupt request
    dt = 0;
}