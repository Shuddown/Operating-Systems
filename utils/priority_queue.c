#pragma once

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "process.c"

typedef struct Heap {
    Process** processes;
    int size;
    int capacity;
} Heap;

Heap* new_heap(size_t capacity){
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    heap->processes = (Process**)malloc(sizeof(Process*) * capacity);
    heap->capacity = capacity;
    heap->size = 0;
    return heap;
}

size_t heap_left(size_t i){
    return 2 * i + 1;
}

size_t heap_right(size_t i){
    return 2 * i + 2;
}

size_t heap_parent(size_t i){
    return (i - 1) / 2;
}

void swap(Heap* heap, size_t i, size_t j){
    Process* temp = heap->processes[i];
    heap->processes[i] = heap->processes[j];
    heap->processes[j] = temp;
}

void bubble_up(Heap* heap, size_t i){
    while(i > 0){
        if(heap->processes[heap_parent(i)]->burst_time > heap->processes[i]->burst_time){
            swap(heap, heap_parent(i), i);
            i = heap_parent(i);
        }
    }
}

void enqueue(Heap* heap, Process* val){
    if(heap->size == heap->capacity) return;

    heap->processes[heap->size] = val;
    bubble_up(heap, heap->size);
    heap->size++;
}

bool has_left(Heap* heap, size_t i){
    return heap_left(i) < heap->size; 
}


bool has_right(Heap* heap, size_t i){
    return heap_right(i) < heap->size; 
}

size_t min_index(Heap* heap, size_t i, size_t j){
    return heap->processes[i]->burst_time <= heap->processes[j]->burst_time ? i : j;
}

void heapify(Heap* heap, size_t index){
    if(!has_left(heap, index)) return;
    size_t min = index;
    if(has_right(heap, index)) min = min_index(heap, heap_right(index), min_index(heap, index, heap_left(index)));
    else min = min_index(heap, index, heap_left(index));
    if(min == index) return;
    swap(heap, min, index);
    heapify(heap, min);
}

Process* dequeue(Heap* heap){
    Process* val = heap->processes[0];
    swap(heap, 0, --heap->size);
    heapify(heap, 0);
    return val;
}

void print_heap(Heap* heap){
    for(int i = 0; i < heap->size; i++){
        printf("%d ", heap->processes[i]->p_id);
    }
    printf("\n");
}

bool is_empty(Heap* heap){
    return heap->size == 0;
}

