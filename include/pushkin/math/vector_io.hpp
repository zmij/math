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
          bin_mode_{rhs.bin_mode_},
          pretty_{rhs.pretty_},
          start_{rhs.start_},
          end_{rhs.end_},
          delim_{rhs.delim_},
          separator_{rhs.separator_},
          row_sep_{rhs.row_sep_},
          offset_{rhs.offset_}
    {}

    bool
    binmode() const
    {
        return bin_mode_;
    }

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
        fct->bin_mode_    = false;
        return fct;
    }

    vector_facet*
    set_binmode(bool val) const
    {
        vector_facet* fct = new vector_facet{*this};
        fct->bin_mode_    = val;
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
    bool        bin_mode_  = false;
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

/**
 * Manipulator to set binary I/O mode
 */
struct binmode {
    binmode(bool m) : bin_mode_{m} {}

    template <typename CharT>
    void
    apply(std::basic_ostream<CharT>& os) const
    {
        auto const& fct = get_facet(os);
        add_facet(os, fct.set_binmode(bin_mode_));
    }

    template <typename CharT>
    void
    apply(std::basic_istream<CharT>& is) const
    {
        auto const& fct = get_facet(is);
        is >> std::noskipws;
        add_facet(is, fct.set_binmode(bin_mode_));
    }

private:
    bool bin_mode_;
};

//@{
/** @name Detect if a struct is an output manipulator */
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
//@}

//@{
/** @name Detect if a struct is an output manipulator */
template <typename T, typename CharT, typename = utils::void_t<>>
struct is_imanip : std::false_type {};
template <typename T, typename CharT>
struct is_imanip<T, CharT,
                 utils::void_t<decltype(std::declval<T const&>().apply(
                     std::declval<std::basic_istream<CharT>&>()))>> : std::true_type {};
template <typename T, typename CharT>
using is_imanip_t = typename is_imanip<T, CharT>::type;
template <typename T, typename CharT>
constexpr bool is_imanip_v = is_imanip_t<T, CharT>::value;
template <typename T, typename CharT>
using enable_for_imanip = std::enable_if_t<is_imanip_v<T, CharT>>;
//@}

template <typename CharT, typename T, typename = enable_for_omanip<T, CharT>>
std::basic_ostream<CharT>&
operator<<(std::basic_ostream<CharT>& os, T const& v)
{
    v.apply(os);
    return os;
}

template <typename CharT, typename T, typename = enable_for_imanip<T, CharT>>
std::basic_istream<CharT>&
operator>>(std::basic_istream<CharT>& is, T const& v)
{
    v.apply(is);
    return is;
}

//----------------------------------------------------------------------------
// A very naïve implementation for reading and writing binary data. Most likely
// won't work with wchar_t
template <typename CharT, typename T>
std::basic_ostream<CharT>&
write_binary(std::basic_ostream<CharT>& os, T const& val)
{
    std::ostreambuf_iterator<CharT> out{os};
    CharT const*                    p   = reinterpret_cast<CharT const*>(&val);
    auto                            end = p + sizeof(T);
    while (p != end) {
        *out++ = *p++;
    }
    return os;
}

template <typename CharT, typename T>
std::basic_istream<CharT>&
read_binary(std::basic_istream<CharT>& is, T& val)
{
    using std::swap;
    std::istreambuf_iterator<CharT> in{is};
    std::istreambuf_iterator<CharT> end_in;
    T                               tmp{0};
    CharT*                          p   = reinterpret_cast<CharT*>(&tmp);
    auto                            end = p + sizeof(T);
    while (p != end && in != end_in) {
        *p++ = *in++;
    }
    if (p != end) {
        is.setstate(std::ios::failbit);
        return is;
    }
    swap(val, tmp);
    return is;
}

template <typename CharT, typename T, T (*F)(T const&)>
std::basic_istream<CharT>&
read_binary(std::basic_istream<CharT>& is, value_policy::value_clamp<T, F>&& val)
{
    T tmp;
    read_binary(is, tmp);
    val = tmp;
    return is;
}

} /* namespace io */

namespace value_policy {

template <typename T, T (*F)(T const&)>
std::istream&
operator>>(std::istream& is, value_clamp<T, F>&& val)
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

template <typename CharT, typename Vector, std::size_t... Indexes>
std::basic_ostream<CharT>&
write_binary(std::basic_ostream<CharT>& os, Vector const& val, std::index_sequence<Indexes...>)
{
    using value_type = typename Vector::value_type;
    (io::write_binary(os, (value_type)val.template at<Indexes>()), ...);
    return os;
}

}    // namespace detail

template <typename Expression, typename = enable_if_vector_expression<Expression>>
std::ostream&
operator<<(std::ostream& os, Expression const& v)
{
    using expression_type = Expression;
    std::ostream::sentry s(os);
    if (s) {
        auto const& fct = io::get_facet(os);
        if (fct.binmode()) {
            io::write_binary(os, expression_type::size);
            detail::write_binary(os, v, typename expression_type::index_sequence_type{});
        } else {
            os << fct.start();
            if (fct.pretty())
                os << fct.separator();
            detail::vector_output<expression_type::size - 1, expression_type>::output(os, v);
            if (fct.pretty())
                os << fct.separator();
            os << fct.end();
        }
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

template <typename CharT, typename Vector, std::size_t... Indexes>
std::basic_istream<CharT>&
read_binary(std::basic_istream<CharT>& is, Vector& v, std::index_sequence<Indexes...>)
{
    (io::read_binary(is, get<Indexes>(v)), ...);
    return is;
}

}    // namespace detail

template <typename T, std::size_t Size, typename Axes>
std::istream&
operator>>(std::istream& is, vector<T, Size, Axes>& v)
{
    using vector_type = vector<T, Size, Axes>;
    std::istream::sentry s(is);
    if (s) {
        auto const& fct = io::get_facet(is);
        if (fct.binmode()) {
            std::size_t sz{0};
            io::read_binary(is, sz);
            if (sz != vector_type::size) {
                is.setstate(std::ios::failbit);
                return is;
            }
            detail::read_binary(is, v, typename vector_type::index_sequence_type{});
        } else {
            char c = '\0';
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
    }
    return is;
}

}    // namespace math
} /* namespace psst */

#endif /* PUSHKIN_MATH_VECTOR_IO_HPP_ */
