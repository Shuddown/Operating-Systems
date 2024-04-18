#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/integer-input.c"
#include "../utils/priority_queue.c"
#include "../utils/process.c"
#define SIZE(arr) sizeof(arr)/sizeof(arr[0])

void SJF(Process* processes[], uint16_t size){
    qsort(processes, size, sizeof(Process*), &process_cmp);
    Heap* priority_q = new_heap(size);
    uint16_t elapsed_time = 0;
    uint16_t process_index = 0;
    while(process_index < size){
        if(processes[process_index]->arrival_time <= elapsed_time){
            enqueue(priority_q, processes[process_index]);
            process_index++;
        }
        else if(!is_empty(priority_q)) {
            Process* process = dequeue(priority_q);
            execute_process(process, &elapsed_time);
        }

        else elapsed_time++;
    }
    while(!is_empty(priority_q)){
        Process *process = dequeue(priority_q);
        execute_process(process, &elapsed_time);
    }
}