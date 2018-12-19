/*
 * Matrix.hpp
 *
 *  Created on: 19.01.2012
 *      Author: zmij
 */

#ifndef PUSHKIN_MATH_MATRIX_HPP_
#define PUSHKIN_MATH_MATRIX_HPP_

#include <pushkin/math/detail/matrix_detail.hpp>

namespace psst {
namespace math {

/**
 * Matrix template.
 * @tparam T type of value in a matrix cell
 * @tparam RC row count
 * @tparam CC column count;
 */
template < typename T, ::std::size_t RC, ::std::size_t CC, typename Axes >
struct matrix : detail::matrix_builder< std::make_index_sequence< RC >, CC, T, Axes > {

    using base_type         = detail::matrix_builder<
                                std::make_index_sequence< RC >, CC, T, Axes >;

    using this_type         = matrix< T, RC, CC, Axes >;
    using transposed_type   = matrix< T, CC, RC, Axes >;

    using row_type          = typename base_type::row_type;
    using value_type        = typename base_type::value_type;

    using pointer           = typename base_type::pointer;
    using const_pointer     = typename base_type::const_pointer;

    constexpr matrix() = default;

    constexpr matrix( std::initializer_list< std::initializer_list< value_type > > const& args) :
        base_type(args) {}

    template < typename ... E >
    constexpr matrix(E const& ... args) :
        base_type(args ... ) {}

    using base_type::data;
    using base_type::at;
    using base_type::begin;
    using base_type::cbegin;
    using base_type::end;
    using base_type::cend;
    using base_type::row_begin;
    using base_type::row_end;
    using base_type::operator[];

    this_type
    operator - ()
    {
        this_type res(*this);
        res *= -1;
        return res;
    }

    template < typename U >
    this_type&
    operator += (matrix<U, RC, CC, Axes> const& rhs)
    {
        detail::matrix_addition< RC - 1, this_type, matrix<U, RC, CC, Axes> >{}(*this, rhs);
        return *this;
    }

    template < typename U >
    this_type&
    operator -= (matrix<U, RC, CC, Axes> const& rhs)
    {
        detail::matrix_subtraction<RC - 1, this_type, matrix<U, RC, CC, Axes>>{}(*this, rhs);
        return *this;
    }

    template < typename U >
    this_type&
    operator *= (U s)
    {
        detail::matrix_scalar_multiplication< RC - 1, this_type >()(*this, s);
        return *this;
    }

    template < typename U >
    this_type&
    operator /= (U s)
    {
        detail::matrix_scalar_division< RC - 1, this_type >()(*this, s);
        return *this;
    }

    transposed_type
    transpose() const
    {
        transposed_type res;

        detail::matrix_transpose< RC - 1, this_type >()(res, *this);

        return res;
    }

    /**
     * Implicit conversion to pointer to element
     */
    operator pointer()
    { return data(); }
    /**
     * Implicit conversion to const pointer to element
     */
    operator const_pointer() const
    { return data(); }

    template < typename U = T >
    static typename ::std::enable_if< RC == CC, matrix<U, RC, CC, Axes> >::type const&
    identity()
    {
        static this_type _identity(detail::identity_matrix< RC - 1, this_type >::build_matrix());
        return _identity;
    }
};

template < typename T, typename U, ::std::size_t RC, ::std::size_t CC, typename Axes >
bool
operator == (matrix<T, RC, CC, Axes> const& lhs, matrix<U, RC, CC, Axes> const& rhs)
{
    using left_side     = matrix<T, RC, CC, Axes>;
    using right_side    = matrix<U, RC, CC, Axes>;
    return detail::matrix_cmp<RC - 1, left_side, right_side>::eq(lhs, rhs);
}

template < typename T, typename U, ::std::size_t RC, ::std::size_t CC, typename Axes >
bool
operator != (matrix<T, RC, CC, Axes> const& lhs, matrix<U, RC, CC, Axes> const& rhs)
{
    return !(lhs == rhs);
}

template < typename T, typename U, ::std::size_t RC, ::std::size_t CC, typename Axes >
bool
operator < (matrix<T, RC, CC, Axes> const& lhs, matrix<U, RC, CC, Axes> const& rhs)
{
    using left_side     = matrix<T, RC, CC, Axes>;
    using right_side    = matrix<U, RC, CC, Axes>;
    return detail::matrix_cmp<RC - 1, left_side, right_side>::less(lhs, rhs);
}

template <typename T, typename U, ::std::size_t RC, ::std::size_t CC, typename Axes >
matrix< T, RC, CC, Axes >
operator + (matrix<T, RC, CC, Axes> const& lhs, matrix<U, RC, CC, Axes> const& rhs)
{
    matrix< T, RC, CC, Axes > res(lhs);
    res += rhs;
    return res;
}

template <typename T, typename U, ::std::size_t RC, ::std::size_t CC, typename Axes >
matrix< T, RC, CC, Axes >
operator - (matrix<T, RC, CC, Axes> const& lhs, matrix<U, RC, CC, Axes> const& rhs)
{
    matrix< T, RC, CC, Axes > res(lhs);
    res -= rhs;
    return res;
}

/**
 * Matrix scalar multiplication
 */
template < typename T, typename U, ::std::size_t RC, ::std::size_t CC, typename Axes >
matrix< T, RC, CC, Axes >
operator * (matrix<T, RC, CC, Axes> const& lhs, U rhs)
{
    matrix<T, RC, CC, Axes> res(lhs);
    res *= rhs;
    return res;
}

template < typename T, typename U, ::std::size_t RC, ::std::size_t CC, typename Axes >
matrix< T, RC, CC, Axes >
operator * (U lhs, matrix<T, RC, CC, Axes> const& rhs)
{
    matrix<T, RC, CC, Axes> res(rhs);
    res *= lhs;
    return res;
}

/**
 * Matrix scalar division
 */
template < typename T, typename U, ::std::size_t RC, ::std::size_t CC, typename Axes >
matrix< T, RC, CC, Axes >
operator / (matrix<T, RC, CC, Axes> const& lhs, U rhs)
{
    matrix<T, RC, CC, Axes> res(lhs);
    res /= rhs;
    return res;
}

/**
 * Multiplication of two matrices
 * @param lhs
 * @param rhs
 * @return
 */
template < typename T, typename U, ::std::size_t R1, ::std::size_t C1, ::std::size_t C2, typename Axes >
matrix< T, R1, C2, Axes >
operator * (matrix< T, R1, C1, Axes > const& lhs, matrix< U, C1, C2, Axes > const& rhs )
{
    using left_side_type = matrix< T, R1, C1, Axes >;
    using right_side_type = matrix< U, C1, C2, Axes >;
    matrix< T, R1, C2, Axes > res;

    detail::matrix_row_multiply< R1 - 1, left_side_type,  right_side_type >()(res, lhs, rhs.transpose());

    return res;
}

/**
 * Multiplication of a square matrix and a vector.
 * Vector is considered a column matrix.
 */
template < typename T, typename U, ::std::size_t C, typename Axes >
vector< T, C, Axes >
operator * (matrix< T, C, C, Axes > const& m, vector< U, C, Axes > const& v)
{
    using multiplicator = detail::matrix_vector_multiply< C -1,
                            matrix< T, C, C, Axes >, vector< U, C, Axes > >;
    using result_type   = vector< T, C, Axes >;

    result_type res;

    multiplicator()(res, m, v);

    return res;
}

/**
 * Multiplication of a vector and a matrix.
 * Vector is considered a row matrix.
 * @param v
 * @param m
 * @return
 */
template < typename T, typename U, ::std::size_t C, typename Axes >
vector< T, C, Axes >
operator * (vector< U, C, Axes > const& v, matrix< T, C, C, Axes > const& m)
{
    return m.transpose() * v;
}

// TODO matrix minor
// TODO matrix cofactor
// TODO matrix determinant

template < ::std::size_t R, ::std::size_t C >
struct matrix_size {
    static constexpr ::std::size_t rows = R;
    static constexpr ::std::size_t cols = C;
    static constexpr ::std::size_t size = rows * cols;
};

template < typename T >
struct matrix_traits;

template < typename T, ::std::size_t R, ::std::size_t C, typename Axes >
struct matrix_traits<matrix<T, R, C, Axes>> {
    using matrix_type                   = matrix<T, R, C, Axes>;
    using value_type                    = typename matrix_type::value_type;
    using size_type                     = matrix_size<R, C>;
    using axes_names                    = Axes;
    static constexpr ::std::size_t rows = size_type::rows;
    static constexpr ::std::size_t cols = size_type::cols;
    static constexpr ::std::size_t size = size_type::size;
};


} // namespace math
}  /* namespace psst */

//
//template < typename T, ::std::size_t Rows, ::std::size_t Cols >
//Matrix< T, Rows - 1, Cols - 1 >
//minor( Matrix< T, Rows, Cols> const& m, ::std::size_t r, ::std::size_t c )
//{
//    Matrix< T, Rows - 1, Cols - 1 > res;
//
//    ::std::size_t rr = 0;
//    for (::std::size_t i = 0; i < Rows; ++i) {
//        if (i == r)
//            continue;
//        ::std::size_t rc = 0;
//        for (::std::size_t j = 0; j < Cols; ++j) {
//            if (j == c)
//                continue;
//            res[rr][rc] = m[i][j];
//            ++rc;
//        }
//        ++rr;
//    }
//
//    return res;
//}
//
//template < typename T >
//T
//determinant( Matrix< T, 1, 1> const& m )
//{
//    return m[0][0];
//}
//
//template < typename T >
//T
//determinant( Matrix< T, 2, 2> const& m )
//{
//    return m[0][0] * m[1][1] - m[0][1] * m [1][0];
//}
//
//template < typename T >
//T
//cofactor( Matrix< T, 1, 1> const& )
//{
//    return 0;
//}
//
//template < typename T, ::std::size_t Size >
//T
//cofactor( Matrix<T, Size, Size> const& m, ::std::size_t r, ::std::size_t c )
//{
//    // sign == -1 ^ (r + c)
//    int sign = ((r + c) % 2) ? -1 : 1;
//
//    return sign * determinant( minor(m, r, c) );
//}
//
//template < typename T, ::std::size_t Size >
//T
//determinant( Matrix< T, Size, Size > const& m )
//{
//    T det = 0;
//    for (::std::size_t i = 0; i < Size; ++i) {
//        det += m[0][i] * cofactor(m, 0, i);
//    }
//    return det;
//}
//

#endif /* PUSHKIN_MATRIX_HPP_ */
