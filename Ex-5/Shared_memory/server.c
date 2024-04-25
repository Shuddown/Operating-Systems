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

#define BUFF_SIZE 2048
#define NAME_SIZE 256


typedef struct {
    int status;
    char filename[NAME_SIZE];
    char contents[BUFF_SIZE];
}FileInfo;

int main(){
    char buff[BUFF_SIZE];

    key_t mem_key = ftok("Ex-5/shared.bin", 'b');
    printf("%d\n", mem_key);
    if(mem_key == -1){
        perror("Error: ");
        exit(1);
    }
    int shmid = shmget(mem_key,BUFF_SIZE, 0);
    if (shmid == -1){
        perror("Error: ");
        shmctl(shmid,IPC_RMID, NULL);
        exit(1);

    }
    FileInfo* file_info = shmat(shmid, NULL, 0);
    if(file_info == (FileInfo*) -1){
        perror("Error: ");
        shmdt(file_info);
        shmctl(shmid,IPC_RMID, NULL);
        exit(1);
    }
    while(file_info->status != 2);
    printf("Status: %d\n", file_info->status);
    printf("Filename: %s\n", file_info->filename);

    FILE* fp = fopen(file_info->filename, "r");
    if(fp == NULL){
        printf("Error, Could not access file!\n");
        file_info->status = -1;
        shmdt(file_info);
        shmctl(shmid,IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }
    char c = 0;
    while((c = fgetc(fp)) != EOF){
        putchar(c);
        char letter[2] = {0};
        letter[0] = c;
        strcat(file_info->contents, letter);
    } 
    fclose(fp);
    file_info->status = 1;
    shmdt(file_info);
    shmctl(shmid,IPC_RMID, NULL);

    return 0;
}