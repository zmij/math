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
#include <pushkin/math/detail/utils.hpp>

#include <type_traits>

namespace psst {
namespace math {

namespace tag {

struct scalar {};
struct vector {};
struct matrix {};

}  // namespace tag

namespace detail {

template < typename T, typename Enable = utils::void_t<> >
struct value_tag_impl {
  using type = tag::scalar;
};

template <typename T>
struct value_tag_impl<T, utils::void_t<typename T::value_tag>> {
    using type = typename T::value_tag;
};
}  // namespace detail

template < typename T >
struct value_tag : detail::value_tag_impl<T> {
};

template < typename T >
using value_tag_t = typename value_tag<T>::type;

template <typename T, ::std::size_t Size, typename Axes>
struct value_tag<vector<T, Size, Axes>> {
    using type = tag::vector;
};
template <typename T, ::std::size_t RC, ::std::size_t CC, typename Axes>
struct value_tag<matrix<T, RC, CC, Axes>> {
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

}  /* namespace detail */

template < typename T >
struct scalar_value_traits : detail::compare_traits<::std::decay_t<T>> {
    using value_type        = ::std::decay_t<T>;
    using type              = value_type;
    using value_tag         = tag::scalar;
    using lvalue_reference  = ::std::add_lvalue_reference_t< value_type >;
    using const_reference   = ::std::add_lvalue_reference_t<::std::add_const_t< value_type > >;

    using pointer           = ::std::add_pointer_t<T>;
    using const_pointer     = ::std::add_pointer_t<T const>;

    using magnitude_traits  = detail::magnitude_traits_impl<T, ::std::is_integral<value_type>::value>;
    using magnitude_type    = typename magnitude_traits::magnitude_type;
};


template < typename VectorType >
struct vector_traits;

template < typename T, ::std::size_t Size, typename Axes >
struct vector_traits< vector<T, Size, Axes> > {
    using vector_type           = vector<T, Size, Axes>;
    using axes_names            = Axes;
    using element_type          = T;
    using type                  = vector_type;
    using value_tag             = tag::vector;
    using value_traits          = scalar_value_traits<T>;
    using value_type            = typename value_traits::value_type;
    using lvalue_reference      = typename value_traits::lvalue_reference;
    using const_reference       = typename value_traits::const_reference;
    using magnitude_type        = typename value_traits::magnitude_type;

    using pointer               = typename value_traits::pointer;
    using const_pointer         = typename value_traits::const_pointer;

    using iterator              = pointer;
    using const_iterator        = const_pointer;

    static constexpr auto size  = Size;

    using index_sequence_type   = ::std::make_index_sequence<size>;
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
    using type                  = matrix_type;
    using value_tag             = tag::matrix;
    using row_type              = vector<T, CC, Axes>;
    using size_type             = matrix_size<RC, CC>;
    using axes_names            = Axes;
    using element_type          = T;
    using value_traits          = scalar_value_traits<T>;
    using value_type            = typename value_traits::value_type;
    using lvalue_reference      = typename value_traits::lvalue_reference;
    using const_reference       = typename value_traits::const_reference;
    using magnitude_type        = typename value_traits::magnitude_type;

    using pointer               = typename value_traits::pointer;
    using const_pointer         = typename value_traits::const_pointer;

    using iterator              = pointer;
    using const_iterator        = const_pointer;

    static constexpr ::std::size_t rows = size_type::rows;
    static constexpr ::std::size_t cols = size_type::cols;
    static constexpr ::std::size_t size = size_type::size;

    using row_indexes_type      = ::std::make_index_sequence<rows>;
    using col_indexes_type      = ::std::make_index_sequence<cols>;
};

//@{
template <typename T, typename = utils::void_t<>>
struct is_scalar : ::std::false_type {};
template <typename T>
struct is_scalar<T, ::std::enable_if_t< ::std::is_same<value_tag_t<T>, tag::scalar>::value > >
    : ::std::true_type {};
template <typename T>
using is_scalar_t = typename is_scalar<T>::type;
template <typename T>
constexpr bool is_scalar_v = is_scalar_t<T>::value;
//@}

//@{
/** @name is_vector trait */
template <typename T>
struct is_vector : ::std::false_type {};
template <typename T, ::std::size_t S, typename Axes >
struct is_vector<vector<T, S, Axes>> : ::std::true_type {};
template <typename T>
using is_vector_t = typename is_vector<T>::type;
template <typename T>
constexpr bool is_vector_v = is_vector_t<T>::value;
//@}

//@{
/** @name is_matrix trait */
template <typename T>
struct is_matrix : ::std::false_type {};
template <typename T, ::std::size_t RC, ::std::size_t CC, typename Axes>
struct is_matrix<matrix<T, RC, CC, Axes>> : ::std::true_type {};
template <typename T>
using is_matrix_t = typename is_matrix<T>::type;
template <typename T>
constexpr bool is_matrix_v = is_matrix_t<T>::value;
//@}

//@{
/** @name is_expression trait */
template <typename T, typename = utils::void_t<>>
struct is_expression : ::std::false_type {};
template <typename T>
struct is_expression<T,
    utils::void_t<typename T::expression_type, typename T::result_type>> : ::std::true_type {};
template <typename T>
using is_expression_t = typename is_expression<T>::type;
template <typename T>
constexpr bool is_expression_v = is_expression_t<T>::value;
//@}

//@{
/** @name is_scalar_expression trait */
template <typename T, typename = utils::void_t<>>
struct is_scalar_expression : ::std::false_type {};
template <typename T>
struct is_scalar_expression<T,
       ::std::enable_if_t<
          is_expression_v<T> && is_scalar_v<typename T::result_type>
        >> : ::std::true_type {};
template <typename T>
using is_scalar_expression_t = typename is_scalar_expression<T>::type;
template <typename T>
constexpr bool is_scalar_expression_v = is_scalar_expression_t<T>::type;
//@}

//@{
/** @name is_vector_expression trait */
template <typename T, typename = utils::void_t<>>
struct is_vector_expression : ::std::false_type {};
template <typename T>
struct is_vector_expression<T,
       ::std::enable_if_t<
          is_expression_v<T> && is_vector_v<typename T::result_type>
        >> : ::std::true_type {};
template <typename T>
using is_vector_expression_t = typename is_vector_expression<T>::type;
template <typename T>
constexpr bool is_vector_expression_v = is_vector_expression_t<T>::value;
//@}

//@{
/** @name is_matrix_expression trait */
template <typename T, typename = utils::void_t<>>
struct is_matrix_expression : ::std::false_type {};
template <typename T>
struct is_matrix_expression<T,
       ::std::enable_if_t<
          is_expression_v<T> && is_matrix_v<typename T::result_type>
        >> : ::std::true_type {};
template <typename T>
using is_matrix_expression_t = typename is_matrix_expression<T>::type;
template <typename T>
constexpr bool is_matrix_expression_v = is_matrix_expression_t<T>::value;
//@}

//@{
/** @name Selector of traits based on the value tag */
namespace detail {

template <typename T, template<typename> class Traits>
constexpr auto trait_select()
{
    if constexpr (is_vector_v<T>) {
        return Traits<T>{};
    } else if constexpr (is_matrix_v<T>) {
        return Traits<T>{};
    } else if constexpr (is_expression_v<T>) {
        return Traits<typename T::result_type>{};
    } else {
        return Traits<T>{};
    }
}

template <typename T, template<typename> class Traits>
struct trait_selector {
    using type = decltype(trait_select<T, Traits>());
};

template <typename T, typename Tag>
struct value_traits_impl;

template <typename T>
struct value_traits_impl<T, tag::scalar> {
    using type = typename trait_selector<T, scalar_value_traits>::type;
};

template <typename T>
struct value_traits_impl<T, tag::vector> {
  using type = typename trait_selector<T, vector_traits>::type;
};

template <typename T>
struct value_traits_impl<T, tag::matrix> {
    using type = typename trait_selector<T, matrix_traits>::type;
};

template <typename T>
using value_traits_impl_t = typename value_traits_impl<T, value_tag_t<T>>::type;

}  // namespace detail

template < typename T >
struct value_traits {
    using type = detail::value_traits_impl_t<T>;
};

template < typename T >
using value_traits_t = typename value_traits<T>::type;
//@}


namespace detail {

// Detect value type of expression, that might be:
// A scalar
// A scalar expression
// A vector expression
// A matrix expression
template <typename T>
constexpr auto
detect_expression_value_type()
{
    if constexpr (is_expression_v<T>) {
        return typename T::value_type{};
    } else {
        static_assert(is_scalar_v<T>,
            "At this point only scalars should remain");
        return T{};
    }
}

template <typename T, typename U>
constexpr auto
detect_expression_result_value_type()
{
    using lhs_value_type = decltype(detect_expression_value_type<T>());
    using rhs_value_type = decltype(detect_expression_value_type<U>());
    return utils::most_presize_type_t<lhs_value_type, rhs_value_type>{};
}

template <typename T, typename U>
constexpr auto
detect_vector_exression_result()
{
    static_assert(is_vector_expression_v<T> || is_vector_expression_v<U>,
        "At least one of the arguments to a vector expression must be a vector expression");
    using result_value_type = decltype(detect_expression_result_value_type<T, U>());
    if constexpr (is_vector_expression_v<T> && is_vector_expression_v<U>) {
        // check the sizes and axes
        static_assert(T::size == U::size,
            "Cannot detect result of an exprssion with vectors of different sizes");
        static_assert((::std::is_same< typename T::axes_names, typename U::axes_names >::value), "");
        return vector<result_value_type, T::size, typename T::axes_names>{};
    } else if constexpr (is_vector_expression_v<T>) {
        // left-hand side is vector expression
        return vector<result_value_type, T::size, typename T::axes_names>{};
    } else {
        // right-hand side is vector expression
        return vector<result_value_type, U::size, typename U::axes_names>{};
    }
}

}  // namespace detail

template < typename T, typename U >
struct vector_expression_result {
    using type = decltype(detail::detect_vector_exression_result<T, U>());
};
template < typename T, typename U >
using vector_expression_result_t = typename vector_expression_result<T, U>::type;


}  /* namespace math */
}  /* namespace psst */

#endif /* INCLUDE_PUSHKIN_MATH_DETAIL_VALUE_TRAITS_HPP_ */
