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
#include "../../utils/integer-input.c"

#define NUMS_MAX 4096
#define BUFF_MAX 8
#define RAND(num) rand() % num + 1
#define RAND_VAL_MAX 10

typedef struct Shared_Memory{
    char buff[BUFF_MAX];
    sem_t mutex;
    sem_t empty;
    sem_t full;
    bool client_ready;
    bool server_ready;
    bool client_done;
    bool server_done;
}Shared_Memory;


int main(){
    srand(42069);
    printf("Client: Producer\n");
    int fd = shm_open("/mem", O_CREAT | O_RDWR | O_TRUNC, 0664);
    printf("%d\n",fd);
    ftruncate(fd, sizeof(Shared_Memory));

    Shared_Memory* mem = mmap(NULL, sizeof(Shared_Memory), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(mem == (Shared_Memory*) -1){
        perror("Error allocating memeory!");
        close(fd);
    }
    mem->client_ready = false;
    mem->server_ready = false;

    if(sem_init(&mem->empty, true, BUFF_MAX) == -1){
        perror("Semaphore initialization error!");
        munmap(mem, sizeof(Shared_Memory));
    } 
    if(sem_init(&mem->full, true, 0) == -1){
        perror("Semaphore initialization error!");
        munmap(mem, sizeof(Shared_Memory));
    } 
    if(sem_init(&mem->mutex, true, 1) == -1){
        perror("Semaphore initialization error!");
        munmap(mem, sizeof(Shared_Memory));
    } 
    int n;
    printf("What is the number of random numbers you want: ");
    getIntegerFromStdin(&n);

    mem->client_ready = true;
    mem->client_done = false;
    mem->server_done = false;

    while(!mem->server_ready);
    for(int i = 0; i < n; i++){
        sem_wait(&mem->empty);
        sem_wait(&mem->mutex);
        int rand_num = RAND(RAND_VAL_MAX);
        printf("Produced %d\n", rand_num);
        mem->buff[i % BUFF_MAX] = rand_num;
        sem_post(&mem->mutex);
        sem_post(&mem->full);
    }
    mem->client_done = true;
    while(!mem->server_done);
    munmap(mem, sizeof(Shared_Memory));
    close(fd);
    return 0;
}
