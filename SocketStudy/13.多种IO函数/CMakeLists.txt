cmake_minimum_required(VERSION 3.18)
project(Chapter13 LANGUAGES C)

add_executable("recv" oob_recv.c)
add_executable("send" oob_send.c)
