/*
 * Matrix.hpp
 *
 *  Created on: 19.01.2012
 *      Author: zmij
 */

#ifndef ZMIJ_MATH_MATRIX_HPP_
#define ZMIJ_MATH_MATRIX_HPP_

#include "math/detail/matrix_detail.hpp"
#include <array>

namespace math {

/**
 * Matrix template.
 * @tparam T type of value in a matrix cell
 * @tparam RC row count
 * @tparam CC column count;
 */
template < typename T, size_t RC, size_t CC >
	struct matrix : detail::matrix_builder< typename detail::index_builder< RC >::type, CC, T > {

		typedef detail::matrix_builder<
				typename detail::index_builder< RC >::type, CC, T > super_type;

		typedef matrix< T, RC, CC > this_type;
		typedef matrix< T, CC, RC > transposed_type;

		typedef typename super_type::row_type row_type;

		typedef typename super_type::value_type value_type;

		matrix() :
			super_type()
		{
		}

		matrix( std::initializer_list< std::initializer_list< value_type > > const& args) :
			super_type(args)
		{
		}

		template < typename ... E >
		matrix(E const& ... args) :
			super_type(args ... )
		{
		}

		template < typename U >
		this_type&
		operator += (matrix<U, RC, CC> const& rhs)
		{
			detail::matrix_addition< RC - 1, this_type >()(*this, rhs);
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

		static this_type const&
		identity(typename boost::enable_if_c< RC == CC >::type* = 0)
		{
			static this_type _identity(detail::identity_matrix< RC - 1, this_type >::build_matrix());
			return _identity;
		}
	};

template <typename T, typename U, size_t RC, size_t CC >
matrix< T, RC, CC >
operator + (matrix<T, RC, CC> const& lhs, matrix<U, RC, CC> const& rhs)
{
	matrix< T, RC, CC > res(lhs);
	res += rhs;
	return res;
}

/**
 * Matrix scalar multiplication
 */
template < typename T, typename U, size_t RC, size_t CC >
matrix< T, RC, CC >
operator * (matrix<T, RC, CC> const& lhs, U rhs)
{
	matrix<T, RC, CC> res(lhs);
	res *= rhs;
	return res;
}

template < typename T, typename U, size_t RC, size_t CC >
matrix< T, RC, CC >
operator * (U lhs, matrix<T, RC, CC> const& rhs)
{
	matrix<T, RC, CC> res(rhs);
	res *= lhs;
	return res;
}

/**
 * Matrix scalar division
 */
template < typename T, typename U, size_t RC, size_t CC >
matrix< T, RC, CC >
operator / (matrix<T, RC, CC> const& lhs, U rhs)
{
	matrix<T, RC, CC> res(lhs);
	res /= rhs;
	return res;
}

/**
 * Multiplication of two matrices
 * @param lhs
 * @param rhs
 * @return
 */
template < typename T, typename U, size_t R1, size_t C1, size_t C2 >
matrix< T, R1, C2 >
operator * (matrix< T, R1, C1 > const& lhs, matrix< U, C1, C2 > const& rhs )
{
	typedef matrix< T, R1, C1 > left_side_type;
	typedef matrix< U, C1, C2 > right_side_type;
	matrix< T, R1, C2 > res;

	detail::matrix_row_multiply< R1 - 1, left_side_type,  right_side_type >()(res, lhs, rhs.transpose());

	return res;
}

/**
 * Multiplication of a matrix and a vector.
 * Vector is considered a column matrix.
 */
template < typename T, typename U, size_t C >
vector< T, C >
operator * (matrix< T, C, C > const& m, vector< U, C> const& v)
{
	typedef detail::matrix_vector_multiply< C -1, matrix< T, C, C >, vector< U, C > >
		multiplicator;

	typedef vector< T, C > result_type;

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
template < typename T, typename U, size_t C >
vector< T, C >
operator * (vector< U, C> const& v, matrix< T, C, C > const& m)
{
	return m.transpose() * v;
}

// TODO matrix minor
// TODO matrix cofactor
// TODO matrix determinant

} // namespace math

//
//template < typename T, size_t Rows, size_t Cols >
//Matrix< T, Rows - 1, Cols - 1 >
//minor( Matrix< T, Rows, Cols> const& m, size_t r, size_t c )
//{
//	Matrix< T, Rows - 1, Cols - 1 > res;
//
//	size_t rr = 0;
//	for (size_t i = 0; i < Rows; ++i) {
//		if (i == r)
//			continue;
//		size_t rc = 0;
//		for (size_t j = 0; j < Cols; ++j) {
//			if (j == c)
//				continue;
//			res[rr][rc] = m[i][j];
//			++rc;
//		}
//		++rr;
//	}
//
//	return res;
//}
//
//template < typename T >
//T
//determinant( Matrix< T, 1, 1> const& m )
//{
//	return m[0][0];
//}
//
//template < typename T >
//T
//determinant( Matrix< T, 2, 2> const& m )
//{
//	return m[0][0] * m[1][1] - m[0][1] * m [1][0];
//}
//
//template < typename T >
//T
//cofactor( Matrix< T, 1, 1> const& )
//{
//	return 0;
//}
//
//template < typename T, size_t Size >
//T
//cofactor( Matrix<T, Size, Size> const& m, size_t r, size_t c )
//{
//	// sign == -1 ^ (r + c)
//	int sign = ((r + c) % 2) ? -1 : 1;
//
//	return sign * determinant( minor(m, r, c) );
//}
//
//template < typename T, size_t Size >
//T
//determinant( Matrix< T, Size, Size > const& m )
//{
//	T det = 0;
//	for (size_t i = 0; i < Size; ++i) {
//		det += m[0][i] * cofactor(m, 0, i);
//	}
//	return det;
//}
//

#endif /* ZMIJ_MATRIX_HPP_ */
