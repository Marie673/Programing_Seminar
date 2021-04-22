//////////////////////////////////////////////////////////////
// Created by okazaki on 2021/04/21.
//
//(1) ./a.out file1 file2 のようにコマンドラインから実行すると、
//file1の内容をコピーして、file2という新しいファイルを作成する。
//ただしfile1の内容はテキストのみを仮定してよい。
//
/////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

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

    original_fp = fopen(original_file_name, "r");
    if(original_fp == 0){
        fprintf(stderr, "Couldn't open the file: %s\n", original_file_name);
        exit(1);
    }

    target_fp = fopen(target_file_name, "w");
    if(target_fp == 0){
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