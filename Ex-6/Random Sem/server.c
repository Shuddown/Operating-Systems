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
    printf("Server: Consumer\n");

    int fd = shm_open("/mem",  O_RDWR , 0664);
    printf("%d\n", fd);
    ftruncate(fd, sizeof(Shared_Memory));

    Shared_Memory* mem = mmap(NULL, sizeof(Shared_Memory), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(mem == (Shared_Memory*) -1){
        perror("Error allocating memeory!");
        close(fd);
    }
    mem->server_ready = true;
    while(!mem->client_ready);
    int out = 0;
    int val;
    while(true){
        sem_getvalue(&mem->full,&val);
        if(mem->client_done && (val == 0)) break;
        sem_wait(&mem->full);
        sem_wait(&mem->mutex);
        int rand_val = mem->buff[out % BUFF_MAX];
        out++;
        printf("The random value consumed is: %d\n", rand_val);
        sem_post(&mem->mutex);
        sem_post(&mem->empty);
    }
    mem->server_done = true;
    munmap(mem, sizeof(Shared_Memory));
    close(fd);
    return 0;
}
