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

template <template <typename, std::size_t> class ExpressionType,
              std::size_t N, typename Argument>
constexpr auto
make_unary_expression(Argument&& arg)
{
  using expression_type = extract_expression_type_t< Argument >;
  if constexpr (std::is_rvalue_reference<Argument&&>{}) {
      return ExpressionType<expression_type&&, N>{
        static_cast<expression_type&&>(arg)};
  } else {
      return ExpressionType<expression_type, N>{
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

}  // namespace expr
}  // namespace math
}  // namespace psst

#endif /* PSST_MATH_DETAIL_VECTOR_EXPRESSIONS_HPP_ */
