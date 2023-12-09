#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__


#ifdef USING_ATMEGA
#include <avr/io.h>
#else
#include <stdint.h>
#endif // USING_ATMEGA

#define RING_BUFFER_SIZE 511
#define ABS( x ) ( ((x) < 0) ? -(x) : (x) )
#define NULL 0x00

struct ring_buffer {
    uint16_t read_pointer;
    uint16_t write_pointer;
    uint16_t available_bytes;

    uint8_t buffer[RING_BUFFER_SIZE];
};

// Initialise le buffer circulaire
void ring_buffer_init(struct ring_buffer *rb);

// Ajoute un octet dans le buffer circulaire
void ring_buffer_put(struct ring_buffer *rb, uint8_t data);

// Récupère un octet du buffer circulaire
uint8_t ring_buffer_get(struct ring_buffer *rb);

// Indique le nombre d'octets disponibles dans le buffer circulaire
uint16_t ring_buffer_available_bytes(struct ring_buffer *rb);

// Indique si le buffer circulaire est plein
uint8_t ring_buffer_is_full(struct ring_buffer *rb);

#endif // __RING_BUFFER_H__