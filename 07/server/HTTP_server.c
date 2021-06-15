//
// Created by marie on 2021/06/08.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEFAULT_PORT 80
#define QUEUE_LIMIT 5

#define BUF_SIZE 1024

struct REQUEST{
    char method[BUF_SIZE];
    char target[BUF_SIZE];
    char version[BUF_SIZE];
    char host[BUF_SIZE];
};

struct HEADER{
    int state;
    int content_length;
};


struct REQUEST *parseInitialRequest(char *message) {
    struct REQUEST *req;
    req = malloc(sizeof(struct REQUEST));

    strncpy(req->version, "HTTP/0.9", strlen("HTTP/0.9"));
    sscanf(message, "%s %s %s", req->method, req->target, req->version);

    printf("<-- method: %s, target: %s, version: %s -->\n", req->method, req->target, req->version);
    return req;
}

void getAdditionalRequest(FILE *read_fp, struct REQUEST *req) {
    char buf[BUF_SIZE];

    while (fgets(buf, BUF_SIZE, read_fp) != NULL) {
        printf("%s", buf);
        if (buf[0] == '\n') {
            break;
        }
        else if (strncpy(buf, "Host:", strlen("Host:")) == 0) {
            sscanf(buf, "Host: %s", req->host);
        }
    }
}

struct HEADER *makeHeader(struct REQUEST *req) {
    struct HEADER *header;
    header = malloc(sizeof(struct HEADER));
    struct stat target_stat;

   stat(&(req->target[1]), &target_stat);
   header->content_length = (int) target_stat.st_size;
   // その他構造体のHEADERフィールド初期化とか

   return header;
}

void sendHeader(FILE *write_fp, struct HEADER *head) {
    char send_buf[BUF_SIZE];
    char buf[BUF_SIZE];
    memset(send_buf, 0, BUF_SIZE);

    switch (head->state) {
        case 200:
            strcat(send_buf, "HTTP/1.1 200 OK\n");
            break;
        case 400:
            strcat(send_buf, "HTTP/1.1 400 Bad Request\n");
            break;
        default:
            break;
    }
    sprintf(buf, "Content-length: %d\n", head->content_length);
    strcat(send_buf, buf);

    fwrite(send_buf, sizeof(char), strlen(send_buf), write_fp);
    fflush(write_fp);
}

int getMethod(FILE *write_fp, struct REQUEST *req){
    struct HEADER *head;

    if (strcmp(req->target, "/") == 0) {
        strncpy(req->target, "/index.html", strlen("/index.html"));
    }
    if (strncmp(req->target, "../", strlen("../")) == 0) {
        char data[BUF_SIZE];
        sprintf(data, "Access denied\r\n");
        fwrite(data, sizeof(char), strlen(data), write_fp);
        fflush(write_fp);
        return -1;
    }

    head = makeHeader(req);

    struct stat target_stat;
    if (stat(&(req->target[1]), &target_stat) == -1) {
        head->state = 404;
    }
    else {
        head->state = 200;
        head->content_length = (int) target_stat.st_size;
    }

    sendHeader(write_fp, head);

    FILE *target_file;
    target_file = fopen(&(req->target[1]), "r");
    int write_size = 0;
    char data[BUF_SIZE];
    while (write_size < head->content_length) {
        int size = (int) fread(data, sizeof(char), sizeof(data), target_file);
        fwrite(data, sizeof(char), size, write_fp);
        fflush(write_fp);
        write_size += size;
    }
    fclose(target_file);

    return 0;
}

int http(int sock) {
    FILE *read_fp;
    FILE *write_fp;
    char buf[BUF_SIZE];

    read_fp = fdopen(sock, "r");
    write_fp = fdopen(sock, "w");
    if (fgets(buf, BUF_SIZE, read_fp) == NULL) {
        printf("initial REQUEST failed");
        return -1;
    }
    printf("%s", buf);

    struct REQUEST *req = NULL;
    if ((req = parseInitialRequest(buf)) == NULL) {
        printf("parseRequestMessage failed\n");
        return -1;
    }
    if (strncpy(req->version, "HTTP/1", strlen("HTTP/1")) == 0) {
        getAdditionalRequest(read_fp, req);
    }
    if (strncpy(req->version, "HTTP/1.1", strlen("HTTP/1.1")) == 0) {
        if (req->host[0] == '\0') {
            // Bad request
        }
    }

    if (strncmp(req->method, "GET", strlen(req->method)) == 0) {
        if (getMethod(write_fp, req) == -1) {
            printf("getMethod() failed\n");
            return -1;
        }
    }
    else {
        printf("method: %s is not supported", req->method);
        return -1;
    }

    return 0;
}


int main(int argc, char **argv) {

    struct sockaddr_in serverSockAddr, clientSockAddr;
    int serverSock, clientSock;
    int port = DEFAULT_PORT;
    socklen_t len;

    char *end_ptr;

    if (argc != 2) {
        fprintf(stderr, "missing Server Port\n");
        exit(1);
    }
    port = (int) strtol(*++argv, &end_ptr, 10);

    if ((serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("socket() failed");
        exit(1);
    }

    memset(&serverSockAddr, 0, sizeof(serverSockAddr));
    serverSockAddr.sin_family = AF_INET;
    serverSockAddr.sin_port = htons(port);
    serverSockAddr.sin_addr.s_addr = INADDR_ANY;

    int yes = 1;
    if (setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, (const char *) &yes, sizeof(yes)) < 0) {
        perror("setsockopt() failed");
        exit(1);
    }

    if (bind(serverSock, (struct sockaddr *) &serverSockAddr, sizeof(serverSockAddr)) < 0) {
        perror("bind() failed");
        exit(1);
    }

    if (listen(serverSock, QUEUE_LIMIT) < 0) {
        perror("listen() failed");
        exit(1);
    }

    while (1) {
        len = sizeof(clientSockAddr);
        if ((clientSock = accept(serverSock, (struct sockaddr *) &clientSockAddr, &len)) < 0) {
            perror("accept() failed");
            exit(1);
        }

        printf("connected from %s\n", inet_ntoa(clientSockAddr.sin_addr));

        int pid;
        if ((pid = fork()) == 0) {
            close(clientSock);
            continue;
        }
        else if (pid >= 1) {
            http(clientSock);
            close(clientSock);
        }
    }

    close(serverSock);

    return 0;
}