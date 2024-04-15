#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <sys/stat.h>
#include <pwd.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h>

#define BUFF_MAX 1024

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
    char path[BUFF_MAX];
    snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);
    
    struct stat file_stat;
    if(stat(path, &file_stat) == -1){
        perror("Error getting file status");
        return;
    }

    char* last_modified = ctime(&file_stat.st_mtime);
    last_modified[strlen(last_modified) - 1] = '\0';
    
    printf("%s %ld %s %s %ld %s %s\n",
        add_dir_to_perms(S_ISDIR(file_stat.st_mode),octal_to_str(file_stat.st_mode & 0777)),
        file_stat.st_nlink,
        getpwuid(file_stat.st_uid)->pw_name,
        getpwuid(file_stat.st_gid)->pw_name,
        file_stat.st_size,
        last_modified,
        entry->d_name
    );
}

void list(struct dirent* entry){
    printf("%s ", entry->d_name);
}

int entry_cmp(const void* a, const void* b){
    struct dirent* entry_a = (struct dirent*) a;
    struct dirent* entry_b = (struct dirent*) b;
    return strcasecmp(entry_a->d_name, entry_b->d_name);
}

void ls(const char* dirname, bool recurse, bool verbose){
    char path[BUFF_MAX];
    DIR* dir = opendir(dirname);
    char* subdir_names[BUFF_MAX];
    unsigned subdir_num = 0;
    if(dir == NULL) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }
    struct dirent* entry;

    struct dirent entries[BUFF_MAX];
    unsigned entry_count = 0;

    struct stat file_stat;

    
    if(recurse) printf("%s:\n", dirname);
    while((entry = readdir(dir)) != NULL){

        if(entry->d_name[0] == '.') 
            continue;

        entries[entry_count++] = *entry;

    }

    qsort(entries, entry_count, sizeof(struct dirent), &entry_cmp);
    
    for(unsigned i = 0; i < entry_count; i++){
        entry = &entries[i];
        if(verbose) 
            list_verbose(dirname, entry);
        else 
            list(entry);

        snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);
        if(stat(path, &file_stat) == -1){
            perror("Error getting file status");
            return;
        }
        if(S_ISDIR(file_stat.st_mode)) subdir_names[subdir_num++] = strdup(path);
    }

    printf("\n\n");
    for(unsigned i = 0; recurse && i < subdir_num; i++) {
        ls(subdir_names[i], recurse, verbose);
        free(subdir_names[i]);
    }

    closedir(dir);
}


int main(int argc, char* argv[]){
    bool VERBOSE = false;
    bool RECURSIVE = false;

    if(argc < 1|| argc > 3){
        fprintf(stderr, "Usage: %s [-l] <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int opt;
    while ((opt = getopt(argc, argv, "lR")) != -1) {
        switch (opt) {
            case 'l':
                VERBOSE = true;
                break;
            case 'R':
                RECURSIVE = true;
                break;
            default:  
                fprintf(stderr, "Usage: %s [-lR] <directory>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    char* dirname;
    char buff[1024];
    if (optind < argc)
        dirname = argv[optind];
    else if ((argc == 3 && RECURSIVE && VERBOSE) ||(argc == 2 && RECURSIVE) || (argc == 2 && VERBOSE) || (argc == 1)){
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

    ls(dirname, RECURSIVE, VERBOSE);

    return 0;
}
