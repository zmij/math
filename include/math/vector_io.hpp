/*
 * vector_io.hpp
 *
 *  Created on: 13.02.2012
 *      Author: zmij
 */

#ifndef ZMIJ_MATH_VECTOR_IO_HPP_
#define ZMIJ_MATH_VECTOR_IO_HPP_

#include <math/vector.hpp>
#include <iostream>

namespace math {
namespace io {

template < typename CharT >
class vector_facet : public ::std::locale::facet {
public:
    static ::std::locale::id id;
public:
    using char_type     = CharT;
    using string_type   = ::std::basic_string<CharT>;
public:
    vector_facet()
        : facet{}
    {};
    vector_facet(char_type start, char_type end)
        : facet{},
          start_{start},
          end_{end}
    {};

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
    { return pretty_; }

    char_type
    start() const
    { return start_; }
    char_type
    end() const
    { return end_; }

    char_type
    delim() const
    { return delim_; }

    char_type
    separator() const
    { return separator_; }

    string_type const&
    row_separator() const
    { return row_sep_; }
    string_type const&
    offset() const
    { return offset_; }

    vector_facet*
    make_pretty(bool val) const
    {
        vector_facet* fct = new vector_facet{*this};
        fct->pretty_ = val;
        return fct;
    }

    vector_facet*
    set_braces(char_type start, char_type end) const
    {
        vector_facet* fct = new vector_facet{*this};
        fct->start_ = start;
        fct->end_ = end;
        return fct;
    }
private:
    bool pretty_            = false;
    char_type start_        = '{';
    char_type end_          = '}';
    char_type delim_        = ',';
    char_type separator_    = ' ';
    string_type row_sep_    = "\n";
    string_type offset_     = "\t";
};

template < typename CharT >
::std::locale::id vector_facet<CharT>::id;

template < typename CharT >
vector_facet<char> const&
get_facet(::std::basic_ostream<CharT>& os)
{
    using facet_type = vector_facet<CharT>;
    ::std::locale loc = os.getloc();
    if (!::std::has_facet<facet_type>(loc)) {
        os.imbue(::std::locale(loc, new facet_type{}));
        loc = os.getloc();
    }
    return ::std::use_facet< facet_type >(loc);
}

template < typename CharT >
void
add_facet(::std::basic_ostream<CharT>& os, vector_facet<CharT>* fct)
{
    ::std::locale loc = os.getloc();
    os.imbue(::std::locale(loc, fct));
}

template < typename CharT >
::std::basic_ostream<CharT>&
vector_pretty(::std::basic_ostream<CharT>& os)
{
    auto const& fct = get_facet(os);
    if (!fct.pretty()) {
        add_facet(os, fct.make_pretty(true));
    }
    return os;
}

template < typename CharT >
struct basic_set_braces {
    using char_type = CharT;

    basic_set_braces(char_type start, char_type end)
        : start_{start}, end_{end}
    {
    }

    void
    apply(::std::basic_ostream<CharT>& os) const
    {
        auto const& fct = get_facet(os);
        add_facet(os, fct.set_braces(start_, end_));
    }
private:
    char_type start_;
    char_type end_;
};

template < typename CharT >
::std::basic_ostream<CharT>&
operator << (::std::basic_ostream<CharT>& os, basic_set_braces<CharT> const& v)
{
    v.apply(os);
    return os;
}

using set_braces = basic_set_braces<char>;

}  /* namespace io */

namespace detail {

template< size_t, typename T >
struct vector_output;

template< size_t N, typename T, size_t Size >
struct vector_output< N, vector< T, Size > > {
    using vector_type = vector< T, Size >;
    static void
    output(std::ostream& os, vector_type const& v)
    {
        vector_output< N - 1, vector_type >::output(os, v);
        auto const& fct = io::get_facet(os);
        os << fct.delim();
        if (fct.pretty()) {
            os << fct.separator();
        }
        os << v.template at< N >();
    }
};

template< typename T, size_t Size >
struct vector_output< 0, vector< T, Size > > {
    using vector_type = vector< T, Size >;
    static void
    output(std::ostream& out, vector_type const& v)
    {
        out << v.template at< 0 >();
    }
};

} // namespace detail

template < typename T, size_t Size >
std::ostream&
operator << (std::ostream& os, vector<T, Size> const& v)
{
    std::ostream::sentry s(os);
    if (s) {
        auto const& fct = io::get_facet(os);
        os << fct.start();
        if (fct.pretty())
            os << fct.separator();
        detail::vector_output< Size - 1, vector<T, Size> >::output(os, v);
        if (fct.pretty())
            os << fct.separator();
        os << fct.end();
    }
    return os;
}

} // namespace math

#endif /* ZMIJ_MATH_VECTOR_IO_HPP_ */
