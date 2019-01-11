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

template <std::size_t N, typename Matrix>
struct matrix_output {
    static void
    output(std::ostream& os, Matrix const& m)
    {
        matrix_output<N - 1, Matrix>::output(os, m);
        auto const& fct = io::get_facet(os);
        if (!fct.binmode()) {
            os << fct.delim();
            if (fct.pretty())
                os << fct.row_separator() << fct.offset();
        }
        os << row<N>(m);
    }
};

template <typename Matrix>
struct matrix_output<0, Matrix> {
    static void
    output(std::ostream& out, Matrix const& m)
    {
        out << row<0>(m);
    }
};

}    // namespace detail

template <typename Expression, typename Result>
std::ostream&
operator<<(std::ostream& os, matrix_expression<Expression, Result> const& m)
{
    using expression_type = matrix_expression<Expression, Result>;
    std::ostream::sentry s(os);
    if (s) {
        auto const& fct = io::get_facet(os);
        if (fct.binmode()) {
            io::write_binary(os, expression_type::size);
            detail::matrix_output<expression_type::rows - 1, expression_type>::output(os, m);
        } else {
            os << fct.start();
            if (fct.pretty())
                os << fct.row_separator() << fct.offset();
            detail::matrix_output<expression_type::rows - 1, expression_type>::output(os, m);
            if (fct.pretty())
                os << fct.row_separator();
            os << fct.end();
        }
    }
    return os;
}

}    // namespace m

}    // namespace expr

namespace detail {

template <typename CharT, typename Matrix, std::size_t... RI>
std::basic_istream<CharT>&
matrix_read_binary(std::basic_istream<CharT>& is, Matrix& m, std::index_sequence<RI...>)
{
    (is >> ... >> get<RI>(m));
    return is;
}

}    // namespace detail

template <typename T, std::size_t RC, std::size_t CC, typename Axes>
std::istream&
operator>>(std::istream& is, matrix<T, RC, CC, Axes>& mtx)
{
    using matrix_type = matrix<T, RC, CC, Axes>;
    std::istream::sentry s(is);
    if (s) {
        auto const& fct = io::get_facet(is);
        if (fct.binmode()) {
            std::size_t sz{0};
            io::read_binary(is, sz);
            if (sz != matrix_type::size) {
                is.setstate(std::ios::failbit);
                return is;
            }
            detail::matrix_read_binary(is, mtx, typename matrix_type::row_indexes_type{});
        } else {
            char c = '\0';
            if (!(is >> c)) {
                return is;
            }
            if (c != fct.start()) {
                is.setstate(std::ios::failbit);
                return is;
            }
            detail::data_input<RC - 1, matrix<T, RC, CC, Axes>>::input(is, mtx);
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

#endif /* PUSHKIN_MATH_MATRIX_IO_HPP_ */
