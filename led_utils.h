#ifndef __LED_UTILS_H__
#define __LED_UTILS_H__

#include <avr/io.h>
#define MAX_HEIGHT 500
#define MAX_WIDTH 500

struct led_controller
{
    
};

typedef int16_t pcoord;

void LED_on_at(pcoord x, pcoord y);
void LEF_off_at(pcoord x, pcoord y);

#endif