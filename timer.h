#ifndef __TIMER_H__
#define __TIMER_H__

#include <avr/common.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void __vector_on_timer_end() __attribute__((interrupt)); 

uint32_t get_timer(); 

void timer_init();
void timer_reset();


#endif // __TIMER_H__