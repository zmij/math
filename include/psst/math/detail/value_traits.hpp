/*
 * value_traits.hpp
 *
 *  Created on: 18 дек. 2016 г.
 *      Author: sergey.fedorov
 */

#ifndef PSST_MATH_DETAIL_VALUE_TRAITS_HPP_
#define PSST_MATH_DETAIL_VALUE_TRAITS_HPP_

#include <psst/math/detail/utils.hpp>
#include <psst/math/matrix_fwd.hpp>
#include <psst/math/vector_fwd.hpp>

#include <type_traits>

namespace psst {
namespace math {

namespace traits {
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

template <typename T, std::size_t Size, typename Components>
struct value_tag<vector<T, Size, Components>> {
    using type = tag::vector;
};
template <typename T, std::size_t RC, std::size_t CC, typename Components>
struct value_tag<matrix<T, RC, CC, Components>> {
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
    static constexpr double value = 1e-15;
};
template <>
struct iota<long double> {
    static constexpr long double value = 1e-30;
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

template <typename T, std::size_t Size, typename Components>
struct vector_traits<vector<T, Size, Components>> {
    using vector_type      = vector<T, Size, Components>;
    using component_names  = Components;
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

template <typename T, std::size_t RC, std::size_t CC, typename Components>
struct matrix_traits<matrix<T, RC, CC, Components>> {
    using matrix_type      = matrix<T, RC, CC, Components>;
    using transposed_type  = matrix<T, CC, RC, Components>;
    using type             = matrix_type;
    using value_tag        = tag::matrix;
    using row_type         = vector<T, CC, Components>;
    using col_type         = vector<T, RC, Components>;
    using size_type        = matrix_size<RC, CC>;
    using component_names  = Components;
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
template <typename T, std::size_t S, typename Components>
struct is_vector<vector<T, S, Components>> : std::true_type {};
template <typename T, std::size_t S, typename Components>
struct is_vector<vector_view<T, S, Components>> : std::true_type {};
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
/** @name is_mutable_vector */
template <typename T>
struct is_mutable_vector : std::false_type {};
template <typename T>
using is_mutable_vector_t = typename is_mutable_vector<T>::type;
template <typename T>
constexpr bool is_mutable_vector_v = is_mutable_vector_t<T>::value;

template <typename T, std::size_t S, typename Components>
struct is_mutable_vector<vector<T, S, Components>> : std::true_type {};
template <typename T, std::size_t S, typename Components, component_order Order>
struct is_mutable_vector<vector_view<T*, S, Components, Order>> : std::true_type {};
template <typename T, std::size_t S, typename Components, component_order Order>
struct is_mutable_vector<vector_view<T const*, S, Components, Order>> : std::false_type {};

template <typename T>
struct is_mutable_vector<T&> : is_mutable_vector<T> {};

template <typename T>
using enable_if_mutable_vector = std::enable_if_t<is_mutable_vector_v<T>>;
//@}

//@{
/** @name is_matrix trait */
template <typename T>
struct is_matrix : std::false_type {};
template <typename T, std::size_t RC, std::size_t CC, typename Components>
struct is_matrix<matrix<T, RC, CC, Components>> : std::true_type {};
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

template <typename T>
using enable_if_expression = std::enable_if_t<is_expression_v<T>>;
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
/** @name Components names trait */
namespace detail {

template <typename T>
constexpr auto
detect_component_names()
{
    if constexpr (is_vector_expression_v<T> || is_matrix_expression_v<T>) {
        return typename std::decay_t<T>::component_names{};
    } else {
        return components::none{};
    }
}

}    // namespace detail
template <typename T>
struct component_names {
    using type = decltype(detail::detect_component_names<T>());
};
template <typename T>
using component_names_t = typename component_names<T>::type;
//@}

//@{
template <typename T, typename... Components>
struct has_components
    : utils::bool_constant<(std::is_same<component_names_t<T>, Components>{} || ...)> {};
template <typename T, typename... Components>
using has_components_t = typename has_components<T, Components...>::type;
template <typename T, typename... Components>
constexpr bool has_components_v = has_components_t<T, Components...>::value;

template <typename T, typename... Components>
using enable_for_components = std::enable_if_t<has_components_v<T, Components...>>;
template <typename T, typename... Components>
using disable_for_components = std::enable_if_t<!has_components_v<T, Components...>>;
//@}
//@{
/** @name undefined_components */
template <typename T>
struct undefined_components : has_components<T, components::none> {};
template <typename T>
using undefined_components_t = typename undefined_components<T>::type;
template <typename T>
constexpr bool undefined_components_v = undefined_components_t<T>::value;
//@}
//@{
/** @name same_components */
template <typename... T>
struct same_components : utils::is_same_t<component_names_t<T>...> {};
template <typename... T>
using same_components_t = typename same_components<T...>::type;
template <typename... T>
constexpr bool same_components_v = same_components_t<T...>::value;
template <typename T>
struct same_components<T> : std::true_type {};

template <typename... T>
using enable_for_same_components = std::enable_if_t<same_components_v<T...>>;
//@}

//@{
/** @name components_are_compatible */
/**
 * Check that components types are compatible. Compatible components are the same components or
 * components::none.
 */
template <typename... T>
struct components_are_compatible
    : utils::bool_constant<
          utils::is_same_v<T...> || ((utils::is_same_v<components::none, T> || ...))> {};
template <typename... T>
using components_are_compatible_t = typename components_are_compatible<T...>::type;
template <typename... T>
constexpr bool components_are_compatible_v = components_are_compatible_t<T...>::value;

template <typename T>
struct components_are_compatible<T> : std::true_type {};

/**
 * Enable if the types of components are considered compatible
 */
template <typename... T>
using enable_if_compatible_components = std::enable_if_t<components_are_compatible_v<T...>>;
//@}

//@{
template <typename... T>
struct compatible_components
    : utils::bool_constant<
          utils::is_same_v<component_names_t<T>...> || ((undefined_components_v<T> || ...))> {};
template <typename... T>
using compatible_components_t = typename compatible_components<T...>::type;
template <typename... T>
constexpr bool compatible_components_v = compatible_components_t<T...>::value;

/**
 * Enable if the types of components of expressions are considered compatible
 */
template <typename... T>
using enable_for_compatible_components = std::enable_if_t<compatible_components_v<T...>>;
//@}

//@{
/** @name select_component_names */
template <typename... T>
struct select_component_names;
template <typename... T>
using select_component_names_t = typename select_component_names<T...>::type;
template <typename T>
struct select_component_names<T> {
    using type = T;
};
template <>
struct select_component_names<> {
    using type = components::none;
};
template <typename T, typename U>
struct select_component_names<T, U> {
    using type = std::conditional_t<
        utils::is_same_v<T, U>, T,
        std::conditional_t<components::undefined_v<T>, U,
                           std::conditional_t<components::undefined_v<U>, T, components::none>>>;
};
template <typename T, typename U, typename... V>
struct select_component_names<T, U, V...>
    : select_component_names<select_component_names_t<T, U>, select_component_names_t<V...>> {};
//@}

//@{
/** @name component_names_for */
template <typename... T>
struct component_names_for : select_component_names<component_names_t<T>...> {};
template <typename... T>
using component_names_for_t = typename component_names_for<T...>::type;
//@}

//@{
/** @name vector_expression_size */
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
    static_assert((compatible_components_v<T...>), "All vectors should have the same components");
    using result_value_type = scalar_expression_result_t<T...>;
    using component_names   = component_names_for_t<T...>;
    using size_type         = min_vector_size_t<T...>;
    return vector<result_value_type, size_type::value, component_names>{};
}

}    // namespace detail

template <typename... T>
struct vector_expression_result {
    using type = decltype(detail::detect_vector_exression_result<std::decay_t<T>...>());
};
template <typename... T>
using vector_expression_result_t = typename vector_expression_result<T...>::type;

//@{
/** @name addition defined */
template <typename T, typename U, typename = utils::void_t<>>
struct addition_defined : std::false_type {};
template <typename T, typename U>
using addition_defined_t = typename addition_defined<T, U>::type;
template <typename T, typename U>
constexpr bool addition_defined_v = addition_defined_t<T, U>::value;

template <typename T, typename U>
struct addition_defined<T, U, utils::void_t<decltype(std::declval<T>() + std::declval<U>())>>
    : std::true_type {};

template <typename T, typename U>
using enable_if_addition_defined = std::enable_if_t<addition_defined_v<T, U>>;
//@}

//@{
/** @name difference defined */
template <typename T, typename U, typename = utils::void_t<>>
struct difference_defined : std::false_type {};
template <typename T, typename U>
using difference_defined_t = typename difference_defined<T, U>::type;
template <typename T, typename U>
constexpr bool difference_defined_v = difference_defined_t<T, U>::value;

template <typename T, typename U>
struct difference_defined<T, U, utils::void_t<decltype(std::declval<T>() - std::declval<U>())>>
    : std::true_type {};

template <typename T, typename U>
using enable_if_difference_defined = std::enable_if_t<difference_defined_v<T, U>>;
//@}

//@{
/** @name multiplication_defined */
template <typename T, typename U, typename = utils::void_t<>>
struct multiplication_defined : std::false_type {};
template <typename T, typename U>
using multiplication_defined_t = typename multiplication_defined<T, U>::type;
template <typename T, typename U>
constexpr bool multiplication_defined_v = multiplication_defined_t<T, U>::value;

template <typename T, typename U>
struct multiplication_defined<T, U, utils::void_t<decltype(std::declval<T>() * std::declval<U>())>>
    : std::true_type {};

template <typename T, typename U>
using enable_if_multiplication_defined = std::enable_if_t<multiplication_defined_v<T, U>>;

// template <typename T, typename U>
// using enable_if_multiplication_defined_and_result_is_compatible = std::enable_if_t<
// multiplication_defined_t<T, U>
//@}

//@{
/** @name division_defined */
template <typename T, typename U, typename = utils::void_t<>>
struct division_defined : std::false_type {};
template <typename T, typename U>
using division_defined_t = typename division_defined<T, U>::type;
template <typename T, typename U>
constexpr bool division_defined_v = division_defined_t<T, U>::value;

template <typename T, typename U>
struct division_defined<T, U, utils::void_t<decltype(std::declval<T>() / std::declval<U>())>>
    : std::true_type {};

template <typename T, typename U>
using enable_if_division_defined = std::enable_if_t<division_defined_v<T, U>>;
//@}

//@{
/** @name dot_product_defined */
template <typename T, typename U, typename = utils::void_t<>>
struct dot_product_defined : std::false_type {};
template <typename T, typename U>
using dot_product_defined_t = typename dot_product_defined<T, U>::type;
template <typename T, typename U>
constexpr bool dot_product_defined_v = dot_product_defined_t<T, U>::value;

template <typename T, typename U>
struct dot_product_defined<
    T, U, utils::void_t<decltype(dot_product(std::declval<T>(), std::declval<U>()))>>
    : std::true_type {};

template <typename T, typename U>
using enable_if_dot_product_defined = std::enable_if_t<dot_product_defined_v<T, U>>;
//@}

}    // namespace traits
}    // namespace math
}    // namespace psst

#endif /* PSST_MATH_DETAIL_VALUE_TRAITS_HPP_ */
