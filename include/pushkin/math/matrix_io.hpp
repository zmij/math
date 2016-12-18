/*
 * Matrix_io.hpp
 *
 *  Created on: 04.12.2012
 *      Author: zmij
 */

#ifndef PUSHKIN_MATH_MATRIX_IO_HPP_
#define PUSHKIN_MATH_MATRIX_IO_HPP_

#include <pushkin/math/matrix.hpp>
#include <pushkin/math/vector_io.hpp>

#include <iostream>

namespace psst {
namespace math {

namespace detail {

template < ::std::size_t, typename T >
struct matrix_output;

template < ::std::size_t N, typename T,
    ::std::size_t RC, ::std::size_t CC,
    typename Axes >
struct matrix_output< N, matrix< T, RC, CC, Axes > > {
    using matrix_type = matrix< T, RC, CC, Axes >;
    static void
    output(std::ostream& os, matrix_type const& m)
    {
        matrix_output< N - 1, matrix_type >::output(os, m);
        auto const& fct = io::get_facet(os);
        os << fct.delim();
        if (fct.pretty())
            os << fct.row_separator() << fct.offset();
        os << m.template at< N >();
    }
};

template < typename T, ::std::size_t RC, ::std::size_t CC, typename Axes >
struct matrix_output< 0, matrix< T, RC, CC, Axes > > {
    using matrix_type = matrix< T, RC, CC, Axes >;
    static void
    output(std::ostream& out, matrix_type const& m)
    {
        out << m.template at< 0 >();
    }
};

} // namespace detail

template < typename T, ::std::size_t RC, ::std::size_t CC, typename Axes >
std::ostream&
operator << (std::ostream& os, matrix< T, RC, CC, Axes > const& m)
{
    std::ostream::sentry s(os);
    if (s) {
        auto const& fct = io::get_facet(os);
        os << fct.start();
        if (fct.pretty())
            os << fct.row_separator() << fct.offset();
        detail::matrix_output< RC - 1, matrix< T, RC, CC, Axes > >::output(os, m);
        if (fct.pretty())
            os << fct.row_separator();
        os << fct.end();
    }
    return os;
}


} // namespace math
}  /* namespace psst */

#endif /* PUSHKIN_MATH_MATRIX_IO_HPP_ */
