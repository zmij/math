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

inline namespace s {

//----------------------------------------------------------------------------
template <typename Expression, typename Result>
struct scalar_expression {
    static_assert(!std::is_reference<Result>{},
                  "Result type for a scalar expression cannot be a reference");
    using expression_type = Expression;
    using result_type     = Result;
    using traits          = value_traits_t<result_type>;
    using value_type      = typename traits::type;
    using value_tag       = typename traits::value_tag;

    constexpr value_type
    value() const
    {
        return rebind().value();
    }
    constexpr operator value_type() const { return this->value(); }

private:
    constexpr expression_type const&
    rebind() const&
    {
        return static_cast<expression_type const&>(*this);
    }
};

template <template <typename> class Expression, typename Arg,
          typename Result = scalar_result_t<Arg>>
using unary_scalar_expression = scalar_expression<Expression<Arg>, Result>;
template <template <typename, typename> class Expression, typename LHS, typename RHS,
          typename Result = scalar_expression_result_t<LHS, RHS>>
using binary_scalar_expression = scalar_expression<Expression<LHS, RHS>, Result>;

//----------------------------------------------------------------------------
template <typename T>
struct scalar_constant : scalar_expression<scalar_constant<T>, std::decay_t<T>> {
    static_assert(is_scalar_v<T>, "Can use scalar const only with scalar values");
    static_assert(!is_expression_v<T>, "Can use scalar const only with scalar values");
    using base_type  = scalar_expression<scalar_constant<T>, std::decay_t<T>>;
    using value_type = typename base_type::value_type;

    scalar_constant(value_type const& arg) : arg_{arg} {}
    constexpr value_type
    value() const
    {
        return this->arg_;
    }

private:
    value_type arg_;
};

template <typename T>
constexpr auto
make_scalar_constant(T&& v)
{
    return make_unary_expression<scalar_constant>(std::forward<T>(v));
}

namespace detail {

template <typename Arg, typename = utils::void_t<>>
struct wrap_arg {
    using type = scalar_constant<std::decay_t<Arg>>;
};

template <typename Arg>
struct wrap_arg<Arg, std::enable_if_t<is_expression_v<Arg>>> {
    using type = expression_argument_t<Arg>;
};

template <typename Arg>
using wrap_arg_t = typename wrap_arg<Arg>::type;

template <template <typename, typename> class ExpressionType, typename LHS, typename RHS>
constexpr auto
wrap_non_expression_args(LHS&& lhs, RHS&& rhs)
{
    return make_binary_expression<ExpressionType>(wrap_arg_t<LHS&&>(std::forward<LHS>(lhs)),
                                                  wrap_arg_t<RHS&&>(std::forward<RHS>(rhs)));
}

template <template <typename, typename, typename> class ExpressionType, typename Result,
          typename LHS, typename RHS>
constexpr auto
wrap_non_expression_args(LHS&& lhs, RHS&& rhs)
{
    return make_binary_expression<ExpressionType, Result>(
        wrap_arg_t<LHS&&>(std::forward<LHS>(lhs)), wrap_arg_t<RHS&&>(std::forward<RHS>(rhs)));
}

template <template <typename...> class ExpressionType, typename... Args>
constexpr auto
wrap_non_expression_args(Args&&... args)
{
    return make_n_ary_expression<ExpressionType>(wrap_arg_t<Args&&>(std::forward<Args>(args))...);
}

}    // namespace detail

//----------------------------------------------------------------------------
//@{
/** @name Inverse expression result */
template <typename Expression>
struct not_ : unary_scalar_expression<not_, Expression, bool>, unary_expression<Expression> {
    static_assert(is_scalar_v<Expression>, "Can apply not_ only to scalar expressions");
    using expression_base = unary_expression<Expression>;

    using expression_base::expression_base;

    constexpr bool
    value() const
    {
        return !this->arg_.value();
    }
};

template <typename Expression, typename = enable_if_scalar_expression<Expression>>
constexpr auto operator!(Expression&& exp)
{
    return make_unary_expression<not_>(std::forward<Expression>(exp));
}
//@}

//----------------------------------------------------------------------------
//@{
/** @name Sum two scalar expressions */
template <typename LHS, typename RHS>
struct scalar_value_sum : binary_scalar_expression<scalar_value_sum, LHS, RHS>,
                          binary_expression<LHS, RHS> {
    using base_type       = binary_scalar_expression<scalar_value_sum, LHS, RHS>;
    using value_type      = typename base_type::value_type;
    using expression_base = binary_expression<LHS, RHS>;

    using expression_base::expression_base;

    constexpr value_type
    value() const
    {
        return this->lhs_.value() + this->rhs_.value();
    }
};

template <typename LHS, typename RHS, typename = enable_if_scalar_args<LHS, RHS>>
constexpr auto
operator+(LHS&& lhs, RHS&& rhs)
{
    return detail::wrap_non_expression_args<scalar_value_sum>(std::forward<LHS>(lhs),
                                                              std::forward<RHS>(rhs));
}
//@}

//----------------------------------------------------------------------------
//@{
/** @name Subtract one scalar expression from another */
template <typename LHS, typename RHS>
struct scalar_value_diff : binary_scalar_expression<scalar_value_diff, LHS, RHS>,
                           binary_expression<LHS, RHS> {
    using base_type       = binary_scalar_expression<scalar_value_diff, LHS, RHS>;
    using value_type      = typename base_type::value_type;
    using expression_base = binary_expression<LHS, RHS>;

    using expression_base::expression_base;

    constexpr value_type
    value() const
    {
        return this->lhs_.value() - this->rhs_.value();
    }
};

template <typename LHS, typename RHS, typename = enable_if_scalar_args<LHS, RHS>>
constexpr auto
operator-(LHS&& lhs, RHS&& rhs)
{
    return detail::wrap_non_expression_args<scalar_value_diff>(std::forward<LHS>(lhs),
                                                               std::forward<RHS>(rhs));
}
//@}

//----------------------------------------------------------------------------
//@{
/** @name Multiply two scalar expressions */
template <typename LHS, typename RHS>
struct scalar_value_multiply : binary_scalar_expression<scalar_value_multiply, LHS, RHS>,
                               binary_expression<LHS, RHS> {
    using base_type       = binary_scalar_expression<scalar_value_multiply, LHS, RHS>;
    using value_type      = typename base_type::value_type;
    using expression_base = binary_expression<LHS, RHS>;

    using expression_base::expression_base;

    constexpr value_type
    value() const
    {
        return this->lhs_.value() * this->rhs_.value();
    }
};

template <typename LHS, typename RHS, typename = enable_if_scalar_args<LHS, RHS>>
constexpr auto operator*(LHS&& lhs, RHS&& rhs)
{
    return detail::wrap_non_expression_args<scalar_value_multiply>(std::forward<LHS>(lhs),
                                                                   std::forward<RHS>(rhs));
}
//@}

//----------------------------------------------------------------------------
//@{
/** @name Divide one scalar expression by another */
template <typename LHS, typename RHS>
struct scalar_value_divide : binary_scalar_expression<scalar_value_divide, LHS, RHS>,
                             binary_expression<LHS, RHS> {
    using base_type       = binary_scalar_expression<scalar_value_divide, LHS, RHS>;
    using value_type      = typename base_type::value_type;
    using expression_base = binary_expression<LHS, RHS>;

    using expression_base::expression_base;

    constexpr value_type
    value() const
    {
        return this->lhs_.value() / this->rhs_.value();
    }
};

template <typename LHS, typename RHS, typename = enable_if_scalar_args<LHS, RHS>>
constexpr auto
operator/(LHS&& lhs, RHS&& rhs)
{
    return detail::wrap_non_expression_args<scalar_value_divide>(std::forward<LHS>(lhs),
                                                                 std::forward<RHS>(rhs));
}
//@}

//----------------------------------------------------------------------------
//@{
/** @name Square root expression */
template <typename Expression>
struct square_root : unary_scalar_expression<square_root, Expression>,
                     unary_expression<Expression> {
    static_assert(is_scalar_v<Expression>, "Can apply square_root only to scalar expressions");
    using base_type       = unary_scalar_expression<square_root, Expression>;
    using value_type      = typename base_type::value_type;
    using expression_base = unary_expression<Expression>;

    using expression_base::expression_base;

    constexpr value_type
    value() const
    {
        using std::sqrt;
        if (value_cache_ == nval) {
            value_cache_ = sqrt(this->arg_.value());
        }
        return value_cache_;
    }

private:
    // TODO Optional
    static constexpr value_type nval         = std::numeric_limits<value_type>::min();
    mutable value_type          value_cache_ = nval;
};

template <typename Expression, typename = enable_if_scalar_expression<Expression>>
constexpr auto
sqrt(Expression&& ex)
{
    return make_unary_expression<square_root>(std::forward<Expression>(ex));
}
//@}

}    // namespace s

}    // namespace expr
}    // namespace math
}    // namespace psst

#endif /* PUSHKIN_MATH_DETAIL_SCALAR_EXPRESSIONS_HPP_ */
