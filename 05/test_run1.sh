#!/bin/bash

./HTTP_client -p /index.html 165.242.111.77 80
./HTTP_client -v HTTP/1.0 -p /index.html  165.242.111.77 80
./HTTP_client -v HTTP/1.1 -p /index.html -p /members.html -h www.net.info.hiroshima-cu.ac.jp 165.242.111.77 80
