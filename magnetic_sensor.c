#include "magnetic_sensor.h"
#include "spi_utils.h"

ISR(INT0_vect) { // Interrupt request handler
    PORTD |= (1 << PD6);
    spi_transmit_array(0b1111111111111111);
    _delay_ms(0.01);
    PORTD &= ~(1 << PD6);
    spi_transmit_array(0b0000000000000000);
}

void magnetic_sensor_init() {
    DDRD &= ~(1 << PD2); // PD2 as input
        
    EICRA &= ~(1 << ISC00);  // Enable PD2 low level interrupt
    EIMSK |= (1 << INT0); // Enable interrupt request
}