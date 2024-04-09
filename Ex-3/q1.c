#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "integer-input.c"
#define SIZE(arr) sizeof(arr)/sizeof(arr[0])

typedef struct result {
    uint16_t waiting_time;
    uint16_t turnaround_time;
    uint16_t response_time;
}Result;

typedef struct {
    int p_id;
    uint16_t arrival_time;
    uint16_t burst_time;
    Result* result;
}Patient;

Patient** get_processes(uint16_t n){
    Patient** patients =(Patient**) malloc(n * sizeof(Patient*));
    char buff[1024];
    printf("Number of processes: %u\n", n);
    for(int i = 0; i<n; i++){
        patients[i] = (Patient*) malloc(sizeof(Patient));
        printf("Process Id: ");
        fgets(buff, sizeof(buff), stdin);
        patients[i]->p_id = atoi(&(buff[1]));
        printf("Arrival Time: ");
        fgets(buff, sizeof(buff), stdin);
        patients[i]->arrival_time = atoi(buff);
        printf("Burst Time: ");
        fgets(buff, sizeof(buff), stdin);
        patients[i]->burst_time = atoi(buff);
        patients[i]->result = NULL;
    }
    return patients;
} 

int process_cmp(const void* p1, const void* p2){
    return ((Patient*)p1)->arrival_time - ((Patient*)p2)->arrival_time;
}

void execute_process(Patient* patient, uint16_t* exec_time){
    patient->result = (Result*) malloc(sizeof(Result));
    Result* result = patient->result;
    result->waiting_time = *exec_time - patient->arrival_time;
    result->response_time = result->waiting_time;
    result->turnaround_time = result->waiting_time + patient->burst_time;
    *exec_time += patient->burst_time;
}

void print_results(Patient* patients[], uint16_t size){
    printf("Process ID  Arrival Time    Burst Time  Turnaround Time  Waiting Time\n");
    uint16_t total_waiting_time = 0;
    uint16_t total_turnaround_time = 0;
    for(uint16_t i = 0; i<size; i++){
        Patient* p = patients[i];
        printf("P%d         %d               %d         %d               %d             \n",
            p->p_id,
            p->arrival_time,
            p->burst_time,
            p->result->turnaround_time,
            p->result->waiting_time 
        );
        total_waiting_time += p->result->waiting_time;
        total_turnaround_time += p->result->turnaround_time;
    }
    float avg_waiting_time = (float) total_waiting_time / size;
    float avg_turnaround_time = (float) total_turnaround_time / size;
    printf("Average waiting time: %lf\n", avg_waiting_time);
    printf("Average turnaround time: %lf\n", avg_turnaround_time);
}

void fcfs(Patient* patients[], uint16_t size){
    qsort(patients, size, sizeof(Patient*), &process_cmp);
    uint16_t elapsed_time = 0;
    uint16_t process_index = 0;
    while(process_index < size){

        if(patients[process_index]->arrival_time <= elapsed_time) {
            execute_process(patients[process_index], &elapsed_time);
            process_index++;
        }else{
            elapsed_time++;
        }
    }
    print_results(patients, size);    
}

int main(){
    printf("Enter the number of processes: ");
    int n;
    getIntegerFromStdin(&n);
    Patient** patients = get_processes(n);
    fcfs(patients, n);
}