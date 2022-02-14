/*
 * component_names.hpp
 *
 *  Created on: 18 дек. 2016 г.
 *      Author: sergey.fedorov
 */

#ifndef PSST_MATH_DETAIL_COMPONENT_ACCESS_HPP_
#define PSST_MATH_DETAIL_COMPONENT_ACCESS_HPP_

#include <psst/math/detail/permute_macros.hpp>
#include <psst/math/detail/value_policy.hpp>
#include <psst/math/detail/value_traits.hpp>

#include <cstddef>
#include <type_traits>

namespace psst {
namespace math {

namespace component_access {

template <typename VectorType, typename T, typename Components>
struct basic_component_access {
    using expression_type  = VectorType;
    using value_type       = typename std::decay<T>::type;
    using coord_names      = Components;
    using value_policies   = value_policy::value_policies<T, Components>;
    using lvalue_reference = typename std::add_lvalue_reference<value_type>::type;
    using const_reference =
        typename std::add_lvalue_reference<typename std::add_const<value_type>::type>::type;

    template <std::size_t N>
    using value_policy = typename value_policies::template value_policy<N>;

    template <std::size_t N>
    using accessor_type = typename value_policy<N>::accessor_type;

protected:
    expression_type&
    rebind()
    {
        return static_cast<expression_type&>(*this);
    }
    constexpr expression_type const&
    rebind() const
    {
        return static_cast<expression_type const&>(*this);
    }
};

/**
 * Primary component access template
 */
template <std::size_t Size, typename Components, typename VectorType, typename T>
struct component_access : component_access<Size - 1, Components, VectorType, T> {};

/**
 * Terminal component access template
 */
template <typename Components, typename VectorType, typename T>
struct component_access<0, Components, VectorType, T> {};

//@{
/** @name xyzw components names */
template <typename VectorType, typename T>
struct component_access<1, components::xyzw, VectorType, T>
    : basic_component_access<VectorType, T, components::xyzw> {

    using base_type = basic_component_access<VectorType, T, components::xyzw>;

    PSST_MATH_COMPONENT_ACCESS(x)
};

template <typename VectorType, typename T>
struct component_access<2, components::xyzw, VectorType, T>
    : component_access<1, components::xyzw, VectorType, T> {

    using base_type  = component_access<1, components::xyzw, VectorType, T>;
    using value_type = typename base_type::value_type;

    using base_type::x;

    PSST_MATH_COMPONENT_ACCESS(y)

    //@{
    /** @name Coordinate permutations */
    PSST_MATH_COMPONENT_PERMUTATION2(components::xyzw, x, y);
    //@}
};

template <typename VectorType, typename T>
struct component_access<3, components::xyzw, VectorType, T>
    : component_access<2, components::xyzw, VectorType, T> {

    using base_type  = component_access<2, components::xyzw, VectorType, T>;
    using value_type = typename base_type::value_type;

    using base_type::x;
    using base_type::y;

    PSST_MATH_COMPONENT_ACCESS(z)

    //@{
    /** @name Coordinate permutations */
    PSST_MATH_COMPONENT_PERMUTATION2(components::xyzw, x, z)
    PSST_MATH_COMPONENT_PERM2(components::xyzw, y, z)
    PSST_MATH_COMPONENT_PERMUTATIONS3(components::xyzw, x, y, z)
    //@}
};

template <typename VectorType, typename T>
struct component_access<4, components::xyzw, VectorType, T>
    : component_access<3, components::xyzw, VectorType, T> {

    using base_type  = component_access<3, components::xyzw, VectorType, T>;
    using value_type = typename base_type::value_type;

    using base_type::x;
    using base_type::y;
    using base_type::z;

    PSST_MATH_COMPONENT_ACCESS(w)

    //@{
    /** @name Coordinate permutations */
    PSST_MATH_COMPONENT_PERMUTATION2(components::xyzw, x, w)
    PSST_MATH_COMPONENT_PERM2(components::xyzw, y, w)
    PSST_MATH_COMPONENT_PERM2(components::xyzw, z, w)
    PSST_MATH_COMPONENT_PERMUTATIONS4(components::xyzw, x, y, z, w)
    //@}
};
//@}

//@{
/** @name xyw components names */
template <typename VectorType, typename T>
struct component_access<1, components::xyw, VectorType, T>
    : basic_component_access<VectorType, T, components::xyw> {

    using base_type = basic_component_access<VectorType, T, components::xyw>;

    PSST_MATH_COMPONENT_ACCESS(x)
};

template <typename VectorType, typename T>
struct component_access<2, components::xyw, VectorType, T>
    : component_access<1, components::xyw, VectorType, T> {

    using base_type  = component_access<1, components::xyw, VectorType, T>;
    using value_type = typename base_type::value_type;

    using base_type::x;

    PSST_MATH_COMPONENT_ACCESS(y)

    //@{
    /** @name Coordinate permutations */
    PSST_MATH_COMPONENT_PERMUTATION2(components::xyw, x, y);
    //@}
};

template <typename VectorType, typename T>
struct component_access<3, components::xyw, VectorType, T>
    : component_access<2, components::xyw, VectorType, T> {

    using base_type  = component_access<2, components::xyw, VectorType, T>;
    using value_type = typename base_type::value_type;

    using base_type::x;
    using base_type::y;

    PSST_MATH_COMPONENT_ACCESS(w)

    //@{
    /** @name Coordinate permutations */
    PSST_MATH_COMPONENT_PERMUTATION2(components::xyw, x, w)
    PSST_MATH_COMPONENT_PERM2(components::xyw, y, w)
    PSST_MATH_COMPONENT_PERMUTATIONS3(components::xyw, x, y, w)
    //@}
};
//@}

}    // namespace component_access

namespace detail {

template <typename Tag>
struct component_names {
    template <std::size_t ComponentCount, typename VectorType, typename T>
    using type = component_access::component_access<ComponentCount, Tag, VectorType, T>;
};

template <>
struct component_names<components::none> {
    template <std::size_t ComponentCount, typename VectorType, typename T>
    using type = component_access::basic_component_access<VectorType, T, components::none>;
};

template <std::size_t Size, typename Components, typename VectorType, typename T>
using component_names_t = typename component_names<Components>::template type<Size, VectorType, T>;

}    // namespace detail

}    // namespace math */
}    // namespace psst */

#endif /* PSST_MATH_DETAIL_COMPONENT_ACCESS_HPP_ */
