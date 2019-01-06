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
#include <pushkin/math/polar_coord.hpp>    // For spherical-polar conversions

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

namespace expr {

inline namespace v {
//@{
/** @name Vector scalar muliply for spherical coordinates */
template <typename LHS, typename RHS>
struct vector_scalar_multiply<axes::spherical, LHS, RHS>
    : binary_vector_expression_axes<vector_scalar_multiply, axes::spherical, LHS, RHS>,
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

//@{
/** @name Spherical to XYZW conversion */
template <typename T, typename U, std::size_t Cartesian, typename Expression>
struct conversion<vector<T, 3, axes::spherical>, vector<U, Cartesian, axes::xyzw>, Expression>
    : vector_conversion_expression<vector<T, 3, axes::spherical>, vector<U, Cartesian, axes::xyzw>,
                                   Expression>,
      unary_expression<Expression> {
    using base_type = vector_conversion_expression<vector<T, 3, axes::spherical>,
                                                   vector<U, Cartesian, axes::xyzw>, Expression>;

    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr auto
    at() const
    {
        using std::cos;
        using std::sin;
        static_assert(N < base_type::size,
                      "Spherical to XYZW conversion component index is out of bounds");
        if constexpr (N == axes::xyzw::x) {
            // REally?
            return projection_length() * cos(this->arg_.theta());
        } else if constexpr (N == axes::xyzw::y) {
            // REally?
            return projection_length() * sin(this->arg_.theta());
        } else if constexpr (N == axes::xyzw::z) {
            return this->arg_.rho() * sin(this->arg_.phi());
        } else {
            return U{0};
        }
    }

private:
    constexpr auto
    projection_length() const
    {
        return this->arg_.rho() * cos(this->arg_.phi());
    }
};
//@}

//@{
/** @name XYZW to spherical conversion */
template <typename T, typename U, std::size_t Cartesian, typename Expression>
struct conversion<vector<U, Cartesian, axes::xyzw>, vector<T, 3, axes::spherical>, Expression>
    : vector_conversion_expression<vector<U, Cartesian, axes::xyzw>, vector<T, 3, axes::spherical>,
                                   Expression>,
      unary_expression<Expression> {
    using base_type = vector_conversion_expression<vector<U, Cartesian, axes::xyzw>,
                                                   vector<T, 3, axes::spherical>, Expression>;

    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr auto
    at() const
    {
        using std::asin;
        using std::atan2;
        static_assert(N < base_type::size,
                      "XYZW to spherical conversion component index is out of bounds");
        if constexpr (N == axes::spherical::rho) {
            return magnitude(this->arg_);
        } else if constexpr (N == axes::spherical::inclination) {
            if (this->arg_.z() == 0) {
                return T{0};
            } else {
                return minus_plus_half_pi(asin(this->arg_.z() / magnitude(this->arg_)));
            }
        } else if constexpr (N == axes::spherical::azimuth) {
            return zero_to_two_pi(atan2(this->arg_.y(), this->arg_.x()));
        } else {
            return T{0};
        }
    }

private:
    constexpr auto
    projection_length() const
    {
        using std::sqrt;
        return sqrt(this->arg_.x() * this->arg_.x() + this->arg_.y() * this->arg_.y());
    }
};
//@}

//@{
/** @name Spherical to polar conversion */
template <typename T, typename U, typename Expression>
struct conversion<vector<T, 3, axes::spherical>, vector<U, 2, axes::polar>, Expression>
    : vector_conversion_expression<vector<T, 3, axes::spherical>, vector<U, 2, axes::polar>,
                                   Expression>,
      unary_expression<Expression> {
    using base_type = vector_conversion_expression<vector<T, 3, axes::spherical>,
                                                   vector<U, 2, axes::polar>, Expression>;

    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr auto
    at() const
    {
        using std::cos;
        using std::sin;
        static_assert(N < base_type::size,
                      "Spherical to polar conversion component index is out of bounds");
        if constexpr (N == axes::polar::rho) {
            return this->arg_.rho() * cos(this->arg_.phi());
        } else if constexpr (N == axes::polar::azimuth) {
            return this->arg_.azimuth();
        } else {
            return U{0};
        }
    }
};
//@}

//@{
/** @name Spherical to polar conversion */
template <typename T, typename U, typename Expression>
struct conversion<vector<T, 2, axes::polar>, vector<U, 3, axes::spherical>, Expression>
    : vector_conversion_expression<vector<T, 2, axes::polar>, vector<U, 3, axes::spherical>,
                                   Expression>,
      unary_expression<Expression> {
    using base_type = vector_conversion_expression<vector<T, 2, axes::polar>,
                                                   vector<U, 3, axes::spherical>, Expression>;

    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr auto
    at() const
    {
        using std::cos;
        using std::sin;
        static_assert(N < base_type::size,
                      "Polar to spherical conversion component index is out of bounds");
        if constexpr (N == axes::spherical::rho) {
            return this->arg_.rho();
        } else if constexpr (N == axes::spherical::azimuth) {
            return this->arg_.azimuth();
        } else {
            return U{0};
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

#endif /* PUSHKIN_MATH_SPHERICAL_COORD_HPP_ */
