/*
 * vector_io.hpp
 *
 *  Created on: 13.02.2012
 *      Author: zmij
 */

#ifndef PUSHKIN_MATH_VECTOR_IO_HPP_
#define PUSHKIN_MATH_VECTOR_IO_HPP_

#include <pushkin/math/vector.hpp>

#include <iomanip>
#include <iostream>

namespace psst {
namespace math {
namespace io {

template <typename CharT>
class vector_facet : public std::locale::facet {
public:
    static std::locale::id id;

public:
    using char_type   = CharT;
    using string_type = std::basic_string<CharT>;

    static constexpr std::size_t npos = std::numeric_limits<std::size_t>::max();

public:
    vector_facet() : facet{} {};
    vector_facet(char_type start, char_type end) : facet{}, start_{start}, end_{end} {};

    vector_facet(vector_facet const& rhs)
        : facet{},
          pretty_{rhs.pretty_},
          start_{rhs.start_},
          end_{rhs.end_},
          delim_{rhs.delim_},
          separator_{rhs.separator_},
          row_sep_{rhs.row_sep_},
          offset_{rhs.offset_}
    {}

    bool
    pretty() const
    {
        return pretty_;
    }

    char_type
    start() const
    {
        return start_;
    }
    char_type
    end() const
    {
        return end_;
    }

    char_type
    delim() const
    {
        return delim_;
    }

    char_type
    separator() const
    {
        return separator_;
    }

    string_type const&
    row_separator() const
    {
        return row_sep_;
    }
    string_type const&
    offset() const
    {
        return offset_;
    }

    std::size_t
    col_width() const
    {
        return col_width_;
    }

    vector_facet*
    make_pretty(bool val) const
    {
        vector_facet* fct = new vector_facet{*this};
        fct->pretty_      = val;
        return fct;
    }

    vector_facet*
    set_braces(char_type start, char_type end) const
    {
        vector_facet* fct = new vector_facet{*this};
        fct->start_       = start;
        fct->end_         = end;
        return fct;
    }

    vector_facet*
    set_col_width(std::size_t w) const
    {
        vector_facet* fct = new vector_facet{*this};
        fct->col_width_   = w;
        return fct;
    }

private:
    bool        pretty_    = false;
    char_type   start_     = '{';
    char_type   end_       = '}';
    char_type   delim_     = ',';
    char_type   separator_ = ' ';
    string_type row_sep_   = "\n";
    string_type offset_    = "  ";

    std::size_t col_width_ = npos;
};

template <typename CharT>
std::locale::id vector_facet<CharT>::id;

template <typename CharT>
vector_facet<CharT> const&
get_facet(std::basic_ostream<CharT>& os)
{
    using facet_type = vector_facet<CharT>;
    std::locale loc  = os.getloc();
    if (!std::has_facet<facet_type>(loc)) {
        os.imbue(std::locale(loc, new facet_type{}));
        loc = os.getloc();
    }
    return std::use_facet<facet_type>(loc);
}

template <typename CharT>
void
add_facet(std::basic_ostream<CharT>& os, vector_facet<CharT>* fct)
{
    std::locale loc = os.getloc();
    os.imbue(std::locale(loc, fct));
}

template <typename CharT>
vector_facet<CharT> const&
get_facet(std::basic_istream<CharT>& is)
{
    using facet_type = vector_facet<CharT>;
    std::locale loc  = is.getloc();
    if (!std::has_facet<facet_type>(loc)) {
        is.imbue(std::locale(loc, new facet_type{}));
        loc = is.getloc();
    }
    return std::use_facet<facet_type>(loc);
}

template <typename CharT>
void
add_facet(std::basic_istream<CharT>& is, vector_facet<CharT>* fct)
{
    std::locale loc = is.getloc();
    is.imbue(std::locale(loc, fct));
}

/**
 * Iostream manipulator to turn pretty-printing on
 * @code
 * std::cout << pretty << vector3d{ 0, 1, 2 } << ugly;
 * @endcode
 * @param os
 * @return
 */
template <typename CharT>
std::basic_ostream<CharT>&
pretty(std::basic_ostream<CharT>& os)
{
    auto const& fct = get_facet(os);
    if (!fct.pretty()) {
        add_facet(os, fct.make_pretty(true));
    }
    return os;
}

/**
 * Iostream manipulator to turn pretty-printing off
 * @code
 * std::cout << pretty << vector3d{ 0, 1, 2 } << ugly;
 * @endcode
 * @param os
 * @return
 */
template <typename CharT>
std::basic_ostream<CharT>&
ugly(std::basic_ostream<CharT>& os)
{
    auto const& fct = get_facet(os);
    if (fct.pretty()) {
        add_facet(os, fct.make_pretty(false));
    }
    return os;
}

template <typename CharT>
struct basic_set_braces {
    using char_type = CharT;

    basic_set_braces(char_type start, char_type end) : start_{start}, end_{end} {}

    void
    apply(std::basic_ostream<CharT>& os) const
    {
        auto const& fct = get_facet(os);
        add_facet(os, fct.set_braces(start_, end_));
    }

private:
    char_type start_;
    char_type end_;
};

using set_braces = basic_set_braces<char>;

struct set_col_width {
    set_col_width(std::size_t col_w) : col_width_{col_w} {}

    template <typename CharT>
    void
    apply(std::basic_ostream<CharT>& os) const
    {
        auto const& fct = get_facet(os);
        add_facet(os, fct.set_col_width(col_width_));
    }

private:
    std::size_t col_width_;
};

template <typename T, typename CharT, typename = utils::void_t<>>
struct is_omanip : std::false_type {};
template <typename T, typename CharT>
struct is_omanip<T, CharT,
                 utils::void_t<decltype(std::declval<T const&>().apply(
                     std::declval<std::basic_ostream<CharT>&>()))>> : std::true_type {};
template <typename T, typename CharT>
using is_omanip_t = typename is_omanip<T, CharT>::type;
template <typename T, typename CharT>
constexpr bool is_omanip_v = is_omanip_t<T, CharT>::value;
template <typename T, typename CharT>
using enable_for_omanip = std::enable_if_t<is_omanip_v<T, CharT>>;

template <typename CharT, typename T, typename = enable_for_omanip<T, CharT>>
std::basic_ostream<CharT>&
operator<<(std::basic_ostream<CharT>& os, T const& v)
{
    v.apply(os);
    return os;
}

} /* namespace io */

namespace value_policy {

template <typename T, T (*F)(T const&)>
std::istream&
operator>>(std::istream& is, value_clamp<T, F>& val)
{
    T tmp;
    is >> tmp;
    val = tmp;
    return is;
}

}    // namespace value_policy
namespace expr {
inline namespace v {

namespace detail {

template <std::size_t N, typename Vector>
struct vector_output {
    static void
    output(std::ostream& os, Vector const& v)
    {
        vector_output<N - 1, Vector>::output(os, v);
        auto const& fct = io::get_facet(os);
        os << fct.delim();
        if (fct.pretty()) {
            os << fct.separator();
            if (fct.col_width() != io::vector_facet<char>::npos) {
                os << std::setw(fct.col_width()) << std::setfill(' ')
                   << std::setprecision(fct.col_width() - 2);
            }
        }
        os << get<N>(v);
    }
};

template <typename Vector>
struct vector_output<0, Vector> {
    static void
    output(std::ostream& os, Vector const& v)
    {
        auto const& fct = io::get_facet(os);
        if (fct.pretty()) {
            if (fct.col_width() != io::vector_facet<char>::npos) {
                os << std::setw(fct.col_width()) << std::setfill(' ')
                   << std::setprecision(fct.col_width() - 2);
            }
        }
        os << get<0>(v);
    }
};

}    // namespace detail

template <typename Expression, typename Result>
std::ostream&
operator<<(std::ostream& os, vector_expression<Expression, Result> const& v)
{
    using expression_type = vector_expression<Expression, Result>;
    std::ostream::sentry s(os);
    if (s) {
        auto const& fct = io::get_facet(os);
        os << fct.start();
        if (fct.pretty())
            os << fct.separator();
        detail::vector_output<expression_type::size - 1, expression_type>::output(os, v);
        if (fct.pretty())
            os << fct.separator();
        os << fct.end();
    }
    return os;
}

}    // namespace v

}    // namespace expr

namespace detail {

template <std::size_t N, typename Vector>
struct data_input {
    static void
    input(std::istream& is, Vector& v)
    {
        auto const& fct = io::get_facet(is);
        data_input<N - 1, Vector>::input(is, v);
        char c = '\0';
        if (!(is >> c))
            return;
        if (c != fct.delim()) {
            is.setstate(std::ios::failbit);
            return;
        }
        is >> get<N>(v);
    }
};

template <typename Vector>
struct data_input<0, Vector> {
    static void
    input(std::istream& is, Vector& v)
    {
        is >> get<0>(v);
    }
};

}    // namespace detail

template <typename T, std::size_t Size, typename Axes>
std::istream&
operator>>(std::istream& is, vector<T, Size, Axes>& v)
{
    std::istream::sentry s(is);
    if (s) {
        auto const& fct = io::get_facet(is);
        char        c   = '\0';
        if (!(is >> c)) {
            return is;
        }
        if (c != fct.start()) {
            is.setstate(std::ios::failbit);
            return is;
        }
        detail::data_input<Size - 1, vector<T, Size, Axes>>::input(is, v);
        if (!(is >> c)) {
            return is;
        }
        if (c != fct.end()) {
            is.setstate(std::ios::failbit);
            return is;
        }
    }
    return is;
}

}    // namespace math
} /* namespace psst */

#endif /* PUSHKIN_MATH_VECTOR_IO_HPP_ */
