/**
 * Copyright 2018 Sergei A. Fedorov
 * colors.hpp
 *
 *  Created on: Dec 30, 2018
 *      Author: ser-fedorov
 */

#ifndef PSST_MATH_COLORS_HPP_
#define PSST_MATH_COLORS_HPP_

#include <psst/math/vector.hpp>

namespace psst {
namespace math {
namespace components {

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
struct argb_hex {
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

struct grayscale {
    static constexpr std::size_t min_components = 1;
    static constexpr std::size_t max_components = 2;
    static constexpr std::size_t g              = 0;
    static constexpr std::size_t a              = 1;
    static constexpr std::size_t gray           = g;
    static constexpr std::size_t alpha          = a;

    // clang-format off
    using value_policies = utils::template_tuple<
            math::value_policy::clamp_zero_to_one,    // grey
            math::value_policy::clamp_zero_to_one     // alpha
          >;
    // clang-format on
};

struct grayscale_hex {
    static constexpr std::size_t min_components = 1;
    static constexpr std::size_t max_components = 2;
    static constexpr std::size_t g              = 0;
    static constexpr std::size_t a              = 1;
    static constexpr std::size_t gray           = g;
    static constexpr std::size_t alpha          = a;
};

}    // namespace components

namespace component_access {
//@{
/** @name components::argb components names */
template <typename VectorType, typename T>
struct component_access<4, components::argb, VectorType, T>
    : basic_component_access<VectorType, T, components::argb> {

    using base_type = basic_component_access<VectorType, T, components::argb>;

    PSST_MATH_COMPONENT_ACCESS(a)
    PSST_MATH_COMPONENT_ACCESS(alpha);
    PSST_MATH_COMPONENT_ACCESS(r)
    PSST_MATH_COMPONENT_ACCESS(red)
    PSST_MATH_COMPONENT_ACCESS(g)
    PSST_MATH_COMPONENT_ACCESS(green)
    PSST_MATH_COMPONENT_ACCESS(b)
    PSST_MATH_COMPONENT_ACCESS(blue)

    constexpr vector<T, 4, components::rgba>
    rgba() const
    {
        return {r(), g(), b(), a()};
    }
};
//@}

/** @name components::argb components names */
template <typename VectorType, typename T>
struct component_access<4, components::argb_hex, VectorType, T>
    : basic_component_access<VectorType, T, components::argb_hex> {

    using base_type = basic_component_access<VectorType, T, components::argb_hex>;

    PSST_MATH_COMPONENT_ACCESS(a)
    PSST_MATH_COMPONENT_ACCESS(alpha);
    PSST_MATH_COMPONENT_ACCESS(r)
    PSST_MATH_COMPONENT_ACCESS(red)
    PSST_MATH_COMPONENT_ACCESS(g)
    PSST_MATH_COMPONENT_ACCESS(green)
    PSST_MATH_COMPONENT_ACCESS(b)
    PSST_MATH_COMPONENT_ACCESS(blue)

    constexpr vector<T, 4, components::rgba_hex>
    rgba() const
    {
        return {r(), g(), b(), a()};
    }
};
//@}

//@{
/** @name components::rgba component names */
template <typename VectorType, typename T>
struct component_access<3, components::rgba, VectorType, T>
    : basic_component_access<VectorType, T, components::rgba> {

    using base_type = basic_component_access<VectorType, T, components::rgba>;

    PSST_MATH_COMPONENT_ACCESS(r)
    PSST_MATH_COMPONENT_ACCESS(red)
    PSST_MATH_COMPONENT_ACCESS(g)
    PSST_MATH_COMPONENT_ACCESS(green)
    PSST_MATH_COMPONENT_ACCESS(b)
    PSST_MATH_COMPONENT_ACCESS(blue)
};

template <typename VectorType, typename T>
struct component_access<4, components::rgba, VectorType, T>
    : component_access<3, components::rgba, VectorType, T> {

    using base_type = component_access<3, components::rgba, VectorType, T>;

    using base_type::b;
    using base_type::blue;
    using base_type::g;
    using base_type::green;
    using base_type::r;
    using base_type::red;

    PSST_MATH_COMPONENT_ACCESS(a)
    PSST_MATH_COMPONENT_ACCESS(alpha)

    constexpr vector<T, 4, math::components::argb>
    argb() const
    {
        return {a(), r(), g(), b()};
    }
};
//@}

//@{
/** @name components::rgba component names */
template <typename VectorType, typename T>
struct component_access<3, components::rgba_hex, VectorType, T>
    : basic_component_access<VectorType, T, components::rgba_hex> {

    using base_type = basic_component_access<VectorType, T, components::rgba_hex>;

    PSST_MATH_COMPONENT_ACCESS(r)
    PSST_MATH_COMPONENT_ACCESS(red)
    PSST_MATH_COMPONENT_ACCESS(g)
    PSST_MATH_COMPONENT_ACCESS(green)
    PSST_MATH_COMPONENT_ACCESS(b)
    PSST_MATH_COMPONENT_ACCESS(blue)
};

template <typename VectorType, typename T>
struct component_access<4, components::rgba_hex, VectorType, T>
    : component_access<3, components::rgba_hex, VectorType, T> {

    using base_type = component_access<3, components::rgba_hex, VectorType, T>;

    using base_type::b;
    using base_type::blue;
    using base_type::g;
    using base_type::green;
    using base_type::r;
    using base_type::red;

    PSST_MATH_COMPONENT_ACCESS(a)
    PSST_MATH_COMPONENT_ACCESS(alpha)
};
//@}

//@{
/** @name components::hsva component names */
template <typename VectorType, typename T>
struct component_access<3, components::hsva, VectorType, T>
    : basic_component_access<VectorType, T, components::hsva> {

    using base_type = basic_component_access<VectorType, T, components::hsva>;
    PSST_MATH_COMPONENT_ACCESS(h)
    PSST_MATH_COMPONENT_ACCESS(hue)
    PSST_MATH_COMPONENT_ACCESS(s)
    PSST_MATH_COMPONENT_ACCESS(saturation)
    PSST_MATH_COMPONENT_ACCESS(v)
    PSST_MATH_COMPONENT_ACCESS(value)
};

template <typename VectorType, typename T>
struct component_access<4, components::hsva, VectorType, T>
    : component_access<3, components::hsva, VectorType, T> {

    using base_type = component_access<3, components::hsva, VectorType, T>;

    using base_type::h;
    using base_type::hue;
    using base_type::s;
    using base_type::saturation;
    using base_type::v;
    using base_type::value;

    PSST_MATH_COMPONENT_ACCESS(a)
    PSST_MATH_COMPONENT_ACCESS(alpha)
};
//@}

//@{
/** @name components::hsla component names */
template <typename VectorType, typename T>
struct component_access<3, components::hsla, VectorType, T>
    : basic_component_access<VectorType, T, components::hsla> {

    using base_type = basic_component_access<VectorType, T, components::hsla>;

    PSST_MATH_COMPONENT_ACCESS(h)
    PSST_MATH_COMPONENT_ACCESS(hue)
    PSST_MATH_COMPONENT_ACCESS(s)
    PSST_MATH_COMPONENT_ACCESS(saturation)
    PSST_MATH_COMPONENT_ACCESS(l)
    PSST_MATH_COMPONENT_ACCESS(lightness)
};

template <typename VectorType, typename T>
struct component_access<4, components::hsla, VectorType, T>
    : component_access<3, components::hsla, VectorType, T> {

    using base_type = component_access<3, components::hsla, VectorType, T>;

    using base_type::h;
    using base_type::hue;
    using base_type::l;
    using base_type::lightness;
    using base_type::s;
    using base_type::saturation;

    PSST_MATH_COMPONENT_ACCESS(a)
    PSST_MATH_COMPONENT_ACCESS(alpha)
};
//@}

//@{
template <typename VectorType, typename T>
struct component_access<1, components::grayscale, VectorType, T>
    : basic_component_access<VectorType, T, components::grayscale> {
    using base_type = basic_component_access<VectorType, T, components::grayscale>;

    PSST_MATH_COMPONENT_ACCESS(g)
    PSST_MATH_COMPONENT_ACCESS(gray)
};

template <typename VectorType, typename T>
struct component_access<2, components::grayscale, VectorType, T>
    : component_access<1, components::grayscale, VectorType, T> {
    using base_type = component_access<1, components::grayscale, VectorType, T>;

    using base_type::g;
    using base_type::grey;

    PSST_MATH_COMPONENT_ACCESS(a)
    PSST_MATH_COMPONENT_ACCESS(alpha)
};
//@}

//@{
template <typename VectorType, typename T>
struct component_access<1, components::grayscale_hex, VectorType, T>
    : basic_component_access<VectorType, T, components::grayscale_hex> {
    using base_type = basic_component_access<VectorType, T, components::grayscale_hex>;

    PSST_MATH_COMPONENT_ACCESS(g)
    PSST_MATH_COMPONENT_ACCESS(gray)
};

template <typename VectorType, typename T>
struct component_access<2, components::grayscale_hex, VectorType, T>
    : component_access<1, components::grayscale_hex, VectorType, T> {
    using base_type = component_access<1, components::grayscale_hex, VectorType, T>;

    using base_type::g;
    using base_type::grey;

    PSST_MATH_COMPONENT_ACCESS(a)
    PSST_MATH_COMPONENT_ACCESS(alpha)
};
//@}

}    // namespace component_access
/**
 * Namespace for colour classes and type aliases.
 */
namespace color {

//@{
/** @name is_color */
template <typename T>
struct is_color : traits::has_components_t<T, components::rgba, components::rgba_hex,
                                           components::argb, components::hsla, components::hsva,
                                           components::grayscale, components::grayscale_hex> {};
template <typename T>
using is_color_t = typename is_color<T>::type;
template <typename T>
constexpr bool is_color_v = is_color_t<T>::value;
//@}

/**
 * Type alias for RGB colour (no alpha component).
 */
template <typename T>
using rgb = vector<T, 3, components::rgba>;
/**
 * Type alias for RGBA colour (with alpha component).
 */
template <typename T>
using rgba = vector<T, 4, components::rgba>;

template <typename T>
using argb = vector<T, 4, components::argb>;

template <typename T>
using hsl = vector<T, 3, components::hsla>;
template <typename T>
using hsla = vector<T, 4, components::hsla>;

template <typename T>
using hsv = vector<T, 3, components::hsva>;
template <typename T>
using hsva = vector<T, 4, components::hsva>;

template <typename T>
using grayscale = vector<T, 1, components::grayscale>;
template <typename T>
using grayscale_alpha = vector<T, 2, components::grayscale>;

using rgb_hex  = vector<std::uint8_t, 3, components::rgba_hex>;
using rgba_hex = vector<std::uint8_t, 4, components::rgba_hex>;

using argb_hex = vector<std::uint8_t, 4, components::argb_hex>;

template <typename T>
constexpr T
get_hex_color_component(std::uint8_t hex)
{
    return hex / T{255};
}

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

inline constexpr rgba<double> operator"" _rgba_d(unsigned long long val)
{
    // clang-format off
    rgba<double> res{
        get_hex_color_component<double>((std::uint8_t)((val & 0xff000000) >> 24)),
        get_hex_color_component<double>((std::uint8_t)((val & 0xff0000) >> 16)),
        get_hex_color_component<double>((std::uint8_t)((val & 0xff00) >> 8)),
        get_hex_color_component<double>((std::uint8_t)(val & 0xff))
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

inline constexpr rgba<double> operator"" _rgb_d(unsigned long long val)
{
    // clang-format off
    rgba<double> res{
        get_hex_color_component<double>((std::uint8_t)((val & 0xff0000) >> 16)),
        get_hex_color_component<double>((std::uint8_t)((val & 0xff00) >> 8)),
        get_hex_color_component<double>((std::uint8_t)(val & 0xff))
    };
    // clang-format on
    return res;
}

inline constexpr argb_hex operator"" _argb(unsigned long long val)
{
    // clang-format off
    argb_hex res{
        (std::uint8_t)((val & 0xff000000) >> 24),
        (std::uint8_t)((val & 0xff0000) >> 16),
        (std::uint8_t)((val & 0xff00) >> 8),
        (std::uint8_t)(val & 0xff)
    };
    // clang-format on
    return res;
}

}    // namespace color

namespace expr {

inline namespace v {

template <typename Components, typename Expr>
struct color_chroma;

template <typename Expr>
struct color_chroma<components::hsla, Expr>
    : unary_scalar_expression_components<color_chroma, components::hsla, Expr>,
      unary_expression<Expr> {
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
struct color_chroma<components::hsva, Expr>
    : unary_scalar_expression_components<color_chroma, components::hsva, Expr>,
      unary_expression<Expr> {
    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    constexpr auto
    value() const
    {
        using std::abs;
        return this->arg_.v() * this->arg_.s();
    }
};

template <typename Expr,
          typename = traits::enable_for_components<Expr, components::hsla, components::hsva>>
constexpr auto
chroma(Expr&& expr)
{
    using component_names = traits::component_names_t<Expr>;
    return make_unary_expression<select_unary_impl<component_names, color_chroma>::template type>(
        std::forward<Expr>(expr));
}

//@{
/** @name hex <-> double conversion */
template <typename T, typename Expression>
struct conversion<color::rgb_hex, color::rgb<T>, Expression> : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        using color::get_hex_color_component;
        return color::rgb<T>{get_hex_color_component<T>(this->arg_.r()),
                             get_hex_color_component<T>(this->arg_.g()),
                             get_hex_color_component<T>(this->arg_.b())};
    }
};

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
struct conversion<color::argb_hex, color::argb<T>, Expression> : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        using color::get_hex_color_component;
        return color::argb<T>{
            get_hex_color_component<T>(this->arg_.r()), get_hex_color_component<T>(this->arg_.g()),
            get_hex_color_component<T>(this->arg_.b()), get_hex_color_component<T>(this->arg_.a())};
    }
};

template <typename T, typename Expression>
struct conversion<color::rgb<T>, color::rgb_hex, Expression> : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        return color::rgba_hex{this->arg_.r() * 0xff, this->arg_.g() * 0xff, this->arg_.b() * 0xff};
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

template <typename T, typename Expression>
struct conversion<color::argb<T>, color::argb_hex, Expression> : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        return color::argb_hex{this->arg_.r() * 0xff, this->arg_.g() * 0xff, this->arg_.b() * 0xff,
                               this->arg_.a() * 0xff};
    }
};
//@}

//@{
/** @name HSL -> RGB conversion */
template <typename T, typename U, std::size_t Size, typename Expression>
struct conversion<vector<T, Size, components::hsla>, vector<U, Size, components::rgba>, Expression>
    : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        using value_type  = U;
        using result_type = vector<U, Size, components::rgba>;
        using std::abs;
        using std::fmod;

        value_type c       = chroma(this->arg_);
        value_type segment = this->arg_.hue() * 3 / pi<value_type>::value;
        value_type x       = c * (1 - abs(fmod(segment, 2) - 1));
        value_type m       = this->arg_.l() - c / 2;
        if (0 <= segment && segment < 1) {
            if constexpr (Size >= 4) {
                return result_type{c + m, x + m, m, this->arg_.a()};
            } else {
                return result_type{c + m, x + m, m};
            }
        } else if (1 <= segment && segment < 2) {
            if constexpr (Size >= 4) {
                return result_type{x + m, c + m, m, this->arg_.a()};
            } else {
                return result_type{x + m, c + m, m};
            }
        } else if (2 <= segment && segment < 3) {
            if constexpr (Size >= 4) {
                return result_type{m, c + m, x + m, this->arg_.a()};
            } else {
                return result_type{m, c + m, x + m};
            }
        } else if (3 <= segment && segment < 4) {
            if constexpr (Size >= 4) {
                return result_type{m, x + m, c + m, this->arg_.a()};
            } else {
                return result_type{m, x + m, c + m};
            }
        } else if (4 <= segment && segment < 5) {
            if constexpr (Size >= 4) {
                return result_type{x + m, m, c + m, this->arg_.a()};
            } else {
                return result_type{x + m, m, c + m};
            }
        } else if (5 <= segment && segment < 6) {
            if constexpr (Size >= 4) {
                return result_type{c + m, m, x + m, this->arg_.a()};
            } else {
                return result_type{c + m, m, x + m};
            }
        }
        return result_type{};
    }
};
//@}

//@{
/** @name RGB -> HSL conversion */
template <typename T, typename U, std::size_t Size, typename Expression>
struct conversion<vector<T, Size, components::rgba>, vector<U, Size, components::hsla>, Expression>
    : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        using value_type  = U;
        using result_type = vector<U, Size, components::hsla>;
        using std::abs;
        using std::fmod;
        using std::max;
        using std::min;

        value_type cmax = max(this->arg_.r(), max(this->arg_.g(), this->arg_.b()));
        value_type cmin = min(this->arg_.r(), min(this->arg_.g(), this->arg_.b()));
        value_type d    = cmax - cmin;
        value_type l    = (cmax + cmin) / 2;
        value_type s    = 0;
        if (l != 0 && l != 1) {
            s = d / (1 - abs(2 * l - 1));
        }
        if (d == 0) {
            if constexpr (Size >= 4) {
                return result_type{0, s, l, this->arg_.a()};
            } else {
                return result_type{0, s, l};
            }
        } else if (cmax == this->arg_.r()) {
            if constexpr (Size >= 4) {
                return result_type{pi<value_type>::value / 3
                                       * (value_type)fmod((this->arg_.g() - this->arg_.b()) / d, 6),
                                   s, l, this->arg_.a()};
            } else {
                return result_type{pi<value_type>::value / 3
                                       * (value_type)fmod((this->arg_.g() - this->arg_.b()) / d, 6),
                                   s, l};
            }
        } else if (cmax == this->arg_.g()) {
            if constexpr (Size >= 4) {
                return result_type{pi<value_type>::value / 3
                                       * ((this->arg_.b() - this->arg_.r()) / d + 2),
                                   s, l, this->arg_.a()};
            } else {
                return result_type{
                    pi<value_type>::value / 3 * ((this->arg_.b() - this->arg_.r()) / d + 2), s, l};
            }
        } else if (cmax == this->arg_.b()) {
            if constexpr (Size >= 4) {
                return result_type{pi<value_type>::value / 3
                                       * ((this->arg_.r() - this->arg_.g()) / d + 4),
                                   s, l, this->arg_.a()};
            } else {
                return result_type{
                    pi<value_type>::value / 3 * ((this->arg_.r() - this->arg_.g()) / d + 4), s, l};
            }
        }

        if constexpr (Size >= 4) {
            return result_type{0, s, l, this->arg_.a()};
        } else {
            return result_type{0, s, l};
        }
    }
};
//@}

//@{
/** @name HSV -> RGB conversion */
template <typename T, typename U, std::size_t Size, typename Expression>
struct conversion<vector<T, Size, components::hsva>, vector<U, Size, components::rgba>, Expression>
    : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        using value_type  = U;
        using result_type = vector<U, Size, components::rgba>;
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
struct conversion<vector<T, Size, components::rgba>, vector<U, Size, components::hsva>, Expression>
    : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        using value_type  = U;
        using result_type = vector<U, Size, components::hsva>;
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

}    // namespace math
}    // namespace psst

#endif /* PSST_MATH_COLORS_HPP_ */
