#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <limits.h>

char* octal_to_str(int permissions){
    char octal_str[4];
    sprintf(octal_str, "%o", permissions);
    char perm_str[10] = {0};
    for (int i = 0; i < 3; i++){
        switch (octal_str[i]) {
        case '0':
            strcat(perm_str, "---");
            break;
        case '1':
            strcat(perm_str, "--x");
            break;
        case '2':
            strcat(perm_str, "-w-");
            break;
        case '3':
            strcat(perm_str, "-wx");
            break;
        case '4':
            strcat(perm_str, "r--");
            break;
        case '5':
            strcat(perm_str, "r-x");
            break;
        case '6':
            strcat(perm_str, "rw-");
            break;
        case '7':
            strcat(perm_str, "rwx");
            break;
        
        default:
            break;
        }
    }
    return strdup(perm_str);
}

char* add_dir_to_perms(bool is_dir,char* perms){
    char final[11] = {0};
    is_dir ? strcat(final, "d") : strcat(final, "-");
    return strdup(strcat(final, perms));
}

void list_verbose(const char *dirname, struct dirent* entry){
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);
    
    struct stat file_stat;
    if(stat(path, &file_stat) == -1){
        perror("Error getting file status");
        return;
    }

    printf("%s %ld %s %s %ld %s %s\n",
        add_dir_to_perms(S_ISDIR(file_stat.st_mode),octal_to_str(file_stat.st_mode & 0777)),
        file_stat.st_nlink,
        getpwuid(file_stat.st_uid)->pw_name,
        getpwuid(file_stat.st_gid)->pw_name,
        file_stat.st_size,
        ctime(&file_stat.st_mtime),
        entry->d_name
    );
}

void list(struct dirent* entry){
    printf("%s\n", entry->d_name);
}


int main(int argc, char* argv[]){
    bool VERBOSE = false;

    if(argc < 1|| argc > 3){
        fprintf(stderr, "Usage: %s [-l] <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int opt;
    while ((opt = getopt(argc, argv, "l")) != -1) {
        switch (opt) {
            case 'l':
                VERBOSE = true;
                break;
            default:  
                fprintf(stderr, "Usage: %s [-l] <directory>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    char* dirname;
    char buff[1024];
    if (optind < argc)
        dirname = argv[optind];
    else if (argc == 2 && VERBOSE || argc == 1){
        dirname = getcwd(buff, sizeof(buff));
    }else {
        fprintf(stderr, "No directory specified\n");
        exit(EXIT_FAILURE);
    }

    struct stat path_stat;
    if (stat(dirname, &path_stat) == -1) {
        perror("Error accessing directory");
        exit(EXIT_FAILURE);
    }

    if (!S_ISDIR(path_stat.st_mode)) {
        fprintf(stderr, "%s is not a directory\n", dirname);
        exit(EXIT_FAILURE);
    }

    DIR* dir = opendir(dirname);
    if(dir == NULL) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    while((entry = readdir(dir)) != NULL){
        if(VERBOSE) 
            list_verbose(dirname, entry);
        else 
            list(entry);
    }

    closedir(dir);
    return 0;
}
