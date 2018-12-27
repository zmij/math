pushkin-math
====

Small C++17 template library for vector and matrix computations.

Library provides easy syntax for declaring, assigning vectors and matrices and making calculations. The `vector` and `matrix` classes are designed to have a memory layout as C++ arrays of respective elements, and can be passed to rendering libraries where pointers to floats (for example) are required. The library uses lazy expression templates for calcutations.

#### Usage Synopsis

##### Declaration and assignment

```C++
// Vector
#include <pushkin/math/vector.hpp>

using vector3d = ::psst::math::vector<float, 3>;

vector3d p1{1, 2, 1.5}, p2{2, 3, 5.4};

// Matrix
#include <pushkin/math/matrix.hpp>

using matrix3x3 = ::psst::math::matrix<float, 3, 3>;

matrix3x3 m1 {
  { 1,  2,  3 },
  { 4,  5,  6 },
  { 7,  8,  9 }
};

using affine_matrix = ::psst::math::matrix<float, 4, 4>;

affine_matrix
rotate_x( float a )
{
  using std::cos;
  using std::sin;
  return
  {
    {  1  ,     0   ,      0  ,   0  },
    {  0  , cos(a)  , -sin(a) ,   0  },
    {  0  , sin(a)  ,  cos(a) ,   0  },
    {  0  ,     0   ,      0  ,   1  }
  };
}
```

##### Element access

`vector` class provides access to all elements by indexes (subscript operator) and template function `at<N>()`. First four elements of vector are accessible by named functions `x()`, `y()`, `z()` and `w()` respectively. Those functions are defined only where the size of vector allows it, e.g. for a three-element vector there will be no `w()` function. Vectors elements can be iterated with a C++11 range loop or using iterators.

`matrix` class provides subscript operator and template function `at<N>()` to access rows that are represented by vectors. Iterators to rows are provided by `row_begin()`/`row_end()` function pairs. `begin()`/`end()` pairs will provide iterators over all matrix elements, in the order a C++ two-dimensional array would be layed out.

```C++

using vector4 = ::psst::math::vector<float, 4>;
vector4 v1{1, 2, 3, 4};

auto x = v1[0];
x = v1.at<0>();
x = v1.x();
auto y = v1[1];
y = v1.y();
auto z = v1[2];
z = v1.z();
auto w = v1[3];
w = v1.w();

v1.x() = x;
v1.at<0>() = x;
v1[0] = x;
v1.y() = y;
v1.z() = z;
v1.w() = w;

matrix3x3 m1 {
  { 1,  2,  3 },
  { 4,  5,  6 },
  { 7,  8,  9 }
};
m1[0][0] = 5;
auto a = m1[2].z();
m1[1].y() = 8;
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
auto s = v3.magnitude_square(); // vector magnitude squared
s = v3.magnitude()      // vector magnitude

auto p = v1 | v2; // dot product
v3 = v1 ^ v2;     // cross product. Defined for 3D and 3D homogenous (having 4 elements) vectors only

v3 = projection( v1, v2 ); // projection of vector v2 onto vector v1
v3 = perpendicular( v1, v2 ); // vector that is perpendicular to v1, such as v3 + projection( v1 , v2 ) == v2
std::pair< vector3d, vector3d > pair = project( v1, v2 ); // a pair of projection of v2 onto v1 and a perpendicular to v1

auto s = distance_square(v1, v2); // returns squared magnitude of vectors difference. Semantic sugar when vectors are treated as coordinates
s = distance( v1, v2 );           // magnitude of vectors difference

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

auto m3 = m1 + m2;              // matrix sum
m3 = m1 - m2;                   // matrix difference
m3 = m1 * 5;                    // matrix scalar multiplication
m3 *= 4;
m3 = m2 / 8;                    // matrix scalar division
m3 /= 3;
auto i = matrix3x3::identity(); // identity matrix

// Rectangular matrices
using matrix4x3 = ::psst::math::matrix<float, 4, 3>;
using matrix3x4 = ::psst::math::matrix<float, 3, 4>;

matrix4x3 r1 {
  { 1,  2,  3 },
  { 1,  2,  3 },
  { 1,  2,  3 },
  { 1,  2,  3 }
};
matrix3x4 r2 {
  { 1, 2, 3, 4 },
  { 1, 2, 3, 4 },
  { 1, 2, 3, 4 }
};

matrix3x4 t = transpose(r1);          // matrix transposition
matrix3x3 m4 = r1 * r2;               // matrix multiplication
vector3d v4 = m1 * as_col_matrix(v1); // matrix by vector multiplication
vector3d v5 = as_row_matrix(v1) * m1; // vector by matrix multiplication
```

##### Output

```C++
#include <iostream>
#include <pushkin/math/vector_io.hpp>
#include <pushkin/math/matrix_io.hpp>

namespace io = ::psst::math::io;

std::cout << v1 << "\n";
// output {1,2,1.5}
std::cout << m1 << "\n";
// output {{1,2,3},{4,5,6},{7,8,9}}
std::cout << io::pretty << m1 << io::ugly << "\n";
// output
// {
//      { 1, 2, 3 },
//      { 4, 5, 6 },
//      { 7, 8, 9 }
// }
```
