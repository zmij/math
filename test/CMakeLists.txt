#    /math/test/CMakeLists.txt
#
#    @author zmij
#    @date Dec 13, 2016

cmake_minimum_required(VERSION 2.6)

find_package(ExternalProjectGTest REQUIRED)
if (NOT CMAKE_THREAD_LIBS_INIT)
    find_package(Threads REQUIRED)
endif()

# Add your package dependencies for test here

include_directories(${GTEST_INCLUDE_DIRS})

set(
    test_program_SRCS
    # Add your sources here
    misc_tests.cpp
    vector_test.cpp
    vector_view_tests.cpp
    matrix_test.cpp
    quaternion_tests.cpp
    color_tests.cpp
    random_tests.cpp
)
add_executable(test-psst-math ${test_program_SRCS})
target_link_libraries(
    test-psst-math
    ${GTEST_BOTH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    # Add your libraries here
)

if (GTEST_XML_OUTPUT)
    set (
        TEST_ARGS
        --gtest_output=xml:test-psst-math-detail.xml
    )
endif()

add_test(
    NAME test-psst-math
    COMMAND test-psst-math ${TEST_ARGS}
)
