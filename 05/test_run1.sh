#!/bin/bash

echo ./HTTP_client 165.242.111.77 80
./HTTP_client 165.242.111.77 80
# 2190 index.html

echo ./HTTP_client -v HTTP/1.0 -p /index.html  165.242.111.77 80
./HTTP_client -v HTTP/1.0 -p /index.html  165.242.111.77 80
# 2190 index(1).html

echo ./HTTP_client -v HTTP/1.1 -p /index.html -p /members-j.html -h www.net.info.hiroshima-cu.ac.jp 165.242.111.77 80
./HTTP_client -v HTTP/1.1 -p /index.html -p /members-j.html -h www.net.info.hiroshima-cu.ac.jp 165.242.111.77 80
# 2190 index(2).html
# 5285 members.html

echo ./HTTP_client -v HTTP/1.0 -p /test.html  165.242.111.77 80
./HTTP_client -v HTTP/1.0 -p /test.html  165.242.111.77 80
# Could not get Data

echo ./HTTP_client -v HTTP/1.1 -p /index.html 165.242.111.77 80
./HTTP_client -v HTTP/1.1 -p /index.html 165.242.111.77 80
# HTTP/1.1 need Host

echo ./HTTP_client -v HTTP/1.0 -p /index.html http://www.net.info.hiroshima-cu.ac.jp 80
./HTTP_client -v HTTP/1.0 -p /index.html http://www.net.info.hiroshima-cu.ac.jp 80
# 2190 index(3).html