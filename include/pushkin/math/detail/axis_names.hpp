/*
 * axis_names.hpp
 *
 *  Created on: 18 дек. 2016 г.
 *      Author: sergey.fedorov
 */

#ifndef INCLUDE_PUSHKIN_MATH_DETAIL_AXIS_NAMES_HPP_
#define INCLUDE_PUSHKIN_MATH_DETAIL_AXIS_NAMES_HPP_

#include <pushkin/math/detail/permute_macros.hpp>
#include <pushkin/math/vector_fwd.hpp>

#include <cstddef>
#include <type_traits>

namespace psst {
namespace math {

namespace detail {

template <typename VectorType, typename T>
struct basic_axis_access {
    using builder_type     = VectorType;
    using value_type       = typename std::decay<T>::type;
    using lvalue_reference = typename std::add_lvalue_reference<value_type>::type;
    using const_reference =
        typename std::add_lvalue_reference<typename std::add_const<value_type>::type>::type;

protected:
    builder_type&
    rebind()
    {
        return static_cast<builder_type&>(*this);
    }
    constexpr builder_type const&
    rebind() const
    {
        return static_cast<builder_type const&>(*this);
    }
};

/**
 * Primary axis access template
 */
template <std::size_t AxesCount, typename Axes, typename VectorType, typename T>
struct axis_access : axis_access<AxesCount - 1, Axes, VectorType, T> {};

/**
 * Terminal axis access template
 */
template <typename Axes, typename VectorType, typename T>
struct axis_access<0, Axes, VectorType, T> {};

//@{
/** @name xyzw axes names */
template <typename VectorType, typename T>
struct axis_access<1, axes::xyzw, VectorType, T> : basic_axis_access<VectorType, T> {

    using base_type   = basic_axis_access<VectorType, T>;
    using coord_names = axes::xyzw;

    PSST_MATH_COORD_ACCESS(x, 0)
};

template <typename VectorType, typename T>
struct axis_access<2, axes::xyzw, VectorType, T> : axis_access<1, axes::xyzw, VectorType, T> {

    using base_type  = axis_access<1, axes::xyzw, VectorType, T>;
    using value_type = typename base_type::value_type;

    using base_type::x;

    PSST_MATH_COORD_ACCESS(y, 1)

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

    PSST_MATH_COORD_ACCESS(z, 2)

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
template <typename VectorType, typename T>
struct axis_access<1, axes::wxyz, VectorType, T> : basic_axis_access<VectorType, T> {

    using base_type   = basic_axis_access<VectorType, T>;
    using coord_names = axes::wxyz;

    PSST_MATH_COORD_ACCESS(w, 0)
};

template <typename VectorType, typename T>
struct axis_access<2, axes::wxyz, VectorType, T> : axis_access<1, axes::wxyz, VectorType, T> {

    using base_type  = axis_access<1, axes::wxyz, VectorType, T>;
    using value_type = typename base_type::value_type;

    using base_type::w;

    PSST_MATH_COORD_ACCESS(x, 1)

    //@{
    /** @name Coordinate permutations */
    PSST_MATH_COORD_PERMUTATION2(axes::wxyz, w, x);
    //@}
};

template <typename VectorType, typename T>
struct axis_access<3, axes::wxyz, VectorType, T> : axis_access<2, axes::wxyz, VectorType, T> {

    using base_type  = axis_access<2, axes::wxyz, VectorType, T>;
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

template <typename VectorType, typename T>
struct axis_access<4, axes::wxyz, VectorType, T> : axis_access<3, axes::wxyz, VectorType, T> {

    using base_type  = axis_access<3, axes::wxyz, VectorType, T>;
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
/** @name Polar coordinates */
template <typename VectorType, typename T>
struct axis_access<1, axes::polar, VectorType, T> : basic_axis_access<VectorType, T> {

    using base_type   = basic_axis_access<VectorType, T>;
    using coord_names = axes::polar;

    PSST_MATH_COORD_ACCESS(r, 0);
    PSST_MATH_COORD_ACCESS(rho, 0);
};
template <typename VectorType, typename T>
struct axis_access<2, axes::polar, VectorType, T> : axis_access<1, axes::polar, VectorType, T> {
    using base_type = axis_access<1, axes::polar, VectorType, T>;

    using base_type::r;
    using base_type::rho;

    PSST_MATH_COORD_ACCESS(phi, 1);
    PSST_MATH_COORD_ACCESS(azimuth, 1);
};
//@}

//@{
/** @name Spherical coordinates */
template <typename VectorType, typename T>
struct axis_access<1, axes::spherical, VectorType, T> : basic_axis_access<VectorType, T> {

    using base_type   = basic_axis_access<VectorType, T>;
    using coord_names = axes::spherical;

    PSST_MATH_COORD_ACCESS(r, 0);
    PSST_MATH_COORD_ACCESS(rho, 0);
};
template <typename VectorType, typename T>
struct axis_access<2, axes::spherical, VectorType, T>
    : axis_access<1, axes::spherical, VectorType, T> {
    using base_type = axis_access<1, axes::spherical, VectorType, T>;

    using base_type::r;
    using base_type::rho;

    PSST_MATH_COORD_ACCESS(phi, 1);
    PSST_MATH_COORD_ACCESS(inclination, 1);
};
template <typename VectorType, typename T>
struct axis_access<3, axes::spherical, VectorType, T>
    : axis_access<2, axes::spherical, VectorType, T> {
    using base_type = axis_access<2, axes::spherical, VectorType, T>;

    using base_type::inclination;
    using base_type::phi;
    using base_type::r;
    using base_type::rho;

    PSST_MATH_COORD_ACCESS(theta, 2);
    PSST_MATH_COORD_ACCESS(azimuth, 2);
};
//@}

//@{
/** @name Cylindrical coordinates */
template <typename VectorType, typename T>
struct axis_access<1, axes::cylindrical, VectorType, T> : basic_axis_access<VectorType, T> {

    using base_type   = basic_axis_access<VectorType, T>;
    using coord_names = axes::cylindrical;

    PSST_MATH_COORD_ACCESS(r, 0);
    PSST_MATH_COORD_ACCESS(rho, 0);
};
template <typename VectorType, typename T>
struct axis_access<2, axes::cylindrical, VectorType, T>
    : axis_access<1, axes::cylindrical, VectorType, T> {
    using base_type = axis_access<1, axes::cylindrical, VectorType, T>;

    using base_type::r;
    using base_type::rho;

    PSST_MATH_COORD_ACCESS(phi, 1);
    PSST_MATH_COORD_ACCESS(azimuth, 1);
};
template <typename VectorType, typename T>
struct axis_access<3, axes::cylindrical, VectorType, T>
    : axis_access<2, axes::cylindrical, VectorType, T> {
    using base_type = axis_access<2, axes::cylindrical, VectorType, T>;

    using base_type::azimuth;
    using base_type::phi;
    using base_type::r;
    using base_type::rho;

    PSST_MATH_COORD_ACCESS(z, 2);
    PSST_MATH_COORD_ACCESS(elevation, 2);
};
//@}

//@{
/** @name axes::argb axes names */
template <typename VectorType, typename T>
struct axis_access<1, axes::argb, VectorType, T> : basic_axis_access<VectorType, T> {

    using base_type   = basic_axis_access<VectorType, T>;
    using coord_names = axes::argb;

    PSST_MATH_COORD_ACCESS(a, 0)
};

template <typename VectorType, typename T>
struct axis_access<2, axes::argb, VectorType, T> : axis_access<1, axes::argb, VectorType, T> {

    using base_type = axis_access<1, axes::argb, VectorType, T>;

    using base_type::a;

    PSST_MATH_COORD_ACCESS(r, 1)
};

template <typename VectorType, typename T>
struct axis_access<3, axes::argb, VectorType, T> : axis_access<2, axes::argb, VectorType, T> {

    using base_type = axis_access<2, axes::argb, VectorType, T>;

    using base_type::a;
    using base_type::r;

    PSST_MATH_COORD_ACCESS(g, 2)
};

template <typename VectorType, typename T>
struct axis_access<4, axes::argb, VectorType, T> : axis_access<3, axes::argb, VectorType, T> {

    using base_type  = axis_access<3, axes::argb, VectorType, T>;
    using value_type = typename base_type::value_type;

    using base_type::a;
    using base_type::g;
    using base_type::r;

    PSST_MATH_COORD_ACCESS(b, 3)

    vector<value_type, 4, axes::rgba>
    rgba() const
    {
        return {r(), g(), b(), a()};
    }
};
//@}

//@{
/** @name axes::rgba axes names */
template <typename VectorType, typename T>
struct axis_access<1, axes::rgba, VectorType, T> : basic_axis_access<VectorType, T> {

    using base_type   = basic_axis_access<VectorType, T>;
    using coord_names = axes::rgba;

    PSST_MATH_COORD_ACCESS(r, 0)
};

template <typename VectorType, typename T>
struct axis_access<2, axes::rgba, VectorType, T> : axis_access<1, axes::rgba, VectorType, T> {

    using base_type = basic_axis_access<VectorType, T>;

    PSST_MATH_COORD_ACCESS(g, 1)
};

template <typename VectorType, typename T>
struct axis_access<3, axes::rgba, VectorType, T> : axis_access<2, axes::rgba, VectorType, T> {

    using base_type = basic_axis_access<VectorType, T>;

    PSST_MATH_COORD_ACCESS(b, 2)
};

template <typename VectorType, typename T>
struct axis_access<4, axes::rgba, VectorType, T> : axis_access<3, axes::rgba, VectorType, T> {

    using base_type  = axis_access<3, axes::rgba, VectorType, T>;
    using value_type = typename base_type::value_type;

    using base_type::b;
    using base_type::g;
    using base_type::r;

    PSST_MATH_COORD_ACCESS(a, 3)

    vector<value_type, 4, math::axes::argb>
    argb() const
    {
        return {a(), r(), g(), b()};
    }
};
//@}

//@{
/** @name axes::hsva axes names */
template <typename VectorType, typename T>
struct axis_access<1, axes::hsva, VectorType, T> : basic_axis_access<VectorType, T> {

    using base_type   = basic_axis_access<VectorType, T>;
    using coord_names = axes::hsva;

    PSST_MATH_COORD_ACCESS(h, 0)
};

template <typename VectorType, typename T>
struct axis_access<2, axes::hsva, VectorType, T> : axis_access<1, axes::hsva, VectorType, T> {

    using base_type  = axis_access<1, axes::hsva, VectorType, T>;
    using value_type = typename base_type::value_type;

    using base_type::h;

    PSST_MATH_COORD_ACCESS(s, 1)
};

template <typename VectorType, typename T>
struct axis_access<3, axes::hsva, VectorType, T> : axis_access<2, axes::hsva, VectorType, T> {

    using base_type  = axis_access<2, axes::hsva, VectorType, T>;
    using value_type = typename base_type::value_type;

    using base_type::h;
    using base_type::s;

    PSST_MATH_COORD_ACCESS(v, 2)
};

template <typename VectorType, typename T>
struct axis_access<4, axes::hsva, VectorType, T> : axis_access<3, axes::hsva, VectorType, T> {

    using base_type  = axis_access<3, axes::hsva, VectorType, T>;
    using value_type = typename base_type::value_type;

    using base_type::h;
    using base_type::s;
    using base_type::v;

    PSST_MATH_COORD_ACCESS(a, 3)
};
//@}

//@{
/** @name axes::hsla axes names */
template <typename VectorType, typename T>
struct axis_access<1, axes::hsla, VectorType, T> : basic_axis_access<VectorType, T> {

    using base_type   = basic_axis_access<VectorType, T>;
    using coord_names = axes::hsla;

    PSST_MATH_COORD_ACCESS(h, 0)
};

template <typename VectorType, typename T>
struct axis_access<2, axes::hsla, VectorType, T> : axis_access<1, axes::hsla, VectorType, T> {

    using base_type = basic_axis_access<VectorType, T>;

    PSST_MATH_COORD_ACCESS(s, 1)
};

template <typename VectorType, typename T>
struct axis_access<3, axes::hsla, VectorType, T> : axis_access<2, axes::hsla, VectorType, T> {

    using base_type = basic_axis_access<VectorType, T>;

    PSST_MATH_COORD_ACCESS(l, 2)
};

template <typename VectorType, typename T>
struct axis_access<4, axes::hsla, VectorType, T> : axis_access<3, axes::hsla, VectorType, T> {

    using base_type = basic_axis_access<VectorType, T>;

    using base_type::h;
    using base_type::l;
    using base_type::s;

    PSST_MATH_COORD_ACCESS(a, 3)
};
//@}

template <typename Tag>
struct axes_names {
    template <std::size_t AxesCount, typename VectorType, typename T>
    using type = axis_access<AxesCount, Tag, VectorType, T>;
};

template <>
struct axes_names<axes::none> {
    template <std::size_t AxesCount, typename VectorType, typename T>
    using type = basic_axis_access<VectorType, T>;
};

template <std::size_t Size, typename Axes, typename VectorType, typename T>
using axes_names_t = typename axes_names<Axes>::template type<Size, VectorType, T>;

} /* namespace detail */

} /* namespace math */
} /* namespace psst */

#endif /* INCLUDE_PUSHKIN_MATH_DETAIL_AXIS_NAMES_HPP_ */
