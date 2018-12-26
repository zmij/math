/*
 * expression_ops.hpp
 *
 *  Created on: Dec 26, 2018
 *      Author: ser-fedorov
 */

#ifndef PUSHKIN_MATH_DETAIL_EXPRESSION_OPS_HPP_
#define PUSHKIN_MATH_DETAIL_EXPRESSION_OPS_HPP_

#include <pushkin/math/detail/scalar_expressions.hpp>
#include <pushkin/math/detail/vector_expressions.hpp>

namespace psst {
namespace math {
namespace expr {

//@{
/** @name Scalar expression operators */
inline namespace s {

template <typename LHS, typename RHS,
          typename = std::enable_if_t<
            is_scalar_v<LHS> && is_scalar_v<RHS> &&
            (is_scalar_expression_v<LHS> || is_scalar_expression_v<RHS>)
          >>
constexpr auto
operator + (LHS&& lhs, RHS&& rhs)
{
    return detail::wrap_non_expression_args<scalar_value_sum>(
            std::forward<LHS>(lhs), std::forward<RHS>(rhs)
        );
}

template <typename LHS, typename RHS,
          typename = std::enable_if_t<
            is_scalar_v<LHS> && is_scalar_v<RHS> &&
            (is_scalar_expression_v<LHS> || is_scalar_expression_v<RHS>)
          >>
constexpr auto
operator - (LHS&& lhs, RHS&& rhs)
{
    return detail::wrap_non_expression_args<scalar_value_sum>(
            std::forward<LHS>(lhs), std::forward<RHS>(rhs)
        );
}

template <typename LHS, typename RHS,
          typename = std::enable_if_t<
            is_scalar_v<LHS> && is_scalar_v<RHS> &&
            (is_scalar_expression_v<LHS> || is_scalar_expression_v<RHS>)
          >>
constexpr auto
operator * (LHS&& lhs, RHS&& rhs)
{
    return detail::wrap_non_expression_args<scalar_value_multiply>(
            std::forward<LHS>(lhs), std::forward<RHS>(rhs)
        );
}

template <typename LHS, typename RHS,
          typename = std::enable_if_t<
            is_scalar_v<LHS> && is_scalar_v<RHS> &&
            (is_scalar_expression_v<LHS> || is_scalar_expression_v<RHS>)
          >>
constexpr auto
operator / (LHS&& lhs, RHS&& rhs)
{
    return detail::wrap_non_expression_args<scalar_value_divide>(
            std::forward<LHS>(lhs), std::forward<RHS>(rhs)
        );
}

}  // namespace s
//@}
//@{
/** @name Vector expression operators */
inline namespace v {

template <typename LHS, typename RHS,
          typename = std::enable_if_t<
            is_vector_expression_v<LHS> && is_vector_expression_v<RHS>
          >>
constexpr auto
operator + (LHS&& lhs, RHS&& rhs)
{
    return make_binary_expression<vector_sum>(
            std::forward<LHS>(lhs), std::forward<RHS>(rhs)
        );
}

template <typename LHS, typename RHS,
          typename = std::enable_if_t<
            is_vector_expression_v<LHS> && is_vector_expression_v<RHS>
          >>
constexpr auto
operator - (LHS&& lhs, RHS&& rhs)
{
    return make_binary_expression<vector_diff>(
            std::forward<LHS>(lhs), std::forward<RHS>(rhs)
        );
}

template <typename LHS, typename RHS,
          typename = std::enable_if_t<
            (is_vector_expression_v<LHS> && is_scalar_v<RHS>) ||
            (is_scalar_v<LHS> && is_vector_expression_v<RHS>)
          >>
constexpr auto
operator * (LHS&& lhs, RHS&& rhs)
{
    if constexpr (is_vector_expression_v<LHS> && is_scalar_v<RHS>) {
        return detail::wrap_non_expression_args<vector_scalar_multiply>(
              ::std::forward<LHS>(lhs), ::std::forward<RHS>(rhs)
            );
    } else if constexpr (is_scalar_v<LHS> && is_vector_expression_v<RHS>) {
        return detail::wrap_non_expression_args<vector_scalar_multiply>(
              ::std::forward<RHS>(rhs), ::std::forward<LHS>(lhs)
            );
    }
}

template <typename LHS, typename RHS,
          typename = std::enable_if_t<
            is_vector_expression_v<LHS> && is_scalar_v<RHS>
          >>
constexpr auto
operator / (LHS&& lhs, RHS&& rhs)
{
    return detail::wrap_non_expression_args<vector_scalar_divide>(
          ::std::forward<LHS>(lhs), ::std::forward<RHS>(rhs)
        );
}

}  // namespace v
//@}
inline namespace m {



}  // namespace m

}  // namespace expr
}  // namespace math
}  // namespace psst


#endif /* PUSHKIN_MATH_DETAIL_EXPRESSION_OPS_HPP_ */
