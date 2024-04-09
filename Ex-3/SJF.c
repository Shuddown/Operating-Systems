#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "integer-input.c"
#include "/home/shuddown/Programs/C/OS/utils/priority_queue.c"
#include "/home/shuddown/Programs/C/OS/utils/process.c"
#define SIZE(arr) sizeof(arr)/sizeof(arr[0])

void SJF(Process* processes[], uint16_t size, int* gant_rep, int* gant_size){
    uint16_t gant_size = 0;
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
            add_gant(gant_rep,gant_size, process, elapsed_time);
        }

        else elapsed_time++;
    }
    while(!is_empty(priority_q)){
        Process *process = dequeue(priority_q);
        execute_process(process, &elapsed_time);
        add_gant(gant_rep, gant_size, process, elapsed_time);
    }
}

int main(){
    int n;
    printf("Give the number of processes: ");
    getIntegerFromStdin(&n);
    Process** processes = get_processes(n);
    uint16_t gant_rep[100];
    uint16_t gant_size = 0;
    SJF(processes, n, gant_rep, &gant_size);
    print_gant(gant_rep, gant_size);
    print_results(processes, n);
    return 0;
}