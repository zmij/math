cmake_minimum_required(VERSION 2.6)

set(
	MATH_HDRS
	Matrix_io.hpp
	Matrix.hpp
	Vector_io.hpp
	Vector.hpp
)

install(
	FILES ${MATH_HDRS}
	DESTINATION include/math
)
