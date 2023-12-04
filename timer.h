#ifndef __TIMER_H__
#define __TIMER_H__

#include <avr/common.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define TARGET_TIME 10
#define PRESCALER 64

void __vector_on_timer_end() __attribute__((interrupt));
void __vector_on_timer_comparison_match() __attribute__((interrupt));

uint32_t get_human_timer();
uint32_t get_timer(); 

uint32_t into_atmega_time(uint32_t ms);
uint32_t into_human_time(uint32_t ms);
void timer_init();
void timer_reset();


#endif // __TIMER_H__