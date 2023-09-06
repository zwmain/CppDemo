cmake_minimum_required(VERSION 3.18.0)
project(FmtTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options("/source-charset:utf-8")
endif()

add_subdirectory("fmt/")

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC "./fmt/include")
target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt)
