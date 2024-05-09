#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct node{
    int pid;
    int startaddr;
    int endaddr;
    struct node* next;
}ListNode;

ListNode* create_node(int id,int start,int end, ListNode* next){
    ListNode* new_node = (ListNode*) malloc(sizeof(ListNode));
    new_node->pid = id;
    new_node->startaddr = start;
    new_node->endaddr = end;
    new_node->next = next;
    return new_node;
}


ListNode** new_linked_list(){
    ListNode** head = (ListNode**) malloc(sizeof(ListNode*));
    *head = NULL;
    return head;
}

void insert_begin(ListNode** head, int id,int start,int end){
    *head = create_node(id, start, end, *head);
}

void insert_middle(ListNode** head, size_t index, int id, int start, int end){
    ListNode** tracer = head;
    for(size_t i = 0; i<index && *tracer; i++){
        tracer = &(*tracer)->next;
    }
    *tracer = create_node(id, start, end, *tracer);
}

void insert_end(ListNode** head, int id, int start, int end){
    ListNode** tracer = head;
    while(*tracer != NULL){
        tracer = &(*tracer)->next;
    }
    *tracer = create_node(id, start, end, *tracer);
}

void print(ListNode** head){
    ListNode* curr = *head;
    while(curr != NULL){
        printf("%d %d %d", curr->pid, curr->startaddr, curr->endaddr);
        curr = curr->next;
    }
    printf("\n");
}

void del_head(ListNode** head){
    if(*head == NULL) return;

    ListNode* old_head = *head;
    *head = head[0]->next;
    free(old_head);
}

void del_middle(ListNode** head, size_t index){
    if(*head == NULL) return;

    ListNode** tracer = head;
    for(size_t i = 0; i<index && *tracer; i++){
        tracer = &(*tracer)->next;
    }        
    ListNode* old_node = *tracer;
    *tracer = (*tracer)->next;
    free(old_node);
}

ListNode* del_id(ListNode** head, int id){
    if(*head == NULL) return;

    ListNode** tracer = head;
    while((*tracer) && (*tracer)->pid != id){
        tracer = &(*tracer)->next;
    }
    if((*tracer) == NULL) return NULL;
    ListNode* found_val = *tracer;
    *tracer = (*tracer)->next;
    return found_val;
}

void del_end(ListNode** head){
    if(*head == NULL) return;    

    ListNode** tracer = head;
    while((*tracer)->next != NULL){
        tracer = &(*tracer)->next;
    }
    ListNode* old_node = *tracer;
    *tracer = NULL;
    free(old_node);
}

void del_list(ListNode** head){
    ListNode* curr = *head;
    while(curr != NULL){
        ListNode* next = curr->next;
        free(curr);
        curr = next;
    }
    free(head);
}


    