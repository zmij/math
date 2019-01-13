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
#include <pushkin/math/vector_io.hpp>

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
    using value_policies = utils::template_tuple<
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
    using value_policies = utils::template_tuple<
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
    using value_policies = utils::template_tuple<
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
    using value_policies = utils::template_tuple<
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
struct axis_access<3, rgba_hex, VectorType, T> : basic_axis_access<VectorType, T, rgba_hex> {

    using base_type = basic_axis_access<VectorType, T, rgba_hex>;

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

//@{
/** @name is_color */
template <typename T>
struct is_color : has_axes_t<T, axes::rgba, axes::rgba_hex, axes::argb, axes::hsla, axes::hsva> {};
template <typename T>
using is_color_t = typename is_color<T>::type;
template <typename T>
constexpr bool is_color_v = is_color_t<T>::value;
//@}

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
    rgb_hex res{
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

template <typename Axes, typename Expr>
struct color_chroma;

template <typename Expr>
struct color_chroma<axes::hsla, Expr>
    : unary_scalar_expression_axes<color_chroma, axes::hsla, Expr>, unary_expression<Expr> {
    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    constexpr auto
    value() const
    {
        using std::abs;
        return (1 - abs(2 * this->arg_.l() - 1)) * this->arg_.s();
    }
};

template <typename Expr>
struct color_chroma<axes::hsva, Expr>
    : unary_scalar_expression_axes<color_chroma, axes::hsva, Expr>, unary_expression<Expr> {
    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    constexpr auto
    value() const
    {
        using std::abs;
        return this->arg_.v() * this->arg_.s();
    }
};

template <typename Expr, typename = enable_for_axes<Expr, axes::hsla, axes::hsva>>
constexpr auto
chroma(Expr&& expr)
{
    using axes_names = axes_names_t<Expr>;
    return make_unary_expression<select_unary_impl<axes_names, color_chroma>::template type>(
        std::forward<Expr>(expr));
}

template <typename T, typename Expression>
struct conversion<color::rgba_hex, color::rgba<T>, Expression> : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        using color::get_hex_color_component;
        return color::rgba<T>{
            get_hex_color_component<T>(this->arg_.r()), get_hex_color_component<T>(this->arg_.g()),
            get_hex_color_component<T>(this->arg_.b()), get_hex_color_component<T>(this->arg_.a())};
    }
};

template <typename T, typename Expression>
struct conversion<color::rgba<T>, color::rgba_hex, Expression> : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        return color::rgba_hex{this->arg_.r() * 0xff, this->arg_.g() * 0xff, this->arg_.b() * 0xff,
                               this->arg_.a() * 0xff};
    }
};

//@{
/** @name HSL -> RGB conversion */
template <typename T, typename U, std::size_t Size, typename Expression>
struct conversion<vector<T, Size, axes::hsla>, vector<U, Size, axes::rgba>, Expression>
    : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        using value_type  = U;
        using result_type = vector<U, Size, axes::rgba>;
        using std::abs;
        using std::fmod;

        value_type c       = chroma(this->arg_);
        value_type segment = this->arg_.hue() * 3 / pi<value_type>::value;
        value_type x       = c * (1 - abs(fmod(segment, 2) - 1));
        value_type m       = this->arg_.l() - c / 2;
        if (0 <= segment && segment < 1) {
            return result_type{c + m, x + m, m, this->arg_.a()};
        } else if (1 <= segment && segment < 2) {
            return result_type{x + m, c + m, m, this->arg_.a()};
        } else if (2 <= segment && segment < 3) {
            return result_type{m, c + m, x + m, this->arg_.a()};
        } else if (3 <= segment && segment < 4) {
            return result_type{m, x + m, c + m, this->arg_.a()};
        } else if (4 <= segment && segment < 5) {
            return result_type{x + m, m, c + m, this->arg_.a()};
        } else if (5 <= segment && segment < 6) {
            return result_type{c + m, m, x + m, this->arg_.a()};
        }
        return result_type{};
    }
};
//@}

//@{
/** @name RGB -> HSL conversion */
template <typename T, typename U, std::size_t Size, typename Expression>
struct conversion<vector<T, Size, axes::rgba>, vector<U, Size, axes::hsla>, Expression>
    : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        using value_type  = U;
        using result_type = vector<U, Size, axes::hsla>;
        using std::abs;
        using std::fmod;
        using std::max;
        using std::min;

        value_type cmax = max(this->arg_.r(), max(this->arg_.g(), this->arg_.b()));
        value_type cmin = min(this->arg_.r(), min(this->arg_.g(), this->arg_.b()));
        value_type d    = cmax - cmin;
        value_type l    = (cmax + cmin) / 2;
        value_type s    = 0;
        if (l != 0) {
            s = d / (1 - abs(2 * l - 1));
        }
        if (d == 0) {
            return result_type{0, s, l, this->arg_.a()};
        } else if (cmax == this->arg_.r()) {
            return result_type{pi<value_type>::value / 3
                                   * (value_type)fmod((this->arg_.g() - this->arg_.b()) / d, 6),
                               s, l, this->arg_.a()};
        } else if (cmax == this->arg_.g()) {
            return result_type{pi<value_type>::value / 3
                                   * ((this->arg_.b() - this->arg_.r()) / d + 2),
                               s, l, this->arg_.a()};
        } else if (cmax == this->arg_.b()) {
            return result_type{pi<value_type>::value / 3
                                   * ((this->arg_.r() - this->arg_.g()) / d + 4),
                               s, l, this->arg_.a()};
        }

        return result_type{0, s, l, this->arg_.a()};
    }
};
//@}

//@{
/** @name HSV -> RGB conversion */
template <typename T, typename U, std::size_t Size, typename Expression>
struct conversion<vector<T, Size, axes::hsva>, vector<U, Size, axes::rgba>, Expression>
    : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        using value_type  = U;
        using result_type = vector<U, Size, axes::rgba>;
        using std::abs;
        using std::fmod;

        value_type c       = chroma(this->arg_);
        value_type segment = this->arg_.hue() * 3 / pi<value_type>::value;
        value_type x       = c * (1 - abs(fmod(segment, 2) - 1));
        value_type m       = this->arg_.v() - c;

        if (0 <= segment && segment < 1) {
            return result_type{c + m, x + m, m, this->arg_.a()};
        } else if (1 <= segment && segment < 2) {
            return result_type{x + m, c + m, m, this->arg_.a()};
        } else if (2 <= segment && segment < 3) {
            return result_type{m, c + m, x + m, this->arg_.a()};
        } else if (3 <= segment && segment < 4) {
            return result_type{m, x + m, c + m, this->arg_.a()};
        } else if (4 <= segment && segment < 5) {
            return result_type{x + m, m, c + m, this->arg_.a()};
        } else if (5 <= segment && segment < 6) {
            return result_type{c + m, m, x + m, this->arg_.a()};
        }
        return result_type{};
    }
};
//@}

//@{
/** @name RGB -> HSL conversion */
template <typename T, typename U, std::size_t Size, typename Expression>
struct conversion<vector<T, Size, axes::rgba>, vector<U, Size, axes::hsva>, Expression>
    : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        using value_type  = U;
        using result_type = vector<U, Size, axes::hsva>;
        using std::abs;
        using std::fmod;
        using std::max;
        using std::min;

        value_type cmax = max(this->arg_.r(), max(this->arg_.g(), this->arg_.b()));
        value_type cmin = min(this->arg_.r(), min(this->arg_.g(), this->arg_.b()));
        value_type d    = cmax - cmin;
        value_type s    = 0;
        if (cmax != 0) {
            s = d / cmax;
        }
        if (d == 0) {
            return result_type{0, s, cmax, this->arg_.a()};
        } else if (cmax == this->arg_.r()) {
            return result_type{pi<value_type>::value / 3
                                   * (value_type)fmod((this->arg_.g() - this->arg_.b()) / d, 6),
                               s, cmax, this->arg_.a()};
        } else if (cmax == this->arg_.g()) {
            return result_type{pi<value_type>::value / 3
                                   * ((this->arg_.b() - this->arg_.r()) / d + 2),
                               s, cmax, this->arg_.a()};
        } else if (cmax == this->arg_.b()) {
            return result_type{pi<value_type>::value / 3
                                   * ((this->arg_.r() - this->arg_.g()) / d + 4),
                               s, cmax, this->arg_.a()};
        }

        return result_type{0, s, cmax, this->arg_.a()};
    }
};
//@}

}    // namespace v

}    // namespace expr

inline std::ostream&
operator<<(std::ostream& os, color::rgba_hex const& val)
{
    // clang-format off
    os << '#' << std::hex << std::setfill('0')
       << std::setw(2) << (unsigned int)val.red()
       << std::setw(2) << (unsigned int)val.green()
       << std::setw(2) << (unsigned int)val.blue()
       << std::setw(2) << (unsigned int)val.alpha();
    // clang-format on
    return os;
}

}    // namespace math
}    // namespace psst

#endif /* PUSHKIN_MATH_COLORS_HPP_ */
