/**
 * Copyright 2019 Sergei A. Fedorov
 * spherical_coord.hpp
 *
 *  Created on: Jan 5, 2019
 *      Author: ser-fedorov
 */

#ifndef PSST_MATH_SPHERICAL_COORD_HPP_
#define PSST_MATH_SPHERICAL_COORD_HPP_

#include <psst/math/angles.hpp>
#include <psst/math/detail/conversion.hpp>
#include <psst/math/detail/vector_expressions.hpp>

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

    using value_policies = utils::template_tuple<math::value_policy::no_change,
                                                 math::value_policy::clamp_minus_plus_half_pi,
                                                 math::value_policy::clamp_zero_to_two_pi>;
};

}    // namespace axes

namespace axis_access {
//@{
/** @name Spherical coordinates */
template <typename VectorType, typename T>
struct axis_access<3, axes::spherical, VectorType, T>
    : basic_axis_access<VectorType, T, axes::spherical> {
    using base_type = basic_axis_access<VectorType, T, axes::spherical>;

    PSST_MATH_COORD_ACCESS(r);
    PSST_MATH_COORD_ACCESS(rho);
    PSST_MATH_COORD_ACCESS(phi);
    PSST_MATH_COORD_ACCESS(inclination);
    PSST_MATH_COORD_ACCESS(theta);
    PSST_MATH_COORD_ACCESS(azimuth);
};
//@}

}    // namespace axis_access

namespace expr {

inline namespace v {
//@{
/** @name Vector scalar muliply for spherical coordinates */
template <typename LHS, typename RHS>
struct vector_scalar_multiply<axes::spherical, LHS, RHS>
    : binary_vector_expression_axes<vector_scalar_multiply, axes::spherical, LHS, RHS>,
      binary_expression<LHS, RHS> {
    using base_type
        = binary_vector_expression_axes<vector_scalar_multiply, axes::spherical, LHS, RHS>;
    using value_type = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr value_type
    at() const
    {
        static_assert(N < base_type::size, "Vector multiply element index is out of range");
        if constexpr (N != axes::spherical::rho) {
            // In spherical coordinates only the first component (rho) is multiplied
            return this->lhs_.template at<N>();
        } else {
            return this->lhs_.template at<N>() * this->rhs_;
        }
    }
};
//@}

//@{
/** @name Vector scalar division for spherical coordinates */
template <typename LHS, typename RHS>
struct vector_scalar_divide<axes::spherical, LHS, RHS>
    : binary_vector_expression_axes<vector_scalar_divide, axes::spherical, LHS, RHS>,
      binary_expression<LHS, RHS> {
    using base_type
        = binary_vector_expression_axes<vector_scalar_divide, axes::spherical, LHS, RHS>;
    using value_type = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr value_type
    at() const
    {
        static_assert(N < base_type::size, "Vector divide element index is out of range");
        if constexpr (N != axes::spherical::rho) {
            // In spherical coordinates only the first component (rho) is divided
            return this->lhs_.template at<N>();
        } else {
            return this->lhs_.template at<N>() / this->rhs_;
        }
    }
};
//@}

//@{
/** @name Magnitude squared for spherical coordinates */
template <typename Expr>
struct vector_magnitude_squared<axes::spherical, Expr>
    : scalar_expression<vector_magnitude_squared<axes::spherical, Expr>,
                        scalar_expression_result_t<Expr>>,
      unary_expression<Expr> {
    static_assert(is_vector_expression_v<Expr>, "Argument to magnitude must be a vector");

    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    constexpr auto
    value() const
    {
        return this->arg_.rho() * this->arg_.rho();
    }
};
//@}

//@{
/** @name Magnitude for spherical coordinates */
template <typename Expr>
struct vector_magnitude<axes::spherical, Expr>
    : scalar_expression<vector_magnitude<axes::spherical, Expr>, scalar_expression_result_t<Expr>>,
      unary_expression<Expr> {
    static_assert(is_vector_expression_v<Expr>, "Argument to magnitude must be a vector");

    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    constexpr auto
    value() const
    {
        using std::abs;
        return abs(this->arg_.rho());
    }
};
//@}

//@{
template <typename Expr>
struct vector_normalize<axes::spherical, Expr>
    : unary_vector_expression_axes<vector_normalize, axes::spherical, Expr>,
      unary_expression<Expr> {
    using base_type       = unary_vector_expression_axes<vector_normalize, axes::spherical, Expr>;
    using value_type      = typename base_type::value_type;
    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr auto
    at() const
    {
        static_assert(N < base_type::size, "Vector normalize component index is out of range");
        if constexpr (N == axes::spherical::rho) {
            return value_type{1};
        } else if constexpr (N == axes::spherical::inclination) {
            if (this->arg_.rho() < 0) {
                return -this->arg_.inclination();
            } else {
                return this->arg_.inclination();
            }
        } else {
            if (this->arg_.rho() < 0) {
                return zero_to_two_pi(this->arg_.azimuth() + pi<value_type>::value);
            } else {
                return this->arg_.azimuth();
            }
        }
    }
};
//@}

}    // namespace v
}    // namespace expr

template <typename T>
using spherical_coord = vector<T, 3, axes::spherical>;

}    // namespace math
}    // namespace psst

#endif /* PSST_MATH_SPHERICAL_COORD_HPP_ */
