// led.c
#include <avr/io.h>
#include <util/delay.h>

// define baudrate
#define FOSC 13000000// Clock Speed
#define BAUD 38400
#define MYUBRR ((FOSC/(8*BAUD))-1)


void spi_init(void)
{
    /* Set MOSI and SCK output, all others input */
    DDRB = (1 << PB3) | (1 << PB5);
    DDRC |= (1 << DDC2) | (1 << DDC1);
    PORTC &= ~(1 << PC1); // OE

    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0) ;//| (0 << DORD);
}

void spi_transmit(uint8_t data)
{
    PORTC &= ~(1 << PC2); // LE
    /* Start transmission */

    SPDR = data;        // SDI
    /* Wait for transmission complete */
    while (!(SPSR & (1 << SPIF)))
        ;

    PORTC |= (1 << PC2); // LE
    PORTC &= ~(1 << PC2); // LE
}

void uart_init()
{
    // Set baud rate
    UBRR0H  = (unsigned char)(MYUBRR >> 8);
    UBRR0L  = (unsigned char)MYUBRR;
    UCSR0A = (1 << U2X0);
    // Enable transmission
    UCSR0B = (1 << TXEN0);

    // Set one stop bit
    UCSR0C = (0 << USBS0);

    // Set format for message (8 bits)
    UCSR0C |= (0 << UCSZ02) | (1 << UCSZ01) | (1 << UCSZ00);

    // Set parity to disabled
    UCSR0C |= (0 << UPM01) | (0 << UPM00);
}


void uart_send_byte(uint8_t byte){
    // attente de la fin de transmission
    while (!(UCSR0A & (1 << UDRE0)));
    // chargement du registre, déclenche la transmission
    UDR0 = byte;
}

void uart_send_string(char *string){
    while (*string != '\0'){
        uart_send_byte(*string);
        string++;
    }
}

int main(void)
{

    DDRD |= (1 << PD6); // PB5 en sortie
    PORTD |= (1 << PD6); // PB5 à 1

    // uart_init(BAUD);

    spi_init();
    PORTC &= ~(1 << PC2); // LE
    spi_transmit(0b00001000);
    
    // spi_transmit(0x01);
    // spi_transmit(0xe);
    /* while (1)
    {
        uart_send_byte('H');
        uart_send_string("ello world\n");
        _delay_ms(1000);
    } */
}


