cmake_minimum_required(VERSION 3.18)
project("3.3" LANGUAGES CXX)

set(VER_GLSL_FILE ${PROJECT_SOURCE_DIR}/box.vert)
set(FRG_GLSL_FILE ${PROJECT_SOURCE_DIR}/box.frag)
set(LIGFRG_GLSL_FILE ${PROJECT_SOURCE_DIR}/light.frag)
set(LIGVER_GLSL_FILE ${PROJECT_SOURCE_DIR}/light.vert)
set(MODVER_GLSL_FILE ${PROJECT_SOURCE_DIR}/model.vert)
set(MODFRG_GLSL_FILE ${PROJECT_SOURCE_DIR}/model.frag)

add_definitions(-DTEX_DIR="${TEX_DIR}")
add_definitions(-DVER_GLSL_FILE="${VER_GLSL_FILE}")
add_definitions(-DFRG_GLSL_FILE="${FRG_GLSL_FILE}")
add_definitions(-DLIGFRG_GLSL_FILE="${LIGFRG_GLSL_FILE}")
add_definitions(-DLIGVER_GLSL_FILE="${LIGVER_GLSL_FILE}")
add_definitions(-DMODEL_DIR="${MODEL_DIR}")
add_definitions(-DMODVER_GLSL_FILE="${MODVER_GLSL_FILE}")
add_definitions(-DMODFRG_GLSL_FILE="${MODFRG_GLSL_FILE}")

add_library(
    ${PROJECT_NAME}
    STATIC
    model.hpp
    ${GLAD_SRC_FILE}
    ${SHADER_FILE}
)
set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    HEADER_FILE_ONLY ON
)
target_include_directories(${PROJECT_NAME} PUBLIC "${ROOT_DIR}/3.2.网格")

add_executable(
    "${PROJECT_NAME}.2"
    main.cpp
    ${GLAD_SRC_FILE}
    ${SHADER_FILE}
)
target_include_directories("${PROJECT_NAME}.2" PUBLIC "${ROOT_DIR}/3.2.网格")
target_include_directories("${PROJECT_NAME}.2" PUBLIC ./)
