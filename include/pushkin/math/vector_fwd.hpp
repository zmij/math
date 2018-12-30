/*
 * vector_fwd.hpp
 *
 *  Created on: 18 дек. 2016 г.
 *      Author: sergey.fedorov
 */

#ifndef INCLUDE_PUSHKIN_MATH_VECTOR_FWD_HPP_
#define INCLUDE_PUSHKIN_MATH_VECTOR_FWD_HPP_

#include <pushkin/math/axes.hpp>

namespace psst {
namespace math {

// TODO Parameter for calculus
template <typename T, std::size_t Size, typename Axes = axes::default_axes_t<Size>>
struct vector;

} /* namespace math */
} /* namespace psst */

#endif /* INCLUDE_PUSHKIN_MATH_VECTOR_FWD_HPP_ */
