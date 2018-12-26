#ifndef PSST_MATH_DETAIL_VECTOR_EXPRESSIONS_HPP_
#define PSST_MATH_DETAIL_VECTOR_EXPRESSIONS_HPP_

#include <pushkin/math/vector_fwd.hpp>
#include <pushkin/math/detail/value_traits.hpp>
#include <pushkin/math/detail/utils.hpp>

#include <cmath>
#include <utility>

namespace psst {
namespace math {
namespace expr {

//----------------------------------------------------------------------------
template <typename Expression>
struct unary_expression {
    using arg_type      = Expression const&;
    using by_value      = std::false_type;
    explicit constexpr unary_expression(arg_type arg) : arg_{arg} {}
protected:
    arg_type arg_;
};

template <typename Expression>
struct unary_expression<Expression&&> {
    using arg_type      = Expression;
    using by_value      = std::true_type;
    explicit constexpr unary_expression(arg_type&& arg) : arg_{::std::move(arg)} {}
protected:
    arg_type arg_;
};

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

template <template <typename> class ExpressionType, typename Argument>
constexpr auto
make_unary_expression(Argument&& arg)
{
    using expression_type = extract_expression_type_t< Argument >;
    if constexpr (std::is_rvalue_reference<Argument&&>{}) {
        return ExpressionType<expression_type&&>{static_cast<expression_type&&>(arg)};
    } else {
        return ExpressionType<expression_type>{static_cast<expression_type const&>(arg)};
    }
}

template <template <typename, typename> class UnaryWithResult,
              typename Result, typename Argument>
constexpr auto
make_unary_expression(Argument&& arg)
{
  using expression_type = extract_expression_type_t< Argument >;
  if constexpr (std::is_rvalue_reference<Argument&&>{}) {
      return UnaryWithResult<expression_type&&, Result>{
        static_cast<expression_type&&>(arg)};
  } else {
      return UnaryWithResult<expression_type, Result>{
        static_cast<expression_type const&>(arg)};
  }
}
//----------------------------------------------------------------------------
template <typename LHS, typename RHS>
struct binary_expression {
    using lhs_type      = LHS const&;
    using rhs_type      = RHS const&;

    using lhs_by_value  = std::false_type;
    using rhs_by_value  = std::false_type;

    constexpr binary_expression(lhs_type lhs, rhs_type rhs)
        : lhs_{lhs}, rhs_{rhs} {}
protected:
    lhs_type lhs_;
    rhs_type rhs_;
};

template <typename LHS, typename RHS>
struct binary_expression<LHS&&, RHS> {
    using lhs_type      = LHS;
    using rhs_type      = RHS const&;

    using lhs_by_value  = std::true_type;
    using rhs_by_value  = std::false_type;

    constexpr binary_expression(lhs_type&& lhs, rhs_type rhs)
        : lhs_{std::move(lhs)}, rhs_{rhs} {}
protected:
    lhs_type lhs_;
    rhs_type rhs_;
};

template <typename LHS, typename RHS>
struct binary_expression<LHS, RHS&&> {
    using lhs_type      = LHS const&;
    using rhs_type      = RHS;

    using lhs_by_value  = std::false_type;
    using rhs_by_value  = std::true_type;

    constexpr binary_expression(lhs_type lhs, rhs_type&& rhs)
        : lhs_{lhs}, rhs_{std::move(rhs)} {}
protected:
    lhs_type lhs_;
    rhs_type rhs_;
};

template <typename LHS, typename RHS>
struct binary_expression<LHS&&, RHS&&> {
    using lhs_type      = LHS;
    using rhs_type      = RHS;

    using lhs_by_value  = std::true_type;
    using rhs_by_value  = std::true_type;

    constexpr binary_expression(lhs_type&& lhs, rhs_type&& rhs)
      : lhs_{std::move(lhs)}, rhs_{std::move(rhs)} {}
protected:
    lhs_type lhs_;
    rhs_type rhs_;
};


template <template<typename, typename> class ExpressionType, typename LHS, typename RHS>
constexpr auto
make_binary_expression(LHS&& lhs, RHS&& rhs)
{
    using lhs_expression_type = extract_expression_type_t< ::std::decay_t<LHS> >;
    using rhs_expression_type = extract_expression_type_t< ::std::decay_t<RHS> >;
    if constexpr (std::is_rvalue_reference<LHS&&>{}) {
        if constexpr (std::is_rvalue_reference<RHS&&>{}) {
            return ExpressionType<lhs_expression_type&&, rhs_expression_type&&>{
                  static_cast<lhs_expression_type&&>(lhs),
                  static_cast<rhs_expression_type&&>(rhs)
            };
        } else {
            return ExpressionType<lhs_expression_type&&, rhs_expression_type>{
                  static_cast<lhs_expression_type&&>(lhs),
                  static_cast<rhs_expression_type const&>(rhs)
            };
        }
    } else if constexpr (std::is_rvalue_reference<RHS&&>{}) {
        return ExpressionType<lhs_expression_type, rhs_expression_type&&>{
              static_cast<lhs_expression_type const&>(lhs),
              static_cast<rhs_expression_type&&>(rhs) };
    } else {
        return ExpressionType<lhs_expression_type, rhs_expression_type>{
              static_cast<lhs_expression_type const&>(lhs),
              static_cast<rhs_expression_type const&>(rhs) };
    }
}

template <template<typename, typename, typename> class BinaryWithResult,
                typename Result, typename LHS, typename RHS>
constexpr auto
make_binary_expression(LHS&& lhs, RHS&& rhs)
{
    using lhs_expression_type = extract_expression_type_t< ::std::decay_t<LHS> >;
    using rhs_expression_type = extract_expression_type_t< ::std::decay_t<RHS> >;
    if constexpr (std::is_rvalue_reference<LHS&&>{}) {
        if constexpr (std::is_rvalue_reference<RHS&&>{}) {
            return BinaryWithResult<lhs_expression_type&&, rhs_expression_type&&, Result>{
                  static_cast<lhs_expression_type&&>(lhs),
                  static_cast<rhs_expression_type&&>(rhs)
            };
        } else {
            return BinaryWithResult<lhs_expression_type&&, rhs_expression_type, Result>{
                  static_cast<lhs_expression_type&&>(lhs),
                  static_cast<rhs_expression_type const&>(rhs)
            };
        }
    } else if constexpr (std::is_rvalue_reference<RHS&&>{}) {
        return BinaryWithResult<lhs_expression_type, rhs_expression_type&&, Result>{
              static_cast<lhs_expression_type const&>(lhs),
              static_cast<rhs_expression_type&&>(rhs) };
    } else {
        return BinaryWithResult<lhs_expression_type, rhs_expression_type, Result>{
              static_cast<lhs_expression_type const&>(lhs),
              static_cast<rhs_expression_type const&>(rhs) };
    }
}

//----------------------------------------------------------------------------
template <typename Expression, typename Result>
struct scalar_expression {
    static_assert(!std::is_reference<Result>{},
          "Result type for a scalar expression cannot be a reference");
    using expression_type     = Expression;
    using result_type         = Result;
    using traits              = value_traits_t<result_type>;
    using value_type          = typename traits::type;
    using value_tag           = typename traits::value_tag;

    constexpr value_type
    value() const { return rebind().value(); }
    constexpr operator value_type() const { return this->value(); }

    constexpr operator expression_type const&() const& { return rebind(); }
//    constexpr operator expression_type&&() && { return rebind(); }
private:
    constexpr expression_type const&
    rebind() const& { return static_cast<expression_type const&>(*this); }
    constexpr expression_type&&
    rebind() && { return static_cast<expression_type&&>(*this); }
};

//----------------------------------------------------------------------------
template <typename Expression, typename Result = Expression>
struct vector_expression {
    static_assert(is_vector_v<Result>,
        "Result of vector expression must be a vector");
    using expression_type       = Expression;
    using result_type           = Result;
    using traits                = value_traits_t<result_type>;
    using value_type            = typename traits::value_type;
    using value_tag             = typename traits::value_tag;
    using axes_names            = typename traits::axes_names;
    using index_sequence_type   = typename traits::index_sequence_type;

    static constexpr auto size  = traits::size;

    template < ::std::size_t N >
    constexpr value_type
    at() const
    {
        static_assert(N < size, "Invalid vector expression index");
        return rebind().template at<N>();
    }
    constexpr result_type
    value() const { return result_type(rebind()); }

    constexpr operator expression_type const&() const { return rebind(); }
private:
    constexpr expression_type const&
    rebind() const { return static_cast<expression_type const&>(*this); }
};

template <::std::size_t N, typename Expression, typename Result>
constexpr auto
get(vector_expression<Expression, Result> const& exp)
{
    return exp.template at<N>();
}

template <typename Expression, typename Result>
constexpr ::std::size_t size_of(vector_expression<Expression, Result> const&)
{
    return vector_expression<Expression, Result>::size;
}

namespace detail {

template <typename T>
constexpr ::std::size_t
vector_expression_size()
{
    static_assert(is_vector_expression_v<T>, "The exression is not vector type");
    return T::size;
}

}  // namespace detail

template < typename T >
struct vector_expression_size
    : ::std::integral_constant<::std::size_t, detail::vector_expression_size<T>()> {};
template < typename T >
using vector_exression_size_t = typename vector_expression_size<T>::type;
template < typename T >
constexpr ::std::size_t vector_expression_size_v = vector_exression_size_t<T>::value;

//----------------------------------------------------------------------------
template <typename Expression, typename Result = Expression>
struct matrix_expression {
    static_assert(is_matrix_v<Result>,
        "Result of vector expression must be a vector");
    using expression_type       = Expression;
    using result_type           = Result;
    using traits                = value_traits_t<Result>;
    using value_type            = typename traits::value_type;
    using value_tag             = typename traits::value_tag;
    using axes_names            = typename traits::axes_names;

    static constexpr auto rows  = traits::rows;
    static constexpr auto cols  = traits::cols;
    static constexpr auto size  = traits::size;

    template <::std::size_t R, ::std::size_t C>
    constexpr value_type
    element() const
    {
        static_assert(R < rows, "Invalid matrix expression row index");
        static_assert(C < cols, "Invalid matrix expression col index");
        return rebind().template element<R, C>();
    }
private:
    constexpr expression_type const&
    rebind() const { return static_cast<expression_type const&>(*this); }
};

}  // namespace expr
}  // namespace math
}  // namespace psst

#endif /* PSST_MATH_DETAIL_VECTOR_EXPRESSIONS_HPP_ */
