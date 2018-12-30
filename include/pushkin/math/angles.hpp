/*
 * pi.hpp
 *
 *  Created on: Dec 19, 2016
 *      Author: zmij
 */

#ifndef PUSHKIN_MATH_ANGLES_HPP_
#define PUSHKIN_MATH_ANGLES_HPP_

#include <cmath>

namespace psst {
namespace math {

template <typename T>
struct pi {
    static const T value;
};

template <typename T>
const T pi<T>::value = std::atan((T)1) * 4;

/**
 * Clamp angle in the range of [0, π*2)
 * @param angle
 * @return
 */
template <typename T>
T
clamp_angle(T angle)
{
    const auto double_pi = math::pi<std::decay_t<T>>::value * 2;
    while (angle >= double_pi) {
        angle -= double_pi;
    }
    while (angle < 0) {
        angle += double_pi;
    }
    return angle;
}

template <typename T>
constexpr T
degrees_to_radians(T degrees)
{
    return degrees / 180 * pi<std::decay_t<T>>::value;
}

template <typename T>
constexpr T
radians_to_degrees(T radians)
{
    return radians / pi<std::decay_t<T>>::value * 180;
}

inline constexpr long double operator"" _deg(long double deg)
{
    return degrees_to_radians(deg);
}

inline constexpr long double operator"" _°(long double deg)
{
    return degrees_to_radians(deg);
}

} /* namespace math */
} /* namespace psst */

#endif /* PUSHKIN_MATH_ANGLES_HPP_ */
