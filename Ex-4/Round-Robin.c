#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/process.c"
#include "../utils/queue.c"

#define MAX_PROCESSES 100

bool execute_process_rr(Process* process, int* elapsed_time, uint16_t max_time){
    Result* result = process->result;
    if(result == NULL){
        process->result = (Result*) malloc(sizeof(Result));
        result = process->result;
        result->response_time = *elapsed_time;
        result->waiting_time = 0;
        result->turnaround_time = process->burst_time;
    }
    process->burst_time -= max_time;
    *elapsed_time += max_time;
    if(process->burst_time > 0) return false;
    *elapsed_time += process->burst_time;
    process->burst_time = result->turnaround_time;
    result->waiting_time = *elapsed_time - (process->arrival_time + process->burst_time);
    result->turnaround_time = *elapsed_time - process->arrival_time;
    return true;
}

void round_robin(Process** processes, uint16_t size,uint16_t timer_max){
    qsort(processes,size, sizeof(Process*), process_cmp);
    Queue* process_q = new_queue(MAX_PROCESSES);
    uint16_t process_index = 0;
    int elapsed_time = 0;
    while(process_index < size){
        if(processes[process_index]->arrival_time <= elapsed_time){
            enqueue(process_q, processes[process_index]);
            process_index++;
        }else if(!is_empty(process_q)){
            Process* process = dequeue(process_q);
            bool finished = execute_process_rr(process, &elapsed_time, timer_max);
            if(!finished){
                while(process_index < size && processes[process_index]->arrival_time <= elapsed_time){
                    enqueue(process_q, processes[process_index]);
                    process_index++;
                }
                enqueue(process_q, process); 
            } 
        }else elapsed_time++;
    }
    while(!is_empty(process_q)){
        Process *process = dequeue(process_q);
        bool finished = execute_process_rr(process, &elapsed_time, timer_max);
        if (!finished)
            enqueue(process_q, process);
    }
}
