#include "timer.h"  

volatile uint16_t overflow_counter = 0;
volatile uint32_t max_count = 65536;

ISR(TIMER1_OVF_vect) {
    __vector_on_timer_end();
}

void __vector_on_timer_end() {
    overflow_counter++;
}

uint32_t get_timer() {
    return 64* (TCNT1 + (max_count * overflow_counter)) / 13000;
}

void timer_init() {
    TCCR1B |= (3 << CS10); 
    TIMSK1 |= (1 << TOIE1);  // Enable overflow interrupt

    TCNT1 = 0;
}

void timer_reset()
{
    TCNT1 = 0;
    overflow_counter = 0;
}