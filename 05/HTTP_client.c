//
// Created by marie on 2021/05/31.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <netdb.h>

#define SOCK_BUF_SIZE 64
#define PATH_LEN 128

char *PROTOCOL_VERSION = "HTTP/0.9";
int HTTP_11_FLAG = 0;

struct path{
    char *path_name;
    char save_file[PATH_LEN];
    struct path *next;
};

struct path *list_add (struct path *p, char *path_name) {

    struct path *new = (struct path *) malloc(sizeof(struct path));
    new->path_name = path_name;
    new->next = NULL;

    if (p == NULL){
        return new;
    }
    else {
        struct path *node = p;
        while (node->next != NULL) {
            node = node->next;
        }
        node->next = new;
        return p;
    }
}

int resolve_dns (char *host_name) {
    char *service = "http";
    struct addrinfo hints, *res0, *res;
    int sock;

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = PF_UNSPEC;

    if ((getaddrinfo(host_name, service, &hints, &res0)) != 0) {
        perror("get addr info error");
        exit(1);
    }
    for (res = res0; res != NULL; res = res->ai_next) {
        sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sock < 0)
            continue;
        if(connect(sock, res->ai_addr, res->ai_addrlen) != 0){
            close(sock);
            continue;
        }
        break;
    }
    if (res == NULL) {
        fprintf(stderr, "DNS search failed\n");
        exit(1);
    }

    freeaddrinfo(res0);

    return sock;
}

static void usage(const char *cmd) {
    printf(""
            "Usage: %s [options] host port\n"
            "\n"
            "Options:\n"
            "  -p single path            single path to request\n"
            "  -v <protocol version>     the protocol; either \"HTTP/0.9\"\n"
            "                            (default) or \"HTTP/1.0\" or \"HTTP/1.1\"\n"
            "  -h                        print this help\n"
            "\n",
            cmd);
}


int main(int argc, char **argv) {
    int ch, i;

    char *addr;
    int port;
    char *host = NULL;
    struct path *path = NULL;
    struct path *path_head = NULL;
    int path_num = 0;

    char *end_ptr;

    while ((ch = getopt(argc, argv, "p:v:h:")) != -1) {
        switch (ch) {
            case 'h':
                host = optarg;
                break;
            case 'p':
                path_head = list_add(path_head, optarg++);
                path_num++;
                break;
            case 'v':
                PROTOCOL_VERSION = optarg;
                break;
            default:
                usage(argv[0]);
                exit(1);
        }
    }
    argc -= optind;
    argv += optind;

    if (argc != 2) {
        fprintf(stderr,
                "missing Server IP and Port\n");
        exit(1);
    }
    addr = *argv++;
    port = (int) strtol(*argv++, &end_ptr, 10);

    if (strncmp(PROTOCOL_VERSION, "HTTP/1.1", strlen(PROTOCOL_VERSION)) == 0) {
        if (host == NULL) {
            fprintf(stderr, "HTTP/1.1 need Host\n");
            exit(1);
        }
    }

    if (path_head == NULL) {
        path_head = list_add(path_head, "/index.html");
        path_num++;
    }

    path = path_head;
    while (path != NULL) {
            char save_file[PATH_LEN];
            char *buf;
            char file_path[PATH_LEN/2], extension[16];
            buf = strrchr(path->path_name, '/');
            sscanf(++buf, "%[^.].%s", file_path, extension);
            snprintf(save_file, PATH_LEN, "%s.%s", file_path, extension);

            struct stat stat_buf;
            int file_num = 0;
            while (stat(save_file, &stat_buf) == 0) {
                file_num++;
                snprintf(save_file, PATH_LEN, "%s(%d).%s", file_path, file_num, extension);
            }
            strncpy(path->save_file, save_file, PATH_LEN);
            path = path->next;
        }

    printf("Server IP Address : %s\n", addr);
    printf("Port : %d\n", port);
    printf("Host : %s\n", host);
    printf("Protocol version : %s\n", PROTOCOL_VERSION);
    path = path_head;
    printf("Target path    : %s\n", path->path_name);
    printf("Save file name : %s\n\n", path->save_file);
    while (path->next != NULL) {
            path = path->next;
            printf("               : %s\n", path->path_name);
            printf("               : %s\n\n", path->save_file);
        }

    struct sockaddr_in server;
    int sock;
    FILE *read_fp, *write_fp;

    if (strncmp(addr, "http://", strlen("http://")) == 0) {
        sock = resolve_dns(addr += strlen("http://"));
    }
    else {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        // server.sin_len = sizeof(server);
        inet_pton(AF_INET, addr, &server.sin_addr.s_addr);

        if (connect(sock, (struct sockaddr *) &server, sizeof(server)) != 0) {
            perror("connect failed");
            exit(1);
        }
    }

    read_fp = fdopen(sock, "r");
    write_fp = fdopen(sock, "w");

    char send_buf[124];
    path = path_head;
    for(i = 0; i < path_num; i++){
        if (strncmp(PROTOCOL_VERSION, "HTTP/0.9", strlen(PROTOCOL_VERSION)) == 0) {
            snprintf(send_buf, sizeof(send_buf), "GET %s\r\n\r\n", path->path_name);
        }
        else if (strncmp(PROTOCOL_VERSION, "HTTP/1.0", strlen(PROTOCOL_VERSION)) == 0) {
            snprintf(send_buf, sizeof(send_buf), "GET %s HTTP/1.0\r\n\r\n", path->path_name);
        }
        else if (strncmp(PROTOCOL_VERSION, "HTTP/1.1", strlen(PROTOCOL_VERSION)) == 0) {
            snprintf(send_buf, sizeof(send_buf),"GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path->path_name, host);
        }
        fprintf(write_fp, "%s", send_buf);
        fflush(write_fp);
        path = path->next;
    }

    path = path_head;
    for(i = 0; i < path_num; i++, path = path->next) {
        printf("Getting %s\n", path->path_name);
        FILE *save_fp;
        char buf[SOCK_BUF_SIZE];

        size_t content_length = 0;
        if (strncmp(PROTOCOL_VERSION, "HTTP/1.1", strlen(PROTOCOL_VERSION)) == 0) {
            HTTP_11_FLAG = 1;
        }
        if(strncmp(PROTOCOL_VERSION, "HTTP/1", strlen("HTTP/1")) == 0) {
            if (fgets(buf, sizeof(buf), read_fp) != NULL){
                int num;
                sscanf(buf, "HTTP/1.1 %d OK", &num);
                if (num != 200) {
                    fprintf(stderr, "Could not get Data\n");
                    exit(1);
                }
            }
            else {
                fprintf(stderr, "Could not get Data\n");
                exit(1);
            }
            while (fgets(buf, sizeof(buf), read_fp) != NULL) {
                if (HTTP_11_FLAG) {
                    if (strncmp(buf, "Content-Length:", strlen("Content-Length:")) == 0) {
                        sscanf(buf, "Content-Length: %zu", &content_length);
                        printf("Content-Length: %zu\n", content_length);
                    }
                }
                if (strcmp(buf, "\r\n") == 0) {
                    break;
                }
            }
        }

        save_fp = fopen(path->save_file, "w");
        size_t receive_length = 0;
        while (1) {
            size_t read_size;
            size_t remaining_bytes;

            memset(buf, 0, sizeof(buf));


            if (((remaining_bytes = content_length - receive_length) > sizeof(buf)) || !HTTP_11_FLAG) {
                read_size = fread(buf, sizeof(char), sizeof(buf) / sizeof(char), read_fp);
            } else {
                read_size = fread(buf, sizeof(char), remaining_bytes, read_fp);
            }
            receive_length += read_size;
            fwrite(buf, sizeof(char), read_size, save_fp);
            // printf("read size: %zu\nreceive size: %zu\ncongestion size: %zu\n\n", read_size, receive_length, content_length);
            if (read_size == 0 || (receive_length >= content_length && HTTP_11_FLAG)) {
                fclose(save_fp);
                printf("Receive length = %zu\n", receive_length);
                break;
            }
        }
    }

    return 0;
}