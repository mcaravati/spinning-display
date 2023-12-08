#ifndef _SPI_UTILS_H_
#define _SPI_UTILS_H_

#include <avr/io.h>
#define OE PC1
#define LE PC2
#define SS PB2
#define SCK PB5
#define MOSI PB3

struct frame {
    uint16_t date;
    uint16_t payload;
};

#define FRAME_BUFFER_MAX_SIZE 200

void frame_buffer_put(struct frame* frame);
struct frame* frame_buffer_get();

void spi_init(void);
void spi_transmit_byte(uint8_t data);
void spi_transmit_array(uint16_t data);



#endif // !_SPI_UTILS_H_
