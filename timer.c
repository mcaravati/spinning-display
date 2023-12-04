#include "timer.h"  

volatile uint16_t overflow_counter = 0;
volatile uint32_t max_count = 65536; // uint16_6 max value

ISR(TIMER1_OVF_vect) {
    __vector_on_timer_end();
}

ISR(TIMER0_COMPA_vect) {
    __vector_on_timer_comparison_match();
}

void __vector_on_timer_end() {
    overflow_counter++;
}

void __vector_on_timer_comparison_match() {

}

uint32_t get_timer() {
    return 64 * (TCNT1 + (max_count * overflow_counter)) / 13000;
}

void timer_init() {
    TCCR1B |= (3 << CS10);   // Set prescaler to 64
    TIMSK1 |= (1 << TOIE1);  // Enable overflow interrupt

    TCCR0B |= (3 << CS00);
    TIMSK0 |= (1 << OCIE0A); // Enable comparison interrupt
    TCCR0A |= (2 << COM0A0); // Clear OC0A bit

    int16_t tmp = 5;//13000 / (PRESCALER * 1000) * TARGET_TIME;
    OCR0A = (int8_t) tmp;

    TCNT1 = 0;
    TCNT0 = 0;
}

void timer_reset()
{
    TCNT1 = 0;
    overflow_counter = 0;
}