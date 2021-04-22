//
// Created by okazaki on 2021/04/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
    FILE *original_fp;
    FILE *target_fp;

    if(argc != 3){
        fprintf(stderr, "Argument error\n");
        exit(1);
    }

    char *original_file_name = NULL, *target_file_name = NULL;
    strcpy(original_file_name, argv[1]);
    strcpy(target_file_name, argv[2]);

    original_fp = fopen(original_file_name, "r");
    if(original_fp == 0){
        fprintf(stderr, "original file is error\n");
        exit(1);
    }
    target_fp = fopen(target_file_name, "r");
    if(target_fp != 0){
        fprintf(stderr, "target file exist\n");
        exit(1);
    }



}