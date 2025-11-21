cmake_minimum_required(VERSION 3.10)
project(opengl)

file(GLOB SRC ${PROJECT_SOURCE_DIR}/src/*.cc)
add_executable(opengl ${SRC})

include_directories(include)

target_link_libraries(opengl glfw OpenGL GLEW)
