cmake_minimum_required(VERSION 3.15)
project("01" LANGUAGES CXX)

# Win MSVC下需要手动指定源码编码类型
if(MSVC)
    add_compile_options("/source-charset:utf-8")
endif()

if(WIN32)

    add_executable(serv win_serv_hello.cpp)

    add_executable(clnt win_clnt_hello.cpp)

    target_link_libraries(serv ws2_32)

    target_link_libraries(clnt ws2_32)

else()

    add_executable(serv hello_server.cpp)

    add_executable(clnt hello_client.cpp)

endif()
