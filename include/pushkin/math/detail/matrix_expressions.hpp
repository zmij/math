/*
 * matrix_expressions.hpp
 *
 *  Created on: Dec 26, 2018
 *      Author: ser-fedorov
 */

#ifndef PUSHKIN_MATH_DETAIL_MATRIX_EXPRESSIONS_HPP_
#define PUSHKIN_MATH_DETAIL_MATRIX_EXPRESSIONS_HPP_

#include <pushkin/math/detail/vector_expressions.hpp>

namespace psst {
namespace math {
namespace expr {

inline namespace m {

template < typename MatrixExpression, std::size_t RN >
struct nth_row
    : vector_expression< nth_row< MatrixExpression, RN >, typename MatrixExpression::row_type >,
      unary_expression<MatrixExpression> {

    using matrix_traits = typename MatrixExpression::traits;
    static_assert(RN < matrix_traits::rows,
        "Invalid row index");

    using base_type = vector_expression< nth_row< MatrixExpression, RN >, typename MatrixExpression::row_type >;
    using value_type          = typename base_type::value_type;

    using expression_base = unary_expression<MatrixExpression>;
    using expression_base::expression_base;

    template < std::size_t CN >
    constexpr auto
    at() const
    {
        static_assert(CN < matrix_traits::cols, "Invalid column index");
        return this->arg_.template element<RN, CN>();
    }
};

template < typename MatrixExpression, std::size_t CN >
struct nth_col
    : vector_expression< nth_col< MatrixExpression, CN >, typename MatrixExpression::col_type >,
      unary_expression<MatrixExpression> {

    using matrix_traits = typename MatrixExpression::traits;
    static_assert(CN < matrix_traits::cols,
        "Invalid col index");

    using base_type = vector_expression< nth_row< MatrixExpression, CN >, typename MatrixExpression::col_type >;
    using value_type          = typename base_type::value_type;

    using expression_base = unary_expression<MatrixExpression>;
    using expression_base::expression_base;

    template < std::size_t RN >
    constexpr auto
    at() const
    {
        static_assert(RN < matrix_traits::rows, "Invalid row index");
        return this->arg_.template element<RN, CN>();
    }
};

template <typename Expression, typename Result = Expression>
struct matrix_expression {
    static_assert(is_matrix_v<Result>,
        "Result of matrix expression must be a matrix");
    using expression_type       = Expression;
    using result_type           = Result;
    using traits                = value_traits_t<Result>;
    using value_type            = typename traits::value_type;
    using value_tag             = typename traits::value_tag;
    using axes_names            = typename traits::axes_names;
    using matrix_type           = typename traits::matrix_type;
    using transposed_type       = typename traits::transposed_type;
    using row_type              = typename traits::row_type;
    using col_type              = typename traits::col_type;

    static constexpr auto rows  = traits::rows;
    static constexpr auto cols  = traits::cols;
    static constexpr auto size  = traits::size;

    template <std::size_t R, std::size_t C>
    constexpr value_type
    element() const
    {
        static_assert(R < rows, "Invalid matrix expression row index");
        static_assert(C < cols, "Invalid matrix expression col index");
        return rebind().template element<R, C>();
    }

    template <std::size_t R>
    constexpr auto
    row() const
    {
        return nth_row<expression_type, R>{ rebind() };
    }

    template <std::size_t C>
    constexpr auto
    col() const
    {
        return nth_col<expression_type, C>{ rebind() };
    }
private:
    constexpr expression_type const&
    rebind() const { return static_cast<expression_type const&>(*this); }
};

//@{
/** @name Compare two matrix expressions */
namespace detail {

template <std::size_t R, typename LHS, typename RHS>
struct matrix_expression_cmp;

template <typename LHS, typename RHS>
struct matrix_expression_cmp<0, LHS, RHS> {
    static_assert((is_matrix_expression_v<LHS> && is_matrix_expression_v<RHS>),
          "Both sides to the comparison must be matrix expressions");
    constexpr static auto
    cmp(LHS const& lhs, RHS const& rhs)
    {
        return cmp(lhs.template row<0>(), rhs.template row<0>());
    }
};

template <std::size_t R, typename LHS, typename RHS>
struct matrix_expression_cmp {
    static_assert((is_matrix_expression_v<LHS> && is_matrix_expression_v<RHS>),
          "Both sides to the comparison must be matrix expressions");
    using prev_element = matrix_expression_cmp< R - 1, LHS, RHS >;
    constexpr static auto cmp(LHS const& lhs, RHS const& rhs)
    {
        auto res = prev_element::cmp(lhs, rhs);
        if (res == 0) {
            return cmp(lhs.template row<R>(), rhs.template row<R>());
        }
        return res;
    }
};

}  // namespace detail

template <typename LHS, typename RHS>
struct matrix_cmp
        : binary_scalar_expression<matrix_cmp, LHS, RHS, int>,
          binary_expression<LHS, RHS> {
    static_assert((is_matrix_expression_v<LHS> && is_matrix_expression_v<RHS>),
          "Both sides to the comparison must be matrix expressions");
    static constexpr std::size_t cmp_size
          = utils::min_v< LHS::rows, RHS::rows >;
    using cmp_type = detail::matrix_expression_cmp<cmp_size - 1, LHS, RHS>;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    constexpr auto
    value() const {
        return cmp_type::cmp(this->lhs_, this->rhs_);
    }
};

template <typename LHS, typename RHS,
          typename = std::enable_if_t<
                        is_matrix_expression_v<LHS> &&
                        is_matrix_expression_v<RHS>>>
constexpr auto
cmp(LHS&& lhs, RHS&& rhs)
{
    return make_binary_expression<matrix_cmp>(std::forward<LHS>(lhs), std::forward<RHS>(rhs));
}
//@}

//@{
/** @name Matrix equality */
template <typename LHS, typename RHS>
struct matrix_eq
        : binary_scalar_expression<matrix_eq, LHS, RHS, bool>,
          binary_expression<LHS, RHS> {
    static_assert((is_matrix_expression_v<LHS> && is_matrix_expression_v<RHS>),
          "Both sides to the comparison must be matrix expressions");
    static constexpr std::size_t cmp_size
          = utils::min_v< LHS::rows, RHS::rows >;
    using cmp_type = detail::matrix_expression_cmp<cmp_size - 1, LHS, RHS>;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    constexpr bool
    value() const {
        return cmp_type::cmp(this->lhs_, this->rhs_) == 0;
    }
};

template <typename LHS, typename RHS,
          typename = enable_if_both_matrix_expressions<LHS, RHS>>
constexpr auto
operator == (LHS&& lhs, RHS&& rhs)
{
    return make_binary_expression<matrix_eq>(std::forward<LHS>(lhs), std::forward<RHS>(rhs));
}

template <typename LHS, typename RHS,
          typename = enable_if_both_matrix_expressions<LHS, RHS>>
constexpr auto
operator != (LHS&& lhs, RHS&& rhs)
{
    return !(std::forward<LHS>(lhs) == std::forward<RHS>(rhs));
}
//@}

//@{
/** @name Matrix inequality */
template <typename LHS, typename RHS>
struct matrix_less
        : binary_scalar_expression<matrix_less, LHS, RHS, bool>,
          binary_expression<LHS, RHS> {
    static_assert((is_matrix_expression_v<LHS> && is_matrix_expression_v<RHS>),
          "Both sides to the comparison must be matrix expressions");
    static constexpr std::size_t cmp_size
          = utils::min_v< LHS::rows, RHS::rows >;
    using cmp_type = detail::matrix_expression_cmp<cmp_size - 1, LHS, RHS>;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    constexpr bool
    value() const {
        return cmp_type::cmp(this->lhs_, this->rhs_) < 0;
    }
};

template <typename LHS, typename RHS,
          typename = enable_if_both_matrix_expressions<LHS, RHS>>
constexpr auto
operator < (LHS&& lhs, RHS&& rhs)
{
    return make_binary_expression<matrix_less>(std::forward<LHS>(lhs), std::forward<RHS>(rhs));
}

template <typename LHS, typename RHS,
          typename = enable_if_both_matrix_expressions<LHS, RHS>>
constexpr auto
operator <= (LHS&& lhs, RHS&& rhs)
{
    return !(std::forward<RHS>(rhs) < std::forward<LHS>(lhs));
}

template <typename LHS, typename RHS,
          typename = enable_if_both_matrix_expressions<LHS, RHS>>
constexpr auto
operator > (LHS&& lhs, RHS&& rhs)
{
  return make_binary_expression<matrix_less>(std::forward<RHS>(rhs), std::forward<LHS>(lhs));
}

template <typename LHS, typename RHS,
          typename = enable_if_both_matrix_expressions<LHS, RHS>>
constexpr auto
operator >= (LHS&& lhs, RHS&& rhs)
{
    return !(std::forward<LHS>(lhs) < std::forward<RHS>(rhs));
}
//@}

//@{
template <typename Expr>
struct matrix_transpose
        : matrix_expression<matrix_transpose<Expr>,
              typename std::decay_t<Expr>::transposed_type>,
          unary_expression<Expr> {

    using base_type       = matrix_expression<matrix_transpose<Expr>,
                                typename std::decay_t<Expr>::transposed_type>;
    using value_type      = typename base_type::value_type;

    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    template <std::size_t R, std::size_t C>
    constexpr value_type
    element() const
    {
        static_assert(R < base_type::rows, "Invalid matrix expression row index");
        static_assert(C < base_type::cols, "Invalid matrix expression col index");
        return this->arg_.template element<C, R>();
    }
};

template <typename Expr,
          typename = enable_if_matrix_expression<Expr>>
constexpr auto
transpose(Expr&& expr)
{
    return make_unary_expression<matrix_transpose>(std::forward<Expr>(expr));
}
//@}

//@{
/** @name Matrices sum */
template <typename LHS, typename RHS>
struct matrix_sum_result {
    static_assert((is_matrix_expression_v<LHS> && is_matrix_expression_v<RHS>),
        "Both sides to the expession must be matrix expressions");
    using lhs_type = std::decay_t<LHS>;
    using rhs_type = std::decay_t<RHS>;
    static_assert(lhs_type::cols == rhs_type::cols, "Matrices must be of equal size");
    static_assert(lhs_type::rows == rhs_type::rows, "Matrices must be of equal size");
    static_assert((std::is_same<typename lhs_type::axes_names, typename rhs_type::axes_names>::value),
        "Matrices must have the same axes");
    using value_type  = scalar_expression_result_t<lhs_type, rhs_type>;
    using type        = matrix<value_type, lhs_type::rows, lhs_type::cols, typename lhs_type::axes_names>;
};
template <typename LHS, typename RHS>
using matrix_sum_result_t = typename matrix_sum_result<LHS, RHS>::type;

template <typename LHS, typename RHS>
struct matrix_sum
          : matrix_expression<matrix_sum<LHS, RHS>, matrix_sum_result_t<LHS, RHS>>,
            binary_expression<LHS, RHS> {
    using base_type   = matrix_expression<matrix_sum<LHS, RHS>, matrix_sum_result_t<LHS, RHS>>;
    using value_type  = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template <std::size_t R, std::size_t C>
    constexpr value_type
    element() const
    {
        return this->lhs_.template element<R, C>() + this->rhs_.template element<R, C>();
    }
};

template <typename LHS, typename RHS,
          typename = enable_if_both_matrix_expressions<LHS, RHS>>
constexpr auto
operator + (LHS&& lhs, RHS&& rhs)
{
  return make_binary_expression<matrix_sum>(std::forward<LHS>(lhs), std::forward<RHS>(rhs));
}
//@}

//@{
/** @name Matrix difference */
template <typename LHS, typename RHS>
struct matrix_diff
          : matrix_expression<matrix_diff<LHS, RHS>, matrix_sum_result_t<LHS, RHS>>,
            binary_expression<LHS, RHS> {
    using base_type   = matrix_expression<matrix_diff<LHS, RHS>, matrix_sum_result_t<LHS, RHS>>;
    using value_type  = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template <std::size_t R, std::size_t C>
    constexpr value_type
    element() const
    {
        return this->lhs_.template element<R, C>() - this->rhs_.template element<R, C>();
    }
};

template <typename LHS, typename RHS,
          typename = enable_if_both_matrix_expressions<LHS, RHS>>
constexpr auto
operator - (LHS&& lhs, RHS&& rhs)
{
  return make_binary_expression<matrix_diff>(std::forward<LHS>(lhs), std::forward<RHS>(rhs));
}
//@}

//@{
/** @name Matrix by scalar multiplication */
template <typename LHS, typename RHS>
struct matrix_scalar_mul_result {
    static_assert((is_matrix_expression_v<LHS>),
        "Left side to the expession must be matrix expressions");
    static_assert(is_scalar_v<RHS>,
        "Right side to the expression must be a scalar expression");
    using lhs_type = std::decay_t<LHS>;
    using rhs_type = std::decay_t<RHS>;
    using value_type  = scalar_expression_result_t<lhs_type, rhs_type>;
    using type        = matrix<value_type, lhs_type::rows, lhs_type::cols, typename lhs_type::axes_names>;
};
template <typename LHS, typename RHS>
using matrix_scalar_mul_result_t = typename matrix_scalar_mul_result<LHS, RHS>::type;

template <typename LHS, typename RHS>
struct matrix_scalar_multiply
        : matrix_expression< matrix_scalar_multiply<LHS, RHS>, matrix_scalar_mul_result_t<LHS, RHS> >,
          binary_expression<LHS, RHS> {
    using base_type  = matrix_expression< matrix_scalar_multiply<LHS, RHS>, matrix_scalar_mul_result_t<LHS, RHS> >;
    using value_type = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template <std::size_t R, std::size_t C>
    constexpr value_type
    element() const
    {
        return this->lhs_.template element<R, C>() * this->rhs_;
    }
};
//@}

//@{
/** @name Matrix by scalar division */
template <typename LHS, typename RHS>
struct matrix_scalar_divide
        : matrix_expression< matrix_scalar_divide<LHS, RHS>, matrix_scalar_mul_result_t<LHS, RHS> >,
          binary_expression<LHS, RHS> {
    using base_type  = matrix_expression< matrix_scalar_divide<LHS, RHS>, matrix_scalar_mul_result_t<LHS, RHS> >;
    using value_type = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template <std::size_t R, std::size_t C>
    constexpr value_type
    element() const
    {
        return this->lhs_.template element<R, C>() / this->rhs_;
    }
};

template <typename LHS, typename RHS,
          typename = std::enable_if_t<
            is_matrix_expression_v<LHS> && is_scalar_v<RHS>
          >>
constexpr auto
operator / (LHS&& lhs, RHS&& rhs)
{
    return s::detail::wrap_non_expression_args<matrix_scalar_divide>(
          ::std::forward<LHS>(lhs), ::std::forward<RHS>(rhs)
        );
}
//@}

//@{
/** @name Matrix-matrix multiplication */
template < typename LHS, typename RHS >
struct matrix_matrix_mul_result {
    static_assert((is_matrix_expression_v<LHS> && is_matrix_expression_v<RHS>),
        "Both sides to the expession must be matrix expressions");
    using lhs_type = std::decay_t<LHS>;
    using rhs_type = std::decay_t<RHS>;
    static_assert(lhs_type::cols == rhs_type::rows, "Left hand columns must be equal to right hand rows");
    static_assert((std::is_same<typename lhs_type::axes_names, typename rhs_type::axes_names>::value),
        "Matrices must have the same axes");
    using value_type  = scalar_expression_result_t<lhs_type, rhs_type>;
    using type        = matrix<value_type, lhs_type::rows, rhs_type::cols, typename lhs_type::axes_names>;
};

template <  typename LHS, typename RHS >
using matrix_matrix_mul_result_t = typename matrix_matrix_mul_result<LHS, RHS>::type;

template < typename LHS, typename RHS>
struct matrix_matrix_multiply
        : matrix_expression< matrix_matrix_multiply<LHS, RHS>, matrix_matrix_mul_result_t<LHS, RHS> >,
          binary_expression<LHS, RHS> {

    using base_type = matrix_expression< matrix_matrix_multiply<LHS, RHS>, matrix_matrix_mul_result_t<LHS, RHS> >;
    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template <std::size_t R, std::size_t C>
    constexpr auto
    element() const
    {
        return dot_product(this->lhs_.template row<R>(), this->rhs_.template col<C>());
    }
};

template <typename LHS, typename RHS,
          typename = std::enable_if_t<
            (is_matrix_expression_v<LHS> && is_scalar_v<RHS>) ||
            (is_scalar_v<LHS> && is_matrix_expression_v<RHS>) ||
            (is_matrix_expression_v<LHS> && is_matrix_expression_v<RHS>)
          >>
constexpr auto
operator * (LHS&& lhs, RHS&& rhs)
{
    if constexpr (is_matrix_expression_v<LHS> && is_scalar_v<RHS>) {
        return s::detail::wrap_non_expression_args<matrix_scalar_multiply>(
              ::std::forward<LHS>(lhs), ::std::forward<RHS>(rhs)
            );
    } else if constexpr (is_scalar_v<LHS> && is_matrix_expression_v<RHS>) {
        return s::detail::wrap_non_expression_args<matrix_scalar_multiply>(
              ::std::forward<RHS>(rhs), ::std::forward<LHS>(lhs)
            );
    } else if constexpr (is_matrix_expression_v<LHS> && is_matrix_expression_v<RHS>) {
        return make_binary_expression<matrix_matrix_multiply>(\
              ::std::forward<LHS>(lhs), ::std::forward<RHS>(rhs)
            );
    }
}
//@}

}  // namespace m

}  // namespace expr
}  // namespace math
}  // namespace psst

#endif /* PUSHKIN_MATH_DETAIL_MATRIX_EXPRESSIONS_HPP_ */
