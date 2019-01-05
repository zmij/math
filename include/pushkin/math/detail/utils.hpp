/*
 * utils.hpp
 *
 *  Created on: Dec 25, 2018
 *      Author: ser-fedorov
 */

#ifndef PUSHKIN_MATH_DETAIL_UTILS_HPP_
#define PUSHKIN_MATH_DETAIL_UTILS_HPP_

#include <limits>
#include <type_traits>
#include <utility>

namespace psst {
namespace math {
namespace utils {

#if __cpp_lib_void_t >= 201411
template <typename... T>
using void_t = std::void_t<T...>;
#else

template <typename... T>
struct make_void_t {
    using type = void;
};
template <typename... T>
using void_t = typename make_void_t<T...>::type;

#endif

template <bool V>
using bool_constant = std::integral_constant<bool, V>;
template <std::size_t V>
using size_constant = std::integral_constant<std::size_t, V>;

template <std::size_t Index, typename T>
struct value_fill {
    T value;
};

template <typename... T>
struct most_precise_type;
template <typename... T>
using most_precise_type_t = typename most_precise_type<T...>::type;

template <typename T, typename U>
struct most_precise_type<T, U> {
    using type = decltype(std::declval<T>() / std::declval<U>());
};
template <typename T, typename... U>
struct most_precise_type<T, U...> : most_precise_type<T, most_precise_type_t<U...>> {};
template <typename T>
struct most_precise_type<T> {
    using type = T;
};

template <typename T, typename U>
struct shorter_sequence;

template <typename T, T... LHS, T... RHS>
struct shorter_sequence<std::integer_sequence<T, LHS...>, std::integer_sequence<T, RHS...>> {
    using type
        = std::conditional_t<sizeof...(LHS) <= sizeof...(RHS), std::integer_sequence<T, LHS...>,
                             std::integer_sequence<T, RHS...>>;
};

template <typename T, typename U>
using shorter_sequence_t = typename shorter_sequence<T, U>::type;

template <std::size_t... V>
struct min;
template <std::size_t L, std::size_t R>
struct min<L, R> : size_constant<(L < R) ? L : R> {};
template <std::size_t L, std::size_t R, std::size_t... V>
struct min<L, R, V...> : min<min<L, R>::value, min<V...>::value> {};
template <std::size_t V>
struct min<V> : size_constant<V> {};
template <std::size_t... V>
constexpr std::size_t min_v = min<V...>::value;

template <std::size_t... V>
using make_min_index_sequence = std::make_index_sequence<min_v<V...>>;

using npos = size_constant<std::numeric_limits<std::size_t>::max()>;

//@{
/** @name is_same */
template <typename... T>
struct is_same;
template <typename... T>
using is_same_t = typename is_same<T...>::type;
template <typename... T>
constexpr bool is_same_v = is_same_t<T...>::value;

template <typename T>
struct is_same<T> : bool_constant<true> {};
template <typename T, typename U>
struct is_same<T, U> : std::is_same<T, U> {};
template <typename T, typename U, typename... V>
struct is_same<T, U, V...> : bool_constant<is_same_v<T, U> && is_same_v<U, V...>> {};
//@}

//@{
/** @name is_decl_complete */
namespace detail {

template <typename T, std::size_t = sizeof(T)>
std::true_type
is_decl_complete_impl(T*);

std::false_type
is_decl_complete_impl(...);

}    // namespace detail
template <typename T>
struct is_decl_complete : decltype(detail::is_decl_complete_impl(std::declval<T*>())) {};
template <typename T>
using is_decl_complete_t = typename is_decl_complete<T>::type;
template <typename T>
constexpr bool is_decl_complete_v = is_decl_complete_t<T>::value;
//@}

//@{
template <std::size_t N, template <typename> class... T>
struct nth_template;

template <template <typename> class T, template <typename> class... Y>
struct nth_template<0, T, Y...> {
    template <typename U>
    using type = T<U>;
};

template <::std::size_t N, template <typename> class T, template <typename> class... Y>
struct nth_template<N, T, Y...> : nth_template<N - 1, Y...> {
    static_assert(N <= sizeof...(Y), "Index type is out of range");
};

//@}
//@{
template <template <typename> class... T>
struct template_tuple {
    static constexpr std::size_t size = sizeof...(T);
    template <std::size_t N, typename U>
    using type = typename nth_template<N, T...>::template type<U>;
};
template <>
struct template_tuple<> {
    static constexpr std::size_t size = 0;
};
//@}

}    // namespace utils
}    // namespace math
}    // namespace psst

#endif /* PUSHKIN_MATH_DETAIL_UTILS_HPP_ */
