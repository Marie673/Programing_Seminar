//
// Created by marie on 2021/06/15.
//

#ifndef PROGRAMING_HTTP_CLIENT_H
#define PROGRAMING_HTTP_CLIENT_H

#endif //PROGRAMING_HTTP_CLIENT_H

#define SOCK_BUF_SIZE 1024
#define PATH_LEN 128

char *PROTOCOL_VERSION = "HTTP/0.9";
int IS_HTTP_11 = 0;


struct request{
    char *method;
    struct path *p_head;
    char *protocol_version;
    char *host;
};

struct path{
    char *path_name;
    char save_file[PATH_LEN];
    struct path *next;
};