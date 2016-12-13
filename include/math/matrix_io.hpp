/*
 * Matrix_io.hpp
 *
 *  Created on: 04.12.2012
 *      Author: zmij
 */

#ifndef PUSHKIN_MATH_MATRIX_IO_HPP_
#define PUSHKIN_MATH_MATRIX_IO_HPP_

#include <math/matrix.hpp>
#include <math/vector_io.hpp>

#include <iostream>

namespace psst {
namespace math {

namespace detail {

template < size_t, typename T >
struct matrix_output;

template < size_t N, typename T, size_t RC, size_t CC >
struct matrix_output< N, matrix< T, RC, CC > > {
    using matrix_type = matrix< T, RC, CC >;
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

template < typename T, size_t RC, size_t CC >
struct matrix_output< 0, matrix< T, RC, CC > > {
    using matrix_type = matrix< T, RC, CC >;
    static void
    output(std::ostream& out, matrix_type const& m)
    {
        out << m.template at< 0 >();
    }
};

} // namespace detail

template < typename T, size_t RC, size_t CC >
std::ostream&
operator << (std::ostream& os, matrix< T, RC, CC> const& m)
{
    std::ostream::sentry s(os);
    if (s) {
        auto const& fct = io::get_facet(os);
        os << fct.start();
        if (fct.pretty())
            os << fct.row_separator() << fct.offset();
        detail::matrix_output< RC - 1, matrix< T, RC, CC > >::output(os, m);
        if (fct.pretty())
            os << fct.row_separator();
        os << fct.end();
    }
    return os;
}


} // namespace math
}  /* namespace psst */

#endif /* PUSHKIN_MATH_MATRIX_IO_HPP_ */
