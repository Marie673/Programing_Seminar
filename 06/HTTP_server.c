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

struct request{
    char method[BUF_SIZE];
    char target[BUF_SIZE];
    char version[BUF_SIZE];
    char host[BUF_SIZE];
};


struct request *parseRequestMessage(char *message) {
    struct request *req;
    req = malloc(sizeof(struct request));
    char *line = NULL;

    line = strtok(message, "\r\n");
    strncpy(req->version, "HTTP/0.9", strlen("HTTP/0.9"));
    sscanf(line, "%s %s %s", req->method, req->target, req->version);
    if (strncmp(req->version, "HTTP/1.1", strlen("HTTP/1.1")) == 0) {

    }

    printf("<-- method: %s, target: %s, version: %s -->\n", req->method, req->target, req->version);
    return req;
}

void communicate_to_client(int sock) {

    FILE *write_fp;
    unsigned int read_size;
    char buf[BUF_SIZE];

    write_fp = fdopen(sock, "w");

    read_size = read(sock, buf, sizeof(buf));
    if (read_size == -1) {
        printf("read() failed\n");
        return;
    }
    if (read_size == 0) {
        printf("connection ended\n");
        return;
    }
    // show receive message.
    int i;
    for (i = 0; i < read_size; ++i) {
        printf("%c", buf[i]);
    }

    struct request *req = NULL;
    if ((req = parseRequestMessage(buf)) == NULL) {
        printf("parseRequestMessage failed\n");
        return;
    }

    if (strncmp(req->method, "GET", strlen(req->method)) == 0) {
        if (strcmp(req->target, "/") == 0) {
            strncpy(req->target, "/index.html", strlen("/index.html"));
        }
        if (strncmp(req->target, "../", strlen("../")) == 0) {
            char data[BUF_SIZE];
            sprintf(data, "Access denied\r\n");
            fwrite(data, sizeof(char), strlen(data), write_fp);
            fflush(write_fp);
            return;
        }

        FILE *target_file;
        struct stat target_stat;

        if (stat(&(req->target[1]), &target_stat) == 0) {
            target_file = fopen(&(req->target[1]), "r");
            int file_size = (int) target_stat.st_size;
            int write_size = 0;

            char data[BUF_SIZE];
            while (write_size < file_size) {
                int size = (int) fread(data, sizeof(char), sizeof(data), target_file);
                fwrite(data, sizeof(char), size, write_fp);
                fflush(write_fp);
                write_size += size;
            }
            fclose(target_file);
        }
        else {
            printf("File is not existing\n");
            return;
        }
    }
    else {
        printf("method: %s is not supported", req->method);
    }

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
        communicate_to_client(clientSock);

        close(clientSock);
    }

    close(serverSock);

    return 0;
}