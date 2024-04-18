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
    char name_buff[1024];
    int shmid = shmget(IPC_PRIVATE, sizeof(name_buff), 0);
    pid_t p_id = fork();
    if(p_id > 0){
        char* addr = shmat(shmid, NULL, 0);
        if(addr == (char *) -1){
            perror("Error attaching memory\n");
            exit(EXIT_FAILURE);
        }
        addr[0] = '\0';

        printf("Give name: \n");
        fgets(name_buff, sizeof(name_buff), stdin);
        int len = strlen(name_buff);
        name_buff[strlen(name_buff) - 1] = '\0';
        memcpy(addr, name_buff, len);
        wait(NULL);
        int success = shmdt(addr);
    }
    else if (p_id == 0){
        char* str = shmat(shmid, NULL, 0);
        while(str[0] == '\0');
        for(int i = 0; i < strlen(str); i++){
            name_buff[i] = toupper(str[i]);
        }
        name_buff[strlen(str)] = '\0';
        printf("Converted name: %s\n", name_buff);
        shmdt(str);
    }

    shmctl(shmid, IPC_RMID, NULL);
    return 0;

}
