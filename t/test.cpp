/*
 * test.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: zmij
 */

#include "math/vector.hpp"
#include "math/matrix.hpp"

#include "math/vector_io.hpp"

#include <iostream>

typedef math::vector<int, 2> point;

int
main(int argc, char* argv[]) {

	point p1{1, 2}, p2{2, 3};

	std::cout << p1 << " " << p2
			<< " " << p1*p2
			<< " " << p1+p2
			<< std::endl;

	return 0;
}


