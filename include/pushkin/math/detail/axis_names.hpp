/*
 * axis_names.hpp
 *
 *  Created on: 18 дек. 2016 г.
 *      Author: sergey.fedorov
 */

#ifndef INCLUDE_PUSHKIN_MATH_DETAIL_AXIS_NAMES_HPP_
#define INCLUDE_PUSHKIN_MATH_DETAIL_AXIS_NAMES_HPP_

#include <cstddef>
#include <type_traits>
#include <pushkin/math/vector_fwd.hpp>
#include <pushkin/math/detail/permute_macros.hpp>

namespace psst {
namespace math {

namespace detail {

template < typename VectorBuilder, typename T >
struct basic_axis_access {
    using builder_type          = VectorBuilder;
    using value_type            = typename ::std::decay<T>::type;
    using lvalue_reference      = typename ::std::add_lvalue_reference<value_type>::type;
    using const_reference        = typename ::std::add_lvalue_reference<
                                            typename ::std::add_const<value_type>::type>::type;
protected:
    builder_type&
    rebind()
    {
        return static_cast<builder_type&>(*this);
    }
    builder_type const&
    rebind() const
    {
        return static_cast<builder_type const&>(*this);
    }
};

/**
 * Primary axis access template
 */
template < ::std::size_t AxesCount, typename Axes, typename VectorBuilder, typename T >
struct axis_access : axis_access< AxesCount - 1, Axes, VectorBuilder, T > {};

/**
 * Terminal axis access template
 */
template < typename Axes, typename VectorBuilder, typename T >
struct axis_access< 0, Axes, VectorBuilder, T > {};

//@{
/** @name xyzw axes names */
template < typename VectorBuilder, typename T >
struct axis_access< 1, axes::xyzw, VectorBuilder, T >
    : basic_axis_access< VectorBuilder, T > {

    using base_type     = basic_axis_access<VectorBuilder, T>;
    using coord_names   = axes::xyzw;

    PSST_MATH_COORD_ACCESS(x, 0)
};

template < typename VectorBuilder, typename T >
struct axis_access< 2, axes::xyzw, VectorBuilder, T >
    : axis_access< 1, axes::xyzw, VectorBuilder, T > {

    using base_type = axis_access<1, axes::xyzw, VectorBuilder, T>;
    using value_type = typename base_type::value_type;

    using base_type::x;

    PSST_MATH_COORD_ACCESS(y, 1)

    //@{
    /** @name Coordinate permutations */
    PSST_MATH_COORD_PERMUTATION2(axes::xyzw, x, y);
    //@}
};

template < typename VectorBuilder, typename T >
struct axis_access< 3, axes::xyzw, VectorBuilder, T >
    : axis_access< 2, axes::xyzw, VectorBuilder, T > {

    using base_type = axis_access<2, axes::xyzw, VectorBuilder, T>;
    using value_type = typename base_type::value_type;

    using base_type::x;
    using base_type::y;

    PSST_MATH_COORD_ACCESS(z, 2)

    //@{
    /** @name Coordinate permutations */
    PSST_MATH_COORD_PERMUTATION2(axes::xyzw, x, z)
    PSST_MATH_COORD_PERM2(axes::xyzw, y, z)
    PSST_MATH_COORD_PERMUTATIONS3(axes::xyzw, x, y, z)
    //@}
};

template < typename VectorBuilder, typename T >
struct axis_access< 4, axes::xyzw, VectorBuilder, T >
    : axis_access< 3, axes::xyzw, VectorBuilder, T > {

    using base_type = axis_access<3, axes::xyzw, VectorBuilder, T>;
    using value_type = typename base_type::value_type;

    using base_type::x;
    using base_type::y;
    using base_type::z;

    PSST_MATH_COORD_ACCESS(w, 3)

    //@{
    /** @name Coordinate permutations */
    PSST_MATH_COORD_PERMUTATION2(axes::xyzw, x, w)
    PSST_MATH_COORD_PERM2(axes::xyzw, y, w)
    PSST_MATH_COORD_PERM2(axes::xyzw, z, w)
    PSST_MATH_COORD_PERMUTATIONS4(axes::xyzw, x, y, z, w)
    //@}
};
//@}

//@{
/** @name wxyz axes names */
template < typename VectorBuilder, typename T >
struct axis_access< 1, axes::wxyz, VectorBuilder, T >
    : basic_axis_access< VectorBuilder, T > {

    using base_type     = basic_axis_access<VectorBuilder, T>;
    using coord_names   = axes::wxyz;

    PSST_MATH_COORD_ACCESS(w, 0)
};

template < typename VectorBuilder, typename T >
struct axis_access< 2, axes::wxyz, VectorBuilder, T >
    : axis_access< 1, axes::wxyz, VectorBuilder, T > {

    using base_type = axis_access<1, axes::wxyz, VectorBuilder, T>;
    using value_type = typename base_type::value_type;

    using base_type::w;

    PSST_MATH_COORD_ACCESS(x, 1)

    //@{
    /** @name Coordinate permutations */
    PSST_MATH_COORD_PERMUTATION2(axes::wxyz, w, x);
    //@}
};

template < typename VectorBuilder, typename T >
struct axis_access< 3, axes::wxyz, VectorBuilder, T >
    : axis_access< 2, axes::wxyz, VectorBuilder, T > {

    using base_type = axis_access<2, axes::wxyz, VectorBuilder, T>;
    using value_type = typename base_type::value_type;

    using base_type::w;
    using base_type::x;

    PSST_MATH_COORD_ACCESS(y, 2)

    //@{
    /** @name Coordinate permutations */
    PSST_MATH_COORD_PERMUTATION2(axes::wxyz, x, y)
    PSST_MATH_COORD_PERM2(axes::wxyz, w, y)
    PSST_MATH_COORD_PERMUTATIONS3(axes::wxyz, w, x, y)
    //@}
};

template < typename VectorBuilder, typename T >
struct axis_access< 4, axes::wxyz, VectorBuilder, T >
    : axis_access< 3, axes::wxyz, VectorBuilder, T > {

    using base_type = axis_access<3, axes::wxyz, VectorBuilder, T>;
    using value_type = typename base_type::value_type;

    using base_type::w;
    using base_type::x;
    using base_type::y;

    PSST_MATH_COORD_ACCESS(z, 3)

    //@{
    /** @name Coordinate permutations */
    PSST_MATH_COORD_PERMUTATION2(axes::wxyz, x, z)
    PSST_MATH_COORD_PERM2(axes::wxyz, w, z)
    PSST_MATH_COORD_PERM2(axes::wxyz, y, z)
    PSST_MATH_COORD_PERMUTATIONS4(axes::wxyz, w, x, y, z)
    //@}
};
//@}

//@{
/** @name axes::argb axes names */
template < typename VectorBuilder, typename T >
struct axis_access< 1, axes::argb, VectorBuilder, T >
    : basic_axis_access< VectorBuilder, T > {

    using base_type     = basic_axis_access<VectorBuilder, T>;
    using coord_names   = axes::argb;

    PSST_MATH_COORD_ACCESS(a, 0)
};

template < typename VectorBuilder, typename T >
struct axis_access< 2, axes::argb, VectorBuilder, T >
    : axis_access< 1, axes::argb, VectorBuilder, T > {

    using base_type = basic_axis_access<VectorBuilder, T>;

    PSST_MATH_COORD_ACCESS(r, 1)
};

template < typename VectorBuilder, typename T >
struct axis_access< 3, axes::argb, VectorBuilder, T >
    : axis_access< 2, axes::argb, VectorBuilder, T > {

    using base_type = basic_axis_access<VectorBuilder, T>;

    PSST_MATH_COORD_ACCESS(g, 2)
};

template < typename VectorBuilder, typename T >
struct axis_access< 4, axes::argb, VectorBuilder, T >
    : axis_access< 3, axes::argb, VectorBuilder, T > {

    using base_type = axis_access<3, axes::argb, VectorBuilder, T>;
    using value_type = typename base_type::value_type;

    PSST_MATH_COORD_ACCESS(b, 3)

    using base_type::a;
    using base_type::r;
    using base_type::g;

    vector<value_type, 4, axes::rgba>
    rgba() const
    {
        return { r(), g(), b(), a() };
    }
};
//@}

//@{
/** @name axes::rgba axes names */
template < typename VectorBuilder, typename T >
struct axis_access< 1, axes::rgba, VectorBuilder, T >
    : basic_axis_access< VectorBuilder, T > {

    using base_type     = basic_axis_access<VectorBuilder, T>;
    using coord_names   = axes::rgba;

    PSST_MATH_COORD_ACCESS(r, 0)
};

template < typename VectorBuilder, typename T >
struct axis_access< 2, axes::rgba, VectorBuilder, T >
    : axis_access< 1, axes::rgba, VectorBuilder, T > {

    using base_type = basic_axis_access<VectorBuilder, T>;

    PSST_MATH_COORD_ACCESS(g, 1)
};

template < typename VectorBuilder, typename T >
struct axis_access< 3, axes::rgba, VectorBuilder, T >
    : axis_access< 2, axes::rgba, VectorBuilder, T > {

    using base_type = basic_axis_access<VectorBuilder, T>;

    PSST_MATH_COORD_ACCESS(b, 2)
};

template < typename VectorBuilder, typename T >
struct axis_access< 4, axes::rgba, VectorBuilder, T >
    : axis_access< 3, axes::rgba, VectorBuilder, T > {

    using base_type = axis_access<3, axes::rgba, VectorBuilder, T>;
    using value_type = typename base_type::value_type;

    using base_type::r;
    using base_type::g;
    using base_type::b;

    PSST_MATH_COORD_ACCESS(a, 3)

    vector<value_type, 4, math::axes::argb>
    argb() const
    {
        return { a(), r(), g(), b() };
    }
};
//@}

//@{
/** @name axes::hsva axes names */
template < typename VectorBuilder, typename T >
struct axis_access< 1, axes::hsva, VectorBuilder, T >
    : basic_axis_access< VectorBuilder, T > {

    using base_type     = basic_axis_access<VectorBuilder, T>;
    using coord_names   = axes::hsva;

    PSST_MATH_COORD_ACCESS(h, 0)
};

template < typename VectorBuilder, typename T >
struct axis_access< 2, axes::hsva, VectorBuilder, T >
    : axis_access< 1, axes::hsva, VectorBuilder, T > {

    using base_type = basic_axis_access<VectorBuilder, T>;

    PSST_MATH_COORD_ACCESS(s, 1)
};

template < typename VectorBuilder, typename T >
struct axis_access< 3, axes::hsva, VectorBuilder, T >
    : axis_access< 2, axes::hsva, VectorBuilder, T > {

    using base_type = basic_axis_access<VectorBuilder, T>;

    PSST_MATH_COORD_ACCESS(v, 2)
};

template < typename VectorBuilder, typename T >
struct axis_access< 4, axes::hsva, VectorBuilder, T >
    : axis_access< 3, axes::hsva, VectorBuilder, T > {

    using base_type = basic_axis_access<VectorBuilder, T>;

    PSST_MATH_COORD_ACCESS(a, 3)
};
//@}

//@{
/** @name axes::hsla axes names */
template < typename VectorBuilder, typename T >
struct axis_access< 1, axes::hsla, VectorBuilder, T >
    : basic_axis_access< VectorBuilder, T > {

    using base_type     = basic_axis_access<VectorBuilder, T>;
    using coord_names   = axes::hsla;

    PSST_MATH_COORD_ACCESS(h, 0)
};

template < typename VectorBuilder, typename T >
struct axis_access< 2, axes::hsla, VectorBuilder, T >
    : axis_access< 1, axes::hsla, VectorBuilder, T > {

    using base_type = basic_axis_access<VectorBuilder, T>;

    PSST_MATH_COORD_ACCESS(s, 1)
};

template < typename VectorBuilder, typename T >
struct axis_access< 3, axes::hsla, VectorBuilder, T >
    : axis_access< 2, axes::hsla, VectorBuilder, T > {

    using base_type = basic_axis_access<VectorBuilder, T>;

    PSST_MATH_COORD_ACCESS(l, 2)
};

template < typename VectorBuilder, typename T >
struct axis_access< 4, axes::hsla, VectorBuilder, T >
    : axis_access< 3, axes::hsla, VectorBuilder, T > {

    using base_type = basic_axis_access<VectorBuilder, T>;

    PSST_MATH_COORD_ACCESS(a, 3)
};
//@}

template < typename Tag >
struct axes_names {
    template < ::std::size_t AxesCount, typename BuilderType, typename T >
    using type = basic_axis_access<BuilderType, T>;
};

template <>
struct axes_names<axes::xyzw> {
    template < ::std::size_t AxesCount, typename BuilderType, typename T >
    using type = axis_access< AxesCount, axes::xyzw, BuilderType, T>;
};

template <>
struct axes_names<axes::argb> {
    template < ::std::size_t AxesCount, typename BuilderType, typename T >
    using type = axis_access< AxesCount, axes::argb, BuilderType, T>;
};

template <>
struct axes_names<axes::rgba> {
    template < ::std::size_t AxesCount, typename BuilderType, typename T >
    using type = axis_access< AxesCount, axes::rgba, BuilderType, T>;
};

template <>
struct axes_names<axes::hsva> {
    template < ::std::size_t AxesCount, typename BuilderType, typename T >
    using type = axis_access< AxesCount, axes::hsva, BuilderType, T>;
};

template <>
struct axes_names<axes::hsla> {
    template < ::std::size_t AxesCount, typename BuilderType, typename T >
    using type = axis_access< AxesCount, axes::hsla, BuilderType, T>;
};

}  /* namespace detail */

}  /* namespace math */
}  /* namespace psst */


#endif /* INCLUDE_PUSHKIN_MATH_DETAIL_AXIS_NAMES_HPP_ */
