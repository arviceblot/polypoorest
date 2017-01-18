cmake_minimum_required(VERSION 2.8)
project(lab-12)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

find_package(SFML 1 COMPONENTS system window graphics audio REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})
link_libraries(${OPENGL_LIBRARY})

add_executable(lab-12
    renderToTexture.cpp
)
target_link_libraries(lab-12
    sive-rtutil
    graphics-lib
    ${Boost_PROGRAM_OPTIONS_LIBRARIES}
    ${SFML_LIBRARIES}
    GLEW
)
