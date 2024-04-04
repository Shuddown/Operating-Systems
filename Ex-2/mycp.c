#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "utils/command_utils.c"

#define SIZE(arr) sizeof(arr)/sizeof(arr[0])

char* directorify(char* dir_name){
    if (dir_name[strlen(dir_name) - 1] == '/') return dir_name;
    char* corrected_name = (char*) malloc(strlen(dir_name) + 2);
    *corrected_name = '\0';
    return strcat(strcat(corrected_name, dir_name), "/");
}

char* get_full_path(char* dir_name, char* file_name){
    dir_name = directorify(dir_name);
    char* full_path = (char*) malloc(strlen(dir_name) + strlen(file_name) + 1);
    strcpy(full_path, dir_name);
    strcat(full_path, file_name);
    return full_path;
}

void copy(int fd1, int fd2){
    int count;
    char buffer[1024];
    while ((count = read(fd1, buffer, sizeof(buffer))) != 0)
        write(fd2, buffer, count);
}

int main(int argc, char* argv[]){
    char valid_tags[] = {'i'};
    command_info* info = (argc, argv, valid_tags, SIZE(valid_tags));
    if (info->arg_count < 3){
        fprintf(stdout, "The cp command requires two or more arguments!\n");
        return 1;
    }
    bool CHECK = in_char_array('i', info->options, info->op_count);
    char* dst = info->args[info->arg_count - 1];
    struct stat st;
    if (stat(dst, &st) == 0 && S_ISDIR(st.st_mode)){ 
        for (int i = 1; i < info->arg_count - 1; i++){
            char* new_file_path = get_full_path(dst, info->args[i]);
            int fd1 = open(info->args[i], O_RDONLY);
            if (fd1 == -1){
                fprintf(stdout, "The file %s does not exist!\n", info->args[i]);
                return 1;
            }
            if((stat(new_file_path, &st) == 0) && CHECK){
                char ch[1];
                printf("The file %s already exists. Do you want to overwrite it? (y/n) \n", new_file_path);
                fgets(ch, 1, stdin);
                if(ch[0] == 'n') continue;
            }
            int fd2 = open(new_file_path, O_WRONLY | O_CREAT, 0664);
            copy(fd1, fd2);
            close(fd1); 
            close(fd2); 
            free(new_file_path); 
	}
    } else {
        if (info->arg_count != 3){
            fprintf(stdout, "%s is not a directory!\n", dst);
            return -1;
        } else {
            int fd1 = open(info->args[1], O_RDONLY);
            if (fd1 == -1){
                fprintf(stdout, "The file %s does not exist!\n", info->args[1]);
                return 1;
            }
            if((stat(info->args[2], &st) == 0) && CHECK){
                char ch[1];
                printf("The file %s already exists. Do you want to overwrite it? (y/n) \n", info->args[2]);
                fgets(ch, 1, stdin);
                if(ch[0] == 'n') return 0;
            }
            int fd2 = open(info->args[2], O_WRONLY | O_CREAT, 0664);
            copy(fd1, fd2);
            close(fd1); 
            close(fd2); 
        }
    }
    return 0;
}

