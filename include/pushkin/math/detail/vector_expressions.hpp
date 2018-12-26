/*
 * vector_expressions.hpp
 *
 *  Created on: Dec 25, 2018
 *      Author: ser-fedorov
 */

#ifndef PUSHKIN_MATH_DETAIL_VECTOR_EXPRESSIONS_HPP_
#define PUSHKIN_MATH_DETAIL_VECTOR_EXPRESSIONS_HPP_

#include <pushkin/math/detail/expressions.hpp>
#include <pushkin/math/detail/scalar_expressions.hpp>

namespace psst {
namespace math {
namespace expr {

namespace detail {

template < ::std::size_t N, typename LHS, typename RHS>
struct vector_expression_cmp;

template < typename LHS, typename RHS >
struct vector_expression_cmp<0, LHS, RHS> {
    static_assert((is_vector_expression_v<LHS> && is_vector_expression_v<RHS>),
          "Both sides to the comparison must be vector expressions");
    using traits_type = value_traits_t< typename LHS::value_type >;

    constexpr static int cmp(LHS const& lhs, RHS const& rhs)
    {
        return traits_type::cmp(lhs.template at<0>(), rhs.template at<0>());
    }
};

template < ::std::size_t N, typename LHS, typename RHS >
struct vector_expression_cmp {
    static_assert((is_vector_expression_v<LHS> && is_vector_expression_v<RHS>),
          "Both sides to the comparison must be vector expressions");
    using prev_element = vector_expression_cmp< N - 1, LHS, RHS >;
    using traits_type  = typename prev_element::traits_type;
    constexpr static int cmp(LHS const& lhs, RHS const& rhs)
    {
        auto res = prev_element::cmp(lhs, rhs);
        if (res == 0) {
            return traits_type::cmp(lhs.template at<N>(), rhs.template at<N>());
        }
        return res;
    }
};

}  // namespace detail

template <typename LHS, typename RHS>
struct vector_cmp
        : scalar_expression<vector_cmp<LHS, RHS>, int>,
          binary_expression<LHS, RHS> {

    static_assert((is_vector_expression_v<LHS> && is_vector_expression_v<RHS>),
          "Both sides to the comparison must be vector expressions");
    static constexpr ::std::size_t cmp_size = utils::min_v< vector_expression_size_v<LHS>, vector_expression_size_v<RHS> >;
    using cmp_type = detail::vector_expression_cmp<cmp_size - 1, LHS, RHS>;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    constexpr int
    value() const {
        return cmp_type::cmp(this->lhs_, this->rhs_);
    }
};

template <typename LHS, typename RHS>
struct vector_eq
        : scalar_expression<vector_eq<LHS, RHS>, bool>,
          binary_expression<LHS, RHS> {
    static_assert((is_vector_expression_v<LHS> && is_vector_expression_v<RHS>),
          "Both sides to the comparison must be vector expressions");
    static constexpr ::std::size_t cmp_size = utils::min_v< vector_expression_size_v<LHS>, vector_expression_size_v<RHS> >;
    using cmp_type = detail::vector_expression_cmp<cmp_size - 1, LHS, RHS>;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    constexpr bool
    value() const {
        return cmp_type::cmp(this->lhs_, this->rhs_) == 0;
    }
};


template <typename LHS, typename RHS>
struct vector_less
        : scalar_expression<vector_less<LHS, RHS>, bool>,
          binary_expression<LHS, RHS> {
    static_assert((is_vector_expression_v<LHS> && is_vector_expression_v<RHS>),
          "Both sides to the comparison must be vector expressions");
    static constexpr ::std::size_t cmp_size = utils::min_v< vector_expression_size_v<LHS>, vector_expression_size_v<RHS> >;
    using cmp_type = detail::vector_expression_cmp<cmp_size - 1, LHS, RHS>;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    constexpr bool
    value() const {
        return cmp_type::cmp(this->lhs_, this->rhs_) < 1;
    }
};

template <typename LHS, typename RHS, typename Result>
struct vector_sum
        : vector_expression< vector_sum<LHS, RHS, Result>, Result>,
          binary_expression<LHS, RHS> {
    using base_type           = vector_expression< vector_sum<LHS, RHS, Result>, Result>;
    using value_type          = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template < ::std::size_t N >
    constexpr value_type
    at() const
    {
        static_assert(N < base_type::size, "Vector sum element index is out of range");
        return this->lhs_.template at<N>() + this->rhs_.template at<N>();
    }
};

template <typename LHS, typename RHS, typename Result>
struct vector_diff
        : vector_expression< vector_diff<LHS, RHS, Result>, Result>,
          binary_expression<LHS, RHS> {
    using base_type           = vector_expression< vector_diff<LHS, RHS, Result>, Result>;
    using value_type          = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template < ::std::size_t N >
    constexpr value_type
    at() const
    {
        static_assert(N < base_type::size, "Vector difference element index is out of range");
        return this->lhs_.template at<N>() - this->rhs_.template at<N>();
    }
};

template <typename LHS, typename RHS, typename Result>
struct vector_scalar_multiply
        : vector_expression< vector_scalar_multiply<LHS, RHS, Result>, Result >,
          binary_expression<LHS, RHS> {
    using base_type           = vector_expression< vector_scalar_multiply<LHS, RHS, Result>, Result >;
    using value_type          = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template < ::std::size_t N >
    constexpr value_type
    at() const
    {
        static_assert(N < base_type::size, "Vector multiply element index is out of range");
        return this->lhs_.template at<N>() * this->rhs_;
    }
};

template <typename LHS, typename RHS, typename Result>
struct vector_scalar_divide
        : vector_expression< vector_scalar_divide<LHS, RHS, Result>, Result >,
          binary_expression<LHS, RHS> {
    using base_type           = vector_expression< vector_scalar_divide<LHS, RHS, Result>, Result >;
    using value_type          = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template < ::std::size_t N >
    constexpr value_type
    at() const
    {
        static_assert(N < base_type::size, "Vector divide element index is out of range");
        return this->lhs_.template at<N>() / this->rhs_;
    }
};

template < typename LHS, typename Result >
struct vector_element_sum
        : scalar_expression< vector_element_sum< LHS, Result >, Result >,
          unary_expression<LHS> {
    using base_type         = scalar_expression< vector_element_sum< LHS, Result >, Result >;
    using value_type          = typename base_type::value_type;

    using expression_base = unary_expression<LHS>;
    using source_index_type   = typename expression_base::arg_type::index_sequence_type;

    using expression_base::expression_base;

    constexpr value_type
    value() const
    {
        return sum(source_index_type{});
    }
private:
    template <::std::size_t ... Indexes>
    constexpr value_type
    sum(::std::index_sequence<Indexes...>) const
    {
        return (get<Indexes>(this->arg_) + ...);
    }
};

template <typename LHS, typename Result>
struct vector_magnitude_squared
        : scalar_expression< vector_magnitude_squared<LHS, Result>, Result >,
          unary_expression<LHS> {
    using base_type         = scalar_expression< vector_magnitude_squared<LHS, Result>, Result >;
    using value_type          = typename base_type::value_type;

    using expression_base = unary_expression<LHS>;
    using source_index_type   = typename expression_base::arg_type::index_sequence_type;

    using expression_base::expression_base;

    constexpr value_type
    value() const
    {
        return sum(source_index_type{});
    }
private:
    template <::std::size_t ... Indexes>
    constexpr value_type
    sum(::std::index_sequence<Indexes...>) const
    {
        return ((get<Indexes>(this->arg_) * get<Indexes>(this->arg_)) + ...);
    }
};


template <typename LHS, typename RHS,
    typename = std::enable_if_t<is_vector_expression_v<LHS> && is_vector_expression_v<RHS>>>
constexpr auto
operator == (LHS&& lhs, RHS&& rhs)
{
    return make_binary_expression<vector_eq>(std::forward<LHS>(lhs), std::forward<RHS>(rhs));
}

template <typename LHS, typename RHS,
    typename = std::enable_if_t<is_vector_expression_v<LHS> && is_vector_expression_v<RHS>>>
constexpr auto
operator != (LHS&& lhs, RHS&& rhs)
{
    return !(std::forward<LHS>(lhs) == std::forward<RHS>(rhs));
}

template <typename LHS, typename RHS,
    typename = std::enable_if_t<is_vector_expression_v<LHS> && is_vector_expression_v<RHS>>>
constexpr auto
operator < (LHS&& lhs, RHS&& rhs)
{
    return make_binary_expression<vector_less>(std::forward<LHS>(lhs), std::forward<RHS>(rhs));
}

template <typename LHS, typename RHS,
    typename = std::enable_if_t<is_vector_expression_v<LHS> && is_vector_expression_v<RHS>>>
constexpr auto
operator <= (LHS&& lhs, RHS&& rhs)
{
    return !(std::forward<RHS>(rhs) < std::forward<LHS>(lhs));
}

template <typename LHS, typename RHS,
    typename = std::enable_if_t<is_vector_expression_v<LHS> && is_vector_expression_v<RHS>>>
constexpr auto
operator > (LHS&& lhs, RHS&& rhs)
{
  return make_binary_expression<vector_less>(std::forward<RHS>(rhs), std::forward<LHS>(lhs));
}

template <typename LHS, typename RHS,
    typename = std::enable_if_t<is_vector_expression_v<LHS> && is_vector_expression_v<RHS>>>
constexpr auto
operator >= (LHS&& lhs, RHS&& rhs)
{
    return !(std::forward<LHS>(lhs) < std::forward<RHS>(rhs));
}

template <typename Expr, typename T>
constexpr auto
magnitude_sq(vector_expression<Expr, T> const& expr)
{
    using expr_type         = vector_expression<Expr, T>;
    // TODO Special handling for non-cartesian coordinate systems
    //using axes_names        = typename expr_type::axes_names;
    using result_type       = typename expr_type::value_type;
    return vector_magnitude_squared<Expr, result_type>{expr};
}

template <typename Expr, typename T>
constexpr auto
magnitude(vector_expression<Expr, T> const& expr)
{
    using expr_type         = vector_expression<Expr, T>;
    // TODO Special handling for non-cartesian coordinate systems
    //using axes_names        = typename expr_type::axes_names;
    using result_type       = typename expr_type::value_type;
    return sqrt(vector_magnitude_squared<Expr, result_type>{expr});
}

}  // namespace expr
}  // namespace math
}  // namespace psst


#endif /* PUSHKIN_MATH_DETAIL_VECTOR_EXPRESSIONS_HPP_ */

