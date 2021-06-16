//
// Created by marie on 2021/06/15.
//

#include "send_request.h"
#include "HTTP_client.h"

#include <stdio.h>
#include <string.h>


int send_request(FILE *write_fp, struct request *req) {
    char send_buf[SOCK_BUF_SIZE];
    struct path *path;

    int i;
    path = req->p_head;
    for(i = 0; i < req->path_num; i++){
        if (strncmp(PROTOCOL_VERSION, "HTTP/0.9", strlen(PROTOCOL_VERSION)) == 0) {
            snprintf(send_buf, sizeof(send_buf), "GET %s\r\n\r\n", path->path_name);
        }
        else if (strncmp(PROTOCOL_VERSION, "HTTP/1.0", strlen(PROTOCOL_VERSION)) == 0) {
            snprintf(send_buf, sizeof(send_buf), "GET %s HTTP/1.0\r\n\r\n", path->path_name);
        }
        else if (strncmp(PROTOCOL_VERSION, "HTTP/1.1", strlen(PROTOCOL_VERSION)) == 0) {
            snprintf(send_buf, sizeof(send_buf),"GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path->path_name, req->host);
        }
        fprintf(write_fp, "%s", send_buf);
        fflush(write_fp);
        path = path->next;
    }

    return 0;
}
