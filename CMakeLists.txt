cmake_minimum_required(VERSION 2.8)
project(ray-tracer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cs5721Lib/cs5721GraphicsLib/cmake/Modules" ${CMAKE_MODULE_PATH})

# find png and zlib libs
find_package(PNG REQUIRED)
if (PNG_FOUND)
    message(STATUS "Found png: ${PNG_INCLUDE_DIR}, ${PNG_LIBRARY}")
    include_directories(${PNG_INCLUDE_DIR})
endif (PNG_FOUND)

# boost imports
set(Boost_MULTITHREADED TRUE)
set(Boost_USE_STATIC_LIBS TRUE)

find_package(Boost REQUIRED date_time program_options system thread unit_test_framework)
if(${Boost_FOUND})
    include_directories(${Boost_INCLUDE_DIR})
endif()

# Add appropriate debug flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -Wextra")

include_directories(
    ${CMAKE_SOURCE_DIR}/cs5721Lib/cs5721GraphicsLib/src
    ${CMAKE_SOURCE_DIR}/graphics-lib
    )

# Add the CS5721 Graphics Library directory
add_subdirectory(cs5721Lib/cs5721GraphicsLib)

# add local graphics lib
add_subdirectory(graphics-lib)

# Add local code
add_subdirectory(src)

#add_subdirectory(lab8)
#add_subdirectory(lab12)

#
# Unit Tests - by enabling testing with cmake, we can create a target
# that lets any of your unit tests be run when issuing a make command. After re-building your project with
# ENABLE_TESTING called, a make target, called 'test' will be enabled. You can then type
#
#  make test
#
# to perform the unit tests that you associate with the target. For more detail, see my
# CMakeLists.txt file in the utests directory of the cs5721GraphicsLib source
# for more details. Also, see the unit test .cpp files in the same utests directory for
# more examples on using Boost's unit test system.
#

#ENABLE_TESTING()
#add_subdirectory(utests)
