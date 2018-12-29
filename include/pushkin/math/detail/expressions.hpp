#ifndef PSST_MATH_DETAIL_VECTOR_EXPRESSIONS_HPP_
#define PSST_MATH_DETAIL_VECTOR_EXPRESSIONS_HPP_

#include <pushkin/math/detail/utils.hpp>
#include <pushkin/math/detail/value_traits.hpp>
#include <pushkin/math/vector_fwd.hpp>

#include <cmath>
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
    using type = std::conditional_t<std::is_rvalue_reference<T>{}, extract_expression_type_t<T>&&,
                                    extract_expression_type_t<T> const&>;
};
template <typename T>
using expression_argument_t = typename expression_argument<T>::type;

template <typename T>
struct expression_argument_storage {
    using type = std::conditional_t<std::is_rvalue_reference<T>{}, extract_expression_type_t<T>,
                                    extract_expression_type_t<T> const&>;
};
template <typename T>
using expression_argument_storage_t = typename expression_argument_storage<T>::type;

template <typename T>
struct arg_by_value : std::integral_constant<bool, std::is_rvalue_reference<T>{}> {};
template <typename T>
using arg_by_value_t = typename arg_by_value<T>::type;

//----------------------------------------------------------------------------
template <typename Expression>
struct unary_expression {
    using arg_type         = expression_argument_t<Expression>;
    using by_value         = arg_by_value_t<Expression>;
    using arg_storage_type = expression_argument_storage_t<Expression>;

    explicit constexpr unary_expression(arg_type arg) : arg_{std::forward<arg_type>(arg)} {}

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
//----------------------------------------------------------------------------
template <typename LHS, typename RHS>
struct binary_expression {
    using lhs_type         = expression_argument_t<LHS>;
    using rhs_type         = expression_argument_t<RHS>;
    using lhs_storage_type = expression_argument_storage_t<LHS>;
    using rhs_storage_type = expression_argument_storage_t<RHS>;
    using lhs_by_value     = arg_by_value_t<LHS>;
    using rhs_by_value     = arg_by_value_t<RHS>;

    constexpr binary_expression(lhs_type lhs, rhs_type rhs)
        : lhs_{std::forward<lhs_type>(lhs)}, rhs_{std::forward<rhs_type>(rhs)}
    {}

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

}    // namespace expr
}    // namespace math
}    // namespace psst

#endif /* PSST_MATH_DETAIL_VECTOR_EXPRESSIONS_HPP_ */
