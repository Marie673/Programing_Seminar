//
// Created by okako on 2021/04/23.
//
//(3) file2がすでに存在してファイルである場合、上書きを避けるため
//標準エラー出力(stderr)にエラーを表示して終了する。
//※たとえばstat()関数を利用

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char **argv){
    FILE *original_fp;
    FILE *target_fp;

    if(argc != 3){
        fprintf(stderr, "Argument error\n");
        exit(1);
    }

    char *original_file_name = NULL, *target_file_name = NULL;
    original_file_name = argv[1];
    target_file_name = argv[2];

    if(strcmp(original_file_name, target_file_name) == 0){
        fprintf(stderr, "the same File Name cannot be specified.\n");
        exit(1);
    }

    if((original_fp = fopen(original_file_name, "r")) == 0){
        fprintf(stderr, "Couldn't open the file: %s\n", original_file_name);
        exit(1);
    }


    struct stat stat_buf;
    if(stat(target_file_name, &stat_buf) == 0){
        fprintf(stderr, "[%s] is existing\n", target_file_name);
        exit(1);
    }
    if((target_fp = fopen(target_file_name, "w")) == 0){
        fprintf(stderr, "Couldn't open the file: %s\n", target_file_name);
        exit(1);
    }

    int ch;
    while((ch = fgetc(original_fp)) != EOF){
        if(fputc(ch, target_fp) == EOF){
            fprintf(stderr, "copy error\n");
            exit(1);
        }
    }

    fclose(original_fp);
    fclose(target_fp);

    return 0;
}