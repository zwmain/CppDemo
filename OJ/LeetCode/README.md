# LeetCode算法题

LeetCode刷题列表

```cmake
cmake_minimum_required(VERSION 3.18.0)

project(ProjectName LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Win MSVC下需要手动指定源码编码类型
if (MSVC)
    add_compile_options("/utf-8")
endif()

set(UTIL_DIR "../0000.Utils")
set(UTIL_FILE "${UTIL_DIR}/Utils.hpp")

add_executable(${PROJECT_NAME} main.cpp main.h ${UTIL_FILE})

target_include_directories(${PROJECT_NAME} PUBLIC ${UTIL_DIR})

```

## 简单

1480.一维数组的动态和
