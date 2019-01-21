/*
 * vector_expressions.hpp
 *
 *  Created on: Dec 25, 2018
 *      Author: ser-fedorov
 */

#ifndef PSST_MATH_DETAIL_VECTOR_EXPRESSIONS_HPP_
#define PSST_MATH_DETAIL_VECTOR_EXPRESSIONS_HPP_

#include <psst/math/detail/component_access.hpp>
#include <psst/math/detail/expressions.hpp>
#include <psst/math/detail/scalar_expressions.hpp>

#include <stdexcept>

namespace psst {
namespace math {
namespace expr {

inline namespace v {

//----------------------------------------------------------------------------
template <typename Expression, typename Result = Expression>
struct vector_expression
    : math::detail::component_names_t<traits::value_traits_t<Result>::size,
                                 typename traits::value_traits_t<Result>::component_names, Expression,
                                 typename traits::value_traits_t<Result>::value_type> {
    static_assert(traits::is_vector_v<Result>, "Result of vector expression must be a vector");
    using expression_type     = Expression;
    using result_type         = Result;
    using traits              = traits::value_traits_t<result_type>;
    using value_type          = typename traits::value_type;
    using value_tag           = typename traits::value_tag;
    using component_names          = typename traits::component_names;
    using index_sequence_type = typename traits::index_sequence_type;
    using component_access = math::detail::component_names_t<traits::size, typename traits::component_names,
                                                   Expression, typename traits::value_type>;

    static constexpr auto size = traits::size;
    static_assert(
        size <= component_names::max_components,
        "The number of components in vector expression is greater than allowed by components names");
    static_assert(
        size >= component_names::min_components,
        "The number of components in vector expression is less than allowed by components names");
};

template <template <typename> class Expression, typename Arg,
          typename Result = traits::vector_expression_result_t<Arg>>
using unary_vector_expression = vector_expression<Expression<Arg>, Result>;
template <template <typename, typename> class Expression, typename Components, typename Arg,
          typename Result = traits::vector_expression_result_t<Arg>>
using unary_vector_expression_components = vector_expression<Expression<Components, Arg>, Result>;

template <template <typename, typename> class Expression, typename LHS, typename RHS,
          typename Result = traits::vector_expression_result_t<LHS, RHS>>
using binary_vector_expression = vector_expression<Expression<LHS, RHS>, Result>;
template <template <typename, typename, typename> class Expression, typename Components, typename LHS,
          typename RHS, typename Result = traits::vector_expression_result_t<LHS, RHS>>
using binary_vector_expression_components = vector_expression<Expression<Components, LHS, RHS>, Result>;

template <std::size_t N, typename Expression,
          typename = traits::enable_if_vector_expression<Expression>>
constexpr auto
get(Expression const& exp)
{
    using expression_type = extract_expression_type_t<Expression>;
    return static_cast<expression_type const&>(exp).template at<N>();
}

template <typename Vector>
struct vector_fill
    : vector_expression<vector_fill<Vector>, typename std::decay_t<Vector>::result_type> {
    static_assert(traits::is_vector_expression_v<Vector>,
                  "Template argument must be a vector expression");
    using base_type
        = vector_expression<vector_fill<Vector>, typename std::decay_t<Vector>::result_type>;
    using value_type = typename base_type::value_type;

    vector_fill(value_type v) : arg_{v} {}

    template <std::size_t N>
    constexpr value_type
    at() const
    {
        return arg_;
    }

private:
    value_type arg_;
};

//----------------------------------------------------------------------------
//@{
/** @name Compare vector expressions */
namespace detail {

template <std::size_t N, typename LHS, typename RHS>
struct vector_expression_cmp;

template <typename LHS, typename RHS>
struct vector_expression_cmp<0, LHS, RHS> {
    static_assert((traits::is_vector_expression_v<LHS> && traits::is_vector_expression_v<RHS>),
                  "Both sides to the comparison must be vector expressions");
    using lhs_type    = std::decay_t<LHS>;
    using rhs_type    = std::decay_t<RHS>;
    using traits_type = traits::value_traits_t<typename lhs_type::value_type>;

    constexpr static int
    cmp(lhs_type const& lhs, rhs_type const& rhs)
    {
        return traits_type::cmp(lhs.template at<0>(), rhs.template at<0>());
    }
};

template <std::size_t N, typename LHS, typename RHS>
struct vector_expression_cmp {
    static_assert((traits::is_vector_expression_v<LHS> && traits::is_vector_expression_v<RHS>),
                  "Both sides to the comparison must be vector expressions");
    using prev_element = vector_expression_cmp<N - 1, LHS, RHS>;
    using lhs_type     = typename prev_element::lhs_type;
    using rhs_type     = typename prev_element::rhs_type;
    using traits_type  = typename prev_element::traits_type;
    constexpr static int
    cmp(lhs_type const& lhs, rhs_type const& rhs)
    {
        auto res = prev_element::cmp(lhs, rhs);
        if (res == 0) {
            return traits_type::cmp(lhs.template at<N>(), rhs.template at<N>());
        }
        return res;
    }
};

}    // namespace detail

template <typename LHS, typename RHS>
struct vector_cmp : binary_scalar_expression<vector_cmp, LHS, RHS, int>,
                    binary_expression<LHS, RHS> {

    static_assert((traits::is_vector_expression_v<LHS> && traits::is_vector_expression_v<RHS>),
                  "Both sides to the comparison must be vector expressions");
    static constexpr std::size_t cmp_size = utils::min_v<traits::vector_expression_size_v<LHS>,
                                                         traits::vector_expression_size_v<RHS>>;
    using cmp_type                        = detail::vector_expression_cmp<cmp_size - 1, LHS, RHS>;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    constexpr int
    value() const
    {
        return cmp_type::cmp(this->lhs_, this->rhs_);
    }
};
template <typename LHS, typename RHS, typename = traits::enable_if_vector_expressions<LHS, RHS>>
constexpr int
cmp(LHS&& lhs, RHS&& rhs)
{
    return make_binary_expression<vector_cmp>(std::forward<LHS>(lhs), std::forward<RHS>(rhs));
}
//@}

//----------------------------------------------------------------------------
//@{
/** @name Check two vector expressions equality */
template <typename LHS, typename RHS>
struct vector_eq : binary_scalar_expression<vector_eq, LHS, RHS, bool>,
                   binary_expression<LHS, RHS> {
    static_assert((traits::is_vector_expression_v<LHS> && traits::is_vector_expression_v<RHS>),
                  "Both sides to the comparison must be vector expressions");
    static constexpr std::size_t cmp_size = utils::min_v<traits::vector_expression_size_v<LHS>,
                                                         traits::vector_expression_size_v<RHS>>;
    using cmp_type                        = detail::vector_expression_cmp<cmp_size - 1, LHS, RHS>;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    constexpr bool
    value() const
    {
        return cmp_type::cmp(this->lhs_, this->rhs_) == 0;
    }
};

template <typename LHS, typename RHS, typename = traits::enable_if_vector_expressions<LHS, RHS>>
constexpr auto
operator==(LHS&& lhs, RHS&& rhs)
{
    static_assert((traits::compatible_components_v<LHS, RHS>),
                  "Components on the sides of the expression must match or be none");
    return make_binary_expression<vector_eq>(std::forward<LHS>(lhs), std::forward<RHS>(rhs));
}

template <typename LHS, typename RHS, typename = traits::enable_if_vector_expressions<LHS, RHS>>
constexpr auto
operator!=(LHS&& lhs, RHS&& rhs)
{
    static_assert((traits::compatible_components_v<LHS, RHS>),
                  "Components on the sides of the expression must match or be none");
    return !(std::forward<LHS>(lhs) == std::forward<RHS>(rhs));
}
//@}

//----------------------------------------------------------------------------
//@{
/** @name Vector comparison */
template <typename LHS, typename RHS>
struct vector_less : binary_scalar_expression<vector_less, LHS, RHS, bool>,
                     binary_expression<LHS, RHS> {
    static_assert((traits::is_vector_expression_v<LHS> && traits::is_vector_expression_v<RHS>),
                  "Both sides to the comparison must be vector expressions");
    static constexpr std::size_t cmp_size = utils::min_v<traits::vector_expression_size_v<LHS>,
                                                         traits::vector_expression_size_v<RHS>>;
    using cmp_type                        = detail::vector_expression_cmp<cmp_size - 1, LHS, RHS>;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    constexpr bool
    value() const
    {
        return cmp_type::cmp(this->lhs_, this->rhs_) < 0;
    }
};

template <typename LHS, typename RHS, typename = traits::enable_if_vector_expressions<LHS, RHS>>
constexpr auto
operator<(LHS&& lhs, RHS&& rhs)
{
    static_assert((traits::compatible_components_v<LHS, RHS>),
                  "Components on the sides of the expression must match or be none");
    return make_binary_expression<vector_less>(std::forward<LHS>(lhs), std::forward<RHS>(rhs));
}

template <typename LHS, typename RHS, typename = traits::enable_if_vector_expressions<LHS, RHS>>
constexpr auto
operator<=(LHS&& lhs, RHS&& rhs)
{
    static_assert((traits::compatible_components_v<LHS, RHS>),
                  "Components on the sides of the expression must match or be none");
    return !(std::forward<RHS>(rhs) < std::forward<LHS>(lhs));
}

template <typename LHS, typename RHS, typename = traits::enable_if_vector_expressions<LHS, RHS>>
constexpr auto
operator>(LHS&& lhs, RHS&& rhs)
{
    static_assert((traits::compatible_components_v<LHS, RHS>),
                  "Components on the sides of the expression must match or be none");
    return make_binary_expression<vector_less>(std::forward<RHS>(rhs), std::forward<LHS>(lhs));
}

template <typename LHS, typename RHS, typename = traits::enable_if_vector_expressions<LHS, RHS>>
constexpr auto
operator>=(LHS&& lhs, RHS&& rhs)
{
    static_assert((traits::compatible_components_v<LHS, RHS>),
                  "Components on the sides of the expression must match or be none");
    return !(std::forward<LHS>(lhs) < std::forward<RHS>(rhs));
}
//@}

//----------------------------------------------------------------------------
//@{
/** @name Sum of two vectors */
template <typename LHS, typename RHS>
struct vector_sum : binary_vector_expression<vector_sum, LHS, RHS>, binary_expression<LHS, RHS> {
    using base_type  = binary_vector_expression<vector_sum, LHS, RHS>;
    using value_type = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr value_type
    at() const
    {
        static_assert(N < base_type::size, "Vector sum component index is out of range");
        return this->lhs_.template at<N>() + this->rhs_.template at<N>();
    }
};

template <typename LHS, typename RHS, typename = traits::enable_if_vector_expressions<LHS, RHS>,
          typename = traits::enable_for_compatible_components<LHS, RHS>,
          typename = traits::disable_for_components<LHS, components::polar, components::spherical, components::cylindrical>,
          typename = traits::disable_for_components<RHS, components::polar, components::spherical, components::cylindrical>>
constexpr auto
operator+(LHS&& lhs, RHS&& rhs)
{
    return make_binary_expression<vector_sum>(std::forward<LHS>(lhs), std::forward<RHS>(rhs));
}
//@}

//----------------------------------------------------------------------------
//@{
/** @name Substraction of two vectors */
template <typename LHS, typename RHS>
struct vector_diff : binary_vector_expression<vector_diff, LHS, RHS>, binary_expression<LHS, RHS> {
    using base_type  = binary_vector_expression<vector_diff, LHS, RHS>;
    using value_type = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr value_type
    at() const
    {
        static_assert(N < base_type::size, "Vector difference component index is out of range");
        return this->lhs_.template at<N>() - this->rhs_.template at<N>();
    }
};

template <typename LHS, typename RHS, typename = traits::enable_if_vector_expressions<LHS, RHS>,
          typename = traits::enable_for_compatible_components<LHS, RHS>,
          typename = traits::disable_for_components<LHS, components::polar, components::spherical, components::cylindrical>,
          typename = traits::disable_for_components<RHS, components::polar, components::spherical, components::cylindrical>>
constexpr auto
operator-(LHS&& lhs, RHS&& rhs)
{
    return make_binary_expression<vector_diff>(std::forward<LHS>(lhs), std::forward<RHS>(rhs));
}
//@}

//----------------------------------------------------------------------------
//@{
/** @name Multiplication of a vector by scalar expression */
template <typename Components, typename LHS, typename RHS>
struct vector_scalar_multiply
    : binary_vector_expression_components<vector_scalar_multiply, Components, LHS, RHS>,
      binary_expression<LHS, RHS> {
    using base_type  = binary_vector_expression_components<vector_scalar_multiply, Components, LHS, RHS>;
    using value_type = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr value_type
    at() const
    {
        static_assert(N < base_type::size, "Vector multiply component index is out of range");
        return this->lhs_.template at<N>() * this->rhs_;
    }
};
//@}

//@{
/** @name Vector product */
template <typename Components, typename LHS, typename RHS>
struct vector_vector_multiply;

template <typename LHS, typename RHS>
struct vector_vector_multiply<components::xyzw, LHS, RHS>
    : binary_vector_expression_components<vector_vector_multiply, components::xyzw, LHS, RHS>,
      binary_expression<LHS, RHS> {
    using base_type = binary_vector_expression_components<vector_vector_multiply, components::xyzw, LHS, RHS>;
    static_assert(
        base_type::size == 3,
        "Vector cross product is defined only for 3 and 7 dimensions, implemented only for 3");
    using value_type      = typename base_type::value_type;
    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr value_type
    at() const
    {
        static_assert(N < base_type::size, "Vector multiply component index is out of range");
        if constexpr (N == components::xyzw::x) {
            return this->lhs_.template at<1>() * this->rhs_.template at<2>()
                   - this->lhs_.template at<2>() * this->rhs_.template at<1>();
        } else if constexpr (N == components::xyzw::y) {
            return this->lhs_.template at<2>() * this->rhs_.template at<0>()
                   - this->lhs_.template at<0>() * this->rhs_.template at<2>();
        } else if constexpr (N == components::xyzw::z) {
            return this->lhs_.template at<0>() * this->rhs_.template at<1>()
                   - this->lhs_.template at<1>() * this->rhs_.template at<0>();
        }
    }
};
//@}

//@{
/** @name Multiply operator */
template <
    typename LHS, typename RHS,
    typename = std::enable_if_t<
        (traits::is_vector_expression_v<LHS> && traits::is_scalar_v<RHS>)
        || (traits::is_scalar_v<LHS> && traits::is_vector_expression_v<RHS>)
        || (traits::is_vector_expression_v<
                LHS> && traits::is_vector_expression_v<RHS> && traits::same_components_v<LHS, RHS>)>>
constexpr auto operator*(LHS&& lhs, RHS&& rhs)
{
    if constexpr (traits::is_vector_expression_v<LHS> && traits::is_scalar_v<RHS>) {
        using component_names = traits::component_names_t<LHS>;
        return s::detail::wrap_non_expression_args<
            select_binary_impl<component_names, vector_scalar_multiply>::template type>(
            std::forward<LHS>(lhs), std::forward<RHS>(rhs));
    } else if constexpr (traits::is_scalar_v<LHS> && traits::is_vector_expression_v<RHS>) {
        using component_names = traits::component_names_t<RHS>;
        return s::detail::wrap_non_expression_args<
            select_binary_impl<component_names, vector_scalar_multiply>::template type>(
            std::forward<RHS>(rhs), std::forward<LHS>(lhs));
    } else if constexpr (
        traits::is_vector_expression_v<
            LHS> && traits::is_vector_expression_v<RHS> && traits::same_components_v<LHS, RHS>) {
        using component_names = traits::component_names_t<RHS>;
        static_assert((utils::is_decl_complete_v<vector_vector_multiply<component_names, LHS, RHS>>),
                      "Vector multiplication is not defined for this components");
        return make_binary_expression<
            select_binary_impl<component_names, vector_vector_multiply>::template type>(
            std::forward<LHS>(lhs), std::forward<RHS>(rhs));
    }
}
//@}

//----------------------------------------------------------------------------
//@{
/** @name Division of a vector by a scalar expression */
template <typename Components, typename LHS, typename RHS>
struct vector_scalar_divide : binary_vector_expression_components<vector_scalar_divide, Components, LHS, RHS>,
                              binary_expression<LHS, RHS> {
    using base_type  = binary_vector_expression_components<vector_scalar_divide, Components, LHS, RHS>;
    using value_type = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr value_type
    at() const
    {
        static_assert(N < base_type::size, "Vector divide component index is out of range");
        return this->lhs_.template at<N>() / this->rhs_;
    }
};

template <typename LHS, typename RHS,
          typename
          = std::enable_if_t<traits::is_vector_expression_v<LHS> && traits::is_scalar_v<RHS>>>
constexpr auto
operator/(LHS&& lhs, RHS&& rhs)
{
    using component_names = traits::component_names_t<LHS>;
    return s::detail::wrap_non_expression_args<
        select_binary_impl<component_names, vector_scalar_divide>::template type>(
        std::forward<LHS>(lhs), std::forward<RHS>(rhs));
}
//@}

//----------------------------------------------------------------------------
//@{
template <typename LHS>
struct vector_element_sum : unary_scalar_expression<vector_element_sum, LHS>,
                            unary_expression<LHS> {
    using base_type  = unary_scalar_expression<vector_element_sum, LHS>;
    using value_type = typename base_type::value_type;

    using expression_base   = unary_expression<LHS>;
    using source_index_type = typename expression_base::arg_type::index_sequence_type;

    using expression_base::expression_base;

    constexpr value_type
    value() const
    {
        return sum(source_index_type{});
    }

private:
    template <std::size_t... Indexes>
    constexpr value_type
    sum(std::index_sequence<Indexes...>) const
    {
        return s::sum(get<Indexes>(this->arg_)...);
    }
};
//@}

//----------------------------------------------------------------------------
//@{
/** @name Magnitude (squared and not) */
template <typename Components, typename Vector>
struct vector_magnitude_squared : scalar_expression<vector_magnitude_squared<Components, Vector>,
                                                    traits::scalar_expression_result_t<Vector>>,
                                  unary_expression<Vector> {
    static_assert(traits::is_vector_expression_v<Vector>, "Argument to magnitude must be a vector");
    using base_type  = scalar_expression<vector_magnitude_squared<Components, Vector>,
                                        traits::scalar_expression_result_t<Vector>>;
    using value_type = typename base_type::value_type;

    using expression_base   = unary_expression<Vector>;
    using source_index_type = typename std::decay_t<Vector>::index_sequence_type;

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
    template <std::size_t... Indexes>
    constexpr value_type
    sum(std::index_sequence<Indexes...>) const
    {
        return s::detail::unchecked_scalar_sum(
            (get<Indexes>(this->arg_) * get<Indexes>(this->arg_))...);
    }
    // TODO Optional
    static constexpr value_type nval         = std::numeric_limits<value_type>::min();
    mutable value_type          value_cache_ = nval;
};

template <typename Expr, typename = traits::enable_if_vector_expression<Expr>>
constexpr auto
magnitude_square(Expr&& expr)
{
    // TODO Special handling for non-cartesian coordinate systems
    using component_names = traits::component_names_t<Expr>;
    return make_unary_expression<
        select_unary_impl<component_names, vector_magnitude_squared>::template type>(
        std::forward<Expr>(expr));
}

template <typename Components, typename Expr>
struct vector_magnitude;

template <typename Expr, typename = traits::enable_if_vector_expression<Expr>>
constexpr auto
magnitude(Expr&& expr)
{
    using component_names = traits::component_names_t<Expr>;
    if constexpr (utils::is_decl_complete_v<vector_magnitude<component_names, Expr>>) {
        return make_unary_expression<
            select_unary_impl<component_names, vector_magnitude>::template type>(
            std::forward<Expr>(expr));
    } else {
        return sqrt(magnitude_square(std::forward<Expr>(expr)));
    }
}

template <typename LHS, typename RHS, typename = traits::enable_if_vector_expressions<LHS, RHS>>
constexpr auto
distance_square(LHS&& lhs, RHS&& rhs)
{
    return magnitude_square(lhs - rhs);
}

template <typename LHS, typename RHS, typename = traits::enable_if_vector_expressions<LHS, RHS>>
constexpr auto
distance(LHS&& lhs, RHS&& rhs)
{
    return magnitude(lhs - rhs);
}
//@}

//@{
template <typename Components, typename Expr>
struct vector_normalize;

template <typename Expr, typename = traits::enable_if_vector_expression<Expr>>
constexpr auto
normalize(Expr&& expr)
{
    // TODO Special handling for non-cartesian coordinate systems
    using component_names = traits::component_names_t<Expr>;
    if constexpr (utils::is_decl_complete_v<vector_normalize<component_names, Expr>>) {
        return make_unary_expression<
            select_unary_impl<component_names, vector_normalize>::template type>(
            std::forward<Expr>(expr));
    } else {
        return expr / magnitude(expr);
    }
}
//@}

//----------------------------------------------------------------------------
/** @name Apply a predicate to each vector component */
template <typename Expr, typename Predicate>
struct vector_apply
    : vector_expression<vector_apply<Expr, Predicate>, traits::vector_expression_result_t<Expr>>,
      binary_expression<Expr, Predicate> {
    using base_type       = vector_expression<vector_apply<Expr, Predicate>,
                                        traits::vector_expression_result_t<Expr>>;
    using value_type      = typename base_type::value_type;
    using expression_base = binary_expression<Expr, Predicate>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr value_type
    at() const
    {
        return this->rhs_(this->lhs_.template at<N>());
    }
};

template <typename Expr, typename Predicate, typename = traits::enable_if_vector_expression<Expr>>
constexpr auto
apply(Expr&& expr, Predicate&& pred)
{
    return make_binary_expression<vector_apply>(std::forward<Expr>(expr),
                                                std::forward<Predicate>(pred));
}
//----------------------------------------------------------------------------
//@{
/** @name Dot product of two vectors */
template <typename LHS, typename RHS>
struct vector_dot_product : binary_scalar_expression<vector_dot_product, LHS, RHS>,
                            binary_expression<LHS, RHS> {
    static_assert((traits::is_vector_expression_v<LHS> && traits::is_vector_expression_v<RHS>),
                  "Both sides to the dot product must be vector expressions");
    // TODO Replace with trait
    static_assert(std::decay_t<LHS>::size == std::decay_t<RHS>::size,
                  "Vector expressions must be of the same size");

    using base_type  = binary_scalar_expression<vector_dot_product, LHS, RHS>;
    using value_type = typename base_type::value_type;

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
    template <std::size_t... Indexes>
    constexpr value_type
    sum(std::index_sequence<Indexes...>) const
    {
        return s::detail::unchecked_scalar_sum(
            (get<Indexes>(this->lhs_) * get<Indexes>(this->rhs_))...);
    }
    // TODO Optional
    static constexpr value_type nval         = std::numeric_limits<value_type>::min();
    mutable value_type          value_cache_ = nval;
};

template <typename LHS, typename RHS, typename = traits::enable_if_vector_expressions<LHS, RHS>,
          typename = traits::enable_for_compatible_components<LHS, RHS>,
          typename = traits::disable_for_components<LHS, components::polar, components::spherical, components::cylindrical>,
          typename = traits::disable_for_components<RHS, components::polar, components::spherical, components::cylindrical>>
constexpr auto
dot_product(LHS&& lhs, RHS&& rhs)
{
    return make_binary_expression<vector_dot_product>(std::forward<LHS>(lhs),
                                                      std::forward<RHS>(rhs));
}
//@}

//----------------------------------------------------------------------------
// TODO Move to a separate header
template <typename Start, typename End, typename U,
          typename = std::enable_if_t<
              traits::is_vector_expression_v<
                  Start> && traits::is_vector_expression_v<End> && traits::is_scalar_v<U>>>
constexpr auto
lerp(Start&& start, End&& end, U&& percent)
{
    return start + (end - start) * percent;
}

//----------------------------------------------------------------------------
// TODO Move to a separate header
template <typename Start, typename End, typename U,
          typename = std::enable_if_t<
              traits::is_vector_expression_v<
                  Start> && traits::is_vector_expression_v<End> && traits::is_scalar_v<U>>>
traits::vector_expression_result_t<Start, End>
slerp(Start&& start, End&& end, U&& percent)
{
    using value_traits = typename std::decay_t<Start>::traits::value_traits;
    using std::acos;
    using std::cos;
    using std::sin;

    auto s_mag = magnitude(start);
    auto s_n   = start / s_mag;    // normalized

    auto e_mag = magnitude(end);
    auto e_n   = end / e_mag;    // normalized
    // Lerp magnitude
    auto res_mag = s_mag + (e_mag - s_mag) * percent;

    auto dot = dot_product(s_n, e_n);
    if (value_traits::eq(dot, 0)) {
        // Perpendicular vectors
        auto theta = acos(dot) * percent;
        auto res   = s_n * cos(theta) + e_n * sin(theta);
        return res * res_mag;
    } else if (value_traits::eq(dot, 1)) {
        // Collinear vectors same direction
        return lerp(start, end, percent);
    } else if (value_traits::eq(dot, -1)) {
        // Collinear vectors opposite direction
        // TODO make a vector pointing to the direction of longer vector with magnitude of 1/2 of
        // magnitude sum
        throw std::runtime_error("Slerp for opposite vectors is undefined");
    } else {
        // Generic formula
        auto omega = acos(dot);
        auto sin_o = sin(omega);
        auto res
            = (s_n * sin((1 - percent) * omega) + e_n * sin(percent * omega)) / sin_o * res_mag;
        return res;
    }
}

}    // namespace v

}    // namespace expr
}    // namespace math
}    // namespace psst

#endif /* PSST_MATH_DETAIL_VECTOR_EXPRESSIONS_HPP_ */
