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

template <typename Expression, typename Result>
struct scalar_expression {
    using expression_type     = Expression;
    using result_type         = Result;
    using traits              = value_traits_t<Result>;
    using value_type          = typename traits::type;
    using value_tag           = typename traits::value_tag;

    constexpr value_type
    value() const { return rebind().value(); }
    constexpr operator value_type() const { return this->value(); }

    constexpr operator expression_type const&() const { return rebind(); }
private:
    constexpr expression_type const&
    rebind() const { return static_cast<expression_type const&>(*this); }
};

template <typename Expression, typename Result = Expression>
struct vector_expression {
    static_assert(is_vector_v<Result>,
        "Result of vector expression must be a vector");
    using expression_type       = Expression;
    using result_type           = Result;
    using traits                = value_traits_t<Result>;
    using value_type            = typename traits::value_type;
    using value_tag             = typename traits::value_tag;
    using axes_names            = typename traits::axes_names;

    static constexpr auto size  = traits::size;

    template < ::std::size_t N >
    constexpr value_type
    at() const
    {
        static_assert(N < size, "Invalid vector expression index");
        return rebind().template at<N>();
    }
    constexpr result_type
    value() const { return result_type(*this); }
    constexpr operator result_type() const { return this->value(); }

    constexpr operator expression_type const&() const { return rebind(); }
private:
    constexpr expression_type const&
    rebind() const { return static_cast<expression_type const&>(*this); }
};

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
