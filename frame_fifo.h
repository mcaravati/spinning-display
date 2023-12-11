#ifndef __FRAME_FIFO_H__
#define __FRAME_FIFO_H__

#include <avr/io.h>

#define MAX_FRAME_FIFO_SIZE 200

struct frame
{
    uint16_t date;
    uint16_t payload;
};
struct frame_fifo
{
    uint8_t frame_write;
    uint8_t frame_amount;
    struct frame frames[MAX_FRAME_FIFO_SIZE];
};

void frame_fifo_init();
void frame_fifo_reset();
void frame_fifo_put(struct frame f);
struct frame * frame_fifo_get();
uint8_t frame_fifo_get_amount();

#endif