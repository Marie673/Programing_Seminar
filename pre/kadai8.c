//
// Created by marie on 2021/04/02.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct list{
    char key[64];
    char value[64];
    struct list* next;
};

struct list *make_list(char* key, char* value, struct list *head){
    struct list *new;
    new = malloc(sizeof(struct list));
    strcpy(new->key, key);
    strcpy(new->value, value);
    new->next = head;
    head = new;
    // printf("debug %s:::%s\n", head->key, head->value);
    return head;
}

void split(char *str, char *str1, char *str2){
    memset(str1, '\0', strlen(str1));
    memset(str2, '\0', strlen(str2));
    int length = (int) strlen(str);

    char buf[64];
    int i;
    for(i = 0; i < length; i++){
        if(str[i] == ':' && str[i+1] == ' '){
            buf[i] = '\0';
            i += 2;
            strcpy(str1, buf);
            break;
        }
        buf[i] = str[i];
    }

    memset(buf, '\0' ,sizeof(buf));
    int j;
    for(j = 0 ; i < length - 1; i++, j++){
        buf[j] = str[i];
    }
    buf[j+1] = '\0';
    strcpy(str2, buf);
}

void search_value(char *key, struct list *head){

    while(head != NULL){
        if(strcmp(key, head->key) == 0){
            printf("%s\n", head->value);
            return;
        }
        head = head->next;
    }
    printf("**not found**\n");
}

int main(){
    int time;
    struct list *head;
    head = NULL;

    char buf[64];
    char *end;
    long num_buf;

    fgets(buf, sizeof(buf), stdin);
    num_buf = strtol(buf, &end, 10);
    time = (int) num_buf;

    for(int t = 0; t < time; t++){
        char key_buf[64];
        char value_buf[64];
        memset(buf, '\0', sizeof(buf));
        memset(key_buf, '\0', sizeof(key_buf));
        memset(value_buf, '\0', sizeof(value_buf));
        fgets(buf, sizeof(buf), stdin);
        split(buf, key_buf, value_buf);
        head = make_list(key_buf, value_buf, head);
    }

    while( fgets(buf, sizeof(buf), stdin) != NULL){
        if(buf[0] == '\0' || buf[0] == '\n'){
            break;
        }
        buf[strlen(buf)-1] = '\0';
        search_value(buf, head);
    }

    return 0;
}