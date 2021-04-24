//
// Created by okako on 2021/04/24.
//
//(8) file1の最終更新日時を読み取り、コピーしたfile2にも
//同じ日時を設定する。
//※たとえばstat()関数やutime()関数を利用

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>

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

    struct stat stat_buf[2];
    // target fileのステータスに関する記述
    if(stat(target_file_name, &stat_buf[0]) == 0) {
        if (S_ISDIR(stat_buf[0].st_mode)) {
            strcat(target_file_name, "/");
            strcat(target_file_name, original_file_name);
            if(stat(target_file_name, &stat_buf[0]) == 0){
                fprintf(stderr, "[%s] is existing\n", target_file_name);
                exit(1);
            }
        }
        else if(S_ISREG(stat_buf[0].st_mode)){
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
        while (fgets(buf, MAX_BUF_SIZE, original_fp) != NULL) {
            if (*buf == '\n')
                break;
        }
    }
    while ((ch = fgetc(original_fp)) != EOF) {
        if (fputc(ch, target_fp) == EOF) {
            fprintf(stderr, "copy error\n");
            exit(1);
        }
    }

    fclose(original_fp);
    fclose(target_fp);

    ///////////////////////////////////////////////
    // 課題8追記部分
    stat(original_file_name, &stat_buf[1]);
    struct utimbuf u_buf;
    u_buf.modtime = stat_buf[1].st_mtime;
    u_buf.actime = stat_buf[1].st_atime;
    if(utime(target_file_name, &u_buf) != 0){
        perror("utime error");
        exit(1);
    }
    ///////////////////////////////////////////////

    return 0;
}