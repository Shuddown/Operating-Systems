#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int fact(int num){
	if (num == 0) return 1;
	return num * fact(num - 1);
}

void print_series(int num){
	for(int i = 1; i <= num; i++){
		printf("%d\n", i);
	}
}


int main(int arg_c, char* arg_v[]){
	if (arg_c != 3 || (atoi(arg_v[1]) == 0 && strcmp(arg_v[1], "0") != 0) || (atoi(arg_v[2]) == 0 &&  strcmp(arg_v[2], "0") != 0)){
	       	printf("You must give two number arguments!\n");
		exit(1);
	}

	int p_id = fork();

	if(p_id){
		printf("In the parent process!\n");
		wait(NULL);
		printf("In parent once more!\n");
		printf("Now printing series.\n");
		int series_end = atoi(arg_v[2]);
		print_series(series_end);
	}else{
		printf("In the child process!\n");
		int fact_num = atoi(arg_v[1]);
		printf("The factorial of %d is %d.\n", fact_num, fact(fact_num));
	}
	return 0;
}

		

	
