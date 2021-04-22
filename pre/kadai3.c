//
// Created by marie on 2021/03/23.
//

#include <stdio.h>
#include <string.h>

int main(){
    int num;
    char str[256];

    char buf[32];
    char *end;

    fgets(str, sizeof(str), stdin);
    int total = 0;
    for(int i = 0; i < strlen(str); i++){
        total += (str[0] == str[i]);
    }

    printf("%d\n", total);

    return 0;
}