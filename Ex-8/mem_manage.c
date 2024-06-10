#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/linked_list.c"
#include "../utils/integer-input.c"
#define BUFF_SIZE 2048


void print_free(ListNode** free_pool){
    if(*free_pool == NULL){
        printf("NULL\n");
        return;
    }

    ListNode* curr = *free_pool;
    while(curr != NULL){
        printf("|H      ");
        curr = curr->next;        
    }
    printf("|\n");
    curr = *free_pool;
    while(curr->next != NULL){
        printf("%-8d", curr->startaddr);        
        curr = curr->next;
    }
    printf("%-8d", curr->startaddr);        
    printf("%d\n", curr->endaddr);
}

void print_allocated(ListNode** allocated){
    if(*allocated == NULL){
        printf("NULL\n");
        return;
    }

    ListNode* curr = *allocated;
    int i = 0;
    while(curr != NULL){
        printf("|P%-8d", curr->pid);        
        curr = curr->next;
    }
    curr = *allocated;
    printf("|\n");
    while(curr->next != NULL){
        printf("%-8d", curr->startaddr);        
        curr = curr->next;
    }
    printf("%-8d", curr->startaddr);
    printf("%d\n", curr->endaddr);
}

void print_mem(ListNode** free_pool, ListNode** allocated){
    ListNode* curr_f = *free_pool;
    ListNode* curr_a = *allocated;

    int total_len = len(free_pool) + len(allocated);
    int indices[total_len + 1];
    int count = 0;
    while(curr_a != NULL && curr_f != NULL){
        if(curr_a->startaddr < curr_f->startaddr){
            printf("P%-8d", curr_a->pid);
            indices[count++] = curr_a->startaddr;
            curr_a = curr_a->next;
        }

        else{
            printf("|H      ");
            indices[count++] = curr_f->startaddr;
            curr_f = curr_f->next;
        }
    }

    while(curr_a != NULL){
            printf("P%-8d", curr_a->pid);
            indices[count++] = curr_a->startaddr;
            curr_a = curr_a->next;
    }

    while(curr_f != NULL){
            printf("|H      ");
            indices[count++] = curr_f->startaddr;
            curr_f = curr_f->next;
    }
    printf("\n");
    for(int i = 0; i < count; i++){
        printf("%-8d", indices[i]);
    }
    printf("\n");
}

void allocate(char algo, ListNode** free_pool, ListNode** allocated){
    char buff[BUFF_SIZE];
    printf("What is the process id: ");
    int id;
    getIntegerFromStdin(&id);
    int size;
    printf("What is the size needed: ");
    getIntegerFromStdin(&size);
    ListNode* found_node;
    switch(algo){
        case 'f':
            found_node = find_size(free_pool, size);
            if(found_node == NULL){
                printf("No space left to allocate!\n");
                return;
            }

            insert_middle(allocated, found_node->startaddr, id, found_node->startaddr, found_node->startaddr + size);
            found_node->startaddr += size;
            break;
        case 'b':
            found_node = find_best(free_pool, size);
            if(found_node == NULL){
                printf("No space left to allocate!\n");
                return;
            }

            insert_middle(allocated, found_node->startaddr, id, found_node->startaddr, found_node->startaddr + size);
            found_node->startaddr += size;
            break;
        case 'w':
            found_node = find_worst(free_pool, size);
            if(found_node == NULL){
                printf("No space left to allocate!\n");
                return;
            }

            insert_middle(allocated, found_node->startaddr, id, found_node->startaddr, found_node->startaddr + size);
            found_node->startaddr += size;
            break;
        default:
            printf("Error!\n");
            break;
    }
}

void deallocate(ListNode** free_pool, ListNode** allocated){
    printf("What is the process id: ");
    int id;
    getIntegerFromStdin(&id);
    ListNode* del = del_id(allocated, id);
    if(del == NULL){
        printf("Process not present!\n");
        return;
    }
    insert_middle(free_pool, del->startaddr, del->pid, del->startaddr, del->endaddr);
    
}

void coalesce_hole(ListNode** free_pool){
    ListNode* curr = *free_pool;
    ListNode* next;
    while(curr != NULL && (next = curr->next) != NULL){
        if(curr->endaddr == next->startaddr){
            curr->endaddr = next->endaddr;
            curr->next = next->next;
            free(next);
        }else curr = curr->next;
    }
}

int main(){

    ListNode** free_pool = new_linked_list();
    ListNode** alloted = new_linked_list();


    char buff[BUFF_SIZE];
    printf("Enter the memory representation\n");
    printf("Enter the number of partitions in memory: ");
    int partitions;
    getIntegerFromStdin(&partitions);
    for(int i = 1; i < partitions + 1; i++){
        printf("Starting and ending address of partition %d: ",i);
        fgets(buff, sizeof(buff), stdin);
        int start = atoi(buff);
        int end = atoi(strchr(buff, ' '));
        insert_end(free_pool, i, start, end);
    }

    print_free(free_pool);
    print_allocated(alloted);
    print_mem(free_pool, alloted);
    char algo = 0;

algorithm:
    while(true){

        printf("1. First Fit\n");
        printf("2. Best Fit\n");
        printf("3. Worst Fit\n");
        printf("4. Exit from program\n");


        printf("Give your choice: ");
        int ch1;
        getIntegerFromStdin(&ch1);
        switch(ch1){
            case 1:
                algo = 'f';
                break;
            case 2:
                algo = 'b';
                break;
            case 3:
                algo = 'w';
                break;
            case 4:
                exit(0);
                break;
            default:
                printf("Give a valid choice!\n");
                goto algorithm;
                break;
        }


        while(true){
            printf("1. Entry/Allocate\n");
            printf("2. Exit/Deallocate\n");
            printf("3. Display\n");
            printf("4. Coalesce holes\n");
            printf("5. Back to algorithm\n");

            printf("Give your choice: ");
            int ch;
            getIntegerFromStdin(&ch);
            
            switch(ch){
                case 1:
                    allocate(algo, free_pool, alloted);
                    break;
                case 2:
                    deallocate(free_pool, alloted);
                    break;
                case 3:
                    print_free(free_pool);
                    print_allocated(alloted);
                    print_mem(free_pool, alloted);
                    break;
                case 4:
                    coalesce_hole(free_pool);
                    break;
                case 5:
                    goto algorithm;
                    break;
                default:
                    printf("Give a valid option!\n");
                    break;
            }
        }
    }
}