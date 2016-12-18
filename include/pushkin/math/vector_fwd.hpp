/*
 * vector_fwd.hpp
 *
 *  Created on: 18 дек. 2016 г.
 *      Author: sergey.fedorov
 */

#ifndef INCLUDE_PUSHKIN_MATH_VECTOR_FWD_HPP_
#define INCLUDE_PUSHKIN_MATH_VECTOR_FWD_HPP_

namespace psst {
namespace math {

namespace axes {
struct none {};
struct xyzw {};
struct wxyz {};

// FIXME Vector calculus for non-Cartesian coordinates
struct polar {};
struct spherical {};
struct cylindrical {};

struct argb {};
struct rgba {};
struct hsva {};
struct hsla {};
}  /* namespace axes */

template < typename T, ::std::size_t Size, typename Axes = axes::xyzw >
struct vector;


}  /* namespace math */
}  /* namespace psst */



#endif /* INCLUDE_PUSHKIN_MATH_VECTOR_FWD_HPP_ */
