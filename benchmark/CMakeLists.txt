# CMakeLists.txt
# Created on: 13 дек. 2016 г.
#     Author: sergey.fedorov

find_package(ExternalProjectGBench REQUIRED)
if (NOT CMAKE_THREAD_LIBS_INIT)
    find_package(Threads REQUIRED)
endif()

# Add your package dependencies for test here

include_directories(${GBENCH_INCLUDE_DIRS})

set(benchmark_SRCS
    vector_benchmarks.cpp
    matrix_benchmarks.cpp
)
add_executable(benchmark-psst-math ${benchmark_SRCS})
target_link_libraries(benchmark-psst-math
    ${GBENCH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)
