//
// Created by marie on 2021/03/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    int n,m;
    int **num;

    char buf[32];
    char *end;
    long buf_num;
    fgets(buf, sizeof(buf), stdin);
    buf_num = strtol(buf, &end, 10);
    m = (int) buf_num;
    fgets(buf, sizeof(buf), stdin);
    buf_num = strtol(buf, &end, 10);
    n = (int) buf_num;

    num = malloc(sizeof(int *) * m);
    for(int i = 0; i < m; i++){
        num[i] = malloc(sizeof(int) * n);
    }

    for(int i = 0; i < m; i++) {
        fgets(buf, sizeof(buf), stdin);
        char b[16];
        int index = 0;
        int n_index = 0;
        for(int c = 0; c <= strlen(buf); c++){
            if(buf[c] != ' ' && buf[c] != '\0'){
                b[index] = buf[c];
                index++;
                continue;
            }
            else{
                index = 0;
                buf_num = strtol(b, &end, 10);
                num[i][n_index] = (int) buf_num;
                n_index++;
                memset(b, '\0', strlen(b));
            }
        }
    }

    int total;
    // iが奇数
    total = 0;
    for(int i = 1; i < m; i += 2){
        for(int j = 0; j < n; j++){
            total += (num[i][j] == 0);
        }
    }
    printf("iが奇数: %d\n", total);
    // iが偶数
    total = 0;
    for(int i = 0; i < m; i += 2){
        for(int j = 0; j < n; j++){
            total += (num[i][j] == 0);
        }
    }
    printf("iが偶数: %d\n", total);
    // jが奇数
    total = 0;
    for(int i = 0; i < m; i ++){
        for(int j = 1; j < n; j+=2){
            total += (num[i][j] == 0);
        }
    }
    printf("jが奇数: %d\n", total);
    // jが偶数
    total = 0;
    for(int i = 0; i < m; i ++){
        for(int j = 0; j < n; j+=2){
            total += (num[i][j] == 0);
        }
    }
    printf("jが偶数: %d\n", total);
    // すべて
    total = 0;
    for(int i = 0; i < m; i ++){
        for(int j = 0; j < n; j++){
            total += (num[i][j] == 0)?1:0;
        }
    }
    printf("すべて: %d\n", total);

    return 0;
}