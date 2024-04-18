#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/priority_process.c"
#include <stdint.h>


int main() {

    printf("%ld", sizeof(void*));
    return 0;

  // Sort processes by arrival time using qsort qsort(processes, 3, sizeof(Priority_Process*), time_cmp);

}