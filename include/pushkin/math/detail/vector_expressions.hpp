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
struct vector_cmp : scalar_expression<vector_cmp<LHS, RHS>, int> {
    static_assert((is_vector_expression_v<LHS> && is_vector_expression_v<RHS>),
          "Both sides to the comparison must be vector expressions");
    static constexpr ::std::size_t cmp_size = utils::min_v< vector_expression_size_v<LHS>, vector_expression_size_v<RHS> >;
    using cmp_type = detail::vector_expression_cmp<cmp_size - 1, LHS, RHS>;

    vector_cmp(LHS const& lhs, RHS const& rhs)
      : lhs_{lhs}, rhs_{rhs} {}
    constexpr int
    value() const {
        return cmp_type::cmp(lhs_, rhs_);
    }
private:
    LHS const& lhs_;
    RHS const& rhs_;
};

template <typename LHS, typename RHS>
struct vector_eq : scalar_expression<vector_eq<LHS, RHS>, bool> {
    static_assert((is_vector_expression_v<LHS> && is_vector_expression_v<RHS>),
          "Both sides to the comparison must be vector expressions");
    static constexpr ::std::size_t cmp_size = utils::min_v< vector_expression_size_v<LHS>, vector_expression_size_v<RHS> >;
    using cmp_type = detail::vector_expression_cmp<cmp_size - 1, LHS, RHS>;

    vector_eq(LHS const& lhs, RHS const& rhs)
      : lhs_{lhs}, rhs_{rhs} {}
    constexpr bool
    value() const {
        return cmp_type::cmp(lhs_, rhs_) == 0;
    }
private:
    LHS const& lhs_;
    RHS const& rhs_;
};


template <typename LHS, typename RHS>
struct vector_less : scalar_expression<vector_less<LHS, RHS>, bool> {
    static_assert((is_vector_expression_v<LHS> && is_vector_expression_v<RHS>),
          "Both sides to the comparison must be vector expressions");
    static constexpr ::std::size_t cmp_size = utils::min_v< vector_expression_size_v<LHS>, vector_expression_size_v<RHS> >;
    using cmp_type = detail::vector_expression_cmp<cmp_size - 1, LHS, RHS>;

    vector_less(LHS const& lhs, RHS const& rhs)
      : lhs_{lhs}, rhs_{rhs} {}
    constexpr bool
    value() const {
        return cmp_type::cmp(lhs_, rhs_) < 1;
    }
private:
    LHS const& lhs_;
    RHS const& rhs_;
};

template <typename LHS, typename RHS, typename Result>
struct vector_sum
      : vector_expression< vector_sum<LHS, RHS, Result>, Result> {
    using base_type           = vector_expression< vector_sum<LHS, RHS, Result>, Result>;
    using value_type          = typename base_type::value_type;
    using left_arg_type       = LHS;
    using right_arg_type      = RHS;

    constexpr vector_sum(left_arg_type const& lhs,
        right_arg_type const& rhs)
      : lhs_{&lhs}, rhs_{&rhs} {}

    template < ::std::size_t N >
    constexpr value_type
    at() const { return lhs_->template at<N>() + rhs_->template at<N>(); }
private:
    left_arg_type const* lhs_;
    right_arg_type const* rhs_;
};

template <typename LHS, typename RHS, typename Result>
struct vector_diff
      : vector_expression< vector_diff<LHS, RHS, Result>, Result> {
    using base_type           = vector_expression< vector_diff<LHS, RHS, Result>, Result>;
    using value_type          = typename base_type::value_type;
    using left_arg_type       = LHS;
    using right_arg_type      = RHS;

    constexpr vector_diff(left_arg_type const& lhs,
        right_arg_type const& rhs)
      : lhs_{&lhs}, rhs_{&rhs} {}

    template < ::std::size_t N >
    constexpr value_type
    at() const { return lhs_->template at<N>() - rhs_->template at<N>(); }
private:
    left_arg_type const* lhs_;
    right_arg_type const* rhs_;
};

template <typename LHS, typename RHS, typename Result>
struct vector_scalar_multiply : vector_expression< vector_scalar_multiply<LHS, RHS, Result>, Result > {
    using base_type           = vector_expression< vector_scalar_multiply<LHS, RHS, Result>, Result >;
    using value_type          = typename base_type::value_type;
    using expr_arg_type       = LHS;
    using scalar_arg_type     = RHS;

    static_assert(!is_expression_v<scalar_arg_type>,
          "Use vector_scalar_exp_multiply for scalar expressions");

    constexpr vector_scalar_multiply(expr_arg_type const& lhs, scalar_arg_type rhs)
        : lhs_{&lhs}, rhs_{rhs} {}

    template < ::std::size_t N >
    constexpr value_type
    at() const
    {
        return lhs_->template at<N>() * rhs_;
    }
private:
    expr_arg_type const* lhs_;
    scalar_arg_type rhs_;
};

template <typename LHS, typename RHS, typename Result>
struct vector_scalar_exp_multiply : vector_expression< vector_scalar_exp_multiply<LHS, RHS, Result>, Result > {
    using base_type           = vector_expression< vector_scalar_exp_multiply<LHS, RHS, Result>, Result >;
    using value_type          = typename base_type::value_type;
    using expr_arg_type       = LHS;
    using scalar_arg_type     = RHS;

    static_assert(is_expression_v<scalar_arg_type>,
          "Use vector_scalar_multiply for scalar values");

    constexpr vector_scalar_exp_multiply(expr_arg_type const& lhs, scalar_arg_type const& rhs)
        : lhs_{lhs}, rhs_{rhs} {}

    template < ::std::size_t N >
    constexpr value_type
    at() const
    {
        return lhs_.template at<N>() * rhs_.value();
    }
private:
    expr_arg_type const& lhs_;
    scalar_arg_type const& rhs_;
};


template <typename LHS, typename RHS, typename Result>
struct vector_scalar_divide : vector_expression< vector_scalar_divide<LHS, RHS, Result>, Result > {
    using base_type           = vector_expression< vector_scalar_divide<LHS, RHS, Result>, Result >;
    using value_type          = typename base_type::value_type;
    using expr_arg_type       = LHS;
    using scalar_arg_type     = RHS;

    static_assert(!is_expression_v<scalar_arg_type>,
          "Use vector_scalar_exp_divide for scalar expressions");

    constexpr vector_scalar_divide(expr_arg_type const& lhs, scalar_arg_type rhs)
        : lhs_{&lhs}, rhs_{rhs} {}

    template < ::std::size_t N >
    constexpr value_type
    at() const
    {
        return lhs_->template at<N>() / rhs_;
    }
private:
    expr_arg_type const* lhs_;
    scalar_arg_type rhs_;
};

template <typename LHS, typename RHS, typename Result>
struct vector_scalar_exp_divide : vector_expression< vector_scalar_exp_divide<LHS, RHS, Result>, Result > {
    using base_type           = vector_expression< vector_scalar_exp_divide<LHS, RHS, Result>, Result >;
    using value_type          = typename base_type::value_type;
    using expr_arg_type       = LHS;
    using scalar_arg_type     = RHS;

    static_assert(is_expression_v<scalar_arg_type>,
          "Use vector_scalar_exp_divide for scalar expressions");

    constexpr vector_scalar_exp_divide(expr_arg_type const& lhs, scalar_arg_type rhs)
        : lhs_{lhs}, rhs_{rhs} {}

    template < ::std::size_t N >
    constexpr value_type
    at() const
    {
        return lhs_.template at<N>() / rhs_.value();
    }
private:
    expr_arg_type const& lhs_;
    scalar_arg_type const& rhs_;
};


template <typename LHS, typename RHS, typename T, typename U>
constexpr auto
operator == (vector_expression<LHS, T> const& lhs,
             vector_expression<RHS, U> const& rhs)
{
    return vector_eq<LHS, RHS>{lhs, rhs};
}

template <typename LHS, typename RHS, typename T, typename U>
constexpr auto
operator != (vector_expression<LHS, T> const& lhs,
             vector_expression<RHS, U> const& rhs)
{
    return !vector_eq<LHS, RHS>{lhs, rhs};
}

template <typename LHS, typename RHS, typename T, typename U>
constexpr auto
operator < (vector_expression<LHS, T> const& lhs,
             vector_expression<RHS, U> const& rhs)
{
    return vector_less<LHS, RHS>{lhs, rhs};
}

template <typename LHS, typename RHS, typename T, typename U>
constexpr auto
operator <= (vector_expression<LHS, T> const& lhs,
             vector_expression<RHS, U> const& rhs)
{
    return !vector_less<RHS, LHS>{rhs, lhs};
}

template <typename LHS, typename RHS, typename T, typename U>
constexpr auto
operator > (vector_expression<LHS, T> const& lhs,
             vector_expression<RHS, U> const& rhs)
{
    return vector_less<RHS, LHS>{rhs, lhs};
}

template <typename LHS, typename RHS, typename T, typename U>
constexpr auto
operator >= (vector_expression<LHS, T> const& lhs,
             vector_expression<RHS, U> const& rhs)
{
    return !vector_less<LHS, RHS>{lhs, rhs};
}


template <typename LHS, typename RHS, typename T, typename U>
constexpr auto
operator + (vector_expression<LHS, T> const& lhs,
            vector_expression<RHS, U> const& rhs)
{
    using lhs_type          = vector_expression<LHS, T>;
    using rhs_type          = vector_expression<RHS, U>;
    using result_type       = vector_expression_result_t<lhs_type, rhs_type>;
    return vector_sum< LHS, RHS, result_type >{lhs, rhs};
}
template <typename LHS, typename RHS, typename T, typename U>
constexpr auto
operator - (vector_expression<LHS, T> const& lhs,
            vector_expression<RHS, U> const& rhs)
{
    using lhs_type          = vector_expression<LHS, T>;
    using rhs_type          = vector_expression<RHS, U>;
    using result_type       = vector_expression_result_t<lhs_type, rhs_type>;
    return vector_diff< LHS, RHS, result_type >{lhs, rhs};
}

template <typename Expr, typename T, typename U>
constexpr auto
operator * (vector_expression<Expr, T> const& expr, U scalar)
{
    using expr_type         = vector_expression<Expr, T>;
    using result_type       = vector_expression_result_t<expr_type, U>;
    return vector_scalar_multiply< Expr, U, result_type>{expr, scalar};
}

template <typename Expr, typename T, typename U>
constexpr auto
operator * (U scalar, vector_expression<Expr, T> const& expr)
{
    return expr * scalar;
}

template <typename RHS, typename LHS, typename T, typename U>
constexpr auto
operator * (vector_expression<LHS, T> const& lhs,
            scalar_expression<RHS, U> const& rhs)
{
    using lhs_type          = vector_expression<LHS, T>;
    using rhs_type          = scalar_expression<RHS, U>;
    using result_type       = vector_expression_result_t<lhs_type, rhs_type>;
    return vector_scalar_exp_multiply< LHS, RHS, result_type>{lhs, rhs};
}

template <typename RHS, typename LHS, typename T, typename U>
constexpr auto
operator * (scalar_expression<LHS, T> const& lhs,
            vector_expression<RHS, U> const& rhs)
{
    return rhs * lhs;
}


template <typename Expr, typename T, typename U>
constexpr auto
operator / (vector_expression<Expr, T> const& expr, U scalar)
{
    using expr_type         = vector_expression<Expr, T>;
    using result_type       = vector_expression_result_t<expr_type, U>;
    return vector_scalar_divide< Expr, U, result_type>{expr, scalar};
}

template <typename RHS, typename LHS, typename T, typename U>
constexpr auto
operator / (vector_expression<LHS, T> const& lhs,
            scalar_expression<RHS, U> const& rhs)
{
    using lhs_type          = vector_expression<LHS, T>;
    using rhs_type          = scalar_expression<RHS, U>;
    using result_type       = vector_expression_result_t<lhs_type, rhs_type>;
    return vector_scalar_exp_divide< LHS, RHS, result_type>{lhs, rhs};
}

}  // namespace expr
}  // namespace math
}  // namespace psst


#endif /* PUSHKIN_MATH_DETAIL_VECTOR_EXPRESSIONS_HPP_ */
