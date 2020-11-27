/*
 * vector_fwd.hpp
 *
 *  Created on: 18 дек. 2016 г.
 *      Author: sergey.fedorov
 */

#ifndef PSST_MATH_VECTOR_FWD_HPP_
#define PSST_MATH_VECTOR_FWD_HPP_

#include <psst/math/components.hpp>

namespace psst {
namespace math {

template <typename T, std::size_t Size,
          typename Components = components::default_components_t<Size>>
struct vector;

/**
 * Ordering of components in a vector view
 */
enum class component_order { forward, reverse };

/**
 * The primary template for a vector_view.
 *
 * vector_view is an utility to read memory regions as vectors, use them in expressions, etc.
 *
 * For a const pointer the vector_view is read-only, for a non-const pointer the vector view can be
 * used to modify the underlying data.
 */
template <typename T, std::size_t Size,
          typename Components = components::default_components_t<Size>,
          component_order     = component_order::forward>
struct vector_view;

} /* namespace math */
} /* namespace psst */

#endif /* PSST_MATH_VECTOR_FWD_HPP_ */
