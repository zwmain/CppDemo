cmake_minimum_required(VERSION 3.18)
project("MultiServer" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable("serv" server.cpp)
target_include_directories("serv" PUBLIC "../../../ThreadPool")

