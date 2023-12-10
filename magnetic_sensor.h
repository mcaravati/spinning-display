#ifndef __MAGNETIC_SENSOR_H__
#define __MAGNETIC_SENSOR_H__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "spi_utils.h"

uint16_t get_round_dt();
uint32_t get_round_time();
void magnetic_sensor_init();

#endif // __MAGNETIC_SENSOR_H__