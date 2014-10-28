cmake_minimum_required(VERSION 2.6)

set (Boost_USE_STATIC_LIBS ON)

set (
	TESTS_SRC
	test.cpp
)

find_package( Boost COMPONENTS unit_test_framework REQUIRED )

include_directories(
	${PROJECT_SOURCE_DIR}/include
)


set ( TEST test_${PROJECT_NAME} )

add_executable( ${TEST} ${TESTS_SRC} )
