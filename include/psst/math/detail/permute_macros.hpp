/*
 * permute_macros.hpp
 *
 *  Created on: 18 дек. 2016 г.
 *      Author: sergey.fedorov
 */

#ifndef PSST_MATH_DETAIL_PERMUTE_MACROS_HPP_
#define PSST_MATH_DETAIL_PERMUTE_MACROS_HPP_

#define PSST_MATH_COMPONENT_ACCESS(name)                                        \
    template <typename _T = typename base_type::expression_type,                \
              typename    = traits::enable_if_mutable_vector<_T>>               \
    constexpr decltype(auto) name()                                             \
    {                                                                           \
        return base_type::rebind().template at<base_type::coord_names::name>(); \
    }                                                                           \
    constexpr auto name() const                                                 \
    {                                                                           \
        return base_type::rebind().template at<base_type::coord_names::name>(); \
    }

#define PSST_MATH_COMPONENT_VEC2(names, a, b) \
    constexpr vector<value_type, 2, names> a##b() const { return {a(), b()}; }

#define PSST_MATH_COMPONENT_PERM2(names, a, b) \
    PSST_MATH_COMPONENT_VEC2(names, a, b)      \
    PSST_MATH_COMPONENT_VEC2(names, b, a)

#define PSST_MATH_COMPONENT_PERMUTATION2(names, a, b) \
    PSST_MATH_COMPONENT_PERM2(names, a, b)            \
    PSST_MATH_COMPONENT_VEC2(names, a, a)             \
    PSST_MATH_COMPONENT_VEC2(names, b, b)

#define PSST_MATH_COMPONENT_VEC3(names, a, b, c) \
    constexpr vector<value_type, 3, names> a##b##c() const { return {a(), b(), c()}; }

#define PSST_MATH_PERM3_3(names, a, b, u, v, w) \
    PSST_MATH_COMPONENT_VEC3(names, a, b, u)    \
    PSST_MATH_COMPONENT_VEC3(names, a, b, v)    \
    PSST_MATH_COMPONENT_VEC3(names, a, b, w)

#define PSST_MATH_COMPONENT_PERM3(names, a, b, c) \
    PSST_MATH_PERM3_3(names, a, a, a, b, c)       \
    PSST_MATH_PERM3_3(names, a, b, a, b, c)       \
    PSST_MATH_PERM3_3(names, a, c, a, b, c)

#define PSST_MATH_COMPONENT_PERMUTATIONS3(names, a, b, c) \
    PSST_MATH_COMPONENT_PERM3(names, a, b, c)             \
    PSST_MATH_COMPONENT_PERM3(names, b, c, a)             \
    PSST_MATH_COMPONENT_PERM3(names, c, a, b)

#define PSST_MATH_COMPONENT4_PERM4_3(names, a, b, u, v, w, x) \
    PSST_MATH_COMPONENT_VEC3(names, a, b, u)                  \
    PSST_MATH_COMPONENT_VEC3(names, a, b, v)                  \
    PSST_MATH_COMPONENT_VEC3(names, a, b, w)                  \
    PSST_MATH_COMPONENT_VEC3(names, a, b, x)

#define PSST_MATH_COMPONENT4_PERM4_2(names, a, u, v, w, x) \
    PSST_MATH_COMPONENT4_PERM4_3(names, a, u, u, v, w, x)  \
    PSST_MATH_COMPONENT4_PERM4_3(names, a, v, u, v, w, x)  \
    PSST_MATH_COMPONENT4_PERM4_3(names, a, w, u, v, w, x)  \
    PSST_MATH_COMPONENT4_PERM4_3(names, a, x, u, v, w, x)

#define PSST_MATH_COMPONENT4_PERM3(names, a, b, c, d)  \
    PSST_MATH_COMPONENT4_PERM4_2(names, a, a, b, c, d) \
    PSST_MATH_COMPONENT4_PERM4_2(names, b, a, b, c, d) \
    PSST_MATH_COMPONENT4_PERM4_2(names, c, a, b, c, d) \
    PSST_MATH_COMPONENT4_PERM4_2(names, d, a, b, c, d)

#define PSST_MATH_COMPONENT_VEC4(names, a, b, c, d) \
    constexpr vector<value_type, 4, names> a##b##c##d() const { return {a(), b(), c(), d()}; }

#define PSST_MATH_PERM4_4(names, a, b, c, u, v, w, x) \
    PSST_MATH_COMPONENT_VEC4(names, a, b, c, u)       \
    PSST_MATH_COMPONENT_VEC4(names, a, b, c, v)       \
    PSST_MATH_COMPONENT_VEC4(names, a, b, c, w)       \
    PSST_MATH_COMPONENT_VEC4(names, a, b, c, x)

#define PSST_MATH_PERM4_3(names, a, b, u, v, w, x) \
    PSST_MATH_PERM4_4(names, a, b, u, u, v, w, x)  \
    PSST_MATH_PERM4_4(names, a, b, v, u, v, w, x)  \
    PSST_MATH_PERM4_4(names, a, b, w, u, v, w, x)  \
    PSST_MATH_PERM4_4(names, a, b, x, u, v, w, x)

#define PSST_MATH_PERM4_2(names, a, u, v, w, x) \
    PSST_MATH_PERM4_3(names, a, u, u, v, w, x)  \
    PSST_MATH_PERM4_3(names, a, v, u, v, w, x)  \
    PSST_MATH_PERM4_3(names, a, w, u, v, w, x)  \
    PSST_MATH_PERM4_3(names, a, x, u, v, w, x)

#define PSST_MATH_COMPONENT_PERM4(names, a, b, c, d) PSST_MATH_PERM4_2(names, a, a, b, c, d)

#define PSST_MATH_COMPONENT_PERMUTATIONS4(names, a, b, c, d) \
    PSST_MATH_COMPONENT4_PERM3(names, a, b, c, d)            \
    PSST_MATH_COMPONENT_PERM4(names, a, b, c, d)             \
    PSST_MATH_COMPONENT_PERM4(names, b, c, d, a)             \
    PSST_MATH_COMPONENT_PERM4(names, c, d, a, b)             \
    PSST_MATH_COMPONENT_PERM4(names, d, a, b, c)

#endif /* PSST_MATH_DETAIL_PERMUTE_MACROS_HPP_ */
