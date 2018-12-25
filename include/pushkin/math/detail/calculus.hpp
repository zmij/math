/*
 * calculus.hpp
 *
 *  Created on: Dec 19, 2016
 *      Author: zmij
 */

#ifndef INCLUDE_PUSHKIN_MATH_DETAIL_CALCULUS_HPP_
#define INCLUDE_PUSHKIN_MATH_DETAIL_CALCULUS_HPP_

#include <pushkin/math/detail/vector_detail.hpp>
#include <pushkin/math/pi.hpp>
#include <type_traits>

namespace psst {
namespace math {
namespace detail {

template < typename Axes >
struct is_vector_calculus : ::std::true_type {};

template <>
struct is_vector_calculus<axes::polar> : ::std::false_type{};
template <>
struct is_vector_calculus<axes::spherical> : ::std::false_type{};
template <>
struct is_vector_calculus<axes::cylindrical> : ::std::false_type{};

template < typename T, ::std::size_t Size, typename Axes >
struct nonvector_calculus;

template < typename T, ::std::size_t Size, typename Axes >
struct vector_calculus {
    using vector_type           = vector<T, Size, Axes>;
    using value_traits          = scalar_value_traits<T>;
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
        rebind() = rebind() * val;
        return rebind();
    }

    template < typename U >
    vector_type&
    operator /= (U val)
    {
        rebind() = rebind() / val;
        return rebind();
    }

    template < typename U >
    vector_type&
    operator += (vector<U, Size, Axes> const& rhs)
    {
        rebind() = rebind() + rhs;
        return rebind();
    }

    template < typename U >
    vector_type&
    operator -= (vector<U, Size, Axes> const& rhs)
    {
        rebind() = rebind() - rhs;
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

/**
 * Polar coordinates calculus
 */
template < typename T >
struct nonvector_calculus<T, 2, axes::polar> {
    using vector_type           = vector<T, 2, axes::polar>;
    using value_traits          = scalar_value_traits<T>;
    using value_type            = typename value_traits::value_type;
    using lvalue_reference      = typename value_traits::lvalue_reference;
    using const_reference       = typename value_traits::const_reference;
    using magnitude_type        = typename value_traits::magnitude_type;

    using pi                    = math::pi<T>;

    vector_type
    operator - ()
    {
        vector_type res{rebind()};
        res.negate();
        return res;
    }

    magnitude_type
    magnitude_square() const
    {
        return rebind().rho() * rebind().rho();
    }
    magnitude_type
    magnitude() const
    {
        return ::std::abs(rebind().rho());
    }

    bool
    is_zero() const
    {
        return value_traits::eq(rebind().rho(), 0);
    }
    bool
    is_unit() const
    {
        return value_traits::eq(rebind().rho(), 1);
    }

    vector_type&
    zero()
    {
        detail::set_all_elements< 1, vector_type >()(rebind(), 0);
        return rebind();
    }

    /**
     * Set rho to 1, if rho is negative, set azimuth to opposite
     * @return
     */
    vector_type&
    normalize()
    {
        if (rebind().rho() < 0) {
            rebind().azimuth() += pi::value;
            clamp_azumuth();
        }
        rebind().rho() = 1;
        return rebind();
    }
private:
    void
    negate()
    {
        if (rebind().rho() < 0) {
            rebind().rho() *= -1;
        } else {
            rebind().azimuth() += pi::value;
            clamp_azumuth();
        }
    }
    void
    clamp_azumuth()
    {
        auto& az = rebind().azimuth();
        while (az >= pi::value * 2) {
            az -= pi::value * 2;
        }
        while (az <= -pi::value * 2) {
            az += pi::value * 2;
        }
    }

    vector_type&
    rebind() { return static_cast<vector_type&>(*this); }
    vector_type const&
    rebind() const { return static_cast<vector_type const&>(*this); }
};

/**
 * Calculus selector template
 */
template < typename T, ::std::size_t Size, typename Axes >
struct calculus_selector
    : ::std::conditional< is_vector_calculus<Axes>::value,
            vector_calculus<T, Size, Axes>,
            nonvector_calculus<T, Size, Axes>>::type {};

}  /* namespace detail */
}  /* namespace math */
}  /* namespace psst */


#endif /* INCLUDE_PUSHKIN_MATH_DETAIL_CALCULUS_HPP_ */
