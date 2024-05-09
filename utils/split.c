#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#pragma once

char** split(char* str, char* delim, int count) {
    if (count == 0) return NULL;

    char** strs = malloc(sizeof(char*) * count);
    if (strs == NULL) {
        return NULL;
    }

    char* tok = strtok(str, delim);
    int i = 0;
    while (tok != NULL && i < count) {
        strs[i] = strdup(tok);
        if (strs[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(strs[j]);
            }
            free(strs);
            return NULL;
        }
        tok = strtok(NULL, delim);
        i++;
    }
    return strs;
}
