#include "magnetic_sensor.h"

ISR(INT0_vect) { // Interrupt request handler
    PORTD |= (1 << PD6);
    _delay_ms(1);
    PORTD &= ~(1 << PD6);
}

void magnetic_sensor_init() {
    DDRD &= ~(1 << PD2); // PD2 as input
        
    EICRA &= ~(1 << ISC00);  // Enable PD2 low level interrupt
    EIMSK |= (1 << INT0); // Enable interrupt request

    sei();
}