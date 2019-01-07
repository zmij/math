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
#include <pushkin/math/polar_coord.hpp>
#include <pushkin/math/spherical_coord.hpp>

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

//@{
/** @name Cylindrical to XYZW conversion */
template <typename T, typename U, std::size_t Cartesian, typename Expression>
struct conversion<vector<T, 3, axes::cylindrical>, vector<U, Cartesian, axes::xyzw>, Expression>
    : vector_conversion_expression<vector<T, 3, axes::cylindrical>,
                                   vector<U, Cartesian, axes::xyzw>, Expression>,
      unary_expression<Expression> {
    using base_type = vector_conversion_expression<vector<T, 3, axes::cylindrical>,
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
                      "Cylindrical to XYZW conversion component index is out of bounds");
        if constexpr (N == axes::xyzw::x) {
            return this->arg_.rho() * cos(this->arg_.phi());
        } else if constexpr (N == axes::xyzw::y) {
            return this->arg_.rho() * sin(this->arg_.phi());
        } else if constexpr (N == axes::xyzw::z) {
            return this->arg_.z();
        } else {
            return U{0};
        }
    }
};
//@}

//@{
/** @name XYZW to cylindrical conversion */
template <typename T, typename U, std::size_t Cartesian, typename Expression>
struct conversion<vector<U, Cartesian, axes::xyzw>, vector<T, 3, axes::cylindrical>, Expression>
    : vector_conversion_expression<vector<U, Cartesian, axes::xyzw>,
                                   vector<T, 3, axes::cylindrical>, Expression>,
      unary_expression<Expression> {
    using base_type = vector_conversion_expression<vector<U, Cartesian, axes::xyzw>,
                                                   vector<T, 3, axes::cylindrical>, Expression>;

    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr auto
    at() const
    {
        using std::asin;
        using std::atan2;
        static_assert(N < base_type::size,
                      "XYZW to cylindrical conversion component index is out of bounds");
        if constexpr (N == axes::cylindrical::rho) {
            return projection_length();
        } else if constexpr (N == axes::cylindrical::azimuth) {
            return zero_to_two_pi(atan2(this->arg_.y(), this->arg_.x()));
        } else if constexpr (N == axes::cylindrical::z) {
            return this->arg_.z();
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
/** @name Cylindrical to polar conversion */
template <typename T, typename U, typename Expression>
struct conversion<vector<T, 3, axes::cylindrical>, vector<U, 2, axes::polar>, Expression>
    : vector_conversion_expression<vector<T, 3, axes::cylindrical>, vector<U, 2, axes::polar>,
                                   Expression>,
      unary_expression<Expression> {
    using base_type = vector_conversion_expression<vector<T, 3, axes::cylindrical>,
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
            return this->arg_.rho();
        } else if constexpr (N == axes::polar::azimuth) {
            return this->arg_.azimuth();
        } else {
            return U{0};
        }
    }
};
//@}

//@{
/** @name Polar to cylindrical conversion */
template <typename T, typename U, typename Expression>
struct conversion<vector<T, 2, axes::polar>, vector<U, 3, axes::cylindrical>, Expression>
    : vector_conversion_expression<vector<T, 2, axes::polar>, vector<U, 3, axes::cylindrical>,
                                   Expression>,
      unary_expression<Expression> {
    using base_type = vector_conversion_expression<vector<T, 2, axes::polar>,
                                                   vector<U, 3, axes::cylindrical>, Expression>;

    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr auto
    at() const
    {
        using std::cos;
        using std::sin;
        static_assert(N < base_type::size,
                      "Polar to cylindrical conversion component index is out of bounds");
        if constexpr (N == axes::cylindrical::rho) {
            return this->arg_.rho();
        } else if constexpr (N == axes::cylindrical::azimuth) {
            return this->arg_.azimuth();
        } else {
            return U{0};
        }
    }
};
//@}

//@{
/** @name Cylindrical to spherical conversion */
template <typename T, typename U, typename Expression>
struct conversion<vector<U, 3, axes::cylindrical>, vector<T, 3, axes::spherical>, Expression>
    : vector_conversion_expression<vector<U, 3, axes::cylindrical>, vector<T, 3, axes::spherical>,
                                   Expression>,
      unary_expression<Expression> {
    using base_type = vector_conversion_expression<vector<U, 3, axes::cylindrical>,
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
                      "Cylindrical to spherical conversion component index is out of bounds");
        if constexpr (N == axes::spherical::rho) {
            return magnitude(this->arg_);
        } else if constexpr (N == axes::spherical::inclination) {
            if (this->arg_.z() == 0) {
                return T{0};
            } else {
                return minus_plus_half_pi(asin(this->arg_.z() / magnitude(this->arg_)));
            }
        } else if constexpr (N == axes::spherical::azimuth) {
            return this->arg_.azimuth();
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
/** @name Spherical to cylindrical conversion */
template <typename T, typename U, typename Expression>
struct conversion<vector<T, 3, axes::spherical>, vector<U, 3, axes::cylindrical>, Expression>
    : vector_conversion_expression<vector<T, 3, axes::spherical>, vector<U, 3, axes::cylindrical>,
                                   Expression>,
      unary_expression<Expression> {
    using base_type = vector_conversion_expression<vector<T, 3, axes::spherical>,
                                                   vector<U, 3, axes::cylindrical>, Expression>;

    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr auto
    at() const
    {
        using std::cos;
        using std::sin;
        static_assert(N < base_type::size,
                      "Spherical to cylindrical conversion component index is out of bounds");
        if constexpr (N == axes::cylindrical::rho) {
            return this->arg_.rho() * cos(this->arg_.phi());
        } else if constexpr (N == axes::cylindrical::azimuth) {
            return this->arg_.azimuth();
        } else if constexpr (N == axes::cylindrical::z) {
            return this->arg_.rho() * sin(this->arg_.phi());
        } else {
            return U{0};
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
