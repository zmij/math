/**
 * Copyright 2018 Sergei A. Fedorov
 * axes.hpp
 *
 *  Created on: Dec 30, 2018
 *      Author: ser-fedorov
 */

#ifndef PUSHKIN_MATH_AXES_HPP_
#define PUSHKIN_MATH_AXES_HPP_

#include <cstddef>
#include <limits>

namespace psst {
namespace math {

namespace axes {
struct none {
    static constexpr std::size_t min_components = 0;
    static constexpr std::size_t max_components = std::numeric_limits<std::size_t>::max();
};
struct xyzw {
    static constexpr std::size_t min_components = 1;
    static constexpr std::size_t max_components = 4;
    static constexpr std::size_t x              = 0;
    static constexpr std::size_t y              = 1;
    static constexpr std::size_t z              = 2;
    static constexpr std::size_t w              = 3;
};
struct wxyz;    // Quaternion

// FIXME Calculus for non-Cartesian coordinates
struct polar;
struct spherical;
struct cylindrical;

template <std::size_t Size>
struct default_axes {
    using type = none;
};
template <std::size_t Size>
using default_axes_t = typename default_axes<Size>::type;

template <>
struct default_axes<1> {
    using type = xyzw;
};
template <>
struct default_axes<2> {
    using type = xyzw;
};
template <>
struct default_axes<3> {
    using type = xyzw;
};
template <>
struct default_axes<4> {
    using type = xyzw;
};

//@{
/** @name undefined */
template <typename T>
struct undefined : std::false_type {};
template <typename T>
using undefined_t = typename undefined<T>::type;
template <typename T>
constexpr bool undefined_v = undefined_t<T>::value;
template <>
struct undefined<none> : std::true_type {};
//@}
} /* namespace axes */

}    // namespace math
}    // namespace psst

#endif /* PUSHKIN_MATH_AXES_HPP_ */
