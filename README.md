# CppDemo

C++实例

ModernCpp：现代C++包括C++11、C++14、C++17。C++20待完善

MyJson：C++ JSON实现方法

ThreadPool：基于C++11的线程池，可以支持任何函数和返回值

ThreadStudy：C++多线程基本知识学习

```cmake
cmake_minimum_required(VERSION 3.18.0)

project(ProjectName LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Win MSVC下需要手动指定源码编码类型
if (MSVC)
    add_compile_options("/source-charset:utf-8")
endif()

add_executable(${PROJECT_NAME} main.cpp)

```
