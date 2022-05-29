cmake_minimum_required(VERSION 3.18.0)

project("0383" LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Win MSVC下需要手动指定源码编码类型
if (MSVC)
    add_compile_options("/utf-8")
endif()

add_executable(${PROJECT_NAME} main.cpp main.h)
