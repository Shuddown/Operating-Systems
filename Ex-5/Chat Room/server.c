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
#include <signal.h>
#include <sys/ipc.h>
#include <unistd.h>
#include "../../utils/char-input.c"


#define MSG_SIZE 4096
#define MSG_SIG SIGUSR1
#define KILL_SIG SIGUSR2


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

bool waiting = true;
int shmid = 0;
Shared_Memory* mem = NULL;

void on_msg_signal(int signal){
    waiting = false;
}

void on_exit_signal(int signal){
    int st = shmdt(mem);
    st = shmctl(shmid, IPC_RMID, NULL);
    exit(0);
}

int main(){

    if(signal(MSG_SIG, on_msg_signal) == SIG_ERR)
        perrorc("Error with message signal: ");
    if(signal(KILL_SIG, on_exit_signal) == SIG_ERR)
        perrorc("Error with kill signal: ");
    if(signal(SIGSEGV, on_exit_signal) == SIG_ERR)
        perrorc("Error with kill signal: ");
    if(signal(SIGINT, on_exit_signal) == SIG_ERR)
        perrorc("Error with kill");

    char buff[MSG_SIZE];

    pid_t pid = getpid();
    key_t key = ftok("/tmp", 'e');
    printf("%d\n", key);
    int shmid = shmget(key, MSG_SIZE*2, 0);
    if(shmid == -1){
        perror("Error: ");
        exit(1);
    }
    Shared_Memory* mem = shmat(shmid, NULL, 0);
    if(mem == (Shared_Memory*) -1) perrorc("Error: ");
    mem->peer2_id = getpid();
    mem->status = SUCCESS;

    while(true){
        while(waiting);
        printf("New Message: %s\n", mem->msg);
        printf("Do you want to exit(y/n): ");
        char opt = get_char("yn", strlen("yn"));
        if(opt == 'y'){
            int st = kill(mem->peer1_id, KILL_SIG);
            if(st == -1) perrorc("Error sending kill signal to client 2");
            break;
        }

        printf("Give a message to send: ");
        fgets(buff, sizeof(buff), stdin);
        buff[strcspn(buff, "\n")] = '\0';
        strcpy(mem->msg, buff);
        int st = kill(mem->peer1_id, MSG_SIG);
        if(st == -1) perrorc("Error sending message signal: ");
        waiting = true;
    }

    int st = shmdt(mem);
    st = shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
