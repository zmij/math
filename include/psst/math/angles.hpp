/*
 * pi.hpp
 *
 *  Created on: Dec 19, 2016
 *      Author: zmij
 */

#ifndef PSST_MATH_ANGLES_HPP_
#define PSST_MATH_ANGLES_HPP_

#include <psst/math/detail/value_policy.hpp>

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
constexpr T
zero_to_two_pi(T const& val)
{
    const auto double_pi = math::pi<std::decay_t<T>>::value * 2;
    T          angle     = val;
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
minus_plus_half_pi(T const& angle)
{
    const auto half_pi = math::pi<std::decay_t<T>>::value / 2;
    if (angle < -half_pi)
        return -half_pi;
    if (angle > half_pi)
        return half_pi;
    return angle;
}

template <typename T>
constexpr T
minus_plus_pi(T const& val)
{
    const auto pi        = math::pi<std::decay_t<T>>::value * 2;
    const auto double_pi = pi * 2;

    T angle = val;
    while (angle > pi) {
        angle -= double_pi;
    }
    while (angle < -pi) {
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

inline constexpr double operator"" _deg(long double deg)
{
    return degrees_to_radians(deg);
}

inline constexpr double operator"" _deg(unsigned long long int deg)
{
    return degrees_to_radians<double>(deg);
}

namespace value_policy {

template <typename T>
using clamp_zero_to_two_pi = value_clamp<T, zero_to_two_pi<T>>;
template <typename T>
using clamp_minus_plus_half_pi = value_clamp<T, minus_plus_half_pi<T>>;

}    // namespace value_policy

} /* namespace math */
} /* namespace psst */

#endif /* PSST_MATH_ANGLES_HPP_ */
