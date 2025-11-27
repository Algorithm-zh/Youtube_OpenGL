cmake_minimum_required(VERSION 3.10)
project(opengl)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# 收集资源文件
file(GLOB SRC ${PROJECT_SOURCE_DIR}/src/*.cc
              ${PROJECT_SOURCE_DIR}/src/vendor/*/*.cc)

add_executable(opengl ${SRC})

include_directories(${PROJECT_SOURCE_DIR}/src/vendor)

#复制资源文件
set(RES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res")
file(COPY ${RES_DIR} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(opengl glfw OpenGL GLEW)
