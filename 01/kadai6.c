//
// Created by okako on 2021/04/24.
//
//(6) バイナリデータ（\0が含まれている可能性があり、改行文字\nが
//含まれているとは限らないデータ）がfile1に保存されていても、
//(1)と同様にfile2にコピーする。
//※たとえばfread()関数を利用

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_BUF_SIZE 256

int b_Flag = 0;

int main(int argc, char **argv){
    FILE *original_fp;
    FILE *target_fp;

    int opt_ch;
    while((opt_ch = getopt(argc, argv, "b")) != -1){
        switch (opt_ch) {
            case 'b':
                b_Flag = 1;
                break;
            default:
                fprintf(stderr, "Couldn't understand option\n");
                exit(1);
        }
    }
    argc -= optind;
    argv += optind;

    if(argc != 2){
        fprintf(stderr, "Argument error\n");
        exit(1);
    }
    char *original_file_name = NULL, *target_file_name = NULL;
    original_file_name = (--argc, *argv++);
    target_file_name = (--argc, *argv);

    if(strcmp(original_file_name, target_file_name) == 0){
        fprintf(stderr, "the same File Name cannot be specified.\n");
        exit(1);
    }

    if((original_fp = fopen(original_file_name, "r")) == 0){
        fprintf(stderr, "Couldn't open the file: %s\n", original_file_name);
        exit(1);
    }

    // target fileのステータスに関する記述
    struct stat stat_buf;
    if(stat(target_file_name, &stat_buf) == 0) {
        if (S_ISDIR(stat_buf.st_mode)) {
            strcat(target_file_name, "/");
            strcat(target_file_name, original_file_name);
            if(stat(target_file_name, &stat_buf) == 0){
                fprintf(stderr, "[%s] is existing\n", target_file_name);
                exit(1);
            }
        }
        else if(S_ISREG(stat_buf.st_mode)){
            fprintf(stderr, "[%s] is existing\n", target_file_name);
            exit(1);
        }
    }
    if((target_fp = fopen(target_file_name, "w")) == 0){
        fprintf(stderr, "Couldn't open the file: %s\n", target_file_name);
        exit(1);
    }

    int ch;
    if(b_Flag == 1) {
        char buf[MAX_BUF_SIZE];
        {
            int flag = 0;
            while (fgets(buf, MAX_BUF_SIZE, original_fp) != NULL) {
                if(flag == 0) {
                    if (*buf == '\n')
                        flag = 1;
                    continue;
                }
                fprintf(target_fp, "%s", buf);
            }
        }
    }
    else {
        while ((ch = fgetc(original_fp)) != EOF) {
            if (fputc(ch, target_fp) == EOF) {
                fprintf(stderr, "copy error\n");
                exit(1);
            }
        }
    }

    fclose(original_fp);
    fclose(target_fp);

    return 0;
}