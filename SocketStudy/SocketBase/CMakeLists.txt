project(SocketBase LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Win MSVC下需要手动指定源码编码类型
if (MSVC)
    add_compile_options("/source-charset:utf-8")
endif()

add_executable("client" client/main.cpp)

add_executable("server" server/main.cpp)
