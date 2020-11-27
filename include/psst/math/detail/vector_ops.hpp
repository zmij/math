/*
 * calculus.hpp
 *
 *  Created on: Dec 19, 2016
 *      Author: zmij
 */

#ifndef PSST_MATH_DETAIL_VECTOR_OPS_HPP_
#define PSST_MATH_DETAIL_VECTOR_OPS_HPP_

#include <psst/math/angles.hpp>
#include <psst/math/detail/vector_expressions.hpp>

namespace psst {
namespace math {
namespace detail {

template <typename T, std::size_t Size, typename Components>
struct vector_ops {
    using vector_type      = vector<T, Size, Components>;
    using value_traits     = traits::scalar_value_traits<T>;
    using value_type       = typename value_traits::value_type;
    using lvalue_reference = typename value_traits::lvalue_reference;
    using const_reference  = typename value_traits::const_reference;

    using magnitude_type = typename value_traits::magnitude_type;

    vector_type
    operator-() const
    {
        vector_type res{rebind()};
        res *= -1;
        return res;
    }

    template <typename U, typename = traits::enable_if_addition_defined<vector_type, U>>
    vector_type&
    operator+=(U&& rhs)
    {
        rebind() = rebind() + std::forward<U>(rhs);
        return rebind();
    }

    template <typename U, typename = traits::enable_if_difference_defined<vector_type, U>>
    vector_type&
    operator-=(U&& rhs)
    {
        rebind() = rebind() - std::forward<U>(rhs);
        return rebind();
    }

    // TODO Check for compatibility of result
    template <typename U, typename = traits::enable_if_multiplication_defined<vector_type, U>>
    vector_type&
    operator*=(U&& rhs)
    {
        rebind() = rebind() * std::forward<U>(rhs);
        return rebind();
    }

    template <typename U, typename = traits::enable_if_division_defined<vector_type, U>>
    vector_type&
    operator/=(U&& rhs)
    {
        rebind() = rebind() / std::forward<U>(rhs);
        return rebind();
    }

    magnitude_type
    magnitude_square() const
    {
        return expr::magnitude_square(rebind());
    }

    magnitude_type
    magnitude() const
    {
        return expr::magnitude(rebind());
    }

    template <typename U, typename = traits::enable_if_dot_product_defined<vector_type, U>>
    constexpr auto
    dot(U&& rhs) const
    {
        return dot_product(rebind(), std::forward<U>(rhs));
    }

    bool
    is_zero() const
    {
        return value_traits::eq(magnitude_square(), 0);
    }

    bool
    is_unit() const
    {
        return value_traits::eq(magnitude_square(), 1);
    }

    vector_type&
    zero()
    {
        return rebind() = expr::vector_fill<vector_type>(0);
    }

    vector_type&
    normalize()
    {
        value_type m = magnitude();
        if (m != 0) {
            if (m != 1) {
                rebind() /= m;
            }
        } else {
            throw std::runtime_error("Cannot normalize a zero vector");
        }
        return rebind();
    }

    vector_type
    normalize() const
    {
        vector_type v(rebind());
        v.normalize();
        return v;
    }

private:
    vector_type&
    rebind()
    {
        return static_cast<vector_type&>(*this);
    }
    vector_type const&
    rebind() const
    {
        return static_cast<vector_type const&>(*this);
    }
};

} /* namespace detail */
} /* namespace math */
} /* namespace psst */

#endif /* PSST_MATH_DETAIL_CALCULUS_HPP_ */
