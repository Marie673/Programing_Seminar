//
// Created by marie on 2021/03/23.
//

#include <stdio.h>
#include <stdlib.h>

int main(){
    int num;
    int sum = 0;
    double average;

    char buf[32];
    char *end;
    long n;

    fgets(buf, sizeof(buf), stdin);
    n = strtol(buf, &end, 10);
    num = (int)n;

    for(int i = 0; i < num; i++){
        fgets(buf, sizeof(buf), stdin);
        n = strtol(buf, &end, 10);
        sum += (int) n;
    }

    average = (double) sum / num;
    printf("%lf\n", average);

    return 0;
}