project(CreateThread)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp)

# 如果没有下面这个链接库，则会报错
# undefined reference to `pthread_create'
# 原因在于，linux下，C++ std::thread 是对pthread的封装
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries (${PROJECT_NAME} pthread)
endif()
