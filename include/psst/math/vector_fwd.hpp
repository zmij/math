/*
 * vector_fwd.hpp
 *
 *  Created on: 18 дек. 2016 г.
 *      Author: sergey.fedorov
 */

#ifndef PSST_MATH_VECTOR_FWD_HPP_
#define PSST_MATH_VECTOR_FWD_HPP_

#include <psst/math/axes.hpp>

namespace psst {
namespace math {

template <typename T, std::size_t Size, typename Axes = axes::default_axes_t<Size>>
struct vector;

/**
 * The primary template for a vector_view.
 *
 * vector_view is an utility to read memory regions as vectors, use them in expressions, etc.
 *
 * For a const pointer the vector_view is read-only, for a non-const pointer the vector view can be
 * used to modify the underlying data.
 */
template <typename T, std::size_t Size, typename Axes = axes::default_axes_t<Size>>
struct vector_view;

} /* namespace math */
} /* namespace psst */

#endif /* PSST_MATH_VECTOR_FWD_HPP_ */
