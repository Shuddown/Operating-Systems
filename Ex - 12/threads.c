#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include "../utils/integer-input.c"
#define ARR_SIZE 1024

typedef struct {
    int arr[ARR_SIZE];
    int n;
}arr_struct;


void* min(void* arri){
    arr_struct* arr_val = arri;
    int n = arr_val->n;
    int* arr = arr_val->arr;
    int min_val = INT32_MAX;

    for(int i = 0; i < n; i++){
       if(min_val > arr[i]) min_val = arr[i]; 
    }

    printf("Thread 3 min is %d\n", min_val);
    pthread_exit(NULL);
}

void* max(void* arri){
    arr_struct* arr_val = arri;
    int n = arr_val->n;
    int* arr = arr_val->arr;
    int max_val = INT32_MIN;

    for(int i = 0; i < n; i++){
       if(max_val < arr[i]) max_val = arr[i]; 
    }
    printf("Thread 1 maximum is %d\n", max_val);
    pthread_exit(NULL);
}

void* avg(void* arri){
    arr_struct* arr_val = arri;
    int n = arr_val->n;
    int* arr = arr_val->arr;
    int sum = 0;

    for(int i = 0; i < n; i++){
        sum += arr[i];
    }
    
    printf("Thread 2 avg is %lf\n", (float) sum / n);
    pthread_exit(NULL);
}


int main(){

    pthread_t thread_max;
    pthread_t thread_avg;
    pthread_t thread_min;

    arr_struct input;
    printf("Give the number of numbers in the array: ");
    getIntegerFromStdin(&input.n);
    for (int i = 0; i < input.n; i++){
        printf("Enter the %dth number: ", i+1);
        getIntegerFromStdin(&input.arr[i]);
    }
    

    pthread_create(&thread_max, NULL, max, &input);
    pthread_create(&thread_avg, NULL, avg, &input);
    pthread_create(&thread_min, NULL, min, &input);

    pthread_join(thread_max, NULL);
    pthread_join(thread_avg, NULL);
    pthread_join(thread_min, NULL);
    return 0;
}