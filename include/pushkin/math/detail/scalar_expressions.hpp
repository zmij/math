/*
 * scalar_epressions.hpp
 *
 *  Created on: Dec 25, 2018
 *      Author: ser-fedorov
 */

#ifndef PUSHKIN_MATH_DETAIL_SCALAR_EXPRESSIONS_HPP_
#define PUSHKIN_MATH_DETAIL_SCALAR_EXPRESSIONS_HPP_

#include <pushkin/math/detail/expressions.hpp>
#include <pushkin/meta/algorithm.hpp>

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
          typename Result = scalar_expression_result_t<Arg>>
using unary_scalar_expression = scalar_expression<Expression<Arg>, Result>;
template <template <typename, typename> class Expression, typename Axes, typename Arg,
          typename Result = scalar_expression_result_t<Arg>>
using unary_scalar_expression_axes = scalar_expression<Expression<Axes, Arg>, Result>;
template <template <typename, typename> class Expression, typename LHS, typename RHS,
          typename Result = scalar_expression_result_t<LHS, RHS>>
using binary_scalar_expression = scalar_expression<Expression<LHS, RHS>, Result>;

//----------------------------------------------------------------------------
//@{
/** @name can_differentiate */
template <typename T, typename = utils::void_t<>>
struct can_differentiate : std::false_type {};
template <typename T>
struct can_differentiate<
    T, utils::void_t<decltype(std::declval<std::decay_t<T> const&>().derivative())>>
    : std::true_type {};
template <typename T>
using can_differentiate_t = typename can_differentiate<T>::type;
template <typename T>
constexpr bool can_differentiate_v = can_differentiate_t<T>::value;
//@}

//@{
/** @name can_partially_differentiate */
template <typename T, typename = utils::void_t<>>
struct can_partially_differentiate : std::false_type {};
template <typename T>
struct can_partially_differentiate<
    T,
    utils::void_t<decltype(std::declval<std::decay_t<T> const&>().template derivative<'a', 0>())>>
    : std::true_type {};
template <typename T>
using can_partially_differentiate_t = typename can_partially_differentiate<T>::type;
template <typename T>
constexpr bool can_partially_differentiate_v = can_partially_differentiate_t<T>::value;
//@}

template <typename T>
constexpr auto
derivative(T&& arg)
{
    static_assert((can_differentiate_v<T>),
                  "Derivative is not defined for this type of expression");
    return arg.derivative();
}

template <char F, std::size_t I, typename T>
constexpr auto
derivative(T&& arg)
{
    static_assert((can_partially_differentiate_v<T>),
                  "Partial derivative is not defined for this type of expression");
    return arg.template derivative<F, I>();
}

//----------------------------------------------------------------------------
//@{
using number_type = long long;

template <number_type V>
struct number : scalar_expression<number<V>, number_type> {
    using base_type  = scalar_expression<number<V>, number_type>;
    using value_type = typename base_type::value_type;

    constexpr value_type
    value() const
    {
        return V;
    }

    constexpr auto
    derivative() const
    {
        return number<0>{};
    }

    template <char Family, std::size_t N>
    constexpr auto
    derivative() const
    {
        return number<0>{};
    }
};

using zero_type = number<0>;
using unit_type = number<1>;

constexpr zero_type zero{};
constexpr unit_type one{};

//@{
/** @name is_zero */
template <typename T>
struct is_zero : std::is_same<std::decay_t<T>, zero_type>::type {};
template <typename T>
using is_zero_t = typename is_zero<T>::type;
template <typename T>
constexpr bool is_zero_v = is_zero_t<T>::value;
//@}

//@{
/** @name is_one */
template <typename T>
struct is_one : std::is_same<std::decay_t<T>, unit_type>::type {};
template <typename T>
using is_one_t = typename is_one<T>::type;
template <typename T>
constexpr bool is_one_v = is_one_t<T>::value;
//@}

template <number_type V>
constexpr auto
operator-(number<V> const& v)
{
    // clang-format off
    return number< -V>{};
    // clang-format on
}
template <number_type LHS, number_type RHS>
constexpr auto
operator+(number<LHS> const& lhs, number<RHS> const& rhs)
{
    return number<LHS + RHS>{};
}
template <number_type LHS, number_type RHS>
constexpr auto
operator+(number<LHS>&& lhs, number<RHS>&& rhs)
{
    return number<LHS + RHS>{};
}

template <number_type LHS, number_type RHS>
constexpr auto
operator-(number<LHS> const& lhs, number<RHS> const& rhs)
{
    return number<LHS - RHS>{};
}
template <number_type LHS, number_type RHS>
constexpr auto
operator-(number<LHS>&& lhs, number<RHS>&& rhs)
{
    return number<LHS - RHS>{};
}

template <number_type LHS, number_type RHS>
constexpr auto operator*(number<LHS> const& lhs, number<RHS> const& rhs)
{
    return number<LHS * RHS>{};
}
template <number_type LHS, number_type RHS>
constexpr auto operator*(number<LHS>&& lhs, number<RHS>&& rhs)
{
    return number<LHS * RHS>{};
}

template <number_type LHS, number_type RHS>
constexpr auto
operator/(number<LHS> const& lhs, number<RHS> const& rhs)
{
    static_assert(RHS != 0, "Cannot divide by zero");
    return number<LHS / RHS>{};
}
template <number_type LHS, number_type RHS>
constexpr auto
operator/(number<LHS>&& lhs, number<RHS>&& rhs)
{
    static_assert(RHS != 0, "Cannot divide by zero");
    return number<LHS / RHS>{};
}
//@}

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

    constexpr auto
    derivative() const
    {
        return zero;
    }

    template <char Family, std::size_t N>
    constexpr auto
    derivative() const
    {
        return zero;
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
struct wrap_arg<Arg, std::enable_if_t<!is_expression_v<Arg> && std::is_rvalue_reference<Arg>{}>> {
    using type = scalar_constant<std::decay_t<Arg>>;
};

template <typename Arg>
struct wrap_arg<Arg, std::enable_if_t<is_expression_v<Arg>>> {
    using type = expression_argument_t<Arg>;
};

template <typename Arg>
using wrap_arg_t = typename wrap_arg<Arg>::type;

template <template <typename> class ExpressionType, typename Arg>
constexpr auto
wrap_non_expression_args(Arg&& arg)
{
    return make_unary_expression<ExpressionType>(wrap_arg_t<Arg&&>(std::forward<Arg>(arg)));
}

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

template <template <typename...> class ExpressionType, std::size_t... Indexes, typename... Args>
constexpr auto
wrap_non_expression_args(std::index_sequence<Indexes...>, Args&&... args)
{
    return make_n_ary_expression<ExpressionType>(
        wrap_arg_t<typename meta::nth_type<Indexes, Args...>::type&&>(
            utils::get_nth_value<Indexes>(std::forward<Args>(args)...))...);
}

}    // namespace detail

//----------------------------------------------------------------------------
template <typename T, char Family, std::size_t N>
struct named_scalar_variable
    : scalar_expression<named_scalar_variable<T, Family, N>, std::decay_t<T>>,
      unary_expression<T> {
    static_assert(is_scalar_v<T>, "Can use scalar variable only with scalar values");
    static_assert(!is_expression_v<T>, "Can use scalar variable only with scalar values");
    using base_type  = scalar_expression<named_scalar_variable<T, Family, N>, std::decay_t<T>>;
    using value_type = typename base_type::value_type;

    using expression_base = unary_expression<T>;
    using expression_base::expression_base;

    constexpr value_type
    value() const
    {
        return this->arg_;
    }

    constexpr auto
    derivative() const
    {
        return one;
    }

    template <char F, std::size_t I>
    constexpr auto
    derivative() const
    {
        if constexpr (F == Family && I == N) {
            return one;
        } else {
            return zero;
        }
    }
};

namespace detail {

template <char F, std::size_t I>
struct named_variable_type {
    template <typename T>
    using type = named_scalar_variable<T, F, I>;
};

}    // namespace detail

template <char F, std::size_t I, typename T>
constexpr auto
make_named_variable(T const& v)
{
    return make_unary_expression<detail::named_variable_type<F, I>::template type>(v);
}

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
struct scalar_add : binary_scalar_expression<scalar_add, LHS, RHS>, binary_expression<LHS, RHS> {
    using base_type       = binary_scalar_expression<scalar_add, LHS, RHS>;
    using value_type      = typename base_type::value_type;
    using expression_base = binary_expression<LHS, RHS>;

    using expression_base::expression_base;

    constexpr value_type
    value() const
    {
        return this->lhs_.value() + this->rhs_.value();
    }

    constexpr auto
    derivative() const
    {
        static_assert(can_differentiate_v<LHS>,
                      "Derivative is not defined for the left hand side of addition");
        static_assert(can_differentiate_v<RHS>,
                      "Derivative is not defined for the right hand side of addition");
        return this->lhs_.derivative() + this->rhs_.derivative();
    }

    template <char F, std::size_t I>
    constexpr auto
    derivative() const
    {
        static_assert(can_partially_differentiate_v<LHS>,
                      "Partial derivative is not defined for the left hand side of addition");
        static_assert(can_partially_differentiate_v<RHS>,
                      "Partial derivative is not defined for the left hand side of addition");
        return this->lhs_.template derivative<F, I>() + this->rhs_.template derivative<F, I>();
    }
};

template <typename LHS, typename RHS, typename = enable_if_scalar_args<LHS, RHS>>
constexpr auto
operator+(LHS&& lhs, RHS&& rhs)
{
    if constexpr (is_zero_v<LHS>) {
        return detail::wrap_arg_t<RHS&&>(std::forward<RHS>(rhs));
    } else if constexpr (is_zero_v<RHS>) {
        return detail::wrap_arg_t<LHS&&>(std::forward<LHS>(lhs));
    } else {
        return detail::wrap_non_expression_args<scalar_add>(std::forward<LHS>(lhs),
                                                            std::forward<RHS>(rhs));
    }
}
//@}

//----------------------------------------------------------------------------
template <typename... T, typename = enable_if_scalar_values<T...>>
constexpr auto
sum(T&&... args);

template <typename... T>
struct scalar_sum : scalar_expression<scalar_sum<T...>, scalar_expression_result_t<T...>>,
                    n_ary_expression<T...> {
    using base_type       = scalar_expression<scalar_sum<T...>, scalar_expression_result_t<T...>>;
    using value_type      = typename base_type::value_type;
    using expression_base = n_ary_expression<T...>;
    using index_sequence_type = std::index_sequence_for<T...>;

    using expression_base::expression_base;

    constexpr value_type
    value() const
    {
        return sum(index_sequence_type{});
    }

    constexpr auto
    derivative() const
    {
        return derivative(index_sequence_type{});
    }

    template <char F, std::size_t I>
    constexpr auto
    derivative() const
    {
        return this->template derivative<F, I>(index_sequence_type{});
    }

private:
    template <std::size_t... Indexes>
    constexpr auto
    sum(std::index_sequence<Indexes...>) const
    {
        return (this->template arg<Indexes>() + ...);
    }

    template <std::size_t... Indexes>
    constexpr auto
    derivative(std::index_sequence<Indexes...>) const
    {
        return s::sum(this->template arg<Indexes>().derivative()...);
    }
    template <char F, std::size_t I, std::size_t... Indexes>
    constexpr auto
    derivative(std::index_sequence<Indexes...>) const
    {
        return s::sum(this->template arg<Indexes>().template derivative<F, I>()...);
    }
};

template <typename T, typename = enable_if_scalar_value<T>>
constexpr auto
sum(T&& arg)
{
    return detail::wrap_arg_t<T&&>(std::forward<T>(arg));
}

template <typename... T, typename>
constexpr auto
sum(T&&... args)
{
    // Filter out zeros
    using remove_zeros = meta::remove_if<expr::is_zero, T...>;
    if constexpr (meta::is_empty_v<typename remove_zeros::type>) {
        return zero;
    } else if constexpr (remove_zeros::type::size == 1) {
        constexpr std::size_t arg_index = utils::nth_index_v<0, typename remove_zeros::indexes>;
        return detail::wrap_arg_t<typename remove_zeros::type::template type<0>&&>(
            utils::get_nth_value<arg_index>(std::forward<T>(args)...));
    } else if constexpr (remove_zeros::type::size != sizeof...(T)) {
        // Forward nth args
        return detail::wrap_non_expression_args<scalar_sum>(typename remove_zeros::indexes{},
                                                            std::forward<T>(args)...);
    } else {
        return detail::wrap_non_expression_args<scalar_sum>(std::forward<T>(args)...);
    }
}

//----------------------------------------------------------------------------
//@{
/** @name Subtract one scalar expression from another */
template <typename LHS, typename RHS>
struct scalar_sub : binary_scalar_expression<scalar_sub, LHS, RHS>, binary_expression<LHS, RHS> {
    using base_type       = binary_scalar_expression<scalar_sub, LHS, RHS>;
    using value_type      = typename base_type::value_type;
    using expression_base = binary_expression<LHS, RHS>;

    using expression_base::expression_base;

    constexpr value_type
    value() const
    {
        return this->lhs_.value() - this->rhs_.value();
    }

    constexpr auto
    derivative() const
    {
        static_assert(can_differentiate_v<LHS>,
                      "Derivative is not defined for the left hand side of subtraction");
        static_assert(can_differentiate_v<RHS>,
                      "Derivative is not defined for the right hand side of subtraction");
        return this->lhs_.derivative() - this->rhs_.derivative();
    }

    template <char F, std::size_t I>
    constexpr auto
    derivative() const
    {
        static_assert(can_partially_differentiate_v<LHS>,
                      "Partial derivative is not defined for the left hand side of subtraction");
        static_assert(can_partially_differentiate_v<RHS>,
                      "Partial derivative is not defined for the left hand side of subtraction");
        return this->lhs_.template derivative<F, I>() - this->rhs_.template derivative<F, I>();
    }
};

template <typename LHS, typename RHS, typename = enable_if_scalar_args<LHS, RHS>>
constexpr auto
operator-(LHS&& lhs, RHS&& rhs)
{
    if constexpr (is_zero_v<LHS>) {
        // TODO negate it
        return -rhs;
    } else if constexpr (is_zero_v<RHS>) {
        return detail::wrap_arg_t<LHS&&>(std::forward<LHS>(lhs));
    } else {
        return detail::wrap_non_expression_args<scalar_sub>(std::forward<LHS>(lhs),
                                                            std::forward<RHS>(rhs));
    }
}
//@}

//----------------------------------------------------------------------------
//@{
/** @name Multiply two scalar expressions */
template <typename LHS, typename RHS>
struct scalar_mul : binary_scalar_expression<scalar_mul, LHS, RHS>, binary_expression<LHS, RHS> {
    using base_type       = binary_scalar_expression<scalar_mul, LHS, RHS>;
    using value_type      = typename base_type::value_type;
    using expression_base = binary_expression<LHS, RHS>;

    using expression_base::expression_base;

    constexpr value_type
    value() const
    {
        return this->lhs_.value() * this->rhs_.value();
    }

    constexpr auto
    derivative() const
    {
        // TODO Check for abs function
        static_assert(can_differentiate_v<LHS>,
                      "Derivative is not defined for the left hand side of multiplication");
        static_assert(can_differentiate_v<RHS>,
                      "Derivative is not defined for the right hand side of multiplication");
        return this->lhs_.derivative() * this->rhs_ + this->lhs_ * this->rhs_.derivative();
    }

    template <char F, std::size_t I>
    constexpr auto
    derivative() const
    {
        // TODO Check for abs function
        static_assert(can_partially_differentiate_v<LHS>,
                      "Partial derivative is not defined for the left hand side of multiplication");
        static_assert(can_partially_differentiate_v<RHS>,
                      "Partial derivative is not defined for the left hand side of multiplication");
        return this->lhs_.template derivative<F, I>() * this->rhs_
               + this->lhs_ * this->rhs_.template derivative<F, I>();
    }
};

template <typename LHS, typename RHS, typename = enable_if_scalar_args<LHS, RHS>>
constexpr auto operator*(LHS&& lhs, RHS&& rhs)
{
    if constexpr (is_zero_v<LHS> || is_zero_v<RHS>) {
        return zero;
    } else if constexpr (is_one_v<LHS>) {
        return detail::wrap_arg_t<RHS&&>(std::forward<RHS>(rhs));
    } else if constexpr (is_one_v<RHS>) {
        return detail::wrap_arg_t<LHS&&>(std::forward<LHS>(lhs));
    } else {
        return detail::wrap_non_expression_args<scalar_mul>(std::forward<LHS>(lhs),
                                                            std::forward<RHS>(rhs));
    }
}
//@}

//----------------------------------------------------------------------------
template <typename... T>
struct scalar_product : scalar_expression<scalar_product<T...>, scalar_expression_result_t<T...>>,
                        n_ary_expression<T...> {
    using base_type  = scalar_expression<scalar_product<T...>, scalar_expression_result_t<T...>>;
    using value_type = typename base_type::value_type;
    using expression_base     = n_ary_expression<T...>;
    using index_sequence_type = std::index_sequence_for<T...>;

    using expression_base::expression_base;

    constexpr value_type
    value() const
    {
        return product(index_sequence_type{});
    }

    constexpr auto
    derivative() const
    {
        return derivative(index_sequence_type{});
    }

    template <char F, std::size_t I>
    constexpr auto
    derivative() const
    {
        return this->template derivative<F, I>(index_sequence_type{});
    }

private:
    template <std::size_t... Indexes>
    constexpr auto
    product(std::index_sequence<Indexes...>) const
    {
        return (this->template arg<Indexes>() * ...);
    }
    template <std::size_t... Indexes>
    constexpr auto
    derivative(std::index_sequence<Indexes...>) const
    {
        return s::derivative((this->template arg<Indexes>() * ...));
    }
    template <char F, std::size_t I, std::size_t... Indexes>
    constexpr auto
    derivative(std::index_sequence<Indexes...>) const
    {
        return s::derivative<F, I>((this->template arg<Indexes>() * ...));
    }
};

template <typename T, typename = enable_if_scalar_value<T>>
constexpr auto
product(T&& arg)
{
    return detail::wrap_arg_t<T&&>(std::forward<T>(arg));
}

template <typename... T, typename = enable_if_scalar_values<T...>>
constexpr auto
product(T&&... args)
{
    using zeros = meta::find_if_t<is_zero, T...>;
    if constexpr (!meta::is_empty_v<zeros>) {
        return zero;
    } else {
        // Filter out ones
        using remove_ones = meta::remove_if<is_one, T...>;
        if constexpr (remove_ones::type::size == 1) {
            constexpr std::size_t arg_index = utils::nth_index_v<0, typename remove_ones::indexes>;
            return detail::wrap_arg_t<typename remove_ones::type::template type<0>&&>(
                utils::get_nth_value<arg_index>(std::forward<T>(args)...));
        } else if constexpr (remove_ones::type::size != sizeof...(T)) {
            // Forward nth args
            return detail::wrap_non_expression_args<scalar_product>(typename remove_ones::indexes{},
                                                                    std::forward<T>(args)...);
        } else {
            return detail::wrap_non_expression_args<scalar_product>(std::forward<T>(args)...);
        }
    }
}

//----------------------------------------------------------------------------
//@{
/** @name Square of a scalar expression */
template <typename Expr>
struct scalar_square : unary_scalar_expression<scalar_square, Expr>, unary_expression<Expr> {
    using base_type  = unary_scalar_expression<scalar_square, Expr>;
    using value_type = typename base_type::value_type;

    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    constexpr auto
    value() const
    {
        return this->arg_ * this->arg_;
    }

    constexpr auto
    derivative() const
    {
        // TODO Check if arg is a scalar constant
        return number<2>{} * this->arg_;
    }

    template <char F, std::size_t I>
    constexpr auto
    derivative() const
    {
        // TODO Check if arg is a named variable or a scalar constant
        return number<2>{} * this->arg_;
    }
};

template <typename T, typename = enable_if_scalar_value<T>>
constexpr auto
square(T&& arg)
{
    return detail::wrap_non_expression_args<scalar_square>(std::forward<T>(arg));
}

//----------------------------------------------------------------------------
//@{
/** @name Divide one scalar expression by another */
template <typename LHS, typename RHS>
struct scalar_div : binary_scalar_expression<scalar_div, LHS, RHS>, binary_expression<LHS, RHS> {
    using base_type       = binary_scalar_expression<scalar_div, LHS, RHS>;
    using value_type      = typename base_type::value_type;
    using expression_base = binary_expression<LHS, RHS>;

    using expression_base::expression_base;

    constexpr value_type
    value() const
    {
        return this->lhs_.value() / this->rhs_.value();
    }

    constexpr auto
    derivative() const
    {
        static_assert(can_differentiate_v<LHS>,
                      "Derivative is not defined for the left hand side of division");
        static_assert(can_differentiate_v<RHS>,
                      "Derivative is not defined for the right hand side of division");
        return (this->lhs_.derivative() * this->rhs_ - this->lhs_ * this->rhs_.derivative())
               / square(this->rhs_);
    }
    template <char F, std::size_t I>
    constexpr auto
    derivative() const
    {
        static_assert(can_partially_differentiate_v<LHS>,
                      "Partial derivative is not defined for the left hand side of division");
        static_assert(can_partially_differentiate_v<RHS>,
                      "Partial derivative is not defined for the right hand side of division");
        return (this->lhs_.template derivative<F, I>() * this->rhs_
                - this->lhs_ * this->rhs_.template derivative<F, I>())
               / square(this->rhs_);
    }
};

template <typename LHS, typename RHS, typename = enable_if_scalar_args<LHS, RHS>>
constexpr auto
operator/(LHS&& lhs, RHS&& rhs)
{
    static_assert(!is_zero_v<RHS>, "Cannot divide by zero");
    if constexpr (is_zero_v<LHS>) {
        return zero;
    } else if constexpr (is_one_v<RHS>) {
        return detail::wrap_arg_t<LHS&&>(std::forward<LHS>(lhs));
    } else {
        return detail::wrap_non_expression_args<scalar_div>(std::forward<LHS>(lhs),
                                                            std::forward<RHS>(rhs));
    }
}
//@}

//----------------------------------------------------------------------------
//@{
template <typename Expression, typename = enable_if_scalar_expression<Expression>>
constexpr auto
sqrt(Expression&& ex);

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

    constexpr auto
    derivative() const
    {
        return one / (number<2>{} * sqrt(this->arg_));
    }

    template <char F, std::size_t I>
    constexpr auto
    derivative() const
    {
        // TODO Check if arg is a named variable
        return one / (number<2>{} * sqrt(this->arg_));
    }

private:
    // TODO Optional
    static constexpr value_type nval         = std::numeric_limits<value_type>::min();
    mutable value_type          value_cache_ = nval;
};

template <typename Expression, typename>
constexpr auto
sqrt(Expression&& ex)
{
    return make_unary_expression<square_root>(std::forward<Expression>(ex));
}
//@}

//@{
template <typename Expression>
struct scalar_abs : unary_scalar_expression<scalar_abs, Expression>, unary_expression<Expression> {
    static_assert(is_scalar_v<Expression>, "Can apply abs only to scalar expressions");
    using base_type       = unary_scalar_expression<scalar_abs, Expression>;
    using value_type      = typename base_type::value_type;
    using expression_base = unary_expression<Expression>;

    using expression_base::expression_base;

    constexpr value_type
    value() const
    {
        using std::abs;
        return abs(this->arg_.value());
    }
};
template <typename Expression, typename = enable_if_scalar_value<Expression>>
constexpr auto
abs(Expression&& ex)
{
    return detail::wrap_non_expression_args<scalar_abs>(std::forward<Expression>(ex));
}
//@}

// TODO Trigonometric and power funcs

}    // namespace s

}    // namespace expr
}    // namespace math
}    // namespace psst

#endif /* PUSHKIN_MATH_DETAIL_SCALAR_EXPRESSIONS_HPP_ */
