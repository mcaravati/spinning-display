#include "spi_utils.h"
#include "uart_utils.h"
#include <stdio.h>
#include "images.h"
#include "timer.h"

volatile uint8_t frame_buffer_cursor_w = 0;
volatile uint8_t frame_buffer_cursor_r = 0;

#define EPS 52
volatile struct frame frame_buffer[FRAME_BUFFER_MAX_SIZE] = {

};
volatile uint8_t frame_buffer_size = 118;

void frame_buffer_put(uint16_t date, uint16_t payload)
{
    if (frame_buffer_size >= FRAME_BUFFER_MAX_SIZE)
        return;

    frame_buffer[frame_buffer_cursor_w++].date = date;
    frame_buffer[frame_buffer_cursor_w++].payload = payload;

    frame_buffer_size++;
}

uint16_t payload_from_time(uint32_t time)
{
    // Do not ask why, but for-loops for upper-left, upper-right, etc do not work here
    // So now we have spaghetti code :(
    // But it works, so don't touch it

    if (time < into_atmega_time(52 / 4)) {
        for (uint8_t i = 0; i < images_library[1][0].size; i++)
        {
            uint16_t date = pgm_read_dword(&(images_library[1][0].frames[i].date));
            if (date >= time - EPS && date <= time + EPS)
            {
                return pgm_read_dword(&(images_library[1][0].frames[i].payload));
            }
        }    
    } else if (time < into_atmega_time(52 / 2)) {
        for (uint8_t i = 0; i < images_library[0][1].size; i++)
        {
            uint16_t date = pgm_read_dword(&(images_library[0][1].frames[i].date));
            if (date >= time - EPS && date <= time + EPS)
            {
                return pgm_read_dword(&(images_library[0][1].frames[i].payload));
            }
        }    
    } else if (time < into_atmega_time(52 * 3 / 4)) {
        for (uint8_t i = 0; i < images_library[2][2].size; i++)
        {
            uint16_t date = pgm_read_dword(&(images_library[2][2].frames[i].date));
            if (date >= time - EPS && date <= time + EPS)
            {
                return pgm_read_dword(&(images_library[2][2].frames[i].payload));
            }
        }    
    } else {
        for (uint8_t i = 0; i < images_library[3][3].size; i++)
        {
            uint16_t date = pgm_read_dword(&(images_library[3][3].frames[i].date));
            if (date >= time - EPS && date <= time + EPS)
            {
                return pgm_read_dword(&(images_library[3][3].frames[i].payload));
            }
        }    
    }

    return 0;
}

struct frame *frame_buffer_get()
{
    if (frame_buffer_cursor_r >= FRAME_BUFFER_MAX_SIZE)
        return NULL;

    struct frame *addr = (struct frame *)&frame_buffer[frame_buffer_cursor_r++];
    frame_buffer_cursor_r %= FRAME_BUFFER_MAX_SIZE;
    return addr;
}

void frame_buffer_reset()
{
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

    SPDR = data; // SDI
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
    PORTC |= (1 << LE);  // LE
    PORTC &= ~(1 << LE); // LE

    PORTB &= ~(1 << SS);
}
