/**
 * Copyright 2018 Sergei A. Fedorov
 * colors.hpp
 *
 *  Created on: Dec 30, 2018
 *      Author: ser-fedorov
 */

#ifndef PUSHKIN_MATH_COLORS_HPP_
#define PUSHKIN_MATH_COLORS_HPP_

#include <pushkin/math/vector.hpp>

namespace psst {
namespace math {
namespace axes {

struct argb {
    static constexpr std::size_t min_components = 4;
    static constexpr std::size_t max_components = 4;
    static constexpr std::size_t a              = 0;
    static constexpr std::size_t r              = 1;
    static constexpr std::size_t g              = 2;
    static constexpr std::size_t b              = 3;
    static constexpr std::size_t alpha          = a;
    static constexpr std::size_t red            = r;
    static constexpr std::size_t green          = g;
    static constexpr std::size_t blue           = b;

    // clang-format off
    using value_policy = utils::template_tuple<
            math::value_policy::clamp_zero_to_one,    // alpha
            math::value_policy::clamp_zero_to_one,    // red
            math::value_policy::clamp_zero_to_one,    // green
            math::value_policy::clamp_zero_to_one     // blue
          >;
    // clang-format on
};
struct rgba {
    static constexpr std::size_t min_components = 3;
    static constexpr std::size_t max_components = 4;
    static constexpr std::size_t r              = 0;
    static constexpr std::size_t g              = 1;
    static constexpr std::size_t b              = 2;
    static constexpr std::size_t a              = 3;
    static constexpr std::size_t red            = r;
    static constexpr std::size_t green          = g;
    static constexpr std::size_t blue           = b;
    static constexpr std::size_t alpha          = a;

    // clang-format off
    using value_policy = utils::template_tuple<
            math::value_policy::clamp_zero_to_one,    // red
            math::value_policy::clamp_zero_to_one,    // green
            math::value_policy::clamp_zero_to_one,    // blue
            math::value_policy::clamp_zero_to_one     // alpha
          >;
    // clang-format on
};
struct rgba_hex {
    static constexpr std::size_t min_components = 3;
    static constexpr std::size_t max_components = 4;
    static constexpr std::size_t r              = 0;
    static constexpr std::size_t g              = 1;
    static constexpr std::size_t b              = 2;
    static constexpr std::size_t a              = 3;
    static constexpr std::size_t red            = r;
    static constexpr std::size_t green          = g;
    static constexpr std::size_t blue           = b;
    static constexpr std::size_t alpha          = a;
};
struct hsva {
    static constexpr std::size_t min_components = 3;
    static constexpr std::size_t max_components = 4;
    static constexpr std::size_t h              = 0;
    static constexpr std::size_t s              = 1;
    static constexpr std::size_t v              = 2;
    static constexpr std::size_t a              = 3;
    static constexpr std::size_t hue            = h;
    static constexpr std::size_t saturation     = s;
    static constexpr std::size_t value          = v;
    static constexpr std::size_t alpha          = a;

    // clang-format off
    using value_policy = utils::template_tuple<
                math::value_policy::clamp_zero_to_two_pi,   // hue,
                math::value_policy::clamp_zero_to_one,      // saturation
                math::value_policy::clamp_zero_to_one,      // value
                math::value_policy::clamp_zero_to_one       // alpha
            >;
    // clang-format on
};
struct hsla {
    static constexpr std::size_t min_components = 3;
    static constexpr std::size_t max_components = 4;
    static constexpr std::size_t h              = 0;
    static constexpr std::size_t s              = 1;
    static constexpr std::size_t l              = 2;
    static constexpr std::size_t a              = 3;
    static constexpr std::size_t hue            = h;
    static constexpr std::size_t saturation     = s;
    static constexpr std::size_t lightness      = l;
    static constexpr std::size_t alpha          = a;

    // clang-format off
    using value_policy = utils::template_tuple<
                math::value_policy::clamp_zero_to_two_pi,   // hue,
                math::value_policy::clamp_zero_to_one,      // saturation
                math::value_policy::clamp_zero_to_one,      // lightness
                math::value_policy::clamp_zero_to_one       // alpha
            >;
    // clang-format on
};

//@{
/** @name axes::argb axes names */
template <typename VectorType, typename T>
struct axis_access<4, argb, VectorType, T> : basic_axis_access<VectorType, T, argb> {

    using base_type = basic_axis_access<VectorType, T, argb>;

    PSST_MATH_COORD_ACCESS(a)
    PSST_MATH_COORD_ACCESS(alpha);
    PSST_MATH_COORD_ACCESS(r)
    PSST_MATH_COORD_ACCESS(red)
    PSST_MATH_COORD_ACCESS(g)
    PSST_MATH_COORD_ACCESS(green)
    PSST_MATH_COORD_ACCESS(b)
    PSST_MATH_COORD_ACCESS(blue)

    constexpr vector<T, 4, rgba>
    rgba() const
    {
        return {r(), g(), b(), a()};
    }
};
//@}

//@{
/** @name axes::rgba axes names */
template <typename VectorType, typename T>
struct axis_access<3, rgba, VectorType, T> : basic_axis_access<VectorType, T, rgba> {

    using base_type = basic_axis_access<VectorType, T, rgba>;

    PSST_MATH_COORD_ACCESS(r)
    PSST_MATH_COORD_ACCESS(red)
    PSST_MATH_COORD_ACCESS(g)
    PSST_MATH_COORD_ACCESS(green)
    PSST_MATH_COORD_ACCESS(b)
    PSST_MATH_COORD_ACCESS(blue)
};

template <typename VectorType, typename T>
struct axis_access<4, rgba, VectorType, T> : axis_access<3, rgba, VectorType, T> {

    using base_type = axis_access<3, rgba, VectorType, T>;

    using base_type::b;
    using base_type::blue;
    using base_type::g;
    using base_type::green;
    using base_type::r;
    using base_type::red;

    PSST_MATH_COORD_ACCESS(a)
    PSST_MATH_COORD_ACCESS(alpha)

    constexpr vector<T, 4, math::axes::argb>
    argb() const
    {
        return {a(), r(), g(), b()};
    }
};
//@}

//@{
/** @name axes::rgba axes names */
template <typename VectorType, typename T>
struct axis_access<3, rgba_hex, VectorType, T> : basic_axis_access<VectorType, T, rgba> {

    using base_type = basic_axis_access<VectorType, T, rgba>;

    PSST_MATH_COORD_ACCESS(r)
    PSST_MATH_COORD_ACCESS(red)
    PSST_MATH_COORD_ACCESS(g)
    PSST_MATH_COORD_ACCESS(green)
    PSST_MATH_COORD_ACCESS(b)
    PSST_MATH_COORD_ACCESS(blue)
};

template <typename VectorType, typename T>
struct axis_access<4, rgba_hex, VectorType, T> : axis_access<3, rgba_hex, VectorType, T> {

    using base_type = axis_access<3, rgba_hex, VectorType, T>;

    using base_type::b;
    using base_type::blue;
    using base_type::g;
    using base_type::green;
    using base_type::r;
    using base_type::red;

    PSST_MATH_COORD_ACCESS(a)
    PSST_MATH_COORD_ACCESS(alpha)
};
//@}

//@{
/** @name axes::hsva axes names */
template <typename VectorType, typename T>
struct axis_access<3, hsva, VectorType, T> : basic_axis_access<VectorType, T, hsva> {

    using base_type = basic_axis_access<VectorType, T, hsva>;
    PSST_MATH_COORD_ACCESS(h)
    PSST_MATH_COORD_ACCESS(hue)
    PSST_MATH_COORD_ACCESS(s)
    PSST_MATH_COORD_ACCESS(saturation)
    PSST_MATH_COORD_ACCESS(v)
    PSST_MATH_COORD_ACCESS(value)
};

template <typename VectorType, typename T>
struct axis_access<4, hsva, VectorType, T> : axis_access<3, hsva, VectorType, T> {

    using base_type = axis_access<3, hsva, VectorType, T>;

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
struct axis_access<3, hsla, VectorType, T> : basic_axis_access<VectorType, T, hsla> {

    using base_type = basic_axis_access<VectorType, T, hsla>;

    PSST_MATH_COORD_ACCESS(h)
    PSST_MATH_COORD_ACCESS(hue)
    PSST_MATH_COORD_ACCESS(s)
    PSST_MATH_COORD_ACCESS(saturation)
    PSST_MATH_COORD_ACCESS(l)
    PSST_MATH_COORD_ACCESS(lightness)
};

template <typename VectorType, typename T>
struct axis_access<4, hsla, VectorType, T> : axis_access<3, hsla, VectorType, T> {

    using base_type = axis_access<3, hsla, VectorType, T>;

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

}    // namespace axes
namespace color {

template <typename T>
using rgb = vector<T, 3, axes::rgba>;
template <typename T>
using rgba = vector<T, 4, axes::rgba>;

template <typename T>
using argb = vector<T, 4, axes::argb>;

template <typename T>
using hsl = vector<T, 3, axes::hsla>;
template <typename T>
using hsla = vector<T, 4, axes::hsla>;

template <typename T>
using hsv = vector<T, 3, axes::hsva>;
template <typename T>
using hsva = vector<T, 4, axes::hsva>;

using rgb_hex  = vector<std::uint8_t, 3, axes::rgba_hex>;
using rgba_hex = vector<std::uint8_t, 4, axes::rgba_hex>;

inline constexpr rgba_hex operator"" _rgba(unsigned long long val)
{
    // clang-format off
    rgba_hex res{
        (std::uint8_t)((val & 0xff000000) >> 24),
        (std::uint8_t)((val & 0xff0000) >> 16),
        (std::uint8_t)((val & 0xff00) >> 8),
        (std::uint8_t)(val & 0xff)
    };
    // clang-format on
    return res;
}

inline constexpr rgb_hex operator"" _rgb(unsigned long long val)
{
    // clang-format off
    rgba_hex res{
        (std::uint8_t)((val & 0xff0000) >> 16),
        (std::uint8_t)((val & 0xff00) >> 8),
        (std::uint8_t)(val & 0xff)
    };
    // clang-format on
    return res;
}

template <typename T>
T
get_hex_color_component(std::uint8_t hex)
{
    return hex / T{255};
}

}    // namespace color

namespace expr {

inline namespace v {

template <typename T, typename Expression>
struct conversion<color::rgba_hex, color::rgba<T>, Expression>
    : vector_expression<conversion<color::rgba_hex, color::rgba<T>, Expression>, color::rgba<T>>,
      unary_expression<Expression> {
    using base_type = vector_expression<conversion<color::rgba_hex, color::rgba<T>, Expression>,
                                        color::rgba<T>>;
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr auto
    at() const
    {
        static_assert(N < base_type::size, "Color conversion component index is out of range");
        return color::get_hex_color_component<T>(this->arg_.template at<N>());
    }
};

template <typename T, typename Expression>
struct conversion<color::rgba<T>, color::rgba_hex, Expression>
    : vector_expression<conversion<color::rgba<T>, color::rgba_hex, Expression>>,
      unary_expression<Expression> {
    using base_type = vector_expression<conversion<color::rgba<T>, color::rgba_hex, Expression>>;
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr auto
    at() const
    {
        static_assert(N < base_type::size, "Color conversion component index is out of range");
        return this->arg_.template at<N>() * 0xff;
    }
};

}    // namespace v

}    // namespace expr
}    // namespace math
}    // namespace psst

#endif /* PUSHKIN_MATH_COLORS_HPP_ */
