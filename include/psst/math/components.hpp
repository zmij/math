/**
 * Copyright 2018 Sergei A. Fedorov
 * components.hpp
 *
 *  Created on: Dec 30, 2018
 *      Author: ser-fedorov
 */

#ifndef PSST_MATH_COMPONENTS_HPP_
#define PSST_MATH_COMPONENTS_HPP_

#include <cstddef>
#include <limits>

namespace psst {
namespace math {

/**
 * Namespace for definitions of vector components.
 */
namespace components {
/**
 * No specific vector components, they are accessed by their index only.
 */
struct none {
    static constexpr std::size_t min_components = 0;
    static constexpr std::size_t max_components = std::numeric_limits<std::size_t>::max();
};
/**
 * Components of Cartesian vector
 */
struct xyzw {
    static constexpr std::size_t min_components = 1;
    static constexpr std::size_t max_components = 4;
    static constexpr std::size_t x              = 0;
    static constexpr std::size_t y              = 1;
    static constexpr std::size_t z              = 2;
    static constexpr std::size_t w              = 3;
};

struct xyw {
    static constexpr std::size_t min_components = 1;
    static constexpr std::size_t max_components = 3;
    static constexpr std::size_t x              = 0;
    static constexpr std::size_t y              = 1;
    static constexpr std::size_t w              = 2;
};

struct wxyz;    // Quaternion

struct polar;
struct spherical;
struct cylindrical;

template <std::size_t Size>
struct default_components {
    using type = none;
};
template <std::size_t Size>
using default_components_t = typename default_components<Size>::type;

template <>
struct default_components<1> {
    using type = xyzw;
};
template <>
struct default_components<2> {
    using type = xyzw;
};
template <>
struct default_components<3> {
    using type = xyzw;
};
template <>
struct default_components<4> {
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
}    // namespace components

}    // namespace math
}    // namespace psst

#endif /* PSST_MATH_COMPONENTS_HPP_ */
