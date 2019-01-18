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

option(PSST_MATH_BUILD_TESTS "Build tests for ${lib_name} library" ON)
option(PSST_MATH_BUILD_BENCHMARKS "Build benchmarks for ${lib_name} library" ON)
option(PSST_MATH_BUILD_EXAMPLES "Build examples for ${lib_name} library" ON)

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-Wall -Werror -Wpedantic)

option(USE_CCACHE "Use ccache for build" ON)
if (USE_CCACHE)
    find_program(CCACHE ccache)
    if (CCACHE)
        message(STATUS "ccache found and enabled")
        set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE})
        set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
    else()
        message(WARNING "ccache enabled, but not found")
    endif()
else()
    message(STATUS "ccache disabled")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_definitions(-Wno-error=unused-command-line-argument)
endif()

add_subdirectory(include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if (PSST_MATH_BUILD_TESTS OR PSST_MATH_BUILD_BENCHMARKS)
find_package(ExternalProjectZmijModules)
endif()

if (PSST_MATH_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if (PSST_MATH_BUILD_BENCHMARKS)
    enable_testing()
    add_subdirectory(benchmark)
endif()

if (PSST_MATH_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()