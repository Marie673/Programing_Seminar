//
// Created by marie on 2021/05/26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#define SOCK_BUF_SIZE 64
#define PATH_LEN 128

char *save_file_path(char *path){
    char *file_path;
    char *path_buf;
    char extension[16];

    file_path = (char *) malloc(sizeof(char) * PATH_LEN);

    path_buf = strtok(++path, ".");
    strcpy(extension, strtok(NULL, ""));

    snprintf(file_path, PATH_LEN, "%s.%s", path_buf, extension);

    struct stat stat_buf;
    int file_num = 0;
    while (stat(file_path, &stat_buf) == 0) {
        file_num++;
        memset(file_path, 0, PATH_LEN);
        snprintf(file_path, PATH_LEN, "%s(%d).%s", path_buf, file_num, extension);
        if (strlen(file_path) >= PATH_LEN) {
            fprintf(stderr, "path name is too long\n");
            exit(1);
        }
    }

    return file_path;
}

void receive_http(char *path, FILE *read_fp){
    char *end_ptr;
    FILE *save_fp;
    save_fp = fopen(save_file_path(path), "w");

    char buf[SOCK_BUF_SIZE];

    size_t content_length;
    while (fgets(buf, sizeof(buf), read_fp) != NULL) {
        if (strncmp(buf, "Content-Length:", strlen("Content-Length:")) == 0) {
            strtok(buf, ": ");
            content_length = strtol(strtok(NULL, ""), &end_ptr, 10);
        } else if (strcmp(buf, "\r\n") == 0) {
            break;
        }
    }

    size_t receive_length = 0;
    while (1) {
        size_t read_size;
        size_t remaining_bytes;

        memset(buf, 0, sizeof(buf));

        if ((remaining_bytes = content_length - receive_length) > sizeof(buf)) {
            read_size = fread(buf, sizeof(char), sizeof(buf) / sizeof(char), read_fp);
        } else {
            read_size = fread(buf, sizeof(char), remaining_bytes, read_fp);
        }
        receive_length += read_size;
        fwrite(buf, sizeof(char), read_size, save_fp);
        // printf("read size: %zu\nreceive size: %zu\ncongestion size: %zu\n\n", read_size, receive_length, content_length);
        if (read_size == 0 || receive_length >= content_length) {
            fclose(save_fp);
            printf("%s: Content length = %zu, Receive length = %zu\n", path, content_length, receive_length);
            break;
        }
    }
}

int main(int argc, char **argv)
{
    char *server_ip;
    int server_port;
    char **path;
    char *host;
    int path_num;

    struct sockaddr_in server;
    int sock;
    FILE *read_fp, *write_fp;

    char *end_ptr;

    if(argc < 5){
        fprintf(stderr, "missing option\n");
        exit(1);
    }

    server_ip = *++argv;
    server_port = (int) strtol(*++argv, &end_ptr, 10);
    host = *++argv;

    path_num = argc - 4;
    path = (char **) calloc(path_num ,sizeof(char *));
    for(int i = 0; i < path_num; i++){
        path[i] = *++argv;
        printf("path[%d] = %s\n", i+1, path[i]);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    // server.sin_len = sizeof(server);
    inet_pton(AF_INET, server_ip, &server.sin_addr.s_addr);

    if(connect(sock, (struct sockaddr *) &server, sizeof(server)) != 0){
        perror("connect failed");
        exit(1);
    }

    read_fp = fdopen(sock, "r");
    write_fp = fdopen(sock, "w");

    char send_buf[124];
    for(int i = 0; i < path_num; i++){
        snprintf(send_buf, sizeof(send_buf),"GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path[i], host);
        fprintf(write_fp, "%s", send_buf);
        fflush(write_fp);
    }

    for(int i = 0; i < path_num; i++) {
        receive_http(path[i], read_fp);
    }
    fclose(write_fp);
    fclose(read_fp);

    close(sock);
    return 0;
}