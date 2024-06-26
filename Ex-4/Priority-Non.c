#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/priority_process.c"
#include "../utils/heap_general.c"
#include "../utils/integer-input.c"

void priority(Priority_Process** processes, uint16_t count){
    qsort(processes, count, sizeof(Priority_Process*), time_cmp);
    Heap* priority_q = new_heap(count, sizeof(Priority_Process*), priority_cmp);
    uint16_t elapsed_time = 0;
    uint16_t process_index = 0;
    while(process_index < count){
        if(processes[process_index]->arrival_time <= elapsed_time){
            enqueue_h(priority_q, processes[process_index]);
            process_index++;
        }
        else if(!is_empty_h(priority_q)) {
            Priority_Process* process = (Priority_Process*) dequeue_h(priority_q);
            bool finished = execute_process_time_priority(process, &elapsed_time, INT16_MAX);
            if(!finished) enqueue_h(priority_q, process);
        }

        else elapsed_time++;
    }
    while(!is_empty_h(priority_q)){
        Priority_Process *process = (Priority_Process*) dequeue_h(priority_q);
        execute_process_time_priority(process, &elapsed_time, INT16_MAX);
    }
}

