#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "process.c"

typedef struct {
    Process** arr;
    int front;
    int rear;
    size_t capacity;
}Queue;

Queue* new_queue(size_t capacity) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->arr = (Process**)malloc(sizeof(Process*) * capacity);
    queue->front = 0;
    queue->rear = 0;
    queue->capacity = capacity;
    return queue;
}

bool is_empty(Queue* queue){
    return queue->front == queue->rear;
}

bool is_full(Queue* queue){
    return queue->rear == queue->capacity;
}

void enqueue(Queue* queue, Process* process){
    if(is_full(queue)) return;
    queue->arr[queue->rear++] = process;
    queue->rear = ((queue->rear % queue->capacity) == queue->front) ? queue->capacity : (queue->rear % queue->capacity);
}

Process* dequeue(Queue* queue){
    if(is_empty(queue)) return NULL;
    if(is_full(queue)) queue->rear = queue->front;
    Process* process = queue->arr[queue->front++];
    queue->front %= queue->capacity;
    return process;
}

Process* peek(Queue* queue){
    if(is_empty(queue)) return NULL;
    return queue->arr[queue->front];
}

size_t queue_size(Queue* queue){
    if(is_full(queue)) return queue->capacity;
    return queue->rear > queue->front ? queue->rear - queue->front : queue->capacity - queue->front + queue->rear;
}

void print_queue(Queue* queue){
    size_t i = queue->front;
    size_t last = is_full(queue) ? queue->front : queue->rear;
    do{
        printf("%d ", queue->arr[i]->p_id);
        i = (i + 1) % queue->capacity;
    }while(i != last);
    printf("\n");
}

// int main(){
//     Queue* queue = new_queue(5);
//     enqueue(queue, 1);
//     enqueue(queue, 2);
//     enqueue(queue, 3);
//     enqueue(queue, 4);
//     print_queue(queue);
//     dequeue(queue);
//     dequeue(queue);
//     print_queue(queue);
//     enqueue(queue, 5);
//     enqueue(queue, 6);
//     print_queue(queue);
//     enqueue(queue, 7);
//     print_queue(queue);
//     return 0;
// }