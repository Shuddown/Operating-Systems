#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "SJF.c"
#include "q1.c"
#include "../utils/integer-input.c"
#include "SJF_preemptive.c"

void menu_scheduler(){
    bool EXIT = false;
    do{
        uint16_t option;
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
                char letter;

                printf("SJF CPU scheduler\n");
                printf("a. Non Premptive SJF\n");
                printf("b. Premptive SJF\n");
                printf("Enter your option:");
                letter = getchar();
                switch (letter) {
                    case 'a':
                        scheduler = &SJF;
                        break;
                    
                    case 'b': 
                        scheduler = &SJF_premptive;
                    

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
        if(!EXIT) return;
        uint16_t n;
        printf("Number of processes: ");
        getIntegerFromStdin(&n);
        Process** processes = get_processes(n);
        scheduler(processes, n);
        print_results(processes, n);
       
    }while(true);
}