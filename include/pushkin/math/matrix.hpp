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
struct matrix : expr::matrix_expression<matrix<T, RC, CC, Axes>>,
            detail::axes_names_t<RC, Axes, matrix<T, RC, CC, Axes>, vector<T, CC, Axes>> {

    using this_type             = matrix< T, RC, CC, Axes >;
    using transposed_type       = matrix< T, CC, RC, Axes >;
    using traits                = matrix_traits<this_type>;

    using row_type              = typename traits::row_type;
    using row_indexes_type      = typename traits::row_indexes_type;
    using col_indexes_type      = typename traits::col_indexes_type;

    using value_type            = typename traits::value_type;
    using lvalue_reference      = typename traits::lvalue_reference;
    using const_reference       = typename traits::const_reference;
    using pointer               = typename traits::pointer;
    using const_pointer         = typename traits::const_pointer;
    using iterator              = typename traits::iterator;
    using const_iterator        = typename traits::const_iterator;

    static constexpr ::std::size_t rows   = traits::rows;
    static constexpr ::std::size_t cols   = traits::cols;
    static constexpr ::std::size_t size   = traits::size;

    using row_iterator          = row_type*;
    using const_row_iterator    = row_type const*;
    using lvalue_row_reference  = ::std::add_lvalue_reference_t< row_type >;
    using const_row_reference   = ::std::add_lvalue_reference_t<::std::add_const_t<row_type>>;

    using multi_dim_type        = value_type[cols];
    using const_multi_dim_type  = value_type const[cols];
    using multi_dim_ptr         = multi_dim_type*;
    using const_multi_dim_ptr   = const_multi_dim_type const*;

    using init_list             = ::std::initializer_list< typename row_type::init_list >;

    constexpr matrix() = default;

    constexpr explicit matrix(value_type val)
        : matrix(val, col_indexes_type{}) {}

    constexpr explicit matrix(const_pointer p)
        : matrix(p, row_indexes_type{}) {}

    constexpr explicit matrix(const_multi_dim_ptr p)
        : matrix(p, row_indexes_type{}) {}

    constexpr matrix(init_list const& args)
        : matrix(args, row_indexes_type{}) {}

    pointer
    data()
    { return std::get<0>(data_).data(); }
    const_pointer
    data() const
    { return std::get<0>(data_).data(); }

    template < ::std::size_t R >
    lvalue_row_reference
    at()
    {
        static_assert(R < rows, "Invalid matrix row index");
        return ::std::get<R>(data_);
    }

    template < ::std::size_t R >
    const_row_reference
    at() const
    {
        static_assert(R < rows, "Invalid matrix row index");
        return ::std::get<R>(data_);
    }

    template < ::std::size_t R, ::std::size_t C >
    lvalue_reference
    element()
    {
        static_assert(C < cols, "Invalid matrix column index");
        return at<R>().template at<C>();
    }
    template < ::std::size_t R, ::std::size_t C >
    const_reference
    element() const
    {
        static_assert(C < cols, "Invalid matrix column index");
        return at<R>().template at<C>();
    }

    iterator
    begin()
    { return data_.front().begin(); }
    const_iterator
    begin() const { return cbegin(); }
    const_iterator
    cbegin() const
    { return data_.front().cbegin(); }

    iterator
    end()
    { return data_.back().end(); }
    const_iterator
    end() const
    { return cend(); }
    const_iterator
    cend() const
    { return data_.back().end(); }

    row_iterator
    row_begin()
    { return data_.begin(); }
    const_row_iterator
    row_begin() const
    { return data_.begin(); }

    row_iterator
    row_end()
    { return data_.end(); }
    const_row_iterator
    row_end() const
    { return data_.end(); }

    lvalue_row_reference
    operator[] (::std::size_t idx)
    {
        assert(idx < rows);
        return data_[idx];
    }
    const_row_reference
    operator[] (::std::size_t idx) const
    {
        assert(idx < rows);
        return data_[idx];
    }

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
private:
    template < ::std::size_t... RI >
    constexpr matrix(value_type val, ::std::index_sequence<RI...>)
        : data_({ utils::value_fill<RI, row_type>{row_type(val)}.value... }) {}
    template < ::std::size_t... RI >
    constexpr matrix(init_list const& args, ::std::index_sequence<RI...>)
        : data_({ row_type( *(args.begin() + RI) ) ... }) {}
    template < ::std::size_t... RI >
    constexpr matrix(const_pointer p, ::std::index_sequence<RI...>)
        : data_({ row_type(p + RI * cols) ... }) {}
    template < ::std::size_t... RI >
    constexpr matrix(const_multi_dim_ptr p, ::std::index_sequence<RI...>)
        : data_({ row_type(p[RI]) ... }) {}
private:
    using data_type     = ::std::array<row_type, rows>;
    data_type data_;
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
