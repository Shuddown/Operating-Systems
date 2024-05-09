#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/integer-input.c"
#include "../utils/char-input.c"
#include "../utils/split.c"

#define BUFF_SIZE 4096

typedef struct {
    int process_count;
    int resource_count;
    int* available;
    int** allocated;
    int** max;
}Matrix;

Matrix* new_matrix(int n, int r){
    Matrix* mat = malloc(sizeof(Matrix));
    mat->process_count = n;
    mat->resource_count = r;
    mat->available = malloc(sizeof(int) * r);
    mat->allocated = malloc(sizeof(int*) * n);
    for(int i = 0; i < n; i++){
        mat->allocated[i] = malloc(sizeof(int) * r);
    }
    mat->max = malloc(sizeof(int*) * n);
    for(int i = 0; i < n; i++){
        mat->max[i] = malloc(sizeof(int) * r);
    }
    return mat;
}

Matrix* copy_matrix(Matrix* old_mat){
    if (old_mat == NULL) return NULL;
    Matrix* new_mat = new_matrix(old_mat->process_count, old_mat->resource_count);
    memcpy(new_mat->available, old_mat->available, sizeof(int) * old_mat->resource_count);
    memcpy(&new_mat->max[0][0], &old_mat->max[0][0], sizeof(int) * old_mat->resource_count * old_mat->process_count);
    memcpy(&new_mat->allocated[0][0], &old_mat->allocated[0][0], sizeof(int) * old_mat->resource_count * old_mat->process_count);
    return new_mat;
}

Matrix* get_matrix(int process_count, int resource_count){
    char buff[BUFF_SIZE];
    Matrix* mat = new_matrix(process_count, resource_count);

    for(int i = 0; i < resource_count; i++){
        printf("Give the number of available instances of %c: ", 'A' + i);
        getIntegerFromStdin(&mat->available[i]);
    }

    for(int i = 0; i < process_count; i++){
        printf("Maximium requirement for P%d: ", i);
        fgets(buff, sizeof(buff), stdin);
        char** strs = split(buff, " ", resource_count);
        for(int j = 0; j < resource_count; j++) mat->max[i][j] = atoi(strs[j]);
    }

    for(int i = 0; i < process_count; i++){
        printf("Allocated instances for P%d: ", i);
        fgets(buff, sizeof(buff), stdin);
        char** strs = split(buff, " ", resource_count);
        for(int j = 0; j < resource_count; j++) 
            mat->allocated[i][j] = atoi(strs[j]);
    }

    return mat;
}

void print_mat(Matrix* mat){
    char* space = " ";
    char resource[BUFF_SIZE];
    for(int i = 0; i < mat->resource_count; i++) 
        sprintf(resource + 2*i, "%c ", 'A' + i);
    resource[mat->resource_count * 2 - 1] = '\0';
    printf("Pid\tAlloc\tMax\tNeed\tAvail\n");
    printf("%s\t%s\t%s\t%s\t%s\n",
    space,resource,resource,resource,resource);
    for (int i = 0; i < mat->process_count; i++){
        printf("P%d\t", i);
        for (int j = 0; j < mat->resource_count; j++){
            printf("%d ", mat->allocated[i][j]);
        }
        printf("\t");
        for (int j = 0; j < mat->resource_count; j++){
            printf("%d ", mat->max[i][j]);
        }
        printf("\t");
        for (int j = 0; j < mat->resource_count; j++){
            printf("%d ", mat->max[i][j] - mat->allocated[i][j]);
        }
        printf("\t");
        for (int j = 0;i == 0 && j < mat->resource_count; j++){
            printf("%d ", mat->available[j]);
        }
        printf("\n");
    }
}

bool greater(int* avail, int* max, int* alloc, int size){
    for(int i = 0; i < size; i++){
        if(max[i] - alloc[i] > avail[i]) return false;
    }
    return true;
}

int process_avail(Matrix* mat){
    for(int i = 0; i < mat->process_count; i++){
        if(mat->allocated[i][0] == -1) continue;
        if(greater(mat->available, mat->max[i], mat->allocated[i], mat->resource_count))
            return i;
    }
    return -1;
}

bool bankers(Matrix* mat){
    int process_sequence[mat->process_count];
    for(int i = 0; i < mat->process_count; i++){
        int avail = process_avail(mat);
        if(avail == -1){
            printf("There is no safe sequence, it is in deadlock!\n");
            return false;
        }
        for(int j = 0; j < mat->resource_count; j++){
            mat->available[j] += mat->allocated[avail][j];
            mat->allocated[avail][j] = -1;
        }
        process_sequence[i] = avail;
    }
    printf("P%d", process_sequence[0]);
    for(int i = 1; i<mat->process_count; i++) 
        printf("->P%d", process_sequence[i]);
    printf("\n");
    return true;
}

void request(Matrix* mat){
    char buff[BUFF_SIZE];
    int request[mat->resource_count];
    int dummy[mat->resource_count];
    memset(dummy, 0, mat->resource_count*sizeof(int));
    printf("Enter the process id of the proecess requesting resources: ");
    fgets(buff, sizeof(buff), stdin);
    int id = (buff[0] == 'P') ? atoi(buff + 1) : atoi(buff);
    printf("Enter the request vector: ");
    fgets(buff, BUFF_SIZE, stdin);
    char** rs = split(buff," ",mat->resource_count);
    for(int i = 0; i < mat->resource_count; i++){
        request[i] = atoi(rs[i]);
    }

    if(!greater(mat->available, request, dummy, mat->resource_count)){
        printf("Requested resources are too much!\n");
        return;
    }

    for(int i = 0; i < mat->resource_count; i++){
        mat->allocated[id][i] += request[i];
        mat->available[i] -= request[i];
    }

    bool able =  bankers(mat);
    if(able){
        printf("Process's request can be granted immediatly\n");
    }
    else
        printf("The request cannot be granted.\n");
}


int main(){
    Matrix* mat = NULL;
    while (true){


        printf("Banker's Algorithm: \n");
        printf("1. Read Data\n");
        printf("2. Print Data\n");
        printf("3. Safety Sequence\n");
        printf("4. Process Request\n");
        printf("5. Exit\n");

        printf("Enter the option: ");
        char op = get_char("12345", sizeof("12345"));
        switch (op){
        case '1':
            int n, r;
            printf("Give the number of processes: ");
            getIntegerFromStdin(&n);
            printf("Give the number of resources: ");
            getIntegerFromStdin(&r);
            mat = get_matrix(n, r);
            break;
        case '2':
            if(mat == NULL) {
                printf("Nothing to display\n");
                break;
            }
            print_mat(mat);
            break;
        case '3':
            if(mat == NULL) {
                printf("Nothing to display\n");
                break;
            }
            bankers(copy_matrix(mat));
            break;
        case '4':
            if(mat == NULL) {
                printf("Nothing to display\n");
                break;
            }
            request(mat);
            break;
        default:
            exit(0);
        }
    }

    return 0;
}
