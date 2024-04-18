#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "Priority.c"
#include "Priority-Non.c"
#include "../utils/priority_process.c"
#include "../utils/integer-input.c"
#include "../utils/char-input.c"
#include "Round-Robin.c"

void main(){
    bool EXIT = false;
    do{
        int option;
        int n;

        printf("CPU scheduling algorithms\n");
        printf("1. Round Robin\n");
        printf("2. Priority\n");
        printf("3. Exit\n");
        printf("Enter your option: ");
        getIntegerFromStdin(&option);

        switch (option) {
            case 1:
                int quanta;
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

                printf("Priority Scheduler\n");
                printf("a. Non Premptive Priority\n");
                printf("b. Premptive Priority\n");
                printf("Enter your option: ");
                char valid_chars[] = {'a', 'b'};
                char letter = get_char(valid_chars,sizeof(valid_chars));
                printf("What is the number of processes: ");
                getIntegerFromStdin(&n);
                struct priority_process** processs = get_processes_priority(n);

                switch (letter) {

                    case 'a':
                        priority(processs, n);
                        print_results_priority(processs, n);
                        break;
                    
                    case 'b': 
                        priority_preemptive(processs, n);
                        print_results_priority(processs, n);
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
       
    }while(!EXIT);
}