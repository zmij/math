/*
 * axis_names.hpp
 *
 *  Created on: 18 дек. 2016 г.
 *      Author: sergey.fedorov
 */

#ifndef PUSHKIN_MATH_DETAIL_AXIS_ACCESS_HPP_
#define PUSHKIN_MATH_DETAIL_AXIS_ACCESS_HPP_

#include <pushkin/math/detail/permute_macros.hpp>
#include <pushkin/math/detail/value_policy.hpp>
#include <pushkin/math/detail/value_traits.hpp>

#include <cstddef>
#include <type_traits>

namespace psst {
namespace math {

namespace axes {

template <typename VectorType, typename T, typename Axes>
struct basic_axis_access {
    using expression_type  = VectorType;
    using value_type       = typename std::decay<T>::type;
    using coord_names      = Axes;
    using value_policies   = value_policy::value_policies<T, Axes>;
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
 * Primary axis access template
 */
template <std::size_t Size, typename Axes, typename VectorType, typename T>
struct axis_access : axis_access<Size - 1, Axes, VectorType, T> {};

/**
 * Terminal axis access template
 */
template <typename Axes, typename VectorType, typename T>
struct axis_access<0, Axes, VectorType, T> {};

//@{
/** @name xyzw axes names */
template <typename VectorType, typename T>
struct axis_access<1, axes::xyzw, VectorType, T> : basic_axis_access<VectorType, T, axes::xyzw> {

    using base_type = basic_axis_access<VectorType, T, axes::xyzw>;

    PSST_MATH_COORD_ACCESS(x)
};

template <typename VectorType, typename T>
struct axis_access<2, axes::xyzw, VectorType, T> : axis_access<1, axes::xyzw, VectorType, T> {

    using base_type  = axis_access<1, axes::xyzw, VectorType, T>;
    using value_type = typename base_type::value_type;

    using base_type::x;

    PSST_MATH_COORD_ACCESS(y)

    //@{
    /** @name Coordinate permutations */
    PSST_MATH_COORD_PERMUTATION2(axes::xyzw, x, y);
    //@}
};

template <typename VectorType, typename T>
struct axis_access<3, axes::xyzw, VectorType, T> : axis_access<2, axes::xyzw, VectorType, T> {

    using base_type  = axis_access<2, axes::xyzw, VectorType, T>;
    using value_type = typename base_type::value_type;

    using base_type::x;
    using base_type::y;

    PSST_MATH_COORD_ACCESS(z)

    //@{
    /** @name Coordinate permutations */
    PSST_MATH_COORD_PERMUTATION2(axes::xyzw, x, z)
    PSST_MATH_COORD_PERM2(axes::xyzw, y, z)
    PSST_MATH_COORD_PERMUTATIONS3(axes::xyzw, x, y, z)
    //@}
};

template <typename VectorType, typename T>
struct axis_access<4, axes::xyzw, VectorType, T> : axis_access<3, axes::xyzw, VectorType, T> {

    using base_type  = axis_access<3, axes::xyzw, VectorType, T>;
    using value_type = typename base_type::value_type;

    using base_type::x;
    using base_type::y;
    using base_type::z;

    PSST_MATH_COORD_ACCESS(w)

    //@{
    /** @name Coordinate permutations */
    PSST_MATH_COORD_PERMUTATION2(axes::xyzw, x, w)
    PSST_MATH_COORD_PERM2(axes::xyzw, y, w)
    PSST_MATH_COORD_PERM2(axes::xyzw, z, w)
    PSST_MATH_COORD_PERMUTATIONS4(axes::xyzw, x, y, z, w)
    //@}
};
//@}

}    // namespace axes

namespace detail {

template <typename Tag>
struct axes_names {
    template <std::size_t AxesCount, typename VectorType, typename T>
    using type = axes::axis_access<AxesCount, Tag, VectorType, T>;
};

template <>
struct axes_names<axes::none> {
    template <std::size_t AxesCount, typename VectorType, typename T>
    using type = axes::basic_axis_access<VectorType, T, axes::none>;
};

template <std::size_t Size, typename Axes, typename VectorType, typename T>
using axes_names_t = typename axes_names<Axes>::template type<Size, VectorType, T>;

} /* namespace detail */

} /* namespace math */
} /* namespace psst */

#endif /* PUSHKIN_MATH_DETAIL_AXIS_ACCESS_HPP_ */
