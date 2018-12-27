# CMakeLists.txt
#   Created on: Dec 27, 2018
#       Author: ser-fedorov

file(GLOB EXAMPLES *.cpp)

foreach(file ${EXAMPLES})
  get_filename_component(tgt ${file} NAME_WE)
  message(STATUS "Adding ${tgt} example")
  add_executable(${tgt} ${file})
endforeach()