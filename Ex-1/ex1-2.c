#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(){
    pid_t child = fork();
    if(child > 0) {
        printf("In parent:\n");
        printf("Id of child process: %d\n", child);
        printf("Waiting for child to finish!\n");
        int wstatus;
        pid_t dead_child = wait(&wstatus);
        printf("Child %d has terminated!\n", dead_child);
    }
    
    else {
        printf("In child:\n");
        printf("Id of parent process: %d\n", getppid());
        printf("Now child will sleep for 10 sec.\n");
        sleep(10);
        printf("Done sleeping!, Now Terminating!\n");
    }

    return 0;
}