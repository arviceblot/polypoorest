cmake_minimum_required(VERSION 2.8)
project(lab-8)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

find_package(SFML 1 COMPONENTS system window graphics audio REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})
link_libraries(${OPENGL_LIBRARY})

add_executable(lab-8
    lab-8.cpp
    GLSL.cpp GLSL.h
    VBO.cpp VBO.h
)
target_link_libraries(lab-8
    sive-rtutil
    graphics-lib
    ${SFML_LIBRARIES}
    GLEW
)
