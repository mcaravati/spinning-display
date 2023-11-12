#ifndef _SPI_UTILS_H_
#define _SPI_UTILS_H_

#include <avr/io.h>
#define OE PC1
#define LE PC2

void spi_init(void);
void spi_transmit(uint8_t data);

#endif // !_SPI_UTILS_H_
