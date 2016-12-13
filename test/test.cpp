/*
 * test.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: zmij
 */

#include "math/vector.hpp"
#include "math/matrix.hpp"

#include "math/vector_io.hpp"
#include "math/matrix_io.hpp"

#include <iostream>

typedef math::vector<double, 3> vector3d;
typedef math::matrix<double, 3, 3> matrix3x3;

int
main(int argc, char* argv[]) {

	vector3d p1{1, 2, 1.5}, p2{2, 3, 5.4};

	std::cout << p1 << " " << p2
			<< " " << p1 * p2
			<< " " << p1 + p2
			<< " " << cross(p1, p2)
			<< std::endl;
	matrix3x3
	m1 {
	  { 1,  2,  3 },
	  { 4,  5,  6 },
	  { 7,  8,  9 }
	},
	m2 {
	  { 9,  8,  7 },
	  { 6,  5,  4 },
	  { 3,  2,  1 }
	};

	std::cout << m1 << "\n"
			<< m2 << "\n"
			<< m1 * m2 << "\n"
			<< m1.transpose() << "\n"
			<< matrix3x3::identity()
			<< std::endl;

	return 0;
}


