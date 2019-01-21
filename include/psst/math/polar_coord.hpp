/**
 * Copyright 2018 Sergei A. Fedorov
 * polar.hpp
 *
 *  Created on: Dec 30, 2018
 *      Author: ser-fedorov
 */

#ifndef PSST_MATH_POLAR_COORD_HPP_
#define PSST_MATH_POLAR_COORD_HPP_

#include <psst/math/angles.hpp>
#include <psst/math/detail/conversion.hpp>
#include <psst/math/detail/vector_expressions.hpp>

namespace psst {
namespace math {

namespace axes {

struct polar {
    static constexpr std::size_t min_components = 2;
    static constexpr std::size_t max_components = 2;
    static constexpr std::size_t rho            = 0;
    static constexpr std::size_t phi            = 1;
    static constexpr std::size_t r              = rho;
    static constexpr std::size_t azimuth        = phi;

    using value_policies = utils::template_tuple<math::value_policy::no_change,
                                                 math::value_policy::clamp_zero_to_two_pi>;
};
}    // namespace axes

namespace axis_access {
//@{
/** @name Polar coordinates */
template <typename VectorType, typename T>
struct axis_access<2, axes::polar, VectorType, T> : basic_axis_access<VectorType, T, axes::polar> {
    using base_type = basic_axis_access<VectorType, T, axes::polar>;

    PSST_MATH_COORD_ACCESS(r);
    PSST_MATH_COORD_ACCESS(rho);
    PSST_MATH_COORD_ACCESS(phi);
    PSST_MATH_COORD_ACCESS(azimuth);
};
//@}

}    // namespace axis_access

namespace expr {

inline namespace v {

//@{
/** @name Vector scalar muliply for polar coordinates */
template <typename LHS, typename RHS>
struct vector_scalar_multiply<axes::polar, LHS, RHS>
    : binary_vector_expression_axes<vector_scalar_multiply, axes::polar, LHS, RHS>,
      binary_expression<LHS, RHS> {
    using base_type  = binary_vector_expression_axes<vector_scalar_multiply, axes::polar, LHS, RHS>;
    using value_type = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr value_type
    at() const
    {
        static_assert(N < base_type::size, "Vector multiply component index is out of range");
        if constexpr (N != axes::polar::rho) {
            // In polar coordinates only the first component (rho) is multiplied
            return this->lhs_.template at<N>();
        } else {
            return this->lhs_.template at<N>() * this->rhs_;
        }
    }
};

//@}
//@{
/** @name Vector scalar division for polar coordinates */
template <typename LHS, typename RHS>
struct vector_scalar_divide<axes::polar, LHS, RHS>
    : binary_vector_expression_axes<vector_scalar_divide, axes::polar, LHS, RHS>,
      binary_expression<LHS, RHS> {
    using base_type  = binary_vector_expression_axes<vector_scalar_divide, axes::polar, LHS, RHS>;
    using value_type = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr value_type
    at() const
    {
        static_assert(N < base_type::size, "Vector divide component index is out of range");
        if constexpr (N != axes::polar::rho) {
            // In polar coordinates only the first component (rho) is divided
            return this->lhs_.template at<N>();
        } else {
            return this->lhs_.template at<N>() / this->rhs_;
        }
    }
};
//@}
//@{
/** @name Magnitude squared for polar coordinates */
template <typename Expr>
struct vector_magnitude_squared<axes::polar, Expr>
    : scalar_expression<vector_magnitude_squared<axes::polar, Expr>,
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
/** @name Magnitude for polar coordinates */
template <typename Expr>
struct vector_magnitude<axes::polar, Expr>
    : scalar_expression<vector_magnitude<axes::polar, Expr>, scalar_expression_result_t<Expr>>,
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
struct vector_normalize<axes::polar, Expr>
    : unary_vector_expression_axes<vector_normalize, axes::polar, Expr>, unary_expression<Expr> {
    using base_type       = unary_vector_expression_axes<vector_normalize, axes::polar, Expr>;
    using value_type      = typename base_type::value_type;
    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr auto
    at() const
    {
        static_assert(N < base_type::size, "Vector normalize component index is out of range");
        if (N == axes::polar::rho) {
            return value_type{1};
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
using polar_coord = vector<T, 2, axes::polar>;

}    // namespace math
}    // namespace psst

#endif /* PSST_MATH_POLAR_COORD_HPP_ */
