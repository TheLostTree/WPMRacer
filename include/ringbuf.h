#ifndef RINGBUF_H
#define RINGBUF_H


// closer to a circular queue but whatever
#include <stdint.h>

struct RingBuf{
    uint8_t* buf;
    uintptr_t head;
    uintptr_t tail;
    uintptr_t max_size;
};

void ringbuf_init(RingBuf* rb, uint8_t* buffer, uintptr_t size){
    rb->buf = buffer;
    rb->head = 0;
    rb->tail = 0;
    rb->max_size = size;
}
bool ringbuf_is_empty(RingBuf* rb){
    return rb->head == rb->tail;
}
bool ringbuf_is_full(RingBuf* rb){
    return ((rb->head + 1) % rb->max_size) == rb->tail;
}

bool ringbuf_enqueue(RingBuf* rb, uint8_t data){
    if(ringbuf_is_full(rb)){
        return false; // full
    }
    rb->buf[rb->head] = data;
    rb->head = (rb->head + 1) % rb->max_size;
    return true;
}

bool ringbuf_dequeue(RingBuf* rb, uint8_t* data){
    if(ringbuf_is_empty(rb)){
        return false; // empty
    }
    *data = rb->buf[rb->tail];
    rb->tail = (rb->tail + 1) % rb->max_size;
    return true;
}

uint8_t ringbuf_read(RingBuf* rb, uintptr_t index){
    if(index >= rb->max_size){
        return 0; // out of bounds
    }
    uintptr_t real_index = (rb->tail + index) % rb->max_size;
    return rb->buf[real_index];
}

#endif /* RINGBUF_H_ */