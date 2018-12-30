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

namespace psst {
namespace math {

namespace axes {
struct none {};
struct xyzw {
    static constexpr std::size_t x = 0;
    static constexpr std::size_t y = 1;
    static constexpr std::size_t z = 2;
    static constexpr std::size_t w = 3;
};
struct wxyz {
    static constexpr std::size_t w = 0;
    static constexpr std::size_t x = 1;
    static constexpr std::size_t y = 2;
    static constexpr std::size_t z = 3;
};

// FIXME Calculus for non-Cartesian coordinates
struct polar {
    static constexpr std::size_t rho     = 0;
    static constexpr std::size_t phi     = 1;
    static constexpr std::size_t r       = rho;
    static constexpr std::size_t azimuth = phi;
};
struct spherical {
    static constexpr std::size_t rho         = 0;
    static constexpr std::size_t phi         = 1;
    static constexpr std::size_t theta       = 2;
    static constexpr std::size_t r           = rho;
    static constexpr std::size_t inclination = phi;
    static constexpr std::size_t azimuth     = theta;
};
struct cylindrical {
    static constexpr std::size_t rho       = 0;
    static constexpr std::size_t phi       = 1;
    static constexpr std::size_t z         = 2;
    static constexpr std::size_t r         = rho;
    static constexpr std::size_t azimuth   = phi;
    static constexpr std::size_t elevation = z;
};

struct argb {
    static constexpr std::size_t a     = 0;
    static constexpr std::size_t r     = 1;
    static constexpr std::size_t g     = 2;
    static constexpr std::size_t b     = 3;
    static constexpr std::size_t alpha = a;
    static constexpr std::size_t red   = r;
    static constexpr std::size_t green = g;
    static constexpr std::size_t blue  = b;
};
struct rgba {
    static constexpr std::size_t r     = 0;
    static constexpr std::size_t g     = 1;
    static constexpr std::size_t b     = 2;
    static constexpr std::size_t a     = 3;
    static constexpr std::size_t red   = r;
    static constexpr std::size_t green = g;
    static constexpr std::size_t blue  = b;
    static constexpr std::size_t alpha = a;
};
struct hsva {
    static constexpr std::size_t h          = 0;
    static constexpr std::size_t s          = 1;
    static constexpr std::size_t v          = 2;
    static constexpr std::size_t a          = 3;
    static constexpr std::size_t hue        = h;
    static constexpr std::size_t saturation = s;
    static constexpr std::size_t value      = v;
    static constexpr std::size_t alpha      = a;
};
struct hsla {
    static constexpr std::size_t h          = 0;
    static constexpr std::size_t s          = 1;
    static constexpr std::size_t l          = 2;
    static constexpr std::size_t a          = 3;
    static constexpr std::size_t hue        = h;
    static constexpr std::size_t saturation = s;
    static constexpr std::size_t lightness  = l;
    static constexpr std::size_t alpha      = a;
};

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
