Psst! Math
====

[![Build Status](https://travis-ci.org/zmij/math.svg?branch=develop)](https://travis-ci.org/zmij/math)

Small C++17 template library for vector and matrix computations.

Library provides easy syntax for declaring, assigning vectors and matrices and making calculations. The `vector` and `matrix` classes are designed to have a memory layout as C++ arrays of respective elements, and can be passed to rendering libraries where pointers to floats (for example) are required. The library uses lazy expression templates for calcutations.

### Vectors and Matrices

#### Usage Synopsis

##### Declaration and assignment

```C++
// Vector
#include <psst/math/vector.hpp>

using vector3d = psst::math::vector<float, 3>;

vector3d p1{1, 2, 1.5}, p2{2, 3, 5.4};

// Matrix
#include <psst/math/matrix.hpp>

using matrix3x3 = psst::math::matrix<float, 3, 3>;

matrix3x3 m1 {
  { 1,  2,  3 },
  { 4,  5,  6 },
  { 7,  8,  9 }
};

using affine_matrix = psst::math::matrix<float, 4, 4>;

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

using vector4 = psst::math::vector<float, 4>;
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
v3 = v1 * v2;     // cross product. Defined for 3D vectors only

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
using matrix4x3 = psst::math::matrix<float, 4, 3>;
using matrix3x4 = psst::math::matrix<float, 3, 4>;

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
#include <psst/math/vector_io.hpp>
#include <psst/math/matrix_io.hpp>

namespace io = psst::math::io;

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

#### Memory buffers as vectors

A memory buffer can be accessed as a container of vectors with certain properties (size, axes). A constant buffer can be used to read data in a structured manner, a non-costant buffer can be used to modify data in the buffer via `vector_view` and `memory_vector_view` utility classes. A `vector_view` is for reading a single element, `memory_vector_view` is for using a buffer as a 'container' of vectors.

```C++
#include <psst/math/vector_view.hpp>

using namespace psst::math;

// Size of the vector is 16 bytes
using vec4f = vector<float, 4>;

char const* const_buffer = "..."; // Get this buffer somewhere
std::size_t buffer_size = 256; // this is 16 vectors

auto const_mem_view = make_memory_view<vec4f>(const_buffer, buffer_size);

for (auto mv : const_mem_view) {
  // do something with the vectors, no modification is available
}


char const* mutable_buffer = "..."; // Get this buffer somewhere

auto mem_view = make_memory_view<vec4f>(mutable_buffer, buffer_size);

for (auto mv : mem_view) {
  // do something with the vectors, modification is available
  mv *= 2;
}

```


### Quaternions

The libbrary provides quaternions and operations with them, such as sum, substraction, multiplication and division by scalar, quaternion multiplication, magnitude, normalize, conjugate and inverse functions. Components of a quaternion are accessible via `w()`, `x()`, `y()` and `z()` accessors, where `w()` is the real part and `x()`, `y()` and `z()` are coefficients for i, j and k respectively. Also, the scalar part is accessible via `scalar_part()` member function, and the vector part is accessible via `vector_part()`.

```C++
#include <psst/math/quaternion.hpp>

using quat = psst::math::quaternion<double>;

quat q1{1, 2, 3, 4}, q2{5, 6, 7, 8};
// quaternion sum and difference
auto sum = q1 + q2;
auto diff = q1 - q2;
// multiplication and division by scalar
auto mul = q1 * 2;
auto div = q2 / 3;
// quaternion multiplication
auto q3 = q1 * q2;
// misc function
auto mag = magnitude(q1);
auto c = conjugate(q2);
auto u = normalize(q1);
auto i = inverse(q2);
```

#### Example of Using Quaternions for Rotation

```C++
#include <psst/math/quaternion.hpp>

using quat = psst::math::quaternion<double>;
using vec3 = psst::math::std::vector<double, 3>;

vec3
rotate(vec3 v, vec3 axis, double angle)
{
    using std::cos;
    using std::sin;
    auto unit = normalize(quat{ 0, axis.x(), axis.y(), axis.z() });
    auto rot =  quat{cos(angle / 2), 0, 0, 0} + unit * sin(angle / 2);
    return (rot * quat{0, v.x(), v.y(), v.z()} * inverse(rot)).vector_part();
}

```

### Polar, Spherical and Cylindrical Coordinates

The library provides polar, spherical and cylindrical coordinates and conversion between them and XYZ coordinates. 

Components of polar coordinates:
1. #0 `r()` or `rho()`, the radius component.
2. #1 `phi()` or `azimuth()`, the azimuth component, the value is in radians between zero and 2π, the value is normalized automatically.

Componetns of spherical coordinates:
1. #0 `r()` or `rho()`, the radius component.
2. #1 `phi()` or `inclination()`, the angle between the projection on the plane and vector. The range of value is [-π/2, π/2], the value is clamped automatically.
3. #2 `theta()` or `azimuth()`, the azimuth component, the value is in radians between zero and 2π, the value is normalized automatically.

Components of cylindrical coordinates:
1. #0 `r()` or `rho()`, the radius component.
2. #1 `phi()` or `azimuth()`, the azimuth component, the value is in radians between zero and 2π, the value is normalized automatically.
3. #2 `z()` or `elevation()`, the height above zero.

Conversion is defined for:
* XYZ <-> polar
* XYZ <-> spherical
* XYZ <-> cylindrical
* polar <-> spherical
* polar <-> cylindrical
* spherical <-> cylindrical

```C++
#include <psst/math/polar_coord.hpp>
#include <psst/math/spherical_coord.hpp>
#include <psst/math/cylindrical_coord.hpp>

using polar_c       = psst::math::polar_coord<double>;
using spherical_c   = psst::math::spherical_coord<double>
using cylindrical_c = psst::math::cylindrical_coord<double>;
using vec3          = psst::math::vector<double, 3>;

using psst::math::operator "" _deg;

polar_c p{10, 180_deg};
spherical_c s = convert<spherical_c>(p);
s.inclination() = 45_deg;
cylindrical_c c = convert<cylindrical_c>(s);
vec3 v = convert<vec3>(c);
```

### Colors

Based on vector class and expressions, the library provides classes for color calculateions in RGB, HSL ans HSV color spaces. For color classes the following operations are defined:
* sum and difference
* multiplication and division by scalar
* lerp and slerp

Components of rgba color:
1. #0 `r()` or `red()`
2. #1 `g()` or `green()`
3. #2 `b()` or `blue()`
4. #3 `a()` or `alpha()`

Components of hsla color:
1. #0 `h()` or `hue()`
2. #1 `s()` or `saturation()`
3. #2 `l()` or `lightness()`
4. #3 `a()` or `alpha()`

Components of hsva color:
1. #0 `h()` or `hue()`
2. #1 `s()` or `saturation()`
3. #2 `v()` or `value()`
4. #3 `a()` or `alpha()`

Conversions are defined for:
* RGB <-> HSL
* RGB <-> HSV

```C++
#include <psst/math/colors.hpp>

using rgba = psst::math::colors::rgba<float>;
using hsla = psst::math::colors::hsla<float>;
using hsva = psst::math::colors::hsva<float>;

using psst::math::operator "" _rgba;

rgba col1 = convert<rgba>( 0xff0000ff_rgba ); // red
hsla hl1  = convert<hsla>(col1);
hsva hv1  = convert<hsva>(col1);
```
