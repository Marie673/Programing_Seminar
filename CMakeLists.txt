cmake_minimum_required(VERSION 3.19)
project(Programing C)

set(CMAKE_C_STANDARD 99)

add_executable(Programing test.c pre/kadai1.c pre/kadai2.c pre/kadai3.c pre/kadai4.c pre/kadai5.c pre/kadai6.c
        pre/kadai7.c pre/kadai8.c 01/kadai1.c 01/kadai2.c 01/kadai3.c 01/kadai4.c 01/kadai5.c 01/kadai6.c
        01/kadai7.c 01/kadai8.c 02/tcp-client.c 02/tcp-server.c
        03/HTTP_09.c 03/HTTP_10.c
        04/HTTP_11.c 04/HTTP_11_pipeline.c 05/HTTP_client.c 06/HTTP09_server.c 06/HTTP_server.c
        07/client/HTTP_client.c 07/server/HTTP_server.c 07/client/send_request.c 07/client/send_request.h 07/client/HTTP_client.h)