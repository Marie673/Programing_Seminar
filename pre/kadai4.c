//
// Created by marie on 2021/03/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct list{
    char str[256];
    struct list *next;
};

struct list *add_list(char *str, struct list *head){
    struct list *p;
    if((p = (struct list *) malloc(sizeof(struct list))) == NULL){
        exit(EXIT_FAILURE);
    }
    strcpy(p->str, str);

    p->next = head;
    head = p;

    return head;
}

void free_list(struct list *p){
    struct list *p0;
    while(p != NULL){
        p0 = p->next;
        free(p);
        p = p0;
    }
}

int main(){
    int num;
    char first[256];
    struct list *l;
    l = NULL;

    char buf[256];
    char *end;

    fgets(buf, sizeof(buf), stdin);
    long n = strtol(buf, &end, 10);
    num = (int)n;

    for(int i = 0; i < num; i++){
        fgets(buf, sizeof(buf), stdin);
        if(i == 0) strcpy(first, buf);
        l = add_list(buf, l);
    }

    int total = 0;
    while(l != NULL){
        total += (strcmp(l->str, first) == 0);
        l = l->next;
    }

    free_list(l);

    printf("%d\n", total);

    return 0;
}