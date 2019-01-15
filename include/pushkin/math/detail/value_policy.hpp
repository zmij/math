/**
 * Copyright 2019 Sergei A. Fedorov
 * value_clamp.hpp
 *
 *  Created on: Jan 5, 2019
 *      Author: ser-fedorov
 */

#ifndef PUSHKIN_MATH_DETAIL_VALUE_POLICY_HPP_
#define PUSHKIN_MATH_DETAIL_VALUE_POLICY_HPP_

#include <pushkin/math/detail/utils.hpp>

namespace psst {
namespace math {
namespace value_policy {

template <typename T>
struct no_change {
    using value_type    = T;
    using accessor_type = T&;

    static constexpr value_type const&
    apply(value_type const& val)
    {
        return val;
    }

    static constexpr value_type&
    accessor(value_type& val)
    {
        return val;
    }
};

template <typename T, T (*ClampFunction)(T const&)>
struct value_clamp {
    using value_type    = T;
    using accessor_type = value_clamp;

    value_clamp(value_type& val) : value{val} {}

    // Implicit conversion to a const reference to the value
    operator T const&() const { return value; }

    //@{
    value_clamp&
    operator=(value_type const& rhs)
    {
        value = apply(rhs);
        return *this;
    }

    value_clamp&
    operator+=(value_type const& rhs)
    {
        value = apply(value + rhs);
        return *this;
    }
    value_clamp&
    operator-=(value_type const& rhs)
    {
        value = apply(value - rhs);
        return *this;
    }

    value_clamp&
    operator*=(value_type const& rhs)
    {
        value = apply(value * rhs);
        return *this;
    }
    value_clamp&
    operator/=(value_type const& rhs)
    {
        value = apply(value / rhs);
        return *this;
    }
    value_clamp&
    operator%=(value_type const& rhs)
    {
        value = apply(value % rhs);
        return *this;
    }

    value_clamp&
    operator&=(value_type const& rhs)
    {
        value = apply(value & rhs);
        return *this;
    }
    value_clamp&
    operator|=(value_type const& rhs)
    {
        value = apply(value | rhs);
        return *this;
    }
    value_clamp&
    operator^=(value_type const& rhs)
    {
        value = apply(value ^ rhs);
        return *this;
    }
    //@}

    static constexpr value_type
    apply(value_type const& val)
    {
        return (*ClampFunction)(val);
    }

    static constexpr value_clamp
    accessor(value_type& val)
    {
        return value_clamp{val};
    }

private:
    value_type& value;
};

//@{
/** @name axes_have_value_policies */
template <typename Axes, typename = utils::void_t<>>
struct axes_have_value_policies : std::false_type {};
template <typename Axes>
struct axes_have_value_policies<Axes, utils::void_t<typename Axes::value_policies>>
    : std::true_type {};
template <typename Axes>
using axes_have_value_policies_t = typename axes_have_value_policies<Axes>::type;
template <typename Axes>
constexpr bool axes_have_value_policies_v = axes_have_value_policies_t<Axes>::value;
//@}

//@{
/** @name axes_have_default_value_policy */
template <typename Axes, typename T, typename = utils::void_t<>>
struct axes_have_default_value_policy : std::false_type {};
template <typename Axes, typename T>
struct axes_have_default_value_policy<
    Axes, T, utils::void_t<typename Axes::template default_value_policy<T>>> : std::true_type {};
template <typename Axes, typename T>
using axes_have_default_value_policy_t = typename axes_have_default_value_policy<Axes, T>::type;
template <typename Axes, typename T>
constexpr bool axes_have_default_value_policy_v = axes_have_default_value_policy_t<Axes, T>::value;
//@}

//@{
/** @name Default value policy */
template <typename Axes, typename T, typename = utils::void_t<>>
struct default_value_policy {
    using type = no_change<T>;
};

template <typename Axes, typename T>
struct default_value_policy<Axes, T, std::enable_if_t<axes_have_default_value_policy_v<Axes, T>>> {
    using type = typename Axes::template default_value_policy<T>;
};

template <typename Axes, typename T>
using default_value_policy_t = typename default_value_policy<Axes, T>::type;
//@}

//@{
template <typename T, typename Axes, typename Policies, std::size_t N, typename = utils::void_t<>>
struct nth_policy {
    using type = default_value_policy<Axes, T>;
};
template <typename T, typename Axes, typename Policies, std::size_t N>
struct nth_policy<T, Axes, Policies, N, std::enable_if_t<(N < Policies::size)>> {
    using type = typename Policies::template type<N, T>;
};
template <typename T, typename Axes, typename Policies, std::size_t N>
using nth_policy_t = typename nth_policy<T, Axes, Policies, N>::type;
//@}

template <typename T, typename Axes, typename = utils::void_t<>>
struct value_policies {
    using axes_value_policy = void;
    template <std::size_t>
    using value_policy = default_value_policy_t<Axes, T>;
};

template <typename T, typename Axes>
struct value_policies<T, Axes, std::enable_if_t<axes_have_value_policies_v<Axes>>> {
    using axes_value_policy = typename Axes::value_policies;
    template <std::size_t N>
    using value_policy = nth_policy_t<T, Axes, axes_value_policy, N>;
};

template <typename T>
constexpr T
zero_to_one(T const& val)
{
    if (val < T{0})
        return T{0};
    if (val > T{1})
        return T{1};
    return val;
}

template <typename T>
constexpr T
minus_one_to_one(T const& val)
{
    if (val < T{-1})
        return T{-1};
    if (val > T{1})
        return T{1};
    return val;
}

template <typename T>
using clamp_zero_to_one = value_clamp<T, zero_to_one<T>>;
template <typename T>
using clamp_minus_one_to_one = value_clamp<T, minus_one_to_one<T>>;

}    // namespace value_policy
}    // namespace math
}    // namespace psst

#endif /* PUSHKIN_MATH_DETAIL_VALUE_POLICY_HPP_ */
