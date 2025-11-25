cmake_minimum_required(VERSION 3.10)
project(opengl)

file(GLOB SRC ${PROJECT_SOURCE_DIR}/src/*.cc)
add_executable(opengl ${SRC})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(RES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res")
file(COPY ${RES_DIR} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
include_directories(include)

target_link_libraries(opengl glfw OpenGL GLEW)
