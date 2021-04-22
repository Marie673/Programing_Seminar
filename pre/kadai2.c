//
// Created by marie on 2021/03/23.
//

#include <stdio.h>
#include <stdlib.h>

struct list{
    int binary;
    struct list *next;
};

struct list *add_list(int binary, struct list *head){
    struct list *p;
    if((p = (struct list *) malloc(sizeof(struct list))) == NULL){
        exit(EXIT_FAILURE);
    }
    p->binary = binary;

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
    struct list *l;
    l = NULL;

    char buf[32];
    char *end;
    long n;

    fgets(buf, sizeof(buf), stdin);
    n = strtol(buf, &end, 10);
    num = (int)n;

    for(int i = 0; i < num; i++){
        fgets(buf, sizeof(buf), stdin);
        n = strtol(buf, &end, 10);
        l = add_list((int) n, l);
    }

    fgets(buf, sizeof(buf), stdin);
    n = strtol(buf, &end, 10);
    int w = (int) n;

    int total = 0;
    while(l != NULL){
        total += (l->binary == w);
        l = l->next;
    }

    free_list(l);

    printf("%d\n", total);

    return 0;
}