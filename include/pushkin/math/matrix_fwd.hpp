/*
 * matrix_fwd.hpp
 *
 *  Created on: 18 дек. 2016 г.
 *      Author: sergey.fedorov
 */

#ifndef INCLUDE_PUSHKIN_MATH_MATRIX_FWD_HPP_
#define INCLUDE_PUSHKIN_MATH_MATRIX_FWD_HPP_

#include <pushkin/math/axes.hpp>

namespace psst {
namespace math {

template <typename T, size_t RC, size_t CC,
          typename Axes = axes::default_axes_t<(CC > RC) ? CC : RC>>
struct matrix;

} /* namespace math */
} /* namespace psst */

#endif /* INCLUDE_PUSHKIN_MATH_MATRIX_FWD_HPP_ */
