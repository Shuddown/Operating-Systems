#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <unistd.h>

#define MSG_SIZE 4096



typedef enum {
    WAITING,
    SUCCESS
}Status;

typedef struct{
    Status status;
    pid_t peer1_id;
    pid_t peer2_id;
    char msg[MSG_SIZE];
}Shared_Memory;

void perrorc(const char* msg){
    perror(msg);
    exit(1);
}

bool waiting = false;
int shmid = 0;
Shared_Memory* mem = NULL;

void on_msg_signal(int signal){
    printf("New Message: %s\n", mem->msg);
    waiting = false;
}

void on_exit_signal(int signal){
    int st = shmdt(mem);
    if(st == -1) perrorc("Error: ");
    st = shmctl(shmid, IPC_RMID, NULL);
    if(st == -1) perrorc("Error: ");
    exit(0);
}

int main(){
    char buff[MSG_SIZE];

    pid_t pid = getpid();
    key_t key = ftok("/home/shuddown/Programs/C/OS/Ex-5/Chat Room/shared.bin", 'c');
    printf("%d\n", key);
    int shmid = shmget(key, BUFF_SIZE, IPC_CREAT | IPC_EXCL | 0664);
    if(shmid == -1){
        perror("Error: ");
        exit(1);
    }




    return 0;
}
