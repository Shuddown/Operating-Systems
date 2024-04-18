#pragma once

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef struct Heap {
    void** base;
    uint16_t count;
    uint16_t capacity;
    int (*comp_func) (const void*, const void*);
} Heap;

Heap* new_heap(uint16_t capacity, size_t member_size, int (*comp) (const void*, const void*)) {
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    heap->base = malloc(sizeof(void*) * capacity);
    heap->capacity = capacity;
    heap->count = 0;
    heap->comp_func = comp;
    return heap;
}

size_t heap_left(size_t i) {
    return 2 * i + 1;
}

size_t heap_right(size_t i) {
    return 2 * i + 2;
}

size_t heap_parent(size_t i) {
    return (i - 1) / 2;
}

void swap(Heap* heap, uint16_t i, uint16_t j) {
    void* temp = NULL;
    temp = heap->base[i];
    heap->base[i] = heap->base[j];
    heap->base[j] = temp;
}

void bubble_up(Heap* heap, uint16_t i) {
    while (i > 0) {
        void* base_i = heap->base[i];
        void* base_parent = heap->base[heap_parent(i)];
        if (heap->comp_func(base_i, base_parent) < 0) {
            swap(heap, heap_parent(i), i);
            i = heap_parent(i);
        } else {
            break; 
        }
    }
}

void enqueue_h(Heap* heap, void* val) {
    if (heap->count == heap->capacity) {
        fprintf(stderr, "Heap is full\n");
        return;
    }
    heap->base[heap->count] = val;
    bubble_up(heap, heap->count);
    heap->count++;
}

bool has_left(Heap* heap, uint16_t i) {
    return heap_left(i) < heap->count; 
}

bool has_right(Heap* heap, uint16_t i) {
    return heap_right(i) < heap->count; 
}

uint16_t min_index(Heap* heap, size_t i, size_t j) {
    void* base_i = heap->base[i];
    void* base_j = heap->base[j];
    return heap->comp_func(base_i, base_j) <= 0 ? i : j;
}

void heapify(Heap* heap, size_t index) {
    if (!has_left(heap, index)) return;
    uint16_t min = index;
    if (has_right(heap, index)) {
        min = min_index(heap, heap_right(index), min_index(heap, index, heap_left(index)));
    } else {
        min = min_index(heap, index, heap_left(index));
    }
    if (min == index) return;
    swap(heap, min, index);
    heapify(heap, min);
}

void* dequeue_h(Heap* heap) {
    if (heap->count == 0) {
        fprintf(stderr, "Heap is empty\n");
        return NULL;
    }
    void* val = heap->base[0];
    swap(heap, 0, --(heap->count));
    heapify(heap, 0);
    return val;
}

bool is_empty_h(Heap* heap) {
    return heap->count == 0;
}

void free_heap(Heap* heap) {
    free(heap->base);
    free(heap);
}