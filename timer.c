#include "timer.h"  

volatile uint32_t overflow_counter = 0;

ISR(TIMER1_OVF_vect) {
    __vector_on_timer_end();
}

void __vector_on_timer_end() {
    overflow_counter++;
}

uint32_t get_timer() {
    return (TCNT1 + (65536 * overflow_counter)) / 13000000;
}

void timer_init() {
    TCCR1B |= (1 << CS10); 
    TIMSK1 |= (1 << TOIE1);  // Enable overflow interrupt

    TCNT1 = 0;
}