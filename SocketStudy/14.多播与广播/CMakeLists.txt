cmake_minimum_required(VERSION 3.18)
project("Chapter14" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DFILEPATH="${PROJECT_SOURCE_DIR}/news.txt")

add_executable("sender" news_sender.cpp)
add_executable("receiver" news_receiver.cpp)
add_executable("brd_sender" news_send_brd.cpp)

