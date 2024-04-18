#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/integer-input.c"
#include "../utils/priority_queue.c"
#include "../utils/process.c"

void SJF_premptive(Process* processes[], uint16_t size){
    qsort(processes, size, sizeof(Process*), process_cmp);
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
            bool finished = execute_process_time(process, &elapsed_time, 1);
            if(!finished) enqueue(priority_q, process);
        }

        else elapsed_time++;
    }
    while(!is_empty(priority_q)){
        Process *process = dequeue(priority_q);
        execute_process_time(process, &elapsed_time, INT16_MAX);
    }
}
