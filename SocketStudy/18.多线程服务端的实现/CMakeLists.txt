cmake_minimum_required(VERSION 3.18)
project("MultiServer" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DDATA_PATH="${CMAKE_SOURCE_DIR}/server.cpp")

add_executable("serv" server.cpp)
add_executable("clnt" client.cpp)
target_include_directories("serv" PUBLIC "../../../ThreadPool")

