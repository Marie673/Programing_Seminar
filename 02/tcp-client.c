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
    if(argc != 2){
        fprintf(stderr, "missing host and port\n");
        exit(1);
    }
    char *host;
    long port;
    host = (--argc, *argv++);
    port = strtol(*argv, &end_buf, 10);
*/
    if(argc != 3){
        fprintf(stderr, "missing host and port\n");
        exit(1);
    }
    char *host;
    long port;
    host = *++argv;
    port = strtol(*++argv, &end_buf, 10);
    


    struct sockaddr_in dest;
    int sock;
    char buf[32];
    int n;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);

    inet_pton(AF_INET, host, &dest.sin_addr.s_addr);

    if(connect(sock, (struct sockaddr*) &dest, sizeof(dest)) != 0){
        perror("connect failed");
        exit(1);
    }

    printf("type a one-line :\n");
    fgets(buf, sizeof(buf), stdin);
    write(sock, buf, strnlen(buf, sizeof(buf)));
    printf("send to server      : %s\n", buf);

    memset(buf, 0, sizeof(buf));
    n = (int) read(sock, buf, sizeof(buf));
    printf("receive from server : %s\n", buf);

    close(sock);

    return 0;
}
