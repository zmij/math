/*
 * scalar_epressions.hpp
 *
 *  Created on: Dec 25, 2018
 *      Author: ser-fedorov
 */

#ifndef PUSHKIN_MATH_DETAIL_SCALAR_EXPRESSIONS_HPP_
#define PUSHKIN_MATH_DETAIL_SCALAR_EXPRESSIONS_HPP_

#include <pushkin/math/detail/expressions.hpp>

#include <cmath>

namespace psst {
namespace math {
namespace expr {

template < typename T >
struct scalar_constant
      : scalar_expression< scalar_constant<T>, std::decay_t<T> > {
    static_assert(is_scalar_v<T>, "Can use scalar const only with scalar values");
    static_assert(!is_expression_v<T>, "Can use scalar const only with scalar values");
    using base_type       = scalar_expression< scalar_constant<T>, std::decay_t<T> >;
    using value_type      = typename base_type::value_type;

    scalar_constant(value_type const& arg) : arg_{arg} {}
    constexpr value_type
    value() const
    {
        return this->arg_;
    }
private:
    value_type arg_;
};

namespace detail {

template < template< typename, typename, typename > class ExpressionType,
    typename Result, typename LHS, typename RHS>
constexpr auto
wrap_non_expression_args(LHS&& lhs, RHS&& rhs)
{
    static_assert(is_expression_v<LHS> || is_expression_v<RHS>,
          "At least one of the arguments to a binary expression must be an expression");
    if constexpr (is_expression_v<LHS> && is_expression_v<RHS>) {
        return make_binary_expression<ExpressionType, Result>(
            std::forward<LHS>(lhs),
            std::forward<RHS>(rhs));
    } else if constexpr (is_expression_v<LHS>) {
        return make_binary_expression<ExpressionType, Result>(
            std::forward<LHS>(lhs),
            scalar_constant<std::decay_t<RHS>>{ std::forward<RHS>(rhs) });
    } else if constexpr (is_expression_v<RHS>) {
        return make_binary_expression<ExpressionType, Result>(
            scalar_constant<std::decay_t<LHS>>{ std::forward<LHS>(lhs) },
            std::forward<RHS>(rhs));
    }
}

}  // namespace detail

template < typename Expression >
struct not_ : scalar_expression< not_<Expression>, bool >,
              unary_expression<Expression> {
    static_assert(is_scalar_v<Expression>, "Can apply not_ only to scalar expressions");
    using expression_base = unary_expression<Expression>;

    using expression_base::expression_base;

    constexpr bool
    value() const
    { return !this->arg_.value(); }
};

template < typename LHS, typename RHS, typename Result >
struct scalar_value_sum
        : scalar_expression< scalar_value_sum<LHS, RHS, Result>, Result >,
          binary_expression<LHS, RHS> {
    using base_type         = scalar_expression< scalar_value_sum<LHS, RHS, Result>, Result >;
    using value_type      = typename base_type::value_type;
    using expression_base   = binary_expression<LHS, RHS>;

    using expression_base::expression_base;

    constexpr value_type
    value() const
    {
        return this->lhs_.value() + this->rhs_.value();
    }
};

template < typename LHS, typename RHS, typename Result >
struct scalar_value_diff
        : scalar_expression< scalar_value_diff<LHS, RHS, Result>, Result >,
          binary_expression<LHS, RHS> {
    using base_type         = scalar_expression< scalar_value_diff<LHS, RHS, Result>, Result >;
    using value_type      = typename base_type::value_type;
    using expression_base   = binary_expression<LHS, RHS>;

    using expression_base::expression_base;

    constexpr value_type
    value() const
    {
        return this->lhs_.value() - this->rhs_.value();
    }
};

template < typename LHS, typename RHS, typename Result >
struct scalar_value_multiply
        : scalar_expression< scalar_value_multiply<LHS, RHS, Result>, Result >,
          binary_expression<LHS, RHS> {
    using base_type         = scalar_expression< scalar_value_multiply<LHS, RHS, Result>, Result >;
    using value_type      = typename base_type::value_type;
    using expression_base   = binary_expression<LHS, RHS>;

    using expression_base::expression_base;

    constexpr value_type
    value() const
    {
        return this->lhs_.value() * this->rhs_.value();
    }
};

template < typename LHS, typename RHS, typename Result >
struct scalar_value_divide
        : scalar_expression< scalar_value_divide<LHS, RHS, Result>, Result >,
          binary_expression<LHS, RHS> {
    using base_type         = scalar_expression< scalar_value_divide<LHS, RHS, Result>, Result >;
    using value_type      = typename base_type::value_type;
    using expression_base   = binary_expression<LHS, RHS>;

    using expression_base::expression_base;

    constexpr value_type
    value() const
    {
        return this->lhs_.value() / this->rhs_.value();
    }
};


template < typename Expression, typename Result >
struct square_root : scalar_expression<square_root<Expression, Result>, Result >,
                     unary_expression<Expression> {
    static_assert(is_scalar_v<Expression>,
          "Can apply square_root only to scalar expressions");
    using base_type   = scalar_expression<square_root<Expression, Result>, Result >;
    using value_type  = typename base_type::value_type;
    using expression_base = unary_expression<Expression>;

    using expression_base::expression_base;

    constexpr value_type
    value() const
    {
        using ::std::sqrt;
        return sqrt(this->arg_.value());
    }
};

template < typename Expression,
    typename = std::enable_if_t<
                    is_scalar_expression_v<std::decay_t<Expression>>>>
constexpr auto
operator ! (Expression&& exp)
{
    return make_unary_expression<not_>(std::forward<Expression>(exp));
}

template <typename Expression,
        typename = std::enable_if_t<
            is_scalar_expression_v<Expression>>>
constexpr auto
sqrt(Expression&& ex)
{
    using result_type = typename std::decay_t<Expression>::result_type;
    return make_unary_expression<square_root, result_type>(
        std::forward<Expression>(ex));
}

}  // namespace expr
}  // namespace math
}  // namespace psst

#endif /* PUSHKIN_MATH_DETAIL_SCALAR_EXPRESSIONS_HPP_ */
