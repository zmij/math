/**
 * Copyright 2019 Sergei A. Fedorov
 * spherical_coord.hpp
 *
 *  Created on: Jan 5, 2019
 *      Author: ser-fedorov
 */

#ifndef PUSHKIN_MATH_SPHERICAL_COORD_HPP_
#define PUSHKIN_MATH_SPHERICAL_COORD_HPP_

#include <pushkin/math/angles.hpp>
#include <pushkin/math/detail/conversion.hpp>
#include <pushkin/math/detail/vector_expressions.hpp>

namespace psst {
namespace math {
namespace axes {

struct spherical {
    static constexpr std::size_t min_components = 3;
    static constexpr std::size_t max_components = 3;
    static constexpr std::size_t rho            = 0;
    static constexpr std::size_t phi            = 1;
    static constexpr std::size_t theta          = 2;
    static constexpr std::size_t r              = rho;
    static constexpr std::size_t inclination    = phi;
    static constexpr std::size_t azimuth        = theta;

    using value_policy = utils::template_tuple<math::value_policy::no_change,
                                               math::value_policy::clamp_minus_plus_half_pi,
                                               math::value_policy::clamp_zero_to_two_pi>;
};

//@{
/** @name Spherical coordinates */
template <typename VectorType, typename T>
struct axis_access<3, spherical, VectorType, T> : basic_axis_access<VectorType, T, spherical> {
    using base_type = basic_axis_access<VectorType, T, spherical>;

    PSST_MATH_COORD_ACCESS(r);
    PSST_MATH_COORD_ACCESS(rho);
    PSST_MATH_COORD_ACCESS(phi);
    PSST_MATH_COORD_ACCESS(inclination);
    PSST_MATH_COORD_ACCESS(theta);
    PSST_MATH_COORD_ACCESS(azimuth);
};
//@}

}    // namespace axes

template <typename T>
using spherical_coord = vector<T, 3, axes::spherical>;

}    // namespace math
}    // namespace psst

#endif /* PUSHKIN_MATH_SPHERICAL_COORD_HPP_ */
