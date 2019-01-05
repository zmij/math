/**
 * Copyright 2018 Sergei A. Fedorov
 * polar.hpp
 *
 *  Created on: Dec 30, 2018
 *      Author: ser-fedorov
 */

#ifndef PUSHKIN_MATH_POLAR_COORD_HPP_
#define PUSHKIN_MATH_POLAR_COORD_HPP_

#include <pushkin/math/angles.hpp>
#include <pushkin/math/detail/conversion.hpp>
#include <pushkin/math/detail/vector_expressions.hpp>

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

    using value_policy = utils::template_tuple<math::value_policy::no_change,
                                               math::value_policy::clamp_zero_to_two_pi>;
};

//@{
/** @name Polar coordinates */
template <typename VectorType, typename T>
struct axis_access<2, polar, VectorType, T> : basic_axis_access<VectorType, T, polar> {
    using base_type = basic_axis_access<VectorType, T, polar>;

    PSST_MATH_COORD_ACCESS(r);
    PSST_MATH_COORD_ACCESS(rho);
    PSST_MATH_COORD_ACCESS(phi);
    PSST_MATH_COORD_ACCESS(azimuth);
};
//@}

}    // namespace axes

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
        static_assert(N < base_type::size, "Vector multiply element index is out of range");
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
        static_assert(N < base_type::size, "Vector divide element index is out of range");
        if constexpr (has_axes_v<LHS, axes::polar> && N != axes::polar::rho) {
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
template <typename Polar>
struct magnitude_squared_calc<axes::polar, Polar>
    : scalar_expression<magnitude_squared_calc<axes::polar, Polar>,
                        scalar_expression_result_t<Polar>>,
      unary_expression<Polar> {
    static_assert(is_vector_expression_v<Polar>, "Argument to magnitude must be a vector");

    using expression_base = unary_expression<Polar>;
    using expression_base::expression_base;

    constexpr auto
    value() const
    {
        return this->arg_.rho() * this->arg_.rho();
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
        static_assert(N < base_type::size, "Vector normalize element index is out of range");
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

//@{
/** @name Polar to XYZW conversion */
template <typename T, typename U, std::size_t Cartesian, typename Expression>
struct conversion<vector<T, 2, axes::polar>, vector<U, Cartesian, axes::xyzw>, Expression>
    : vector_expression<
          conversion<vector<T, 2, axes::polar>, vector<U, Cartesian, axes::xyzw>, Expression>,
          vector<U, Cartesian, axes::xyzw>>,
      unary_expression<Expression> {
    using base_type = vector_expression<
        conversion<vector<T, 2, axes::polar>, vector<U, Cartesian, axes::xyzw>, Expression>,
        vector<U, Cartesian, axes::xyzw>>;

    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr auto
    at() const
    {
        using std::cos;
        using std::sin;
        static_assert(N < base_type::size,
                      "Polar to XYZW conversion element index is out of bounds");
        if constexpr (N == 0) {
            return this->arg_.rho() * cos(this->arg_.phi());
        } else if constexpr (N == 1) {
            return this->arg_.rho() * sin(this->arg_.phi());
        } else {
            return T{0};
        }
    }
};
//@}

//@{
/** @name XYZW to polar conversion */
template <typename T, typename U, std::size_t Cartesian, typename Expression>
struct conversion<vector<U, Cartesian, axes::xyzw>, vector<T, 2, axes::polar>, Expression>
    : vector_expression<
          conversion<vector<U, Cartesian, axes::xyzw>, vector<T, 2, axes::polar>, Expression>,
          vector<T, 2, axes::polar>>,
      unary_expression<Expression> {
    using base_type = vector_expression<
        conversion<vector<U, Cartesian, axes::xyzw>, vector<T, 2, axes::polar>, Expression>,
        vector<T, 2, axes::polar>>;

    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr auto
    at() const
    {
        using std::atan2;
        static_assert(N < base_type::size,
                      "XYZW to polar conversion element index is out of bounds");
        if constexpr (N == 0) {
            return magnitude(this->arg_);
        } else if constexpr (N == 1) {
            return atan2(this->arg_.y(), this->arg_.x());
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

#endif /* PUSHKIN_MATH_POLAR_COORD_HPP_ */
