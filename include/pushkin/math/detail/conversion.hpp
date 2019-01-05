/*
 * conversion.hpp
 *
 *  Created on: 20 дек. 2016 г.
 *      Author: sergey.fedorov
 */

#ifndef PUSHKIN_MATH_DETAIL_CONVERSION_HPP_
#define PUSHKIN_MATH_DETAIL_CONVERSION_HPP_

#include <pushkin/math/detail/vector_expressions.hpp>

namespace psst {
namespace math {

namespace expr {
inline namespace v {

template <typename Source, typename Target, typename Expression = Source>
struct conversion;

//@{
/** @name conversion_exists */
template <typename Source, typename Target>
struct conversion_exists
    : utils::is_decl_complete_t<
          conversion<vector_expression_result_t<std::decay_t<Source>>, Target>> {};
template <typename Source, typename Target>
using conversion_exists_t = typename conversion_exists<Source, Target>::type;
template <typename Source, typename Target>
constexpr bool conversion_exists_v = conversion_exists_t<Source, Target>::value;
//@}

// Conversion within same axes is always defined
template <typename RHS, std::size_t RSize, typename LHS, std::size_t LSize, typename Axes,
          typename Expression>
struct conversion<vector<RHS, RSize, Axes>, vector<LHS, LSize, Axes>, Expression>
    : vector_expression<conversion<vector<RHS, RSize, Axes>, vector<LHS, LSize, Axes>, Expression>>,
      unary_expression<Expression> {
    using base_type = vector_expression<
        conversion<vector<RHS, RSize, Axes>, vector<LHS, LSize, Axes>, Expression>>;
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr auto
    at() const
    {
        static_assert(N < base_type::size, "Invalid component index");
        return this->arg_.template at<N>();
    }
};

// Conversion to axes::none is always defined
template <typename RHS, std::size_t RSize, typename LHS, std::size_t LSize, typename Axes,
          typename Expression>
struct conversion<vector<RHS, RSize, Axes>, vector<LHS, LSize, axes::none>, Expression>
    : vector_expression<
          conversion<vector<RHS, RSize, Axes>, vector<LHS, LSize, axes::none>, Expression>,
          vector<LHS, LSize, axes::none>>,
      unary_expression<Expression> {
    using base_type = vector_expression<
        conversion<vector<RHS, RSize, Axes>, vector<LHS, LSize, axes::none>, Expression>,
        vector<LHS, LSize, axes::none>>;
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr auto
    at() const
    {
        static_assert(N < base_type::size, "Invalid component index");
        return this->arg_.template at<N>();
    }
};

// Conversion from axes::none is always defined
template <typename RHS, std::size_t RSize, typename LHS, std::size_t LSize, typename Axes,
          typename Expression>
struct conversion<vector<RHS, RSize, axes::none>, vector<LHS, LSize, Axes>, Expression>
    : vector_expression<
          conversion<vector<RHS, RSize, axes::none>, vector<LHS, LSize, Axes>, Expression>,
          vector<LHS, LSize, Axes>>,
      unary_expression<Expression> {
    using base_type = vector_expression<
        conversion<vector<RHS, RSize, axes::none>, vector<LHS, LSize, Axes>, Expression>,
        vector<LHS, LSize, Axes>>;
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr auto
    at() const
    {
        static_assert(N < base_type::size, "Invalid component index");
        return this->arg_.template at<N>();
    }
};

template <typename Source, typename Target>
struct bind_conversion_args {
    template <typename Expression>
    using type = conversion<Source, Target, Expression>;
};

}    // namespace v
}    // namespace expr

template <typename Target, typename Expression>
constexpr auto
convert(Expression&& expr)
{
    static_assert(is_vector_expression_v<Expression>,
                  "Source expression must be a vector expression");
    static_assert(is_vector_v<Target>, "Conversion target must be a vector type");
    static_assert((expr::conversion_exists_v<Expression, Target>),
                  "Conversion between theses axes is not defined");
    if constexpr (same_axes_v<Expression, Target>) {
        return std::forward<Expression>(expr);
    } else {
        using source_vector_type = vector_expression_result_t<Expression>;
        return expr::make_unary_expression<
            expr::bind_conversion_args<source_vector_type, Target>::template type>(
            std::forward<Expression>(expr));
    }
}

} /* namespace math */
} /* namespace psst */

#endif /* PUSHKIN_MATH_DETAIL_CONVERSION_HPP_ */
