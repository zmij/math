/*
 * value_traits.hpp
 *
 *  Created on: 18 дек. 2016 г.
 *      Author: sergey.fedorov
 */

#ifndef INCLUDE_PUSHKIN_MATH_DETAIL_VALUE_TRAITS_HPP_
#define INCLUDE_PUSHKIN_MATH_DETAIL_VALUE_TRAITS_HPP_

#include <type_traits>

namespace psst {
namespace math {
namespace detail {

template < typename T >
struct real_type_calc {
    using value_type = typename ::std::decay<T>::type;
    using type = typename ::std::conditional<
        sizeof(value_type) <= 4,
        float,
        typename ::std::conditional<
            (sizeof(value_type) > 8),
            long double,
            double
        >::type
    >::type;
};

/**
 * Magnitude traits for non-integral types
 */
template < typename T, bool >
struct magnitude_traits_impl {
    using value_type        = typename ::std::decay<T>::type;
    using magnitude_type    = value_type;
};

/**
 * Magnitude traits for integral types
 */
template < typename T >
struct magnitude_traits_impl<T, true> {
    using value_type        = typename ::std::decay<T>::type;
    using magnitude_type    = typename real_type_calc<T>::type;
};

template < typename T >
struct vector_value_traits {
    using value_type        = typename ::std::decay<T>::type;
    using lvalue_reference  = typename ::std::add_lvalue_reference< value_type >::type;
    using const_reference   = typename ::std::add_lvalue_reference<
                                typename ::std::add_const< value_type >::type >::type;

    using magnitude_traits  = magnitude_traits_impl<T, ::std::is_integral<value_type>::value>;
    using magnitude_type    = typename magnitude_traits::magnitude_type;
};


}  /* namespace detail */
}  /* namespace math */
}  /* namespace psst */

#endif /* INCLUDE_PUSHKIN_MATH_DETAIL_VALUE_TRAITS_HPP_ */
