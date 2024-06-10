#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/integer-input.c"

#define BUFF_SIZE 1024

void read_input(int* max_frames, int* process_frames, char** str) {
    char buff[BUFF_SIZE];
    printf("What is the number of frames possible: ");
    getIntegerFromStdin(max_frames);
    printf("What is the number of frames required by process: ");
    getIntegerFromStdin(process_frames);
    printf("What is the process frame query order: ");
    fgets(buff, sizeof(buff), stdin);
    buff[strcspn(buff, "\n")] = '\0';
    *str = strdup(buff);
}

typedef struct node {
    int data;
    int count;
    struct node* next;
} ListNode;

ListNode* create_node(int val, ListNode* next) {
    ListNode* new_node = (ListNode*) malloc(sizeof(ListNode));
    new_node->data = val;
    new_node->count = 0;
    new_node->next = next;
    return new_node;
}

ListNode** new_linked_list() {
    ListNode** head = (ListNode**) malloc(sizeof(ListNode*));
    *head = NULL;
    return head;
}

void insert_begin(ListNode** head, int data) {
    *head = create_node(data, *head);
}

void insert_middle(ListNode** head, size_t index, int data) {
    ListNode** tracer = head;
    for (size_t i = 0; i < index && *tracer; i++) {
        tracer = &(*tracer)->next;
    }
    *tracer = create_node(data, *tracer);
}

void change_middle(ListNode** head, size_t index, int data) {
    ListNode** tracer = head;
    for (size_t i = 0; i < index && *tracer; i++) {
        tracer = &(*tracer)->next;
    }
    if (*tracer != NULL) (*tracer)->data = data;
}

void insert_end(ListNode** head, int data) {
    ListNode** tracer = head;
    while (*tracer != NULL) {
        tracer = &(*tracer)->next;
    }
    *tracer = create_node(data, *tracer);
}

void print_list(ListNode** head, int fault_no, int val) {
    printf("%7d\t", val);
    ListNode* curr = *head;
    while (curr != NULL) {
        (curr->data != -1) ? printf("%d ", curr->data) : printf("- ");
        curr = curr->next;
    }
    printf("\t%d ", fault_no);
    printf("\n");
}

int search(ListNode** head, int data) {
    if (*head == NULL) return -1;

    ListNode** tracer = head;
    size_t i;
    for (i = 0; *tracer && (*tracer)->data != data; i++) {
        tracer = &(*tracer)->next;
    }
    return ((*tracer) == NULL) ? -1 : i;
}

void del_head(ListNode** head) {
    if (*head == NULL) return;

    ListNode* old_head = *head;
    *head = head[0]->next;
    free(old_head);
}

void del_middle(ListNode** head, size_t index) {
    if (*head == NULL) return;

    ListNode** tracer = head;
    for (size_t i = 0; i < index && *tracer; i++) {
        tracer = &(*tracer)->next;
    }
    ListNode* old_node = *tracer;
    *tracer = (*tracer)->next;
    free(old_node);
}

void del_end(ListNode** head) {
    if (*head == NULL) return;

    ListNode** tracer = head;
    while ((*tracer)->next != NULL) {
        tracer = &(*tracer)->next;
    }
    ListNode* old_node = *tracer;
    *tracer = NULL;
    free(old_node);
}

void del_list(ListNode** head) {
    ListNode* curr = *head;
    while (curr != NULL) {
        ListNode* next = curr->next;
        free(curr);
        curr = next;
    }
    free(head);
}

void FIFO(ListNode** frames, int frames_num, int process_num, char* ref_str) {
    int most_recent = 0;
    int fault_no = 0;
    char c;
    printf("Page ref \t memory \t PF\n");
    while ((c = *ref_str++) != '\0') {
        int val = ((int)c) - 48;
        int index = search(frames, val);
        if (index == -1) {
            change_middle(frames, most_recent, val);
            most_recent = (most_recent + 1) % frames_num;
            fault_no++;
        }
        print_list(frames, fault_no, val);
    }
    return;
}

int find_farthest(ListNode** frames, int frames_num, int* ref_str, int ref_len, int current_index) {
    int max_distance = -1;
    int frame_to_replace = 0;
    for (int i = 0; i < frames_num; i++) {
        ListNode* node = *frames;
        for (int j = 0; j < i; j++) {
            node = node->next;
        }
        int data = node->data;
        int distance = 0;
        for (int j = current_index + 1; j < ref_len; j++) {
            if (ref_str[j] == data) break;
            distance++;
        }
        if (distance > max_distance) {
            max_distance = distance;
            frame_to_replace = i;
        }
    }
    return frame_to_replace;
}

void optimal(ListNode** frames, int frames_num, int process_num, char* ref_str) {
    int fault_no = 0;
    int ref_len = strlen(ref_str);
    int ref_int[ref_len];
    for (int i = 0; i < ref_len; i++) {
        ref_int[i] = ref_str[i] - '0';
    }

    printf("Page ref \t memory \t PF\n");
    for (int i = 0; i < ref_len; i++) {
        int val = ref_int[i];
        int index = search(frames, val);
        if (index == -1) {
            if (i < frames_num) {
                change_middle(frames, i, val);
            } else {
                int replace_index = find_farthest(frames, frames_num, ref_int, ref_len, i);
                change_middle(frames, replace_index, val);
            }
            fault_no++;
        }
        print_list(frames, fault_no, val);
    }
    return;
}

void LRU(ListNode** frames, int frames_num, int process_num, char* ref_str) {
    int fault_no = 0;
    int time[frames_num];
    memset(time, 0, sizeof(time));
    int clock = 0;
    char c;

    printf("Page ref \t memory \t PF\n");
    while ((c = *ref_str++) != '\0') {
        int val = ((int)c) - 48;
        int index = search(frames, val);
        if (index == -1) {
            int lru = 0;
            for (int i = 1; i < frames_num; i++) {
                if (time[i] < time[lru]) {
                    lru = i;
                }
            }
            change_middle(frames, lru, val);
            time[lru] = ++clock;
            fault_no++;
        } else {
            time[index] = ++clock;
        }
        print_list(frames, fault_no, val);
    }
    return;
}

void LFU(ListNode** frames, int frames_num, int process_num, char* ref_str) {
    int fault_no = 0;
    int freq[frames_num];
    memset(freq, 0, sizeof(freq));
    char c;

    printf("Page ref \t memory \t PF\n");
    while ((c = *ref_str++) != '\0') {
        int val = ((int)c) - 48;
        int index = search(frames, val);
        if (index == -1) {
            int lfu = 0;
            for (int i = 1; i < frames_num; i++) {
                if (freq[i] < freq[lfu]) {
                    lfu = i;
                }
            }
            change_middle(frames, lfu, val);
            freq[lfu] = 1;
            fault_no++;
        } else {
            freq[index]++;
        }
        print_list(frames, fault_no, val);
    }
    return;
}

int main() {
    int frames_num, process_num;
    frames_num = 1;
    char* ref_str;
    ListNode** frames = new_linked_list();

    while (true) {
        printf("PAGE REPLACEMENT ALGORITHMS\n 1. READ_INPUT\n 2. FIFO\n 3. OPTIMAL\n 4. LRU\n 5. LFU\n 6. EXIT\n");
        int ch;
        printf("Enter your option: ");
        getIntegerFromStdin(&ch);
        ListNode** frames = new_linked_list();
        for (int i = 0; i < frames_num; i++) {
            insert_begin(frames, -1);
        }

        switch (ch) {
            case 1:
                read_input(&frames_num, &process_num, &ref_str);
                for (int i = 0; i < frames_num; i++) {
                    insert_begin(frames, -1);
                }
                break;
            case 2:
                FIFO(frames, frames_num, process_num, ref_str);
                break;
            case 3:
                optimal(frames, frames_num, process_num, ref_str);
                break;
            case 4:
                LRU(frames, frames_num, process_num, ref_str);
                break;
            case 5:
                LFU(frames, frames_num, process_num, ref_str);
                break;
            case 6:
                exit(0);
                break;
            default:
                printf("Give a valid option!\n");
                break;
        }
    }
    return 0;
}
