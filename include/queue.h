#ifndef QUEUE_H
#define QUEUE_H
#include <stdint.h>

typedef struct Queue{
    volatile uint8_t head;
    volatile uint8_t tail;
    volatile uint8_t size;
    volatile uint8_t capacity;
    volatile uint8_t* data;
} Queue;

void init_queue(struct Queue* q, uint8_t* buffer, uint8_t capacity){
    q->head = 0;
    q->tail = 0;
    q->size = 0;
    q->capacity = capacity;
    q->data = buffer;
}
bool enqueue(struct Queue* q, uint8_t ch){
    if(q->size >= q->capacity) return false;
    q->data[q->tail] = ch;
    q->tail = (q->tail + 1) % q->capacity;
    q->size++;
    return true;
}
bool dequeue(struct Queue* q, uint8_t* ch){
    if(q->size == 0) return false;
    *ch = q->data[q->head];
    q->head = (q->head + 1) % q->capacity;
    q->size--;

    return true;
}
#endif // QUEUE_H