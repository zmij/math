# CMake file for Math lib
# Created on: 30.10.2012
#    Author: zmij

cmake_minimum_required(VERSION 2.6)

project(math)
add_definitions("-std=c++11")

add_subdirectory(include)
add_subdirectory(t)
