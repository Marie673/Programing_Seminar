//
// Created by marie on 2021/05/11.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    char *end_buf;

/*    argc -= optind;
    argv += optind;
    if(argc != 1){
        fprintf(stderr, "missing host and port\n");
        exit(1);
    }

    long port;
    port = strtol(*argv, &end_buf, 10);
    */
    if(argc != 2){
        fprintf(stderr, "missing host and port\n");
        exit(1);
    }

    long port;
    port = strtol(*++argv, &end_buf, 10);

    int sock0;
    struct sockaddr_in addr;
    struct sockaddr_in client;
    socklen_t len;
    int sock;
    int n;
    char buf[32];
    char addr_str[INET_ADDRSTRLEN];

    sock0 = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sock0, (struct sockaddr*) &addr, sizeof(addr)) != 0){
        perror("bind failed");
        exit(1);
    }

    listen(sock0, 5);

    len = sizeof(client);
    sock = accept(sock0, (struct sockaddr*) &client, &len);

    inet_ntop(AF_INET, &client.sin_addr, addr_str, sizeof(addr_str));

    printf("connection from : %s, port = %d\n", addr_str, ntohs(client.sin_port));

    memset(buf, 0, sizeof(buf));
    read(sock, buf, sizeof(buf));
    printf("message : %s\n", buf);

    n = (int) write(sock, buf, strnlen(buf, sizeof(buf)));

    close(sock);
    close(sock0);

    return 0;
}
