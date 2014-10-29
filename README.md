math
====

Small C++11 library for vector and matrix computations.
Library provides easy syntax for declaring, assigning vectors and matrices and making calculations.

#### Usage Synopsis

##### Declaration and assignment

```C++
// Vector
#include "math/vector.hpp"

typedef math::vector<float, 3> vector3d;

vector3d p1{1, 2, 1.5}, p2{2, 3, 5.4};

// Matrix
#include "math/matrix.hpp"

typedef math::matrix<float, 3, 3> matrix3x3;

matrix3x3 m1 {
  { 1,  2,  3 },
  { 4,  5,  6 },
  { 7,  8,  9 }
};

typedef math::matrix<float, 4, 4> affine_matrix;

affine_matrix
rotate_x( float a )
{
	return
	{
		{	1,		0,				      0,					      0	},
		{	0,		(float)cos(a),	(float)-sin(a),		0	},
		{	0,		(float)sin(a),	(float)cos(a),		0	},
		{	0,		0,				      0,					      1	}
	};
}
```

##### Operations
```C++
// Vector
vector3d v3 = v1 + v2;  // vector sum
v3 = v1 - v2;           // vector difference
v3 = -v1;               // negate vector
v3 *= 3;                // multiply by scalar
v3 = v1 * 5;
v3 /= 5;                // divide by scalar
v3 = v2 / 10;

v3 = normalize(v1);     // normalize vector
vector3d::value_type s = v3.magnitude_square(); // vector magnitude squared
s = v3.magnitude()      // vector magnitude

vector3d::value_type p = v1 * v2; // dot product
v3 = cross(v1, v2);     // cross product. Defined for 3D and 3D homogenous (having 4 elements) vectors only

v3 = projection( v1, v2 ); // projection of vector v2 onto vector v1
v3 = perpendicular( v1, v2 ); // vector that is perpendicular to v1, such as v3 + projection( v1 , v2 ) == v2
std::pair< vector3d, vector3d > pair = project( v1, v2 ); // a pair of projection of v2 onto v1 and a perpendicular to v1

vector3d::value_type s = distance_square(v1, v2); // returns squared magnitude of vectors difference. Semantic sugar when vectors are treated as coordinates
s = distance( v1, v2 );   // magnitude of vectors difference

// Matrix
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

matrix3x3 m3 = m1 + m2; // matrix sum
m3 = m1 * 5; // matrix scalar multiplication
m3 *= 4;
m3 = m2 / 8; // matrix scalar division
m3 /= 3;
matrix3x3 i = matrix3x3::identity(); // identity matrix

// Rectangular matrices
typedef math::matrix<float, 4, 3> matrix4x3;
typedef math::matrix<float, 3, 4> matrix3x4;

matrix4x3 r1 {
  { 1,  2,  3 },
  { 1,  2,  3 },
  { 1,  2,  3 },
  { 1,  2,  3 }
};
matrix3x4 r2 {
  { 1, 2, 3, 4},
  { 1, 2, 3, 4},
  { 1, 2, 3, 4}
};

matrix3x4 t = r1.transpose(); // matrix transposition
matrix3x3 m4 = r1 * r2; // matrix multiplication
vector3d v4 = m1 * v1; // matrix by vector multiplication (vector is considered a 1 column matrix)
vector3d v5 = v1 * m1; // matrix by vector multiplication (vector is considered a 1 row matrix)
```

##### Output

```C++
#include <iostream>
#include "math/vector_io.hpp"
#include "math/matrix_io.hpp"

std::cout << v1 << std::endl;
// output {1, 2, 1.5}
std::cout << m1 << std::endl;
// output {
//	{1,2,3},
//	{4,5,6},
//	{7,8,9}
//}
```
