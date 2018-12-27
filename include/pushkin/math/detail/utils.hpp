/*
 * utils.hpp
 *
 *  Created on: Dec 25, 2018
 *      Author: ser-fedorov
 */

#ifndef PUSHKIN_MATH_DETAIL_UTILS_HPP_
#define PUSHKIN_MATH_DETAIL_UTILS_HPP_

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

template <std::size_t Index, typename T>
struct value_fill {
    T value;
};

template <typename T, typename U>
struct most_precise_type {
    using type = decltype(std::declval<T>() / std::declval<U>());
};

template <typename T, typename U>
using most_presize_type_t = typename most_precise_type<T, U>::type;

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

template <std::size_t L, std::size_t R>
    struct min
    : std::conditional_t
      < L<R, std::integral_constant<std::size_t, L>, std::integral_constant<std::size_t, R>> {};
template <std::size_t L, std::size_t R>
constexpr std::size_t min_v = min<L, R>::value;

template <std::size_t L, std::size_t R>
using make_min_index_sequence = std::make_index_sequence<min_v<L, R>>;

}    // namespace utils
}    // namespace math
}    // namespace psst

#endif /* PUSHKIN_MATH_DETAIL_UTILS_HPP_ */
