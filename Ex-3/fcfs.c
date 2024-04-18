#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/process.c"
#include "../utils/integer-input.c"
#define SIZE(arr) sizeof(arr)/sizeof(arr[0])

void fcfs(Process* processes[], uint16_t size){
    qsort(processes, size, sizeof(Process*), &process_cmp);
    uint16_t elapsed_time = 0;
    uint16_t process_index = 0;
    while(process_index < size){

        if(processes[process_index]->arrival_time <= elapsed_time) {
            execute_process(processes[process_index], &elapsed_time);
            process_index++;
        }else{
            elapsed_time++;
        }
    }
}
