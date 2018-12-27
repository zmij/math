/*
 * pi.hpp
 *
 *  Created on: Dec 19, 2016
 *      Author: zmij
 */

#ifndef PUSHKIN_MATH_PI_HPP_
#define PUSHKIN_MATH_PI_HPP_

#include <cmath>

namespace psst {
namespace math {

template <typename T>
struct pi {
    static const T value;
};

template <typename T>
const T pi<T>::value = std::atan((T)1) * 4;

} /* namespace math */
} /* namespace psst */

#endif /* PUSHKIN_MATH_PI_HPP_ */
