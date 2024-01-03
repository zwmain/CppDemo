cmake_minimum_required(VERSION 3.18)
project("1.3" LANGUAGES CXX)

add_executable(
    ${PROJECT_NAME}
    main.cpp
    ${GLAD_SRC_FILE}
)
