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

template < typename LHS, typename RHS,
        typename = ::std::enable_if_t< is_expression_v<LHS> || is_expression_v<RHS> > >
constexpr auto
operator + (LHS&& lhs, RHS&& rhs)
{
    using lhs_type = std::decay_t<LHS>;
    using rhs_type = std::decay_t<RHS>;
    if constexpr (is_scalar_v<lhs_type> && is_scalar_v<rhs_type>) {
        using result_type = scalar_expression_result_t<LHS, RHS>;
        return detail::wrap_non_expression_args<scalar_value_sum, result_type>(
              ::std::forward<LHS>(lhs), ::std::forward<RHS>(rhs)
            );
    } else if constexpr (is_vector_expression_v<lhs_type> && is_vector_expression_v<rhs_type>) {
        using result_type = vector_expression_result_t<LHS, RHS>;
        return make_binary_expression<vector_sum, result_type>(std::forward<LHS>(lhs), std::forward<RHS>(rhs));
    } else {
        // The types in the static assert are intentionally the same
        static_assert((!std::is_same<value_tag_t<lhs_type>, value_tag_t<lhs_type>>::value),
            "This combination of expressions is not supported");
    }
}

template < typename LHS, typename RHS,
        typename = ::std::enable_if_t< is_expression_v<LHS> || is_expression_v<RHS> > >
constexpr auto
operator - (LHS&& lhs, RHS&& rhs)
{
    using lhs_type = std::decay_t<LHS>;
    using rhs_type = std::decay_t<RHS>;
    if constexpr (is_scalar_v<lhs_type> && is_scalar_v<rhs_type>) {
        using result_type = scalar_expression_result_t<LHS, RHS>;
        return detail::wrap_non_expression_args<scalar_value_diff, result_type>(
              ::std::forward<LHS>(lhs), ::std::forward<RHS>(rhs)
            );
    } else if constexpr (is_vector_expression_v<lhs_type> && is_vector_expression_v<rhs_type>) {
        using result_type = vector_expression_result_t<LHS, RHS>;
        return make_binary_expression<vector_diff, result_type>(std::forward<LHS>(lhs), std::forward<RHS>(rhs));
    } else {
        // The types in the static assert are intentionally the same
        static_assert((!std::is_same<value_tag_t<lhs_type>, value_tag_t<lhs_type>>::value),
            "This combination of expressions is not supported");
    }
}

template < typename LHS, typename RHS,
        typename = ::std::enable_if_t< is_expression_v<LHS> || is_expression_v<RHS> > >
constexpr auto
operator * (LHS&& lhs, RHS&& rhs)
{
    using lhs_type = std::decay_t<LHS>;
    using rhs_type = std::decay_t<RHS>;
    if constexpr (is_scalar_v<lhs_type> && is_scalar_v<rhs_type>) {
        using result_type = scalar_expression_result_t<LHS, RHS>;
        return detail::wrap_non_expression_args<scalar_value_multiply, result_type>(
              ::std::forward<LHS>(lhs), ::std::forward<RHS>(rhs)
            );
    } else if constexpr (is_vector_expression_v<lhs_type> && is_scalar_v<rhs_type>) {
        using result_type = vector_expression_result_t<LHS, RHS>;
        return detail::wrap_non_expression_args<vector_scalar_multiply, result_type>(
              ::std::forward<LHS>(lhs), ::std::forward<RHS>(rhs)
            );
    } else if constexpr (is_scalar_v<lhs_type> && is_vector_expression_v<rhs_type>) {
        using result_type = vector_expression_result_t<LHS, RHS>;
        return detail::wrap_non_expression_args<vector_scalar_multiply, result_type>(
              ::std::forward<RHS>(rhs), ::std::forward<LHS>(lhs)
            );
    } else {
        // The types in the static assert are intentionally the same
        static_assert((!std::is_same<value_tag_t<lhs_type>, value_tag_t<lhs_type>>::value),
            "This combination of expressions is not supported");
    }
}

template < typename LHS, typename RHS,
        typename = ::std::enable_if_t< is_expression_v<LHS> || is_expression_v<RHS> > >
constexpr auto
operator / (LHS&& lhs, RHS&& rhs)
{
    using lhs_type = std::decay_t<LHS>;
    using rhs_type = std::decay_t<RHS>;
    if constexpr (is_scalar_v<lhs_type> && is_scalar_v<rhs_type>) {
        using result_type = scalar_expression_result_t<LHS, RHS>;
        return detail::wrap_non_expression_args<scalar_value_divide, result_type>(
              ::std::forward<LHS>(lhs), ::std::forward<RHS>(rhs)
            );
    } else if constexpr (is_vector_expression_v<lhs_type> && is_scalar_v<rhs_type>) {
      using result_type = vector_expression_result_t<LHS, RHS>;
      return detail::wrap_non_expression_args<vector_scalar_divide, result_type>(
            ::std::forward<LHS>(lhs), ::std::forward<RHS>(rhs)
          );
    } else {
        // The types in the static assert are intentionally the same
        static_assert((!std::is_same<value_tag_t<lhs_type>, value_tag_t<lhs_type>>::value),
            "This combination of expressions is not supported");
    }
}


}  // namespace expr
}  // namespace math
}  // namespace psst


#endif /* PUSHKIN_MATH_DETAIL_EXPRESSION_OPS_HPP_ */
