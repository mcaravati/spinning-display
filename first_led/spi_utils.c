#include "spi_utils.h"


void spi_init(void)
{
    /* Set MOSI and SCK output, all others input */
    DDRB = (1 << PB3) | (1 << PB5);
    DDRC |= (1 << DDC2) | (1 << DDC1);
    PORTC &= ~(1 << OE); // OE

    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0) ;//| (0 << DORD);
}

void spi_transmit(uint8_t data)
{
    PORTC &= ~(1 << LE); // LE
    /* Start transmission */

    SPDR = data;        // SDI
    /* Wait for transmission complete */
    while (!(SPSR & (1 << SPIF)))
        ;

    PORTC |= (1 << LE); // LE
    PORTC &= ~(1 << LE); // LE
}
