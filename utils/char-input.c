#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "array_helpers.c"

static inline void CleanInputBuffer() 
{
	char c = 0;
	// Loop over input buffer and consume chars until buffer is empty
	while ((c = getchar()) != '\n' && c != EOF);
}

char get_char(char* valid_chars, int valid_count){
    while(true){
        char buff[1024] = {0};
        fgets(buff, sizeof(buff), stdin);
        if(in_char_array(buff[0], valid_chars, valid_count)) return buff[0];
        else{
            printf("Please give valid input!\n");
            CleanInputBuffer();
        }
    }
}