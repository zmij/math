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

template <typename VectorType, typename T, typename Axes>
struct basic_axis_access {
    using builder_type     = VectorType;
    using value_type       = typename std::decay<T>::type;
    using coord_names      = Axes;
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
template <std::size_t AxisNo, typename Axes, typename VectorType, typename T>
struct axis_access : axis_access<AxisNo - 1, Axes, VectorType, T> {};

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

//@{
/** @name wxyz axes names */
template <typename VectorType, typename T>
struct axis_access<1, axes::wxyz, VectorType, T> : basic_axis_access<VectorType, T, axes::wxyz> {

    using base_type = basic_axis_access<VectorType, T, axes::wxyz>;

    PSST_MATH_COORD_ACCESS(w)
};

template <typename VectorType, typename T>
struct axis_access<2, axes::wxyz, VectorType, T> : axis_access<1, axes::wxyz, VectorType, T> {

    using base_type  = axis_access<1, axes::wxyz, VectorType, T>;
    using value_type = typename base_type::value_type;

    using base_type::w;

    PSST_MATH_COORD_ACCESS(x)

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

    PSST_MATH_COORD_ACCESS(y)

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

    PSST_MATH_COORD_ACCESS(z)

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
struct axis_access<1, axes::polar, VectorType, T> : basic_axis_access<VectorType, T, axes::polar> {

    using base_type = basic_axis_access<VectorType, T, axes::polar>;

    PSST_MATH_COORD_ACCESS(r);
    PSST_MATH_COORD_ACCESS(rho);
};
template <typename VectorType, typename T>
struct axis_access<2, axes::polar, VectorType, T> : axis_access<1, axes::polar, VectorType, T> {
    using base_type = axis_access<1, axes::polar, VectorType, T>;

    using base_type::r;
    using base_type::rho;

    PSST_MATH_COORD_ACCESS(phi);
    PSST_MATH_COORD_ACCESS(azimuth);
};
//@}

//@{
/** @name Spherical coordinates */
template <typename VectorType, typename T>
struct axis_access<1, axes::spherical, VectorType, T>
    : basic_axis_access<VectorType, T, axes::spherical> {

    using base_type = basic_axis_access<VectorType, T, axes::spherical>;

    PSST_MATH_COORD_ACCESS(r);
    PSST_MATH_COORD_ACCESS(rho);
};
template <typename VectorType, typename T>
struct axis_access<2, axes::spherical, VectorType, T>
    : axis_access<1, axes::spherical, VectorType, T> {
    using base_type = axis_access<1, axes::spherical, VectorType, T>;

    using base_type::r;
    using base_type::rho;

    PSST_MATH_COORD_ACCESS(phi);
    PSST_MATH_COORD_ACCESS(inclination);
};
template <typename VectorType, typename T>
struct axis_access<3, axes::spherical, VectorType, T>
    : axis_access<2, axes::spherical, VectorType, T> {
    using base_type = axis_access<2, axes::spherical, VectorType, T>;

    using base_type::inclination;
    using base_type::phi;
    using base_type::r;
    using base_type::rho;

    PSST_MATH_COORD_ACCESS(theta);
    PSST_MATH_COORD_ACCESS(azimuth);
};
//@}

//@{
/** @name Cylindrical coordinates */
template <typename VectorType, typename T>
struct axis_access<1, axes::cylindrical, VectorType, T>
    : basic_axis_access<VectorType, T, axes::cylindrical> {

    using base_type = basic_axis_access<VectorType, T, axes::cylindrical>;

    PSST_MATH_COORD_ACCESS(r);
    PSST_MATH_COORD_ACCESS(rho);
};
template <typename VectorType, typename T>
struct axis_access<2, axes::cylindrical, VectorType, T>
    : axis_access<1, axes::cylindrical, VectorType, T> {
    using base_type = axis_access<1, axes::cylindrical, VectorType, T>;

    using base_type::r;
    using base_type::rho;

    PSST_MATH_COORD_ACCESS(phi);
    PSST_MATH_COORD_ACCESS(azimuth);
};
template <typename VectorType, typename T>
struct axis_access<3, axes::cylindrical, VectorType, T>
    : axis_access<2, axes::cylindrical, VectorType, T> {
    using base_type = axis_access<2, axes::cylindrical, VectorType, T>;

    using base_type::azimuth;
    using base_type::phi;
    using base_type::r;
    using base_type::rho;

    PSST_MATH_COORD_ACCESS(z);
    PSST_MATH_COORD_ACCESS(elevation);
};
//@}

//@{
/** @name axes::argb axes names */
template <typename VectorType, typename T>
struct axis_access<1, axes::argb, VectorType, T> : basic_axis_access<VectorType, T, axes::argb> {

    using base_type = basic_axis_access<VectorType, T, axes::argb>;

    PSST_MATH_COORD_ACCESS(a)
    PSST_MATH_COORD_ACCESS(alpha);
};

template <typename VectorType, typename T>
struct axis_access<2, axes::argb, VectorType, T> : axis_access<1, axes::argb, VectorType, T> {

    using base_type = axis_access<1, axes::argb, VectorType, T>;

    PSST_MATH_COORD_ACCESS(r)
    PSST_MATH_COORD_ACCESS(red)
};

template <typename VectorType, typename T>
struct axis_access<3, axes::argb, VectorType, T> : axis_access<2, axes::argb, VectorType, T> {

    using base_type = axis_access<2, axes::argb, VectorType, T>;

    PSST_MATH_COORD_ACCESS(g)
    PSST_MATH_COORD_ACCESS(green)
};

template <typename VectorType, typename T>
struct axis_access<4, axes::argb, VectorType, T> : axis_access<3, axes::argb, VectorType, T> {

    using base_type  = axis_access<3, axes::argb, VectorType, T>;
    using value_type = typename base_type::value_type;

    using base_type::a;
    using base_type::alpha;
    using base_type::g;
    using base_type::green;
    using base_type::r;
    using base_type::red;

    PSST_MATH_COORD_ACCESS(b)
    PSST_MATH_COORD_ACCESS(blue)

    constexpr vector<value_type, 4, axes::rgba>
    rgba() const
    {
        return {r(), g(), b(), a()};
    }
};
//@}

//@{
/** @name axes::rgba axes names */
template <typename VectorType, typename T>
struct axis_access<1, axes::rgba, VectorType, T> : basic_axis_access<VectorType, T, axes::rgba> {

    using base_type = basic_axis_access<VectorType, T, axes::rgba>;

    PSST_MATH_COORD_ACCESS(r)
    PSST_MATH_COORD_ACCESS(red)
};

template <typename VectorType, typename T>
struct axis_access<2, axes::rgba, VectorType, T> : axis_access<1, axes::rgba, VectorType, T> {

    using base_type = axis_access<1, axes::rgba, VectorType, T>;

    PSST_MATH_COORD_ACCESS(g)
    PSST_MATH_COORD_ACCESS(green)
};

template <typename VectorType, typename T>
struct axis_access<3, axes::rgba, VectorType, T> : axis_access<2, axes::rgba, VectorType, T> {

    using base_type = axis_access<2, axes::rgba, VectorType, T>;

    using base_type::g;
    using base_type::green;
    using base_type::r;
    using base_type::red;

    PSST_MATH_COORD_ACCESS(b)
    PSST_MATH_COORD_ACCESS(blue)
};

template <typename VectorType, typename T>
struct axis_access<4, axes::rgba, VectorType, T> : axis_access<3, axes::rgba, VectorType, T> {

    using base_type  = axis_access<3, axes::rgba, VectorType, T>;
    using value_type = typename base_type::value_type;

    using base_type::b;
    using base_type::blue;
    using base_type::g;
    using base_type::green;
    using base_type::r;
    using base_type::red;

    PSST_MATH_COORD_ACCESS(a)
    PSST_MATH_COORD_ACCESS(alpha)

    constexpr vector<value_type, 4, math::axes::argb>
    argb() const
    {
        return {a(), r(), g(), b()};
    }
};
//@}

//@{
/** @name axes::hsva axes names */
template <typename VectorType, typename T>
struct axis_access<1, axes::hsva, VectorType, T> : basic_axis_access<VectorType, T, axes::hsva> {

    using base_type = basic_axis_access<VectorType, T, axes::hsva>;

    PSST_MATH_COORD_ACCESS(h)
    PSST_MATH_COORD_ACCESS(hue)
};

template <typename VectorType, typename T>
struct axis_access<2, axes::hsva, VectorType, T> : axis_access<1, axes::hsva, VectorType, T> {

    using base_type  = axis_access<1, axes::hsva, VectorType, T>;
    using value_type = typename base_type::value_type;

    PSST_MATH_COORD_ACCESS(s)
    PSST_MATH_COORD_ACCESS(saturation)
};

template <typename VectorType, typename T>
struct axis_access<3, axes::hsva, VectorType, T> : axis_access<2, axes::hsva, VectorType, T> {

    using base_type  = axis_access<2, axes::hsva, VectorType, T>;
    using value_type = typename base_type::value_type;

    using base_type::h;
    using base_type::hue;
    using base_type::s;
    using base_type::saturation;

    PSST_MATH_COORD_ACCESS(v)
    PSST_MATH_COORD_ACCESS(value)
};

template <typename VectorType, typename T>
struct axis_access<4, axes::hsva, VectorType, T> : axis_access<3, axes::hsva, VectorType, T> {

    using base_type  = axis_access<3, axes::hsva, VectorType, T>;
    using value_type = typename base_type::value_type;

    using base_type::h;
    using base_type::hue;
    using base_type::s;
    using base_type::saturation;
    using base_type::v;
    using base_type::value;

    PSST_MATH_COORD_ACCESS(a)
    PSST_MATH_COORD_ACCESS(alpha)
};
//@}

//@{
/** @name axes::hsla axes names */
template <typename VectorType, typename T>
struct axis_access<1, axes::hsla, VectorType, T> : basic_axis_access<VectorType, T, axes::hsla> {

    using base_type = basic_axis_access<VectorType, T, axes::hsla>;

    PSST_MATH_COORD_ACCESS(h)
    PSST_MATH_COORD_ACCESS(hue)
};

template <typename VectorType, typename T>
struct axis_access<2, axes::hsla, VectorType, T> : axis_access<1, axes::hsla, VectorType, T> {

    using base_type = axis_access<1, axes::hsla, VectorType, T>;

    PSST_MATH_COORD_ACCESS(s)
    PSST_MATH_COORD_ACCESS(saturation)
};

template <typename VectorType, typename T>
struct axis_access<3, axes::hsla, VectorType, T> : axis_access<2, axes::hsla, VectorType, T> {

    using base_type = axis_access<2, axes::hsla, VectorType, T>;

    using base_type::h;
    using base_type::hue;
    using base_type::s;
    using base_type::saturation;

    PSST_MATH_COORD_ACCESS(l)
    PSST_MATH_COORD_ACCESS(lightness)
};

template <typename VectorType, typename T>
struct axis_access<4, axes::hsla, VectorType, T> : axis_access<3, axes::hsla, VectorType, T> {

    using base_type = axis_access<3, axes::hsla, VectorType, T>;

    using base_type::h;
    using base_type::hue;
    using base_type::l;
    using base_type::lightness;
    using base_type::s;
    using base_type::saturation;

    PSST_MATH_COORD_ACCESS(a)
    PSST_MATH_COORD_ACCESS(alpha)
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
    using type = basic_axis_access<VectorType, T, axes::none>;
};

template <std::size_t Size, typename Axes, typename VectorType, typename T>
using axes_names_t = typename axes_names<Axes>::template type<Size, VectorType, T>;

} /* namespace detail */

} /* namespace math */
} /* namespace psst */

#endif /* INCLUDE_PUSHKIN_MATH_DETAIL_AXIS_NAMES_HPP_ */
