#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <unistd.h>

#define NAME_SIZE 256
#define BUFF_SIZE 2048

typedef struct {
    int status;
    char filename[NAME_SIZE];
    char contents[BUFF_SIZE];
}FileInfo;


int main(){
    char buff[BUFF_SIZE];
    key_t mem_key = ftok("Ex-5/shared.bin",'b');
    printf("%d\n", mem_key);
    int shmid = shmget(mem_key, BUFF_SIZE, IPC_CREAT | IPC_EXCL | 0664);
    if(shmid == -1){
        perror("Error: ");
        exit(1);
    }
    FileInfo* file_info = shmat(shmid, NULL, 0);
    if(file_info == (FileInfo *) -1){
        perror("Error: ");
        shmctl(shmid,IPC_RMID, NULL);
    }   
    file_info->status = 0;
    file_info->contents[0] = '\0';
    printf("What is the name of the file you want: ");
    fgets(file_info->filename, NAME_SIZE, stdin);
    file_info->filename[strlen(file_info->filename) - 1] = '\0';
    file_info->status = 2;
    while(file_info->status == 2);
    if(file_info->status == -1){
        printf("Error! Could not find specified file!\n");
        shmdt(file_info);
        shmctl(shmid,IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }

    FILE* fp = fopen("copy.txt", "w");
    if(fp == NULL){
        perror("Error: ");
        shmdt(file_info);
        shmctl(shmid, IPC_RMID, NULL);
    }
    printf("Content: %s\n", file_info->contents);
    fprintf(fp, "%s", file_info->contents);
    fclose(fp);
    file_info->status = 0;
    shmdt(file_info);
    shmctl(shmid,IPC_RMID, NULL);
    return 0;
}