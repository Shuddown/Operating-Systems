#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/linked_list.c"
#include "../utils/integer-input.c"
#define BUFF_SIZE 2048


void print_free(ListNode** free_pool){
    ListNode* curr = *free_pool;
    while(curr != NULL){
        printf("|H      ");        
    }
    printf("|\n");
    while(curr != NULL){
        printf("%-6d", curr->startaddr);        
    }
    printf("%d\n", curr->endaddr);
}
void print_allocated(ListNode** allocated){
    ListNode* curr = *allocated;
    int i = 0
    while(curr != NULL){
        printf("|H      ");        
    }
    printf("|\n");
    while(curr != NULL){
        printf("%-6d", curr->startaddr);        
    }
    printf("%d\n", curr->endaddr);
}

int main(){

    ListNode** free_pool = new_linked_list();
    ListNode** alloted = new_linked_list();


    char buff[BUFF_SIZE];
    printf("Enter the memory representation: ");
    printf("Enter the number of partitions in memory: ");
    int partitions;
    getIntegerFromStdin(&partitions);
    for(int i = 1; i < partitions + 1; i++){
        printf("Starting and ending address of partition %d: ",i);
        fgets(buff, sizeof(buff), stdin);
        int start = atoi(buff);
        int end = atoi(buff);
        insert_end(free_pool, i, start, end);
    }

    print_free(free_pool);

    while(true){
        printf("1. Entry/Allocate\n");
        printf("2. Exit/Deallocate\n");
        printf("3. Display\n");
        printf("4. Coalesce holes\n");



    }
}