cmake_minimum_required(VERSION 2.6)

set (
    TESTS_SRC
    test.cpp
)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)


set ( TEST test_${PROJECT_NAME} )

add_executable( ${TEST} ${TESTS_SRC} )
