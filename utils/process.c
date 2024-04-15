#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "integer-input.c"

typedef struct{
    uint16_t waiting_time;
    uint16_t turnaround_time;
    uint16_t response_time;
}Result;

typedef struct {
    int p_id;
    uint16_t arrival_time;
    uint16_t burst_time;
    Result* result;
}Process;

Process** get_processes(uint16_t n){
    Process** processs =(Process**) malloc(n * sizeof(Process*));
    char buff[1024];
    printf("Number of processes: %u\n", n);
    for(int i = 0; i<n; i++){
        processs[i] = (Process*) malloc(sizeof(Process));
        printf("Process Id: ");
        getIntegerFromStdin(&processs[i]->p_id);
        printf("Arrival Time: ");
        fgets(buff, sizeof(buff), stdin);
        processs[i]->arrival_time = atoi(buff);
        printf("Burst Time: ");
        fgets(buff, sizeof(buff), stdin);
        processs[i]->burst_time = atoi(buff);
        processs[i]->result = NULL;
    }
    return processs;
} 

int process_cmp(const void* p1, const void* p2){
    return ((Process*)p1)->arrival_time - ((Process*)p2)->arrival_time;
}

void add_gant(int* gant_rep, uint16_t* size, Process* process,int time){
    gant_rep[(*size)++] = process->p_id;
    gant_rep[(*size)++] = time; 
}

void print_ascii(char ascii, uint16_t n){
    for(uint16_t i = 0; i < n; i++){
        printf("%c", ascii);
    }
}

void print_gant(int* gant_rep, uint16_t size){
    printf("%d", 0);
    char ascii_block = (char) 219;
    uint16_t total_time = gant_rep[size - 1];
    uint16_t rebased_len = 100;
    uint16_t prev = 0;
    for(uint16_t i = 1; i < size; i+=2){
        printf("%d", prev);
        print_ascii(' ', ((gant_rep[i] - prev) / total_time) * rebased_len - 2);
        prev = gant_rep[i];
    }
    printf("\n");
    print_ascii(ascii_block, rebased_len);
    printf("\n");
}

void execute_process(Process* process, uint16_t* exec_time){
    process->result = (Result*) malloc(sizeof(Result));
    Result* result = process->result;
    result->waiting_time = *exec_time - process->arrival_time;
    result->response_time = result->waiting_time;
    result->turnaround_time = result->waiting_time + process->burst_time;
    *exec_time += process->burst_time;
}

bool execute_process_time(Process* process, int* elapsed_time, uint16_t max_time){
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

void print_results(Process* processs[], uint16_t size){
    printf("Process ID\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\tResponse Time\n");
    uint16_t total_waiting_time = 0;
    uint16_t total_turnaround_time = 0;
    for(uint16_t i = 0; i<size; i++){
        Process* p = processs[i];
        char id[100];
        sprintf(id, "P%d", p->p_id);
        printf("%10s\t%12d\t%10d\t%12d\t%15d\t%13d\n",
            id,
            p->arrival_time,
            p->burst_time,
            p->result->waiting_time,
            p->result->turnaround_time,
            p->result->response_time
        );
        total_waiting_time += p->result->waiting_time;
        total_turnaround_time += p->result->turnaround_time;
    }
    float avg_waiting_time = (float) total_waiting_time / size;
    float avg_turnaround_time = (float) total_turnaround_time / size;
    printf("Average waiting time: %lf\n", avg_waiting_time);
    printf("Average turnaround time: %lf\n", avg_turnaround_time);
}
