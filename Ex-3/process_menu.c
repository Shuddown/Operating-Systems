#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "SJF.c"
#include "fcfs.c"
#include "../utils/integer-input.c"
#include "../utils/char-input.c"
#include "SJF_preemptive.c"

void main(){
    bool EXIT = false;
    do{
        int option;
        void (*scheduler) (Process* processes[], uint16_t size);

        printf("CPU scheduling algorithms\n");
        printf("1. FCFS\n");
        printf("2. SJF\n");
        printf("3. Exit\n");
        printf("Enter your option: ");
        getIntegerFromStdin(&option);

        switch (option) {
            case 1:
                printf("FCFS CPU SCHEDULER\n");
                scheduler = &fcfs;
                break;
            
            case 2: 
                printf("SJF CPU scheduler\n");
                printf("a. Non Premptive SJF\n");
                printf("b. Premptive SJF\n");
                printf("Enter your option:");
                char valid_chars[] = {'a', 'b'};
                char letter = get_char(valid_chars,sizeof(valid_chars));
                switch (letter) {
                    case 'a':
                        scheduler = &SJF;
                        break;
                    
                    case 'b': 
                        scheduler = &SJF_premptive;
                        break;
                    

                    default:
                        printf("Not a valid option\n");
                        break;
                }
                break;

            case 3:
                EXIT = true;
                break;

            default:
                printf("Not a valid option!\n");
                break;
        }
        if(EXIT) return;
        int n;
        printf("Number of processes: ");
        getIntegerFromStdin(&n);
        Process** processes = get_processes(n);
        scheduler(processes, n);
        print_results(processes, n);
       
    }while(true);
}