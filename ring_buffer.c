#include "ring_buffer.h"


void ring_buffer_init(struct ring_buffer *rb) {
    rb->read_pointer = 0;
    rb->write_pointer = 0;
    rb->available_bytes = 0;

    for (uint16_t i = 0; i < RING_BUFFER_SIZE; i++) {
        rb->buffer[i] = 0;
    }
}

void ring_buffer_put(struct ring_buffer *rb, uint8_t data) {
    if (ring_buffer_is_full(rb))
        return;

    rb->buffer[rb->write_pointer] = data;
    rb->write_pointer = (rb->write_pointer + 1) % RING_BUFFER_SIZE;
    rb->available_bytes += 1;
}

uint8_t ring_buffer_get(struct ring_buffer *rb) {
    if (rb->available_bytes == 0)
        return '\0';

    uint8_t return_value = rb->buffer[rb->read_pointer];
    rb->buffer[rb->read_pointer] = NULL; // Reset value
    rb->read_pointer = (rb->read_pointer + 1) % RING_BUFFER_SIZE;
    rb->available_bytes -= 1;

    return return_value;
}

uint16_t ring_buffer_available_bytes(struct ring_buffer *rb) {
    return rb->available_bytes;
}

uint8_t ring_buffer_is_full(struct ring_buffer *rb) {
    return rb->available_bytes == RING_BUFFER_SIZE;
}