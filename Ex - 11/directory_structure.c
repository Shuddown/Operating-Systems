#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

typedef struct File {
    char name[64];
    int address;
    int size;
} File;

typedef struct Directory {
    File files[3];
    int num_files;
    File directories[3];
    int num_directories; 
} Directory;

#define BUF_SIZE 4096

char *input(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	vprintf(msg, args);
	va_end(args);
	static char buf[4096];
	fgets(buf, BUF_SIZE, stdin);
	buf[strlen(buf) - 1] = '\0';
	return buf;
}

int getIndex(File files[], int num_files, const char *dirname) {
    for(int i = 0; i < num_files; i++) {
        if (strcmp(dirname, files[i].name) == 0) return i;
    }
    return -1;
}

void print_files(File files[], int num_files) {
    printf("Name\tAddress\tSize\n");
    for(int i = 0; i < num_files; i++) {
        File *file = &files[i];
        printf("%s\t%d\t%d\n", file->name, file->address, file->size);
    }
}

void print_directory(char memory[], Directory *cur, const char *name) {
    printf("\n");
    printf("%s:\n", name);
    printf("Directories:\n");
    print_files(cur->directories, cur->num_directories);

    printf("\n");
    printf("Files:\n");
    print_files(cur->files, cur->num_files);

    for(int i = 0; i < cur->num_directories; i++) {
        Directory *next = (Directory *)(memory+cur->directories[i].address);
        print_directory(memory, next, cur->directories[i].name);
    }
}


int main() {
    srand(time(NULL));
    printf("1. Single Level Directory\n");
    printf("2. Tree Structured Directory\n");
    int opt = atoi(input("Enter your option: "));
    int cur_pos;
    switch(opt) {
        case 1:
            File directory[BUF_SIZE];
            int num_files = 0;
            cur_pos = 0;
            while(true) {
                printf("1. Create File\n");
                printf("2. List Files\n");
                printf("3. Quit\n");
                int opt2 = atoi(input("Enter an option: "));
                switch(opt2) {
                    case 1:
                        const char *name = input("Enter a file name: ");
                        if (getIndex(directory, num_files, name) != -1) {
                            printf("Error! File with this name already exists!\n");
                            continue;
                        }
                        strcpy(directory[num_files].name, name);
                        directory[num_files].address = cur_pos;
                        directory[num_files].size += rand() % BUF_SIZE + 1;
                        cur_pos += directory[num_files].size;// File size is randomly chosen, b/w 1 byte and 4 KB                    
                        num_files += 1;
                    break;

                    case 2:
                        printf("\n");
                        print_files(directory, num_files);
                    break;

                    default:
                        exit(0);
                    break;
                }
            }
        break;

        case 2:
            char *memory = (char *)malloc(16777216); // 16 MB
            Directory *root = (Directory *)memory;
            root->num_files = 0;
            root->num_directories = 0;
            cur_pos = sizeof(Directory);
            while(true) {
                outerLoop:
                printf("1. Create File\n");
                printf("2. List Files\n");
                printf("3. Quit\n");
                int opt2 = atoi(input("Enter an option: "));
                switch(opt2) {
                    case 1:
                        char *path = input("Enter a file name: ");
                        Directory *cur = root;
                        const char *cur_name = "root";
                        int path_length = strlen(path);
                        char *path_part = strtok(path, "/");
                        while(strlen(path_part) + (path_part - path) < path_length) {
                            int ind = getIndex(cur->directories, cur->num_directories, path_part);
                            if(ind == -1) {
                                if(cur->num_directories == 3) {
                                    printf("Error, can't create directory %s in directory %s, already 3 directories\n", path_part, cur_name);
                                    goto outerLoop;
                                }

                                File *next_dir_entry = &cur->directories[cur->num_directories];
                                strcpy(next_dir_entry->name, path_part);
                                next_dir_entry->address = cur_pos;
                                next_dir_entry->size = sizeof(Directory);
                                Directory *next_dir = (Directory *)(memory+cur_pos);
                                next_dir->num_files = 0;
                                next_dir->num_directories = 0;
                                cur_pos += next_dir_entry->size;
                                ind = cur->num_directories;
                                cur->num_directories+=1;
                            }
                            int dir_addr = cur->directories[ind].address;
                            cur_name = cur->directories[ind].name;
                            cur = (Directory *)(memory + dir_addr);
                            path_part = strtok(NULL, "/");
                        }
                        
                        int ind = getIndex(cur->files, cur->num_files, path_part);
                        if(ind != -1) {
                            printf("File already exists!\n");
                            goto outerLoop;
                        } 
                        if(cur->num_files == 3) {
                            printf("Error, can't create file %s in directory %s, already 3 files\n", path_part, cur_name);
                            goto outerLoop;
                        }
                        File *file_entry = &cur->files[cur->num_files];
                        strcpy(file_entry->name, path_part);
                        file_entry->size = rand() % BUF_SIZE + 1;
                        file_entry->address = cur_pos;
                        cur_pos += file_entry->size;
                        cur->num_files += 1;
                    break;

                    case 2:
                        print_directory(memory, root, "root");
                    break;

                    default:
                        exit(0);
                    break;
                }
            }
        break;
    }
    return 0;
}