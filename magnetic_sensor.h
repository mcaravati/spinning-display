#ifndef __MAGNETIC_SENSOR_H__
#define __MAGNETIC_SENSOR_H__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "spi_utils.h"

void magnetic_sensor_init();

#endif // __MAGNETIC_SENSOR_H__