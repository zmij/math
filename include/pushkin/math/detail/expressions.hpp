#ifndef PSST_MATH_DETAIL_VECTOR_EXPRESSIONS_HPP_
#define PSST_MATH_DETAIL_VECTOR_EXPRESSIONS_HPP_

#include <pushkin/math/detail/utils.hpp>
#include <pushkin/math/detail/value_traits.hpp>
#include <pushkin/math/vector_fwd.hpp>

#include <cmath>
#include <tuple>
#include <utility>

namespace psst {
namespace math {
namespace expr {

//----------------------------------------------------------------------------
template <typename T, typename = utils::void_t<>>
struct extract_expression_type {
    using type = T;
};

template <typename T>
struct extract_expression_type<T, std::enable_if_t<is_expression_v<std::decay_t<T>>>> {
    using type = typename std::decay_t<T>::expression_type;
};
template <typename T>
using extract_expression_type_t = typename extract_expression_type<T>::type;

template <typename T>
struct expression_parameter {
    using type = std::conditional_t<std::is_rvalue_reference<T>{}, extract_expression_type_t<T>&&,
                                    extract_expression_type_t<T>>;
};
template <typename T>
using expression_parameter_t = typename expression_parameter<T>::type;

template <typename T>
struct expression_argument {
    using type = std::conditional_t<
        std::is_pointer<T>{}, extract_expression_type_t<T>,
        std::conditional_t<std::is_rvalue_reference<T>{}, extract_expression_type_t<T>&&,
                           extract_expression_type_t<T> const&>>;
};
template <typename T>
using expression_argument_t = typename expression_argument<T>::type;

template <typename T>
struct arg_by_value : utils::bool_constant<std::is_rvalue_reference<T>{} || std::is_pointer<T>{}> {
};
template <typename T>
using arg_by_value_t = typename arg_by_value<T>::type;
template <typename T>
constexpr bool arg_by_value_v = arg_by_value_t<T>::value;

template <typename T>
struct expression_argument_storage {
    using type = std::conditional_t<arg_by_value_v<T>, extract_expression_type_t<T>,
                                    extract_expression_type_t<T> const&>;
};
template <typename T>
using expression_argument_storage_t = typename expression_argument_storage<T>::type;

//----------------------------------------------------------------------------
template <typename Expression>
struct unary_expression {
    using arg_type         = expression_argument_t<Expression>;
    using by_value         = arg_by_value_t<Expression>;
    using arg_storage_type = expression_argument_storage_t<Expression>;
    using arg_ref = std::add_lvalue_reference_t<std::add_const_t<std::decay_t<Expression>>>;

    explicit constexpr unary_expression(arg_type arg) : arg_{std::forward<arg_type>(arg)} {}

    constexpr arg_type
    arg() &&
    {
        return arg_;
    }
    constexpr arg_ref
    arg() const&
    {
        return arg_;
    }

protected:
    arg_storage_type arg_;
};

template <template <typename> class ExpressionType, typename Argument>
constexpr auto
make_unary_expression(Argument&& arg)
{
    return ExpressionType<expression_parameter_t<Argument&&>>{
        static_cast<expression_argument_t<Argument&&>>(arg)};
}

template <template <typename, typename> class ExpressionType, typename Result, typename Argument>
constexpr auto
make_unary_expression(Argument&& arg)
{
    return ExpressionType<expression_parameter_t<Argument&&>, Result>{
        static_cast<expression_argument_t<Argument&&>>(arg)};
}

template <template <typename, std::size_t> class ExpressionType, std::size_t N, typename Argument>
constexpr auto
make_unary_expression(Argument&& arg)
{
    return ExpressionType<expression_parameter_t<Argument&&>, N>{
        static_cast<expression_argument_t<Argument&&>>(arg)};
}

template <template <typename, std::size_t, std::size_t> class ExpressionType, std::size_t R,
          std::size_t C, typename Argument>
constexpr auto
make_unary_expression(Argument&& arg)
{
    return ExpressionType<expression_parameter_t<Argument&&>, R, C>{
        static_cast<expression_argument_t<Argument&&>>(arg)};
}

//----------------------------------------------------------------------------
template <typename LHS, typename RHS>
struct binary_expression {
    using lhs_type         = expression_argument_t<LHS>;
    using rhs_type         = expression_argument_t<RHS>;
    using lhs_storage_type = expression_argument_storage_t<LHS>;
    using rhs_storage_type = expression_argument_storage_t<RHS>;
    using lhs_by_value     = arg_by_value_t<LHS>;
    using rhs_by_value     = arg_by_value_t<RHS>;
    using lhs_ref          = std::add_lvalue_reference_t<std::add_const_t<std::decay_t<LHS>>>;
    using rhs_ref          = std::add_lvalue_reference_t<std::add_const_t<std::decay_t<RHS>>>;

    constexpr binary_expression(lhs_type lhs, rhs_type rhs)
        : lhs_{std::forward<lhs_type>(lhs)}, rhs_{std::forward<rhs_type>(rhs)}
    {}

    constexpr lhs_type
    lhs() &&
    {
        return lhs_;
    }
    constexpr lhs_ref
    lhs() const&
    {
        return lhs_;
    }

    constexpr rhs_type
    rhs() &&
    {
        return rhs_;
    }
    constexpr rhs_ref
    rhs() const&
    {
        return rhs_;
    }

protected:
    lhs_storage_type lhs_;
    rhs_storage_type rhs_;
};

template <template <typename, typename> class ExpressionType, typename LHS, typename RHS>
constexpr auto
make_binary_expression(LHS&& lhs, RHS&& rhs)
{
    return ExpressionType<expression_parameter_t<LHS&&>, expression_parameter_t<RHS&&>>{
        static_cast<expression_argument_t<LHS&&>>(lhs),
        static_cast<expression_argument_t<RHS&&>>(rhs)};
}

template <template <typename, typename, typename> class ExpressionType, typename Result,
          typename LHS, typename RHS>
constexpr auto
make_binary_expression(LHS&& lhs, RHS&& rhs)
{
    return ExpressionType<expression_parameter_t<LHS&&>, expression_parameter_t<RHS&&>, Result>{
        static_cast<expression_argument_t<LHS&&>>(lhs),
        static_cast<expression_argument_t<RHS&&>>(rhs)};
}

//----------------------------------------------------------------------------
template <typename... T>
struct n_ary_expression {
    using args_storage_type = std::tuple<expression_argument_storage_t<T>...>;
    using by_value          = std::integer_sequence<bool, arg_by_value_v<T>...>;
    using arg_indexes_type  = std::index_sequence_for<T...>;

    constexpr n_ary_expression(expression_argument_t<T>... args)
        : args_{std::forward<expression_argument_t<T>>(args)...}
    {}

    constexpr args_storage_type&&
    args() &&
    {
        return args_;
    }
    constexpr args_storage_type const&
    args() const&
    {
        return args_;
    }

protected:
    template <std::size_t N>
    constexpr auto
    arg() const
    {
        return std::get<N>(args_);
    }
    args_storage_type args_;
};

template <template <typename...> class ExpressionType, typename... Args>
constexpr auto
make_n_ary_expression(Args&&... args)
{
    return ExpressionType<expression_parameter_t<Args&&>...>{
        static_cast<expression_argument_t<Args&&>>(args)...};
}

//----------------------------------------------------------------------------
template <typename Axes, template <typename, typename> class Expression>
struct select_unary_impl {
    template <typename T>
    using type = Expression<Axes, T>;
};

template <typename Axes, template <typename, typename, typename> class Expression>
struct select_binary_impl {
    template <typename T, typename U>
    using type = Expression<Axes, T, U>;
};

template <template <typename, typename...> class Expression, typename Axes>
struct select_n_ary_impl {
    template <typename... T>
    using type = Expression<Axes, T...>;
};

}    // namespace expr
}    // namespace math
}    // namespace psst

#endif /* PSST_MATH_DETAIL_VECTOR_EXPRESSIONS_HPP_ */
