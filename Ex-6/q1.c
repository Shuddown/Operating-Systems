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
    if(sem_init(&sem_mem->empty, 1, BUFF_SIZE) == -1) perror("Semaphore initation error: ");
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
        for(int i = 0; i < strlen(buff); i++){
            sem_wait(&sem_mem->empty);
            sem_wait(&sem_mem->mutex);
            printf("%c produced!\n", buff[i]);
            sem_mem->buff[i % SMOL_SIZE] = buff[i];
            sem_post(&sem_mem->full);
            sem_post(&sem_mem->mutex);
        }
        sem_mem->done = true;
        wait(NULL);
    }

    else if(pid == 0){
        printf("In the child (consumer)\n");
        int out = 0;
        while(sem_mem->done == false){
            sem_wait(&sem_mem->full);
            sem_wait(&sem_mem->mutex);
            printf("Consumed %c!\n", sem_mem->buff[out]);
            out = (out + 1) % SMOL_SIZE;
            sem_post(&sem_mem->empty);
            sem_post(&sem_mem->mutex);
        }
        exit(0);
    }

    if(sem_destroy(&sem_mem->empty) == -1) perror("Error destroying semaphore: ");
    if(sem_destroy(&sem_mem->full) == -1) perror("Error destroying semaphore: ");
    if(sem_destroy(&sem_mem->mutex) == -1) perror("Error destroying semaphore: ");
    munmap(sem_mem, sizeof(Shared_Memory));
    close(fd);
    return 0;
}