cmake_minimum_required(VERSION 3.15)
project("02" LANGUAGES CXX)

# Win MSVC下需要手动指定源码编码类型
if(MSVC)
    add_compile_options("/source-charset:utf-8")
endif()

if(WIN32)

    add_executable(serv win_serv_tcp.cpp)

    add_executable(clnt win_clnt_tcp.cpp)

    target_link_libraries(serv ws2_32)

    target_link_libraries(clnt ws2_32)

else()

    add_executable(serv tcp_server.cpp)

    add_executable(clnt tcp_client.cpp)

endif()
