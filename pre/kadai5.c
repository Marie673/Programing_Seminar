//
// Created by marie on 2021/03/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char str[256];
    int num;

    char buf[32];
    char *end;

    fgets(str, sizeof(str), stdin);
    fgets(buf, sizeof(buf), stdin);
    long n = strtol(buf, &end, 10);
    num = (int) n;

    int count = 0;
    for(int i = 0; i < strlen(str); i++) {
        if(count != num) {
            if (str[i] == ' ') {
                count++;
            }
        }
        else{
            printf("%c", str[i]);
        }
    }

    return 0;
}
