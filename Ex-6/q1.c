#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/wait.h>


#define BUFF_SIZE 4096
#define SMOL_SIZE 8

typedef struct Shared_Memory{
    char buff[SMOL_SIZE];
    sem_t mutex;
    sem_t empty;
    sem_t full;
    bool done;
}Shared_Memory;

int main(){
    int fd =  shm_open("/mem", O_CREAT | O_RDWR | O_TRUNC, 0664);
    ftruncate(fd, sizeof(Shared_Memory));
    void *mem = mmap(NULL, sizeof(Shared_Memory), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(mem == (void *) -1){
        perror("Memory Allocation Error: ");
        close(fd);
    }
    Shared_Memory* sem_mem = mem;
    if(sem_init(&sem_mem->mutex, 1, 1) == -1) perror("Semaphore initation error: ");
    if(sem_init(&sem_mem->empty, 1, SMOL_SIZE) == -1) perror("Semaphore initation error: ");
    if(sem_init(&sem_mem->full, 1, 0) == -1) perror("Semaphore initation error: ");
    sem_mem->done = false;

    pid_t pid = fork();
    if(pid == -1){
        perror("Error forking process!");
        munmap(sem_mem, sizeof(Shared_Memory));
        close(fd);
    }
    if(pid != 0){
        char buff[BUFF_SIZE];
        printf("In the parent(producer)!\n");
        printf("Give a word to produce: ");
        fgets(buff, sizeof(buff), stdin);
        buff[strlen(buff) - 1] = '\0';
        for(int i = 0; i < strlen(buff) + 1; i++){
            sem_wait(&sem_mem->empty);
            printf("Producer aquired semphore Empty\n");
            sem_wait(&sem_mem->mutex);
            printf("Producer aquired semphore Mutex\n");
            sem_mem->buff[i % SMOL_SIZE] = buff[i];
            printf("Producer produced %c!\n", buff[i]);
            sem_post(&sem_mem->mutex);
            printf("Producer released semphore Mutex\n");
            sem_post(&sem_mem->full);
            printf("Producer released semphore Full\n");
        }
        sem_mem->done = true;
        wait(NULL);
    }

    else if(pid == 0){
        char consumed[BUFF_SIZE + 1];
        printf("In the child (consumer)\n");
        int out = 0;
        int val;
        while(true){
            sem_getvalue(&sem_mem->full, &val);
            if(sem_mem->done && (val == 0)) break;
            sem_wait(&sem_mem->full);
            printf("Consumer aquired semphore Full\n");
            sem_wait(&sem_mem->mutex);
            printf("Consumer aquired semphore Mutex\n");
            consumed[out] = sem_mem->buff[out % SMOL_SIZE];
            printf("Consumer consumed %c!\n", consumed[out]);
            out++;
            sem_post(&sem_mem->mutex);
            printf("Consumer released semphore Mutex\n");
            sem_post(&sem_mem->empty);
            printf("Consumer released semphore Empty\n");
        }
        printf("%s\n", consumed);
        exit(0);
    }

    if(sem_destroy(&sem_mem->empty) == -1) perror("Error destroying semaphore: ");
    if(sem_destroy(&sem_mem->full) == -1) perror("Error destroying semaphore: ");
    if(sem_destroy(&sem_mem->mutex) == -1) perror("Error destroying semaphore: ");
    munmap(sem_mem, sizeof(Shared_Memory));
    close(fd);
    return 0;
}