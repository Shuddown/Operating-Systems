#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "Priority.c"
#include "Priority-Non.c"
#include "../utils/priority_process.c"
#include "../utils/integer-input.c"
#include "Round-Robin.c"

void menu_scheduler(){
    bool EXIT = false;
    do{
        uint16_t option;

        printf("CPU scheduling algorithms\n");
        printf("1. Round Robin\n");
        printf("2. Priority\n");
        printf("3. Exit\n");
        printf("Enter your option: ");
        getIntegerFromStdin(&option);

        switch (option) {
            case 1:
                int quanta;
                int n;
                printf("Round Robin Scheduler\n");
                printf("What is the time quanta you'd like: ");
                getIntegerFromStdin(&quanta);
                printf("What is the number of processes: ");
                getIntegerFromStdin(&n);
                Process** processes = get_processes(n);
                round_robin(processes, n, quanta);
                print_results(processes, n);
                break;
            
            case 2: 
                int n;
                char letter;

                printf("Priority Scheduler");
                printf("a. Non Premptive Priority\n");
                printf("b. Premptive Priority\n");
                printf("Enter your option:");
                letter = getchar();
                switch (letter) {

                    case 'a':
                        printf("What is the number of processes: ");
                        getIntegerFromStdin(&n);
                        struct priority_process** processes = get_processes_priority(n);
                        priority(processes, n);
                        print_results_priority(processes, n);
                        break;
                    
                    case 'b': 
                        printf("What is the number of processes: ");
                        getIntegerFromStdin(&n);
                        struct priority_process** processes = get_processes_priority(n);
                        priority_preemptive(processes, n);
                        print_results_priority(processes, n);
                    

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
       
    }while(!EXIT);
}