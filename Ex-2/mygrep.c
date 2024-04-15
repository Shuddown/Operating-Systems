#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include "../utils/strtoke.c"

int check_line_pattern(const char* pattern, const char* filepath, bool invert, bool number, bool count){
    int fd = open(filepath, O_RDONLY);
    if(fd == -1){
        perror("Couldn't Open File!\n");
        return -1;
    } 
    int line_num = 1;
    char buff[1024];
    int match_count = 0;
    while(read(fd, buff, sizeof(buff)) != 0){
        char * token = strtoke(buff, "\n");
        while( token != NULL ) {
            if((strstr(token, pattern) != NULL) ^ invert){ 
                if(!count) 
                number ? printf("%d: %s\n",line_num, token) : printf("%s\n", token);
                match_count++;
            }
            token = strtoke(NULL, "\n");
            line_num++;
        }
    }
    return match_count;
}

int main(int argc, char* argv[]){

    bool count = false;
    bool number = false;
    bool invert = false;

    int opt;
    while ((opt = getopt(argc, argv, "cvn")) != -1) {
        switch (opt) {
            case 'c':
                count = true;
                break;
            case 'n':
                number = true;
                break;
            case 'v':
                invert = true;
                break;
            default:  
                fprintf(stderr, "Usage: %s [-cvn] <directory>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if(argc - optind != 2){
        perror("Two arguments are required!");
        exit(EXIT_FAILURE);
    }
    char* pattern = argv[optind];
    char* filepath = argv[++optind]; 
    int match_count = check_line_pattern(pattern, filepath, invert, number, count);
    if(count) printf("%d\n", match_count);
    return 0;
}