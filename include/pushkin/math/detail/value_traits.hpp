/*
 * value_traits.hpp
 *
 *  Created on: 18 дек. 2016 г.
 *      Author: sergey.fedorov
 */

#ifndef INCLUDE_PUSHKIN_MATH_DETAIL_VALUE_TRAITS_HPP_
#define INCLUDE_PUSHKIN_MATH_DETAIL_VALUE_TRAITS_HPP_

#include <pushkin/math/detail/utils.hpp>
#include <pushkin/math/matrix_fwd.hpp>
#include <pushkin/math/vector_fwd.hpp>

#include <type_traits>

namespace psst {
namespace math {

namespace tag {

struct scalar {};
struct vector {};
struct matrix {};

}    // namespace tag

namespace detail {

template <typename T, typename Enable = utils::void_t<>>
struct value_tag_impl {
    using type = tag::scalar;
};

template <typename T>
struct value_tag_impl<T, utils::void_t<typename T::value_tag>> {
    using type = typename T::value_tag;
};
}    // namespace detail

template <typename T>
struct value_tag : detail::value_tag_impl<T> {};

template <typename T>
using value_tag_t = typename value_tag<T>::type;

template <typename T, std::size_t Size, typename Axes>
struct value_tag<vector<T, Size, Axes>> {
    using type = tag::vector;
};
template <typename T, std::size_t RC, std::size_t CC, typename Axes>
struct value_tag<matrix<T, RC, CC, Axes>> {
    using type = tag::matrix;
};

namespace detail {

template <typename T>
struct real_type_calc {
    using value_type = typename std::decay<T>::type;
    using type       = typename std::conditional<
        sizeof(value_type) <= 4, float,
        typename std::conditional<(sizeof(value_type) > 8), long double, double>::type>::type;
};

template <typename T>
struct iota {
    static constexpr T value = 0;
};

template <typename T>
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
template <typename T, bool>
struct magnitude_traits_impl {
    using value_type     = typename std::decay<T>::type;
    using magnitude_type = value_type;
};

/**
 * Magnitude traits for integral types
 */
template <typename T>
struct magnitude_traits_impl<T, true> {
    using value_type     = typename std::decay<T>::type;
    using magnitude_type = typename real_type_calc<T>::type;
};

template <typename T, bool>
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

template <typename T>
struct compare_traits_impl<T, true> {
    using iota_type = iota<T>;
    template <typename U>
    static bool
    eq(T const& lhs, U const& rhs)
    {
        T diff = rhs - lhs;
        return -iota_type::value <= diff && diff <= iota_type::value;
    }
    template <typename U>
    static bool
    less(T const& lhs, U const& rhs)
    {
        T diff = rhs - lhs;
        return diff > iota_type::value;
    }
    template <typename U>
    static int
    cmp(T const& lhs, U const& rhs)
    {
        T diff = rhs - lhs;
        return diff > iota_type::value ? -1 : diff >= -iota_type::value ? 0 : 1;
    }
};

template <typename T>
struct compare_traits : compare_traits_impl<T, std::is_floating_point<T>::value> {};

} /* namespace detail */

template <typename T>
struct scalar_value_traits : detail::compare_traits<std::decay_t<T>> {
    using value_type       = std::decay_t<T>;
    using type             = value_type;
    using value_tag        = tag::scalar;
    using lvalue_reference = std::add_lvalue_reference_t<value_type>;
    using const_reference  = std::add_lvalue_reference_t<std::add_const_t<value_type>>;

    using pointer       = std::add_pointer_t<T>;
    using const_pointer = std::add_pointer_t<T const>;

    using magnitude_traits = detail::magnitude_traits_impl<T, std::is_integral<value_type>::value>;
    using magnitude_type   = typename magnitude_traits::magnitude_type;
};

template <typename VectorType>
struct vector_traits;

template <typename T, std::size_t Size, typename Axes>
struct vector_traits<vector<T, Size, Axes>> {
    using vector_type      = vector<T, Size, Axes>;
    using axes_names       = Axes;
    using element_type     = T;
    using type             = vector_type;
    using value_tag        = tag::vector;
    using value_traits     = scalar_value_traits<T>;
    using value_type       = typename value_traits::value_type;
    using lvalue_reference = typename value_traits::lvalue_reference;
    using const_reference  = typename value_traits::const_reference;
    using magnitude_type   = typename value_traits::magnitude_type;

    using pointer       = typename value_traits::pointer;
    using const_pointer = typename value_traits::const_pointer;

    using iterator       = pointer;
    using const_iterator = const_pointer;

    static constexpr auto size = Size;

    using index_sequence_type = std::make_index_sequence<size>;
};

template <std::size_t R, std::size_t C>
struct matrix_size {
    static constexpr std::size_t rows = R;
    static constexpr std::size_t cols = C;
    static constexpr std::size_t size = rows * cols;
};

template <typename MatrixType>
struct matrix_traits;

template <typename T, std::size_t RC, std::size_t CC, typename Axes>
struct matrix_traits<matrix<T, RC, CC, Axes>> {
    using matrix_type      = matrix<T, RC, CC, Axes>;
    using transposed_type  = matrix<T, CC, RC, Axes>;
    using type             = matrix_type;
    using value_tag        = tag::matrix;
    using row_type         = vector<T, CC, Axes>;
    using col_type         = vector<T, RC, Axes>;
    using size_type        = matrix_size<RC, CC>;
    using axes_names       = Axes;
    using element_type     = T;
    using value_traits     = scalar_value_traits<T>;
    using value_type       = typename value_traits::value_type;
    using lvalue_reference = typename value_traits::lvalue_reference;
    using const_reference  = typename value_traits::const_reference;
    using magnitude_type   = typename value_traits::magnitude_type;

    using pointer       = typename value_traits::pointer;
    using const_pointer = typename value_traits::const_pointer;

    using iterator       = pointer;
    using const_iterator = const_pointer;

    static constexpr std::size_t rows = size_type::rows;
    static constexpr std::size_t cols = size_type::cols;
    static constexpr std::size_t size = size_type::size;

    using row_indexes_type = std::make_index_sequence<rows>;
    using col_indexes_type = std::make_index_sequence<cols>;
};

//@{
template <typename T, typename = utils::void_t<>>
struct is_scalar : std::false_type {};
template <typename T>
struct is_scalar<T,
                 std::enable_if_t<std::is_same<value_tag_t<std::decay_t<T>>, tag::scalar>::value>>
    : std::true_type {};
template <typename T>
using is_scalar_t = typename is_scalar<std::decay_t<T>>::type;
template <typename T>
constexpr bool is_scalar_v = is_scalar_t<T>::value;

template <typename T>
using enable_if_scalar_value = std::enable_if_t<is_scalar_v<T>>;
template <typename... T>
using enable_if_scalar_values = std::enable_if_t<(is_scalar_v<T> && ...)>;
//@}

//@{
/** @name is_vector trait */
template <typename T>
struct is_vector : std::false_type {};
template <typename T, std::size_t S, typename Axes>
struct is_vector<vector<T, S, Axes>> : std::true_type {};
template <typename T>
using is_vector_t = typename is_vector<std::decay_t<T>>::type;
template <typename T>
constexpr bool is_vector_v = is_vector_t<T>::value;
template <typename T>
struct is_vector<T&> : is_vector<T> {};
template <typename T>
using enable_if_vector = std::enable_if_t<is_vector_v<T>>;
//@}

//@{
/** @name is_matrix trait */
template <typename T>
struct is_matrix : std::false_type {};
template <typename T, std::size_t RC, std::size_t CC, typename Axes>
struct is_matrix<matrix<T, RC, CC, Axes>> : std::true_type {};
template <typename T>
using is_matrix_t = typename is_matrix<std::decay_t<T>>::type;
template <typename T>
constexpr bool is_matrix_v = is_matrix_t<T>::value;
template <typename T>
struct is_matrix<T&> : is_matrix<T> {};
template <typename T>
using enable_if_matrix = std::enable_if_t<is_matrix_v<T>>;
//@}

template <typename T>
using enable_if_vector_or_matrix = std::enable_if_t<is_vector_v<T> || is_matrix_v<T>>;

//@{
/** @name is_expression trait */
template <typename T, typename = utils::void_t<>>
struct is_expression : std::false_type {};
template <typename T>
struct is_expression<T, utils::void_t<typename std::decay_t<T>::expression_type,
                                      typename std::decay_t<T>::result_type>> : std::true_type {};
template <typename T>
using is_expression_t = typename is_expression<std::decay_t<T>>::type;
template <typename T>
constexpr bool is_expression_v = is_expression_t<T>::value;
//@}

//@{
/** @name is_scalar_expression trait */
template <typename T, typename = utils::void_t<>>
struct is_scalar_expression : std::false_type {};
template <typename T>
struct is_scalar_expression<
    T, std::enable_if_t<is_expression_v<T> && is_scalar_v<typename std::decay_t<T>::result_type>>>
    : std::true_type {};
template <typename T>
using is_scalar_expression_t = typename is_scalar_expression<std::decay_t<T>>::type;
template <typename T>
constexpr bool is_scalar_expression_v = is_scalar_expression_t<T>::value;

template <typename T>
using enable_if_scalar_expression = std::enable_if_t<is_scalar_expression_v<T>>;
template <typename... T>
using enable_if_scalar_args
    = std::enable_if_t<(is_scalar_v<T> && ...) && ((is_scalar_expression_v<T> || ...))>;
//@}

//@{
/** @name is_vector_expression trait */
template <typename T, typename = utils::void_t<>>
struct is_vector_expression : std::false_type {};
template <typename T>
struct is_vector_expression<
    T, std::enable_if_t<is_expression_v<T> && is_vector_v<typename std::decay_t<T>::result_type>>>
    : std::true_type {};
template <typename T>
using is_vector_expression_t = typename is_vector_expression<std::decay_t<T>>::type;
template <typename T>
constexpr bool is_vector_expression_v = is_vector_expression_t<T>::value;
template <typename T>
using enable_if_vector_expression = std::enable_if_t<is_vector_expression_v<T>>;
template <typename... T>
constexpr bool all_vector_expressions_v = (is_vector_expression_v<T> && ...);
template <typename... T>
using enable_if_vector_expressions = std::enable_if_t<all_vector_expressions_v<T...>>;
template <typename... T>
constexpr bool contains_vector_expression_v = (is_vector_expression_v<T> || ...);
//@}

//@{
/** @name is_matrix_expression trait */
template <typename T, typename = utils::void_t<>>
struct is_matrix_expression : std::false_type {};
template <typename T>
struct is_matrix_expression<
    T, std::enable_if_t<is_expression_v<T> && is_matrix_v<typename std::decay_t<T>::result_type>>>
    : std::true_type {};
template <typename T>
using is_matrix_expression_t = typename is_matrix_expression<std::decay_t<T>>::type;
template <typename T>
constexpr bool is_matrix_expression_v = is_matrix_expression_t<T>::value;

template <typename T>
using enable_if_matrix_expression = std::enable_if_t<is_matrix_expression_v<T>>;
template <typename... T>
using enable_if_matrix_expressions = std::enable_if_t<(is_matrix_expression_v<T> && ...)>;
//@}

//@{
/** @name Axes names trait */
namespace detail {

template <typename T>
constexpr auto
detect_axes_names()
{
    if constexpr (is_vector_expression_v<T> || is_matrix_expression_v<T>) {
        return typename std::decay_t<T>::axes_names{};
    } else {
        return axes::none{};
    }
}

}    // namespace detail
template <typename T>
struct axes_names {
    using type = decltype(detail::detect_axes_names<T>());
};
template <typename T>
using axes_names_t = typename axes_names<T>::type;
//@}

//@{
template <typename T, typename... Axes>
struct has_axes : utils::bool_constant<(std::is_same<axes_names_t<T>, Axes>{} || ...)> {};
template <typename T, typename... Axes>
using has_axes_t = typename has_axes<T, Axes...>::type;
template <typename T, typename... Axes>
constexpr bool has_axes_v = has_axes_t<T, Axes...>::value;

template <typename T, typename... Axes>
using enable_for_axes = std::enable_if_t<has_axes_v<T, Axes...>>;
template <typename T, typename... Axes>
using disable_for_axes = std::enable_if_t<!has_axes_v<T, Axes...>>;
//@}
//@{
/** @name undefined_axes */
template <typename T>
struct undefined_axes : has_axes<T, axes::none> {};
template <typename T>
using undefined_axes_t = typename undefined_axes<T>::type;
template <typename T>
constexpr bool undefined_axes_v = undefined_axes_t<T>::value;
//@}
//@{
/** @name same_axes */
template <typename... T>
struct same_axes : utils::is_same_t<axes_names_t<T>...> {};
template <typename... T>
using same_axes_t = typename same_axes<T...>::type;
template <typename... T>
constexpr bool same_axes_v = same_axes_t<T...>::value;
template <typename T>
struct same_axes<T> : std::true_type {};
//@}

//@{
template <typename... T>
struct compatible_axes
    : utils::bool_constant<utils::is_same_v<axes_names_t<T>...> || ((undefined_axes_v<T> || ...))> {
};
template <typename... T>
using compatible_axes_t = typename compatible_axes<T...>::type;
template <typename... T>
constexpr bool compatible_axes_v = compatible_axes_t<T...>::value;
//@}

//@{
/** @name select_axes_names */
template <typename... T>
struct select_axes_names;
template <typename... T>
using select_axes_names_t = typename select_axes_names<T...>::type;
template <typename T>
struct select_axes_names<T> {
    using type = T;
};
template <>
struct select_axes_names<> {
    using type = axes::none;
};
template <typename T, typename U>
struct select_axes_names<T, U> {
    using type = std::conditional_t<
        utils::is_same_v<T, U>, T,
        std::conditional_t<axes::undefined_v<T>, U,
                           std::conditional_t<axes::undefined_v<U>, T, axes::none>>>;
};
template <typename T, typename U, typename... V>
struct select_axes_names<T, U, V...>
    : select_axes_names<select_axes_names_t<T, U>, select_axes_names_t<V...>> {};
//@}

//@{
/** @name axes_names_for */
template <typename... T>
struct axes_names_for : select_axes_names<axes_names_t<T>...> {};
template <typename... T>
using axes_names_for_t = typename axes_names_for<T...>::type;
//@}

//@{
namespace detail {

template <typename T>
constexpr std::size_t
vector_expression_size()
{
    if constexpr (is_vector_expression_v<T>) {
        return std::decay_t<T>::size;
    } else {
        return utils::npos::value;
    }
}

}    // namespace detail

template <typename T>
struct vector_expression_size : utils::size_constant<detail::vector_expression_size<T>()> {};
template <typename T>
using vector_exression_size_t = typename vector_expression_size<T>::type;
template <typename T>
constexpr std::size_t vector_expression_size_v = vector_exression_size_t<T>::value;
//@}

//@{
/** @name min_vector_size */
template <typename... T>
struct min_vector_size : utils::min<vector_expression_size_v<T>...> {};
template <typename... T>
using min_vector_size_t = typename min_vector_size<T...>::type;
template <typename... T>
constexpr bool min_vector_size_v = min_vector_size_t<T...>::value;
//@}

//@{
/** @name Selector of traits based on the value tag */
namespace detail {

template <typename T, template <typename> class Traits>
constexpr auto
trait_select()
{
    if constexpr (is_vector_v<T>) {
        return Traits<T>{};
    } else if constexpr (is_matrix_v<T>) {
        return Traits<T>{};
    } else if constexpr (is_expression_v<T>) {
        return Traits<typename std::decay_t<T>::result_type>{};
    } else {
        return Traits<T>{};
    }
}

template <typename T, template <typename> class Traits>
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

}    // namespace detail

template <typename T>
struct value_traits {
    using type = detail::value_traits_impl_t<T>;
};

template <typename T>
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
        return typename std::decay_t<T>::value_type{};
    } else {
        static_assert(is_scalar_v<T>, "At this point only scalars should remain");
        return std::decay_t<T>{};
    }
}

}    // namespace detail

template <typename... T>
struct scalar_expression_result {
    using type = utils::most_precise_type_t<decltype(
        detail::detect_expression_value_type<std::decay_t<T>>())...>;
};
template <typename... T>
using scalar_expression_result_t = typename scalar_expression_result<T...>::type;

namespace detail {

template <typename... T>
constexpr auto
detect_vector_exression_result()
{
    static_assert(
        (contains_vector_expression_v<T...>),
        "At least one of the arguments to a vector expression must be a vector expression");
    static_assert((compatible_axes_v<T...>), "All vectors should have the same axes");
    using result_value_type = scalar_expression_result_t<T...>;
    using axes_names        = axes_names_for_t<T...>;
    using size_type         = min_vector_size_t<T...>;
    return vector<result_value_type, size_type::value, axes_names>{};
}

}    // namespace detail

template <typename... T>
struct vector_expression_result {
    using type = decltype(detail::detect_vector_exression_result<std::decay_t<T>...>());
};
template <typename... T>
using vector_expression_result_t = typename vector_expression_result<T...>::type;

} /* namespace math */
} /* namespace psst */

#endif /* INCLUDE_PUSHKIN_MATH_DETAIL_VALUE_TRAITS_HPP_ */
