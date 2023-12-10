#include "frame_fifo.h"
#include <string.h>
#include <stdlib.h>

struct frame_fifo fifo ;

void frame_fifo_init()
{
    fifo.frame_write = 0;
    fifo.frame_amount = 0;
}

void frame_fifo_reset()
{
    frame_fifo_init();
    // memset(fifo.frames, NULL, sizeof(struct frame) * MAX_FRAME_FIFO_SIZE);
}

void frame_fifo_put(struct frame f)
{
    if(fifo.frame_amount == MAX_FRAME_FIFO_SIZE) return;
   
    fifo.frames[fifo.frame_write].date = f.date;
    fifo.frames[fifo.frame_write++].payload = f.payload;
    fifo.frame_amount++;
}

struct frame * frame_fifo_get()
{
    if(fifo.frame_amount == 0) return NULL;

    return fifo.frames;
}

uint8_t frame_fifo_get_amount()
{
    return fifo.frame_amount;
}