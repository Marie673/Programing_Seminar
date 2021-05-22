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

    struct sockaddr_in server;
    int sock;
    FILE *read_fp, *write_fp;

    char send_buf[SOCK_BUF_SIZE];
    char *end_ptr;

    if(argc != 4){
        fprintf(stderr, "missing option\n");
        exit(1);
    }
    server_ip = *++argv;
    server_port = (int) strtol(*++argv, &end_ptr, 10);
    path = *++argv;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_len = sizeof(server);
    inet_pton(AF_INET, server_ip, &server.sin_addr.s_addr);

    if(connect(sock, (struct sockaddr *) &server, sizeof(server)) != 0){
        perror("connect failed");
        exit(1);
    }

    read_fp = fdopen(sock, "r");
    write_fp = fdopen(sock, "w");

    snprintf(send_buf, sizeof(send_buf),"GET %s HTTP/1.0\r\n\r\n", path);
    fprintf(write_fp, "%s", send_buf);
    fflush(write_fp);

    char *path_buf;
    char extension[16];
    path_buf = strtok(++path, ".");
    strcpy(extension, strtok(NULL, ""));

    char file_path[PATH_LEN] = "";
    strcat(file_path, path_buf);
    strcat(file_path, ".");
    strcat(file_path, extension);

    struct stat stat_buf;
    while(stat(file_path, &stat_buf) == 0){
        memset(file_path, 0, sizeof(file_path));
        strcat(path_buf, "_sub");
        strcat(file_path, path_buf);
        strcat(file_path, ".");
        strcat(file_path, extension);
        if(strlen(file_path) >= sizeof(file_path)){
            fprintf(stderr, "path name is too long\n");
            exit(1);
        }
    }

    FILE *save_fp;
    save_fp = fopen(file_path, "w");
    char buf[SOCK_BUF_SIZE];

    while(fgets(buf, sizeof(buf), read_fp) != NULL){
        if(strcmp(buf, "\r\n") == 0){
            break;
        }
    }

    while(1){
        memset(buf, 0, sizeof(buf));
        size_t read_size;
        read_size = fread(buf, sizeof(char), sizeof(buf) / sizeof(char), read_fp);
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