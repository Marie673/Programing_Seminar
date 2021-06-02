#!/bin/bash

./HTTP_client -p 165.242.111.77 80 > test_result_1.txt
# 2190 index.html

./HTTP_client -v HTTP/1.0 -p /index.html  165.242.111.77 80 > test_result_2.txt
# 2190 index(1).html

./HTTP_client -v HTTP/1.1 -p /index.html -p /members-j.html -h www.net.info.hiroshima-cu.ac.jp 165.242.111.77 80 > test_result_3.txt
# 2190 index(2).html
# 5285 members.html

./HTTP_client -v HTTP/1.0 -p /test.html  165.242.111.77 80 > test_result_4.txt
# Could not get Data

./HTTP_client -v HTTP/1.1 -p /index.html 165.242.111.77 80 > test_result_5.txt
# HTTP/1.1 need Host

./HTTP_client -v HTTP/1.0 -p /index.html http://www.net.info.hiroshima-cu.ac.jp 80 > test_result_6.txt
# 2190 index(3).html