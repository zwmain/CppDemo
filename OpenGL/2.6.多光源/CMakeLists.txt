cmake_minimum_required(VERSION 3.18)
project("2.6" LANGUAGES CXX)

set(VER_GLSL_FILE ${PROJECT_SOURCE_DIR}/box.vert)
set(FRG_GLSL_FILE ${PROJECT_SOURCE_DIR}/box.frag)
set(LIGFRG_GLSL_FILE ${PROJECT_SOURCE_DIR}/light.frag)
set(LIGVER_GLSL_FILE ${PROJECT_SOURCE_DIR}/light.vert)

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
