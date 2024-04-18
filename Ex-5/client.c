#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <unistd.h>

int main(){
    char buff[1024];
    key_t mem_key = ftok("shared.bin",'a');
    int shmid = shmget(mem_key, sizeof(buff), IPC_CREAT | IPC_EXCL);
    char* file_info = shmat(shmid, NULL, 0);
    

    return 0;
}