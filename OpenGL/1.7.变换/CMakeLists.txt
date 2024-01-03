cmake_minimum_required(VERSION 3.18)
project("1.7" LANGUAGES CXX)

set(VER_GLSL_FILE ${PROJECT_SOURCE_DIR}/ver.glsl)
set(FRG_GLSL_FILE ${PROJECT_SOURCE_DIR}/frg.glsl)

add_definitions(-DTEX_DIR="${TEX_DIR}")
add_definitions(-DVER_GLSL_FILE="${VER_GLSL_FILE}")
add_definitions(-DFRG_GLSL_FILE="${FRG_GLSL_FILE}")

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
