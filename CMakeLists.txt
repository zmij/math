# CMake file for Math lib
# Created on: 30.10.2012
#    Author: zmij

cmake_minimum_required(VERSION 2.6)

# Set library name here
set(lib_name psst-math)
string(TOUPPER ${lib_name} LIB_NAME)

set(_pname ${lib_name})
if (PROJECT_VERSION)
    set(_pversion ${PROJECT_VERSION})
else()
    set(_pversion 0.1.0)
endif()

if (${CMAKE_VERSION} VERSION_GREATER "3.0")
    cmake_policy(SET CMP0048 NEW)
    project(${_pname} VERSION ${_pversion})
else()
    project(${_pname})
    set(PROJECT_VERSION ${_pversion})
endif()

option(BUILD_TESTS "Build tests for ${lib_name} library" ON)
option(BUILD_BENCHMARKS "Build benchmarks for ${lib_name} library" ON)

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

add_definitions("-std=c++11")
add_definitions(-Wall -Werror -pedantic)
add_subdirectory(include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if (BUILD_BENCHMARKS)
    enable_testing()
    add_subdirectory(benchmark)
endif()
