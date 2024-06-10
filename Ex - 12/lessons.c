#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct{
    int val;
}thread_arg;

void* print_int(void* arg){
    thread_arg* passed_int = arg; 
    printf("The passed integer to the thread is: %d\n", passed_int->val);
    pthread_exit(NULL);
}



int main(){
    pthread_t thread;
    thread_arg arg;
    arg.val = 5;
    int rc = pthread_create(&thread, NULL, print_int, &arg);
    pthread_join(thread, NULL);
    return 0;
}