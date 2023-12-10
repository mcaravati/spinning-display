#include "spi_utils.h"
#include "uart_utils.h"
#include <stdio.h>

volatile uint8_t frame_buffer_cursor_w = 0;
volatile uint8_t frame_buffer_cursor_r = 0;

#define EPS 52
volatile struct frame frame_buffer[FRAME_BUFFER_MAX_SIZE] = {
{0x0, 0x0},
{0xcb, 0xfc},
{0x196, 0x1c},
{0x261, 0x4},
{0x32c, 0x4},
{0x3f7, 0x0},
{0x4c2, 0x0},
{0x58d, 0x0},
{0x659, 0x0},
{0x724, 0x0},
{0x7ef, 0x0},
{0x8ba, 0x0},
{0x985, 0x0},
{0xa50, 0x0},
{0xb1b, 0x0},
{0xbe6, 0x0},
{0xcb2, 0x0},
{0xd7d, 0x0},
{0xe48, 0x0},
{0xf13, 0x0},
{0xfde, 0x0},
{0x10a9, 0x0},
{0x1174, 0x0},
{0x123f, 0x0},
{0x130b, 0x0},
{0x14a1, 0x0},
{0x156c, 0x0},
{0x1637, 0x0},
{0x1702, 0x0},
{0x17cd, 0x0},
{0x1898, 0x0},
{0x1964, 0x0},
{0x1a2f, 0x0},
{0x1afa, 0x0},
{0x1bc5, 0x4},
{0x1c90, 0xc},
{0x1d5b, 0x3c},
{0x1e26, 0xfffc},
{0x1ef1, 0x0},
{0x1fbd, 0x0},
{0x2088, 0x0},
{0x2153, 0x0},
{0x221e, 0x0},
{0x22e9, 0x0},
{0x23b4, 0x0},
{0x247f, 0x0},
{0x254a, 0x0},
{0x2616, 0x0},
{0x26e1, 0x0},
{0x27ac, 0x0},
};
volatile uint8_t frame_buffer_size = 50;

void frame_buffer_put(uint16_t date, uint16_t payload) {
    if (frame_buffer_size >= FRAME_BUFFER_MAX_SIZE)
        return;

    frame_buffer[frame_buffer_cursor_w++].date = date;
    frame_buffer[frame_buffer_cursor_w++].payload = payload;

    frame_buffer_size++;
}

uint16_t payload_from_time(uint32_t time) {
    for (uint8_t i = 0; i < frame_buffer_size; i++) {
        if (frame_buffer[i].date >= time - EPS && frame_buffer[i].date <= time + EPS) {
            return frame_buffer[i].payload;
        }
    }

    return 0;
}

struct frame* frame_buffer_get() {
    if (frame_buffer_cursor_r >= FRAME_BUFFER_MAX_SIZE)
        return NULL;
    
    struct frame* addr = (struct frame *) &frame_buffer[frame_buffer_cursor_r++];
    frame_buffer_cursor_r %= FRAME_BUFFER_MAX_SIZE;
    return addr;
}

void frame_buffer_reset() {
    frame_buffer_cursor_r = 0;
}

void spi_init(void)
{
    /* Set MOSI and SCK output, all others input */
    DDRB = (1 << MOSI) | (1 << SCK) | (1 << SS);
    DDRC |= (1 << OE) | (1 << LE); 
    PORTC &= ~(1 << OE); // OE

    /* Enable SPI, Master, set clock rate fck/16 */

    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0) | (1 << DORD);
}

void spi_transmit_byte(uint8_t data)
{
    // PORTC &= ~(1 << LE); // LE
    /* Start transmission */

    SPDR = data;        // SDI
    /* Wait for transmission complete */
    while (!(SPSR & (1 << SPIF)))
        ;

}

void spi_transmit_array(uint16_t data)
{
    PORTB |= (1 << SS);

    uint8_t data_h = (data >> 8);
    uint8_t data_l = data;

    spi_transmit_byte(data_l);
    spi_transmit_byte(data_h);
    PORTC |= (1 << LE); // LE
    PORTC &= ~(1 << LE); // LE

    PORTB &= ~(1 << SS);
}
