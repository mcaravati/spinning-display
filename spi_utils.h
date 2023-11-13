#ifndef _SPI_UTILS_H_
#define _SPI_UTILS_H_

#include <avr/io.h>
#define OE PC1
#define LE PC2
#define SS PB2
#define SCK PB5
#define MOSI PB3

void spi_init(void);
void spi_transmit_byte(uint8_t data);
void spi_transmit_array(uint16_t data);

#endif // !_SPI_UTILS_H_
