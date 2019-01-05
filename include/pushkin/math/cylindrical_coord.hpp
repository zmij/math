/**
 * Copyright 2019 Sergei A. Fedorov
 * cylindrical_coord.hpp
 *
 *  Created on: Jan 5, 2019
 *      Author: ser-fedorov
 */

#ifndef PUSHKIN_MATH_CYLINDRICAL_COORD_HPP_
#define PUSHKIN_MATH_CYLINDRICAL_COORD_HPP_

#include <pushkin/math/angles.hpp>
#include <pushkin/math/detail/conversion.hpp>
#include <pushkin/math/detail/vector_expressions.hpp>

namespace psst {
namespace math {
namespace axes {

struct cylindrical {
    static constexpr std::size_t min_components = 3;
    static constexpr std::size_t max_components = 3;
    static constexpr std::size_t rho            = 0;
    static constexpr std::size_t phi            = 1;
    static constexpr std::size_t z              = 2;
    static constexpr std::size_t r              = rho;
    static constexpr std::size_t azimuth        = phi;
    static constexpr std::size_t elevation      = z;

    using value_policy = utils::template_tuple<math::value_policy::no_change,
                                               math::value_policy::clamp_zero_to_two_pi,
                                               math::value_policy::no_change>;
};

//@{
/** @name Cylindrical coordinates */
template <typename VectorType, typename T>
struct axis_access<3, cylindrical, VectorType, T> : basic_axis_access<VectorType, T, cylindrical> {
    using base_type = basic_axis_access<VectorType, T, cylindrical>;

    PSST_MATH_COORD_ACCESS(r);
    PSST_MATH_COORD_ACCESS(rho);
    PSST_MATH_COORD_ACCESS(phi);
    PSST_MATH_COORD_ACCESS(azimuth);
    PSST_MATH_COORD_ACCESS(z);
    PSST_MATH_COORD_ACCESS(elevation);
};
//@}

}    // namespace axes

template <typename T>
using cylindrical_coord = vector<T, 3, axes::cylindrical>;

}    // namespace math
}    // namespace psst

#endif /* PUSHKIN_MATH_CYLINDRICAL_COORD_HPP_ */
