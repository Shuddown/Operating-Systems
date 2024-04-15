#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array_helpers.c"

#define MAX_ARG_COUNT 1024
#define MAX_TAG_COUNT 5

typedef struct{
    char** args;
    int arg_count;
    char* options;
    int op_count;
}command_info;

command_info* get_ops_args(int argc, char* argv[], char* valid_tags, size_t num_valid_tags){
    command_info* cmd = (command_info*) malloc(sizeof(command_info));
    char tags[MAX_TAG_COUNT] = {0};
    char* args[MAX_ARG_COUNT]; 
    size_t arg_count = 0;
    size_t tag_count = 0;
    for(int i = 0; i < argc; i++){
        if (argv[i][0] == '-'){
            int char_index = 1;
            char tag;
            while ((tag = argv[i][char_index++]) != '\0') {
                if (!in_char_array(tag, valid_tags, SIZE(valid_tags))){
                    printf("%c is not a valid tag!\n", tag);
                    exit(1);
                }
                if (!in_char_array(tag, tags, SIZE(tags)))
                    tags[tag_count++] = tag;
            }
        }else{
            args[arg_count++] = argv[i]; 
        }

    }
    cmd->args = (char**) malloc(sizeof(char*) * arg_count);
    for(int i = 0; i<arg_count; i++){
        cmd->args[i] = strdup(args[i]);  
    }
    cmd->arg_count = arg_count;
    cmd->options = strdup(tags);
    cmd->op_count = tag_count;
    return cmd;
}
