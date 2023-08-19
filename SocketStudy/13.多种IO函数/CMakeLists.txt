cmake_minimum_required(VERSION 3.18)
project(Chapter13 LANGUAGES C CXX)

add_executable("recv" oob_recv.c)
add_executable("send" oob_send.c)
add_executable("peek_send" peek_send.cpp)
add_executable("peek_recv" peek_recv.cpp)
