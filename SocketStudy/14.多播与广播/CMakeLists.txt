cmake_minimum_required(VERSION 3.18)
project("Chapter14" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DFILEPATH="${PROJECT_SOURCE_DIR}/news.txt")

message("OS name: ${CMAKE_SYSTEM_NAME}")
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_compile_options("/source-charset:utf-8")
    add_executable("win_brd_recv" new_recv_brd_win.cpp)
    target_link_libraries("win_brd_recv" Ws2_32.lib)
else()
    add_executable("sender" news_sender.cpp)
    add_executable("receiver" news_receiver.cpp)
    add_executable("brd_sender" news_send_brd.cpp)
    add_executable("brd_receiver" news_receiver_brd.cpp)
endif()

