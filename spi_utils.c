#include "spi_utils.h"

volatile uint8_t frame_buffer_size = 0;
volatile uint8_t frame_buffer_cursor_w = 0;
volatile uint8_t frame_buffer_cursor_r = 0;
volatile struct frame frame_buffer[FRAME_BUFFER_MAX_SIZE] = {0};

void frame_buffer_put(uint16_t date, uint16_t payload) {
    if (frame_buffer_size >= FRAME_BUFFER_MAX_SIZE)
        return;

    frame_buffer[frame_buffer_cursor_w++].date = date;
    frame_buffer[frame_buffer_cursor_w++].payload = payload;

    frame_buffer_size++;
}

struct frame* frame_buffer_get() {
    if (frame_buffer_cursor_r >= FRAME_BUFFER_MAX_SIZE)
        return NULL;
    
    struct frame* addr = (struct frame *) &frame_buffer[frame_buffer_cursor_r++];
    
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
