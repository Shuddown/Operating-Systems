#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SIZE(arr) sizeof(arr)/sizeof(arr[0])

bool in_int_array(int num, int* arr, size_t size){
    for(size_t i = 0; i<size; i++){
        if(arr[i] == num) return true;
    }
    return false;
}

bool in_char_array(char c, char* arr, size_t size){
    for(size_t i = 0; i<size; i++){
        if(arr[i] == c) return true;
    }
    return false;
}

bool in_float_array(float num, float* arr, size_t size){
    for(size_t i = 0; i<size; i++){
        if(arr[i] == num) return true;
    }
    return false;
}

bool in_double_array(double num, double* arr, size_t size){
    for(size_t i = 0; i<size; i++){
        if(arr[i] == num) return true;
    }
    return false;
}

bool in_str_array(char* str, char** arr, size_t size){
    for(size_t i = 0; i<size; i++){
        if(strcmp(arr[i], str) == 0) return true;
    }
    return false;
}

