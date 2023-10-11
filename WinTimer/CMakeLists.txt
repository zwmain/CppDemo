cmake_minimum_required(VERSION 3.18)
project("Timer" LANGUAGES CXX)

if(MSVC)
    add_compile_options("/source-charset:utf-8")
endif()

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC "Winmm.lib")

