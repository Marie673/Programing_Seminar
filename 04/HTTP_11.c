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

int main(int argc, char **argv)
{
    char *server_ip;
    int server_port;
    char *path;
    char *host;

    struct sockaddr_in server;
    int sock;
    FILE *read_fp, *write_fp;

    char *end_ptr;

    if(argc != 5){
        fprintf(stderr, "missing option\n");
        exit(1);
    }
    server_ip = *++argv;
    server_port = (int) strtol(*++argv, &end_ptr, 10);
    path = *++argv;
    host = *++argv;

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
    snprintf(send_buf, sizeof(send_buf),"GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path, host);
    fprintf(write_fp, "%s", send_buf);
    fflush(write_fp);

    char *path_buf;
    char extension[16];
    path_buf = strtok(++path, ".");
    strcpy(extension, strtok(NULL, ""));

    char file_path[PATH_LEN] = "";
    snprintf(file_path, sizeof(file_path), "%s.%s", path_buf, extension);

    struct stat stat_buf;
    int file_num = 0;
    while(stat(file_path, &stat_buf) == 0){
        file_num++;
        memset(file_path, 0, sizeof(file_path));
        snprintf(file_path, sizeof(file_path), "%s(%d).%s", path_buf, file_num, extension);
        if(strlen(file_path) >= sizeof(file_path)){
            fprintf(stderr, "path name is too long\n");
            exit(1);
        }
    }

    FILE *save_fp;
    save_fp = fopen(file_path, "w");
    char buf[SOCK_BUF_SIZE];

    size_t content_length;
    while(fgets(buf, sizeof(buf), read_fp) != NULL){
        if(strncmp(buf, "Content-Length:", strlen("Content-Length:")) == 0){
            strtok(buf, ": ");
            content_length = strtol(strtok(NULL, ""), &end_ptr, 10);
        }
        else if(strcmp(buf, "\r\n") == 0){
            break;
        }
    }

    size_t receive_length = 0;

    while(1){
        size_t remaining_bytes;
        memset(buf, 0, sizeof(buf));
        size_t read_size;
        if((remaining_bytes = content_length - receive_length) > sizeof(buf)){
            read_size = fread(buf, sizeof(char), sizeof(buf) / sizeof(char), read_fp);
        }
        else{
            read_size = fread(buf, sizeof(char), remaining_bytes, read_fp);
        }
        receive_length += read_size;
        if(receive_length >= content_length){
            break;
        }
        printf("read size: %zu\nreceive size: %zu\ncongestion size: %zu\n\n", read_size, receive_length, content_length);
        if(read_size == 0) {
            fclose(save_fp);
            break;
        }
        fwrite(buf, sizeof(char), read_size, save_fp);
    }

    fclose(write_fp);
    fclose(read_fp);

    close(sock);
    return 0;
}