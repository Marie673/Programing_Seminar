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

int main()
{
    struct sockaddr_in dest;
    int sock;
    char buf[32];
    int n;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    dest.sin_family = AF_INET;
    dest.sin_port = htons(54321);

    inet_pton(AF_INET, "127.0.0.1", &dest.sin_addr.s_addr);

    if(connect(sock, (struct sockaddr*) &dest, sizeof(dest)) != 0){
        perror("connect failed");
        exit(1);
    }

    snprintf(buf, sizeof(buf), "message from IPv4 client");
    write(sock, buf, strnlen(buf, sizeof(buf)));

    memset(buf, 0, sizeof(buf));
    n = (int) read(sock, buf, sizeof(buf));

    printf("n=%d, %s\n", n, buf);

    close(sock);

    return 0;
}