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

    using value_policies = utils::template_tuple<math::value_policy::no_change,
                                                 math::value_policy::clamp_zero_to_two_pi,
                                                 math::value_policy::no_change>;
};
}    // namespace axes

namespace axis_access {
//@{
/** @name Cylindrical coordinates */
template <typename VectorType, typename T>
struct axis_access<3, axes::cylindrical, VectorType, T>
    : basic_axis_access<VectorType, T, axes::cylindrical> {
    using base_type = basic_axis_access<VectorType, T, axes::cylindrical>;

    PSST_MATH_COORD_ACCESS(r);
    PSST_MATH_COORD_ACCESS(rho);
    PSST_MATH_COORD_ACCESS(phi);
    PSST_MATH_COORD_ACCESS(azimuth);
    PSST_MATH_COORD_ACCESS(z);
    PSST_MATH_COORD_ACCESS(elevation);
};
//@}

}    // namespace axis_access

namespace expr {
inline namespace v {
//@{
/** @name Vector scalar muliply for spherical coordinates */
template <typename LHS, typename RHS>
struct vector_scalar_multiply<axes::cylindrical, LHS, RHS>
    : binary_vector_expression_axes<vector_scalar_multiply, axes::cylindrical, LHS, RHS>,
      binary_expression<LHS, RHS> {
    using base_type
        = binary_vector_expression_axes<vector_scalar_multiply, axes::cylindrical, LHS, RHS>;
    using value_type = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr value_type
    at() const
    {
        static_assert(N < base_type::size, "Vector multiply element index is out of range");
        if constexpr (N == axes::cylindrical::rho || N == axes::cylindrical::elevation) {
            // In cylindrical coordinates only rho and elevation is multiplied
            return this->lhs_.template at<N>() * this->rhs_;
        } else {
            return this->lhs_.template at<N>();
        }
    }
};

//@}
//@{
/** @name Vector scalar division for spherical coordinates */
template <typename LHS, typename RHS>
struct vector_scalar_divide<axes::cylindrical, LHS, RHS>
    : binary_vector_expression_axes<vector_scalar_divide, axes::cylindrical, LHS, RHS>,
      binary_expression<LHS, RHS> {
    using base_type
        = binary_vector_expression_axes<vector_scalar_divide, axes::cylindrical, LHS, RHS>;
    using value_type = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr value_type
    at() const
    {
        static_assert(N < base_type::size, "Vector divide element index is out of range");
        if constexpr (N == axes::cylindrical::rho || N == axes::cylindrical::elevation) {
            // In cylindrical coordinates only rho and elevation is divided
            return this->lhs_.template at<N>() / this->rhs_;
        } else {
            return this->lhs_.template at<N>();
        }
    }
};
//@}
//@{
/** @name Magnitude squared for spherical coordinates */
template <typename Expr>
struct vector_magnitude_squared<axes::cylindrical, Expr>
    : scalar_expression<vector_magnitude_squared<axes::cylindrical, Expr>,
                        scalar_expression_result_t<Expr>>,
      unary_expression<Expr> {
    static_assert(is_vector_expression_v<Expr>, "Argument to magnitude must be a vector");

    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    constexpr auto
    value() const
    {
        return this->arg_.rho() * this->arg_.rho()
               + this->arg_.elevation() * this->arg_.elevation();
    }
};

//@}

//@{
template <typename Expr>
struct vector_normalize<axes::cylindrical, Expr>
    : unary_vector_expression_axes<vector_normalize, axes::cylindrical, Expr>,
      unary_expression<Expr> {
    using base_type       = unary_vector_expression_axes<vector_normalize, axes::cylindrical, Expr>;
    using value_type      = typename base_type::value_type;
    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr auto
    at() const
    {
        static_assert(N < base_type::size, "Vector normalize component index is out of range");
        if constexpr (N == axes::cylindrical::rho || N == axes::cylindrical::elevation) {
            return this->arg_.template at<N>() / magnitude(this->arg_);
        } else {
            return this->arg_.template at<N>();
        }
    }
};
//@}

}    // namespace v
}    // namespace expr

template <typename T>
using cylindrical_coord = vector<T, 3, axes::cylindrical>;

}    // namespace math
}    // namespace psst

#endif /* PUSHKIN_MATH_CYLINDRICAL_COORD_HPP_ */
