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
namespace expr {

inline namespace m {

namespace detail {

template < ::std::size_t N, typename Matrix >
struct matrix_output {
    static void
    output(std::ostream& os, Matrix const& m)
    {
        matrix_output< N - 1, Matrix >::output(os, m);
        auto const& fct = io::get_facet(os);
        os << fct.delim();
        if (fct.pretty())
            os << fct.row_separator() << fct.offset();
        os << row<N>(m);
    }
};

template < typename Matrix >
struct matrix_output< 0, Matrix > {
    static void
    output(std::ostream& out, Matrix const& m)
    {
        out << row<0>(m);
    }
};

} // namespace detail

template < typename Expression, typename Result >
std::ostream&
operator << (std::ostream& os, matrix_expression<Expression, Result> const& m)
{
    using expression_type = matrix_expression<Expression, Result>;
    std::ostream::sentry s(os);
    if (s) {
        auto const& fct = io::get_facet(os);
        os << fct.start();
        if (fct.pretty())
            os << fct.row_separator() << fct.offset();
        detail::matrix_output< expression_type::rows - 1, expression_type >::output(os, m);
        if (fct.pretty())
            os << fct.row_separator();
        os << fct.end();
    }
    return os;
}

}  // namespace m

}  // namespace expr

} // namespace math
}  /* namespace psst */

#endif /* PUSHKIN_MATH_MATRIX_IO_HPP_ */
