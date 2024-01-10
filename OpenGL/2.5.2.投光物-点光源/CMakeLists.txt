cmake_minimum_required(VERSION 3.18)
project("2.5.2" LANGUAGES CXX)

set(VER_GLSL_FILE ${PROJECT_SOURCE_DIR}/ver.glsl)
set(FRG_GLSL_FILE ${PROJECT_SOURCE_DIR}/frg.glsl)
set(LIGFRG_GLSL_FILE ${PROJECT_SOURCE_DIR}/light-frg.glsl)
set(LIGVER_GLSL_FILE ${PROJECT_SOURCE_DIR}/light-ver.glsl)

add_definitions(-DTEX_DIR="${TEX_DIR}")
add_definitions(-DVER_GLSL_FILE="${VER_GLSL_FILE}")
add_definitions(-DFRG_GLSL_FILE="${FRG_GLSL_FILE}")
add_definitions(-DLIGFRG_GLSL_FILE="${LIGFRG_GLSL_FILE}")
add_definitions(-DLIGVER_GLSL_FILE="${LIGVER_GLSL_FILE}")

add_executable(
    ${PROJECT_NAME}
    main.cpp
    ${GLAD_SRC_FILE}
    ${CAM_FILE}
)
