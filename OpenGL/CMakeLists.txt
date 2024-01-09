cmake_minimum_required(VERSION 3.18)
project("LearnOpenGL" LANGUAGES C CXX)

if(MSVC)
    add_compile_options("/source-charset:utf-8")
endif()

set(TRD_DIR ${CMAKE_SOURCE_DIR}/third-party)
set(GLFW_DIR ${TRD_DIR}/glfw-3.3.9)
set(GLAD_DIR ${TRD_DIR}/glad)
set(GLAD_SRC_FILE ${TRD_DIR}/glad/src/glad.c)
set(SHADER_DIR ${TRD_DIR}/shader)
set(SHADER_FILE ${SHADER_DIR}/shader.h)
set(STB_IMAGE ${TRD_DIR}/stb-image)
set(GLM_DIR ${TRD_DIR}/glm)
set(CAM_DIR ${TRD_DIR}/camera)
set(CAM_FILE ${CAM_DIR}/camera.h)

set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)
set(TEX_DIR ${ASSETS_DIR}/texture)

add_subdirectory(${GLFW_DIR})


include_directories(${GLFW_DIR}/include)
include_directories(${GLAD_DIR}/include)
include_directories(${SHADER_DIR})
include_directories(${STB_IMAGE})
include_directories(${GLM_DIR}/include)
include_directories(${CAM_DIR})

link_libraries(glfw)

add_subdirectory("${CMAKE_SOURCE_DIR}/1.3.你好窗口")
add_subdirectory("${CMAKE_SOURCE_DIR}/1.4.你好三角形")
add_subdirectory("${CMAKE_SOURCE_DIR}/1.5.着色器")
add_subdirectory("${CMAKE_SOURCE_DIR}/1.6.纹理")
add_subdirectory("${CMAKE_SOURCE_DIR}/1.7.变换")
add_subdirectory("${CMAKE_SOURCE_DIR}/1.8.坐标系统")
add_subdirectory("${CMAKE_SOURCE_DIR}/1.9.摄像机")
add_subdirectory("${CMAKE_SOURCE_DIR}/2.1.颜色")
add_subdirectory("${CMAKE_SOURCE_DIR}/2.2.基础光照")
add_subdirectory("${CMAKE_SOURCE_DIR}/2.3.材质")
