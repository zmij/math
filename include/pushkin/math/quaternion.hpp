/**
 * Copyright 2019 Sergei A. Fedorov
 * quaternion.hpp
 *
 *  Created on: Jan 7, 2019
 *      Author: ser-fedorov
 */

#ifndef PUSHKIN_MATH_QUATERNION_HPP_
#define PUSHKIN_MATH_QUATERNION_HPP_

#include <pushkin/math/detail/vector_expressions.hpp>

namespace psst {
namespace math {

namespace expr {
inline namespace v {

template <typename Expr>
struct quaternion_scalar_part;
template <typename Expr>
struct quaternion_vector_part;

}    // namespace v
}    // namespace expr

namespace axes {

struct wxyz {
    static constexpr std::size_t min_components = 4;
    static constexpr std::size_t max_components = 4;
    static constexpr std::size_t w              = 0;
    static constexpr std::size_t x              = 1;
    static constexpr std::size_t y              = 2;
    static constexpr std::size_t z              = 3;
    // Alternative names
    static constexpr std::size_t a = w;
    static constexpr std::size_t i = x;
    static constexpr std::size_t j = y;
    static constexpr std::size_t k = z;
};

}    // namespace axes

namespace axis_access {

//@{
/** @name wxyz axes names */
template <typename VectorType, typename T>
struct axis_access<4, axes::wxyz, VectorType, T> : basic_axis_access<VectorType, T, axes::wxyz> {

    using base_type = basic_axis_access<VectorType, T, axes::wxyz>;

    PSST_MATH_COORD_ACCESS(w)
    PSST_MATH_COORD_ACCESS(x)
    PSST_MATH_COORD_ACCESS(y)
    PSST_MATH_COORD_ACCESS(z)

    constexpr auto
    scalar_part() const;
    constexpr auto
    vector_part() const;
};
//@}

}    // namespace axis_access

namespace expr {
inline namespace v {

template <typename Expr>
struct quaternion_scalar_part : unary_scalar_expression<quaternion_scalar_part, Expr>,
                                unary_expression<Expr> {
    using base_type = unary_scalar_expression<quaternion_scalar_part, Expr>;

    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    constexpr auto
    value() const
    {
        return this->arg_.template at<0>();
    }
};

template <typename Expr>
struct quaternion_vector_part
    : unary_vector_expression<quaternion_vector_part, Expr,
                              vector<scalar_expression_result_t<Expr>, 3, axes::xyzw>>,
      unary_expression<Expr> {
    using base_type
        = unary_vector_expression<quaternion_vector_part, Expr,
                                  vector<scalar_expression_result_t<Expr>, 3, axes::xyzw>>;
    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr auto
    at() const
    {
        static_assert(N < base_type::size, "Invalid quaternion component index");
        return this->arg_.template at<N + 1>();
    }
};

template <typename LHS, typename RHS>
struct vector_vector_multiply<axes::wxyz, LHS, RHS>
    : binary_vector_expression_axes<vector_vector_multiply, axes::wxyz, LHS, RHS>,
      binary_expression<LHS, RHS> {
    using base_type = binary_vector_expression_axes<vector_vector_multiply, axes::wxyz, LHS, RHS>;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    /**
     *    w1w2 - x1x2 - y1y2 - z1z2
     * + (w1x2 + x1w2 + y1z2 - z1y2)i
     * + (w1y2 - x1z2 + y1w2 + z1x2)j
     * + (w1z2 + x1y2 - y1x2 + z1w2)k
     * @return
     */
    template <std::size_t N>
    constexpr auto
    at() const
    {
        static_assert(N < base_type::size, "Invalid quaternion component index");
        auto const& lhs = this->lhs_;
        auto const& rhs = this->rhs_;
        if constexpr (N == axes::wxyz::w) {
            return lhs.w() * rhs.w() - lhs.x() * rhs.x() - lhs.y() * rhs.y() - lhs.z() * rhs.z();
        } else if constexpr (N == axes::wxyz::x) {
            return lhs.w() * rhs.x() + lhs.x() * rhs.w() + lhs.y() * rhs.z() - lhs.z() * rhs.y();
        } else if constexpr (N == axes::wxyz::y) {
            return lhs.w() * rhs.y() - lhs.x() * rhs.z() + lhs.y() * rhs.w() + lhs.z() * rhs.x();
        } else if constexpr (N == axes::wxyz::z) {
            return lhs.w() * rhs.z() + lhs.x() * rhs.y() - lhs.y() * rhs.x() + lhs.z() * rhs.w();
        }
    }
};

//@{
template <typename Expr>
struct vector_normalize<axes::wxyz, Expr>
    : unary_vector_expression_axes<vector_normalize, axes::wxyz, Expr>, unary_expression<Expr> {
    using base_type       = unary_vector_expression_axes<vector_normalize, axes::wxyz, Expr>;
    using value_type      = typename base_type::value_type;
    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr auto
    at() const
    {
        static_assert(N < base_type::size, "Invalid quaternion component index");
        auto mag = magnitude(this->arg_);
        if (mag == 0)
            throw std::runtime_error("Cannot normalise a zero quaternion");
        if constexpr (N == axes::wxyz::w) {
            return this->arg_.template at<N>() / mag;
        } else {
            auto val = this->arg_.template at<N>();
            if (val == mag) {
                return -val / mag;
            } else {
                return val / mag;
            }
        }
    }
};
//@}

//@{
/** @name Conjugate */
template <typename Expr>
struct quaternion_conjugate : unary_vector_expression<quaternion_conjugate, Expr>,
                              unary_expression<Expr> {
    using base_type       = unary_vector_expression<quaternion_conjugate, Expr>;
    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr auto
    at() const
    {
        static_assert(N < base_type::size, "Invalid quaternion component index");
        if constexpr (N == axes::wxyz::w) {
            return this->arg_.template at<N>();
        } else {
            return -this->arg_.template at<N>();
        }
    }
};

template <typename Expr, typename = enable_for_axes<Expr, axes::wxyz>>
constexpr auto
conjugate(Expr&& expr)
{
    return make_unary_expression<quaternion_conjugate>(std::forward<Expr>(expr));
}
//@}

//@{
template <typename Expr, typename = enable_for_axes<Expr, axes::wxyz>>
constexpr auto
inverse(Expr&& expr)
{
    auto mag_sq = magnitude_square(expr).value();
    if (mag_sq == 0)
        throw std::runtime_error("Cannot inverse a zero quaternion");
    return conjugate(std::forward<Expr>(expr)) / mag_sq;
}
//@}

}    // namespace v
}    // namespace expr

namespace axis_access {

template <typename VectorType, typename T>
constexpr auto
axis_access<4, axes::wxyz, VectorType, T>::scalar_part() const
{
    return expr::make_unary_expression<expr::v::quaternion_scalar_part>(base_type::rebind());
}

template <typename VectorType, typename T>
constexpr auto
axis_access<4, axes::wxyz, VectorType, T>::vector_part() const
{
    return expr::make_unary_expression<expr::v::quaternion_vector_part>(base_type::rebind());
}

}    // namespace axis_access

template <typename T>
using quaternion = vector<T, 4, axes::wxyz>;

}    // namespace math
}    // namespace psst

#endif /* PUSHKIN_MATH_QUATERNION_HPP_ */
