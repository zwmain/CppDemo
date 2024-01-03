cmake_minimum_required(VERSION 3.18)
project("1.4" LANGUAGES CXX)

add_executable(
    ${PROJECT_NAME}
    main.cpp
    ${GLAD_SRC_FILE}
)

add_executable(
    "${PROJECT_NAME}.2"
    main2.cpp
    ${GLAD_SRC_FILE}
)
