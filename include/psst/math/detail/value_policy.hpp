/**
 * Copyright 2019 Sergei A. Fedorov
 * value_clamp.hpp
 *
 *  Created on: Jan 5, 2019
 *      Author: ser-fedorov
 */

#ifndef PSST_MATH_DETAIL_VALUE_POLICY_HPP_
#define PSST_MATH_DETAIL_VALUE_POLICY_HPP_

#include <psst/math/detail/utils.hpp>

namespace psst {
namespace math {
namespace value_policy {

template <typename T>
struct no_change {
    using value_type    = T;
    using accessor_type = T&;

    static constexpr value_type const&
    apply(value_type const& val) noexcept
    {
        return val;
    }

    static constexpr value_type&
    accessor(value_type& val) noexcept
    {
        return val;
    }
};

template <typename T, T (*ClampFunction)(T const&) noexcept>
struct value_clamp {
    using value_type    = T;
    using accessor_type = value_clamp;

    value_clamp(value_type& val) noexcept : value{val} {}

    // Implicit conversion to a const reference to the value
    operator T const&() const noexcept { return value; }

    //@{
    value_clamp&
    operator=(value_type const& rhs) noexcept
    {
        value = apply(rhs);
        return *this;
    }

    value_clamp&
    operator+=(value_type const& rhs) noexcept
    {
        value = apply(value + rhs);
        return *this;
    }
    value_clamp&
    operator-=(value_type const& rhs) noexcept
    {
        value = apply(value - rhs);
        return *this;
    }

    value_clamp&
    operator*=(value_type const& rhs) noexcept
    {
        value = apply(value * rhs);
        return *this;
    }
    value_clamp&
    operator/=(value_type const& rhs) noexcept
    {
        value = apply(value / rhs);
        return *this;
    }
    value_clamp&
    operator%=(value_type const& rhs) noexcept
    {
        value = apply(value % rhs);
        return *this;
    }

    value_clamp&
    operator&=(value_type const& rhs) noexcept
    {
        value = apply(value & rhs);
        return *this;
    }
    value_clamp&
    operator|=(value_type const& rhs) noexcept
    {
        value = apply(value | rhs);
        return *this;
    }
    value_clamp&
    operator^=(value_type const& rhs) noexcept
    {
        value = apply(value ^ rhs);
        return *this;
    }
    //@}

    static constexpr value_type
    apply(value_type const& val) noexcept
    {
        return (*ClampFunction)(val);
    }

    static constexpr value_clamp
    accessor(value_type& val) noexcept
    {
        return value_clamp{val};
    }

private:
    value_type& value;
};

//@{
/** @name components_have_value_policies */
template <typename Components, typename = utils::void_t<>>
struct components_have_value_policies : std::false_type {};
template <typename Components>
struct components_have_value_policies<Components,
                                      utils::void_t<typename Components::value_policies>>
    : std::true_type {};
template <typename Components>
using components_have_value_policies_t = typename components_have_value_policies<Components>::type;
template <typename Components>
constexpr bool components_have_value_policies_v
    = components_have_value_policies_t<Components>::value;
//@}

//@{
/** @name components_have_default_value_policy */
template <typename Components, typename T, typename = utils::void_t<>>
struct components_have_default_value_policy : std::false_type {};
template <typename Components, typename T>
struct components_have_default_value_policy<
    Components, T, utils::void_t<typename Components::template default_value_policy<T>>>
    : std::true_type {};
template <typename Components, typename T>
using components_have_default_value_policy_t =
    typename components_have_default_value_policy<Components, T>::type;
template <typename Components, typename T>
constexpr bool components_have_default_value_policy_v
    = components_have_default_value_policy_t<Components, T>::value;
//@}

//@{
/** @name Default value policy */
template <typename Components, typename T, typename = utils::void_t<>>
struct default_value_policy {
    using type = no_change<T>;
};

template <typename Components, typename T>
struct default_value_policy<
    Components, T, std::enable_if_t<components_have_default_value_policy_v<Components, T>>> {
    using type = typename Components::template default_value_policy<T>;
};

template <typename Components, typename T>
using default_value_policy_t = typename default_value_policy<Components, T>::type;
//@}

//@{
template <typename T, typename Components, typename Policies, std::size_t N,
          typename = utils::void_t<>>
struct nth_policy {
    using type = default_value_policy<Components, T>;
};
template <typename T, typename Components, typename Policies, std::size_t N>
struct nth_policy<T, Components, Policies, N, std::enable_if_t<(N < Policies::size)>> {
    using type = typename Policies::template type<N, T>;
};
template <typename T, typename Components, typename Policies, std::size_t N>
using nth_policy_t = typename nth_policy<T, Components, Policies, N>::type;
//@}

template <typename T, typename Components, typename = utils::void_t<>>
struct value_policies {
    using components_value_policy = void;
    template <std::size_t>
    using value_policy = default_value_policy_t<Components, T>;
};

template <typename T, typename Components>
struct value_policies<T, Components,
                      std::enable_if_t<components_have_value_policies_v<Components>>> {
    using components_value_policy = typename Components::value_policies;
    template <std::size_t N>
    using value_policy = nth_policy_t<T, Components, components_value_policy, N>;
};

template <typename T>
constexpr T
zero_to_one(T const& val) noexcept
{
    if (val < T{0})
        return T{0};
    if (val > T{1})
        return T{1};
    return val;
}

template <typename T>
constexpr T
minus_one_to_one(T const& val) noexcept
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

#endif /* PSST_MATH_DETAIL_VALUE_POLICY_HPP_ */
