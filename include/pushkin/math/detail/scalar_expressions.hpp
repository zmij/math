/*
 * scalar_epressions.hpp
 *
 *  Created on: Dec 25, 2018
 *      Author: ser-fedorov
 */

#ifndef PUSHKIN_MATH_DETAIL_SCALAR_EXPRESSIONS_HPP_
#define PUSHKIN_MATH_DETAIL_SCALAR_EXPRESSIONS_HPP_

#include <pushkin/math/detail/expressions.hpp>

namespace psst {
namespace math {
namespace expr {

template < typename Expression >
struct not_ : scalar_expression< not_<Expression>, bool > {
    static_assert(is_scalar_v<Expression>, "Can apply not_ only to scalar expressions");
    not_(Expression const& ex) : expr_{ex} {}

    constexpr bool
    value() const
    { return !expr_.value(); }
private:
    Expression const& expr_;
};

template < typename Expression >
constexpr auto
operator ! (scalar_expression<Expression, bool> const& ex)
{
    return not_<Expression>{ex};
}

}  // namespace expr
}  // namespace math
}  // namespace psst

#endif /* PUSHKIN_MATH_DETAIL_SCALAR_EXPRESSIONS_HPP_ */
