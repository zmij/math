/*
 * calculus.hpp
 *
 *  Created on: Dec 19, 2016
 *      Author: zmij
 */

#ifndef INCLUDE_PUSHKIN_MATH_DETAIL_CALCULUS_HPP_
#define INCLUDE_PUSHKIN_MATH_DETAIL_CALCULUS_HPP_

#include <pushkin/math/detail/vector_detail.hpp>
#include <type_traits>

namespace psst {
namespace math {
namespace detail {

template < typename Axes >
struct is_vector_calculus : ::std::true_type {};

template < typename T, ::std::size_t Size, typename Axes >
struct nonvector_calculus;

template < typename T, ::std::size_t Size, typename Axes >
struct vector_calculus {
    using vector_type           = vector<T, Size, Axes>;
    using value_traits          = vector_value_traits<T>;
    using value_type            = typename value_traits::value_type;
    using lvalue_reference      = typename value_traits::lvalue_reference;
    using const_reference       = typename value_traits::const_reference;

    using magnitude_type        = typename value_traits::magnitude_type;

    vector_type
    operator - ()
    {
        vector_type res{rebind()};
        res *= -1;
        return res;
    }

    template < typename U >
    vector_type&
    operator *= (U val)
    {
        vector_scalar_multiplication< Size - 1, vector_type >{}(rebind(), val);
        return rebind();
    }

    template < typename U >
    vector_type&
    operator /= (U val)
    {
        detail::vector_scalar_division< Size - 1, vector_type >()(rebind(), val);
        return rebind();
    }

    template < typename U >
    vector_type&
    operator += (vector<U, Size, Axes> const& rhs)
    {
        detail::vector_addition< Size - 1, vector_type >()(rebind(), rhs);
        return rebind();
    }

    template < typename U >
    vector_type&
    operator -= (vector<U, Size, Axes> const& rhs)
    {
        detail::vector_substraction< Size - 1, vector_type >()(rebind(), rhs);
        return rebind();
    }

    magnitude_type
    magnitude_square() const
    {
        return detail::dot_product< Size - 1, vector_type >()(rebind(), rebind());
    }

    magnitude_type
    magnitude() const
    {
        return ::std::sqrt(magnitude_square());
    }

    bool
    is_zero() const
    {
        return magnitude_square() == 0;
    }

    bool
    is_unit() const
    {
        return magnitude_square() == 1;
    }

    vector_type&
    zero()
    {
        detail::set_all_elements< Size - 1, vector_type >()(rebind(), 0);
        return rebind();
    }

    vector_type&
    normalize()
    {
        value_type m = magnitude();
        if (m != 0) {
            if (m != 1)
                (rebind()) /= m;
        } else {
            throw ::std::runtime_error("Cannot normalize a zero vector");
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
    rebind() { return static_cast<vector_type&>(*this); }
    vector_type const&
    rebind() const { return static_cast<vector_type const&>(*this); }
};

template < typename T, ::std::size_t Size, typename Axes >
struct calculus_selector
    : ::std::conditional< is_vector_calculus<Axes>::value,
            vector_calculus<T, Size, Axes>,
            nonvector_calculus<T, Size, Axes>>::type {};

}  /* namespace detail */
}  /* namespace math */
}  /* namespace psst */


#endif /* INCLUDE_PUSHKIN_MATH_DETAIL_CALCULUS_HPP_ */
