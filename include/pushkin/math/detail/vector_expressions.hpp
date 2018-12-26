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

inline namespace v {

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
        : binary_scalar_expression<vector_cmp, LHS, RHS, int>,
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
        : binary_scalar_expression<vector_eq, LHS, RHS, bool>,
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
        : binary_scalar_expression<vector_less, LHS, RHS, bool>,
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

template <typename LHS, typename RHS>
struct vector_sum
        : binary_vector_expression<vector_sum, LHS, RHS>,
          binary_expression<LHS, RHS> {
    using base_type           = binary_vector_expression<vector_sum, LHS, RHS>;
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

template <typename LHS, typename RHS>
struct vector_diff
        : binary_vector_expression<vector_diff, LHS, RHS>,
          binary_expression<LHS, RHS> {
    using base_type           = binary_vector_expression<vector_diff, LHS, RHS>;
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

template <typename LHS, typename RHS>
struct vector_scalar_multiply
        : binary_vector_expression<vector_scalar_multiply, LHS, RHS>,
          binary_expression<LHS, RHS> {
    using base_type           = binary_vector_expression<vector_scalar_multiply, LHS, RHS>;
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

template <typename LHS, typename RHS>
struct vector_scalar_divide
        : binary_vector_expression<vector_scalar_divide, LHS, RHS>,
          binary_expression<LHS, RHS> {
    using base_type           = binary_vector_expression<vector_scalar_divide, LHS, RHS>;
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

template < typename LHS >
struct vector_element_sum
        : unary_scalar_expression< vector_element_sum, LHS >,
          unary_expression<LHS> {
    using base_type         = unary_scalar_expression< vector_element_sum, LHS >;
    using value_type        = typename base_type::value_type;

    using expression_base   = unary_expression<LHS>;
    using source_index_type = typename expression_base::arg_type::index_sequence_type;

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

template <typename LHS>
struct vector_magnitude_squared
        : unary_scalar_expression< vector_magnitude_squared, LHS >,
          unary_expression<LHS> {
    static_assert(is_vector_expression_v<LHS>,
        "Argument to magnitude must be a vector");
    using base_type         = unary_scalar_expression< vector_magnitude_squared, LHS >;
    using value_type        = typename base_type::value_type;

    using expression_base   = unary_expression<LHS>;
    using source_index_type = typename std::decay_t<LHS>::index_sequence_type;

    using expression_base::expression_base;

    constexpr value_type
    value() const
    {
        if (value_cache_ == nval) {
            value_cache_ = sum(source_index_type{});
        }
        return value_cache_;
    }
private:
    template <::std::size_t ... Indexes>
    constexpr value_type
    sum(::std::index_sequence<Indexes...>) const
    {
        return ((get<Indexes>(this->arg_) * get<Indexes>(this->arg_)) + ...);
    }
    // TODO Optional
    static constexpr value_type nval = std::numeric_limits<value_type>::min();
    mutable value_type value_cache_ = nval;
};

template < typename LHS, typename RHS >
struct vector_dot_product
        : binary_scalar_expression< vector_dot_product, LHS, RHS >,
          binary_expression<LHS, RHS> {
    static_assert((is_vector_expression_v<LHS> && is_vector_expression_v<RHS>),
          "Both sides to the dot product must be vector expressions");
    static_assert(std::decay_t<LHS>::size == std::decay_t<RHS>::size,
          "Vector expressions must be of the same size");

    using base_type         = binary_scalar_expression< vector_dot_product, LHS, RHS >;
    using value_type        = typename base_type::value_type;

    using expression_base   = binary_expression<LHS, RHS>;
    using source_index_type = typename std::decay_t<LHS>::index_sequence_type;

    using expression_base::expression_base;

    constexpr value_type
    value() const
    {
        if (value_cache_ == nval) {
            value_cache_ = sum(source_index_type{});
        }
        return value_cache_;
    }
private:
    template <::std::size_t ... Indexes>
    constexpr value_type
    sum(::std::index_sequence<Indexes...>) const
    {
        return ((get<Indexes>(this->lhs_) * get<Indexes>(this->rhs_)) + ...);
    }
    // TODO Optional
    static constexpr value_type nval = std::numeric_limits<value_type>::min();
    mutable value_type value_cache_ = nval;
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

template <typename Expr, typename = std::enable_if_t< is_vector_expression_v<Expr> >>
constexpr auto
magnitude_square(Expr&& expr)
{
    // TODO Special handling for non-cartesian coordinate systems
    //using axes_names        = typename Expr::axes_names;
    return make_unary_expression<vector_magnitude_squared>(std::forward<Expr>(expr));
}

template <typename Expr, typename = std::enable_if_t< is_vector_expression_v<Expr> >>
constexpr auto
magnitude(Expr&& expr)
{
    // TODO Special handling for non-cartesian coordinate systems
    //using axes_names        = typename Expr::axes_names;
    return sqrt(magnitude_square(std::forward<Expr>(expr)));
}

template <typename Expr, typename = std::enable_if_t< is_vector_expression_v<Expr> >>
constexpr auto
normalize(Expr&& expr)
{
    // TODO Special handling for non-cartesian coordinate systems
    //using axes_names        = typename Expr::axes_names;
    return expr / magnitude(expr);
}

template <typename LHS, typename RHS,
    typename = ::std::enable_if_t< is_vector_expression_v<LHS> && is_vector_expression_v<RHS> >>
constexpr auto
distance_square(LHS&& lhs, RHS&& rhs)
{
    return magnitude_square(lhs - rhs);
}

template <typename LHS, typename RHS,
    typename = ::std::enable_if_t< is_vector_expression_v<LHS> && is_vector_expression_v<RHS> >>
constexpr auto
distance(LHS&& lhs, RHS&& rhs)
{
    return magnitude(lhs - rhs);
}


template <typename LHS, typename RHS,
    typename = ::std::enable_if_t< is_vector_expression_v<LHS> && is_vector_expression_v<RHS> >>
constexpr auto
dot_product(LHS&& lhs, RHS&& rhs)
{
    return make_binary_expression< vector_dot_product >(std::forward<LHS>(lhs), std::forward<RHS>(rhs));
}

template <typename Start, typename End, typename U,
    typename = ::std::enable_if_t<
        is_vector_expression_v<Start> &&
        is_vector_expression_v<End> &&
        is_scalar_v<U>>>
constexpr auto
lerp(Start&& start, End&& end, U&& percent)
{
    return start + (end - start) * percent;
}

template <typename Start, typename End, typename U,
    typename = ::std::enable_if_t<
        is_vector_expression_v<Start> &&
        is_vector_expression_v<End> &&
        is_scalar_v<U>>>
vector_expression_result_t<Start, End>
slerp(Start&& start, End&& end, U&& percent)
{
    using value_traits = typename std::decay_t<Start>::traits::value_traits;
    using ::std::sin;
    using ::std::cos;
    using ::std::acos;

    auto s_mag = magnitude(start);
    auto s_n = start / s_mag; // normalized

    auto e_mag = magnitude(end);
    auto e_n = end / e_mag; // normalized
    // Lerp magnitude
    auto res_mag = s_mag + (e_mag - s_mag) * percent;

    auto dot = dot_product(s_n, e_n);
    if (value_traits::eq(dot, 0)) {
        // Perpendicular vectors
        auto theta = acos(dot) * percent;
        auto res = s_n * cos(theta) + e_n * sin(theta);
        return res * res_mag;
    } else if (value_traits::eq(dot, 1)) {
        // Collinear vectors same direction
        return lerp(start, end, percent);
    } else if (value_traits::eq(dot, -1)) {
        // Collinear vectors opposite direction
        // TODO make a vector pointing to the direction of longer vector with magnitude of 1/2 of magnitude sum
        throw ::std::runtime_error("Slerp for opposite vectors is undefined");
    } else {
        // Generic formula
        auto omega = acos(dot);
        auto sin_o = sin(omega);
        auto res = (s_n * sin((1-percent) * omega) +
                e_n * sin(percent * omega)) / sin_o;
        return res * res_mag;
    }
}

}  // namespace v

}  // namespace expr
}  // namespace math
}  // namespace psst


#endif /* PUSHKIN_MATH_DETAIL_VECTOR_EXPRESSIONS_HPP_ */

