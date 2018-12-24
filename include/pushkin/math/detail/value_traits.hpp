/*
 * value_traits.hpp
 *
 *  Created on: 18 дек. 2016 г.
 *      Author: sergey.fedorov
 */

#ifndef INCLUDE_PUSHKIN_MATH_DETAIL_VALUE_TRAITS_HPP_
#define INCLUDE_PUSHKIN_MATH_DETAIL_VALUE_TRAITS_HPP_

#include <pushkin/math/vector_fwd.hpp>
#include <pushkin/math/matrix_fwd.hpp>

#include <type_traits>

namespace psst {
namespace math {

namespace tag {

struct scalar {};
struct vector {};
struct matrix {};

}  // namespace tag

template < typename T >
struct value_category {
    using type = tag::scalar;
};
template < typename T >
using value_category_t = typename value_category<T>::type;

template <typename ... T>
struct value_category<vector<T...>> {
    using type = tag::vector;
};
template <typename ... T>
struct value_category<matrix<T...>> {
    using type = tag::matrix;
};

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

template < typename T >
struct iota {
    static constexpr T value = 0;
};

template < typename T >
constexpr T iota_v = iota<T>::value;

template <>
struct iota<float> {
    static constexpr float value = 1e-7;
};
template <>
struct iota<double> {
    static constexpr float value = 1e-15;
};
template <>
struct iota<long double> {
    static constexpr float value = 1e-30;
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

template < typename T, bool >
struct compare_traits_impl {
    static bool
    eq(T const& lhs, T const& rhs)
    {
        return lhs == rhs;
    }
    static bool
    less(T const& lhs, T const& rhs)
    {
        return lhs < rhs;
    }
    static int
    cmp(T const& lhs, T const& rhs)
    {
        return less(lhs, rhs) ? -1 : eq(lhs, rhs) ? 0 : 1;
    }
};

template < typename T >
struct compare_traits_impl<T, true> {
    using iota_type = iota<T>;
    template < typename U >
    static bool
    eq(T const& lhs, U const& rhs)
    {
        auto diff = rhs - lhs;
        return -iota_type::value <= diff && diff <= iota_type::value;
    }
    template < typename U >
    static bool
    less(T const& lhs, U const& rhs)
    {
        auto diff = rhs - lhs;
        return diff > iota_type::value;
    }
    template < typename U >
    static int
    cmp(T const& lhs, U const& rhs)
    {
        auto diff = rhs - lhs;
        return diff > iota_type::value ? -1 : diff >= -iota_type::value ? 0 : 1;
    }
};

template < typename T >
struct compare_traits : compare_traits_impl<T, ::std::is_floating_point<T>::value> {};

template < typename T >
struct scalar_value_traits : compare_traits<::std::decay_t<T>> {
    using value_type        = ::std::decay_t<T>;
    using lvalue_reference  = ::std::add_lvalue_reference_t< value_type >;
    using const_reference   = ::std::add_lvalue_reference_t<::std::add_const_t< value_type > >;

    using pointer           = ::std::add_pointer_t<T>;
    using const_pointer     = ::std::add_pointer_t<T const>;

    using magnitude_traits  = magnitude_traits_impl<T, ::std::is_integral<value_type>::value>;
    using magnitude_type    = typename magnitude_traits::magnitude_type;
};

template < typename T, typename U >
struct most_precise_type {
    using type = decltype(std::declval<T>() / std::declval<U>());
};

template < typename T, typename U >
using most_presize_type_t = typename most_precise_type<T, U>::type;

template < typename T, typename U >
struct shorter_sequence;

template <typename T, T... LHS, T... RHS>
struct shorter_sequence<std::integer_sequence<T, LHS...>,
        std::integer_sequence<T, RHS...>> {
    using type = std::conditional_t<
          sizeof...(LHS) <= sizeof...(RHS),
          std::integer_sequence<T, LHS...>,
          std::integer_sequence<T, RHS...>
        >;
};

template < typename T, typename U >
using shorter_sequence_t = typename shorter_sequence<T, U>::type;

template < ::std::size_t L, ::std::size_t R >
struct min : ::std::conditional<
        L < R,
        ::std::integral_constant<::std::size_t, L>,
        ::std::integral_constant<::std::size_t, R>
    >::type {};

}  /* namespace detail */

template < typename VectorType >
struct vector_traits;

template < typename T, ::std::size_t Size, typename Axes >
struct vector_traits< vector<T, Size, Axes> > {
    using vector_type           = vector<T, Size, Axes>;
    using element_type          = T;
    using value_traits          = detail::scalar_value_traits<T>;
    using value_type            = typename value_traits::value_type;
    using lvalue_reference      = typename value_traits::lvalue_reference;
    using const_reference       = typename value_traits::const_reference;
    using magnitude_type        = typename value_traits::magnitude_type;

    using pointer               = typename value_traits::pointer;
    using const_pointer         = typename value_traits::const_pointer;

    static constexpr auto size  = Size;
};

template < ::std::size_t R, ::std::size_t C >
struct matrix_size {
    static constexpr ::std::size_t rows = R;
    static constexpr ::std::size_t cols = C;
    static constexpr ::std::size_t size = rows * cols;
};


template < typename MatrixType >
struct matrix_traits;

template < typename T, ::std::size_t RC, ::std::size_t CC, typename Axes >
struct matrix_traits<matrix<T, RC, CC, Axes>> {
    using matrix_type           = matrix<T, RC, CC, Axes>;
    using row_type              = vector<T, CC, Axes>;
    using size_type             = matrix_size<RC, CC>;
    using axes_names            = Axes;
    using element_type          = T;
    using value_traits          = detail::scalar_value_traits<T>;
    using value_type            = typename value_traits::value_type;
    using lvalue_reference      = typename value_traits::lvalue_reference;
    using const_reference       = typename value_traits::const_reference;
    using magnitude_type        = typename value_traits::magnitude_type;

    using pointer               = typename value_traits::pointer;
    using const_pointer         = typename value_traits::const_pointer;

    static constexpr ::std::size_t rows = size_type::rows;
    static constexpr ::std::size_t cols = size_type::cols;
    static constexpr ::std::size_t size = size_type::size;
};


}  /* namespace math */
}  /* namespace psst */

#endif /* INCLUDE_PUSHKIN_MATH_DETAIL_VALUE_TRAITS_HPP_ */
