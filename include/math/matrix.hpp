/*
 * Matrix.hpp
 *
 *  Created on: 19.01.2012
 *      Author: zmij
 */

#ifndef OPENGAMES_MATH_MATRIX_HPP_
#define OPENGAMES_MATH_MATRIX_HPP_

#include "math/vector.hpp"
#include <array>

namespace math {

namespace detail {

template < typename RowIndexTuple, size_t ColCount, typename T >
	struct matrix_builder;

template < size_t ... RowIndexes, size_t CC, typename T >
	struct matrix_builder < detail::indexes_tuple< RowIndexes ... >, CC, T > :
		detail::data_holder< RowIndexes, vector< T, CC > > ... {

		typedef detail::indexes_tuple< RowIndexes ... > row_index_tuple;
		typedef matrix_builder < row_index_tuple, CC, T > this_type;

		typedef typename detail::index_builder< CC >::type col_index_tuple;
		typedef matrix_builder< col_index_tuple, row_index_tuple::size, T > transposed_type;

		typedef vector< T, CC > row_type;

		typedef row_type* iterator;
		typedef row_type const* const_iterator;

		typedef typename row_type::element_type element_type;
		typedef typename row_type::value_type value_type;
		typedef typename row_type::lvalue_reference lvalue_reference;
		typedef typename row_type::const_reference const_reference;

		typedef typename boost::add_lvalue_reference< row_type >::type lvalue_row_reference;
		typedef typename boost::add_reference<
				typename boost::add_const< row_type >::type >::type const_row_reference;

		matrix_builder() :
			detail::data_holder< RowIndexes, row_type >() ...
		{
		}

		template < typename ... E >
		matrix_builder( vector<E, CC> const& ... args ) :
			detail::data_holder< RowIndexes, row_type >(args) ...
		{
		}

		matrix_builder( std::initializer_list< std::initializer_list< value_type > > const& args) :
			detail::data_holder< RowIndexes, row_type >( row_type(*(args.begin() + RowIndexes)) ) ...
		{
		}

		template < size_t N >
		lvalue_row_reference
		at()
		{
			return this->detail::data_holder< N, row_type >::value;
		}

		template < size_t N >
		const_row_reference
		at() const
		{
			return this->detail::data_holder< N, row_type >::value;
		}

		iterator
		begin()
		{
			return &this->detail::data_holder< 0, row_type >::value;
		}

		const_iterator
		begin() const
		{
			return &this->detail::data_holder< 0, row_type >::value;
		}

		iterator
		end()
		{
			return begin() + row_index_tuple::size;
		}

		const_iterator
		end() const
		{
			return begin() + row_index_tuple::size;
		}

		lvalue_row_reference
		operator[] (size_t idx)
		{
			assert(idx < row_index_tuple::size);
			return begin()[idx];
		}

		const_row_reference
		operator[] (size_t idx) const
		{
			assert(idx < row_index_tuple::size);
			return begin()[idx];
		}
	};

} // namespace detail

// Forward decl
template < typename T, size_t RC, size_t CC >
struct matrix;

namespace detail {

/**
 * Matrix addition metafunction
 */
template < size_t N, typename T >
struct matrix_addition;

template < size_t N, typename T, size_t RC, size_t CC >
struct matrix_addition< N, matrix < T, RC, CC > > {
	template < typename U >
	void
	operator()(matrix<T, RC, CC>& lhs, matrix<U, RC, CC> const& rhs)
	{
		matrix_addition< N - 1, matrix< T, RC, CC > >()(lhs, rhs);
		lhs.template at<N>() += rhs.template at<N>();
	}
};

template < typename T, size_t RC, size_t CC >
struct matrix_addition< 0, matrix < T, RC, CC > > {
	template < typename U >
	void
	operator()(matrix<T, RC, CC>& lhs, matrix<U, RC, CC> const& rhs)
	{
		lhs.template at<0>() += rhs.template at<0>();
	}
};

/**
 * Matrix transposition row loop metafunction
 */
template < size_t R, size_t C, typename T >
struct matrix_row_transpose;

template < size_t R, size_t C, typename T, size_t RC, size_t CC >
struct matrix_row_transpose< R, C, matrix<T, RC, CC > > {
	typedef matrix< T, RC, CC > source_type;
	typedef typename source_type::row_type source_row;
	typedef typename source_type::transposed_type transposed_type;

	void
	operator()(transposed_type& t, source_row const& r)
	{
		matrix_row_transpose< R, C - 1, source_type >()(t, r);
		t.template at<C>().template at<R>() = r.template at<C>();
	}
};

template < size_t R, typename T, size_t RC, size_t CC >
struct matrix_row_transpose< R, 0, matrix<T, RC, CC > > {
	typedef matrix< T, RC, CC > source_type;
	typedef typename source_type::row_type source_row;
	typedef typename source_type::transposed_type transposed_type;

	void
	operator()(transposed_type& t, source_row const& r)
	{
		t.template at<0>().template at<R>() = r.template at<0>();
	}
};

/**
 * Matrix transposition metafunction
 */
template < size_t R, typename T >
struct matrix_transpose;

template < size_t R, typename T, size_t RC, size_t CC >
struct matrix_transpose < R, matrix< T, RC, CC > > {
	typedef matrix< T, RC, CC > source_type;
	typedef typename source_type::transposed_type transposed_type;

	void
	operator()(transposed_type& t, source_type const& s)
	{
		matrix_transpose< R - 1, source_type >()(t, s);
		matrix_row_transpose< R, CC - 1, source_type >()(t, s.template at<R>());
	}
};

template < typename T, size_t RC, size_t CC >
struct matrix_transpose < 0, matrix< T, RC, CC > > {
	typedef matrix< T, RC, CC > source_type;
	typedef typename source_type::transposed_type transposed_type;

	void
	operator()(transposed_type& t, source_type const& s)
	{
		matrix_row_transpose< 0, CC - 1, source_type >()(t, s.template at<0>());
	}
};

/**
 * Matrix by vector multiplication metafunction.
 * Vector is considered a column matrix
 * @tparam C Vector column index / matrix row index
 * @tparam M Matrix type
 * @tparam V Vector type
 */
template < size_t C, typename M, typename V >
struct matrix_vector_multiply;

template < size_t C, typename T, typename U, size_t RC, size_t CC >
struct matrix_vector_multiply< C, matrix< T, RC, CC >, vector< U, CC > > {
	typedef matrix< T, RC, CC > matrix_type;
	typedef vector< U, CC > vector_type;
	typedef vector< T, CC > result_type;

	void
	operator()(result_type& res, matrix_type const& m, vector_type const& v)
	{
		matrix_vector_multiply< C - 1, matrix_type, vector_type >()(res, m, v);
		res.template at<C>() = v * m.template at<C>();
	}
};

template < typename T, typename U, size_t RC, size_t CC >
struct matrix_vector_multiply< 0, matrix< T, RC, CC >, vector< U, CC > > {
	typedef matrix< T, RC, CC > matrix_type;
	typedef vector< U, CC > vector_type;
	typedef vector< T, CC > result_type;

	void
	operator()(result_type& res, matrix_type const& m, vector_type const& v)
	{
		res.template at<0>() = v * m.template at<0>();
	}
};

/**
 * Matrix multiplication row loop metafunction
 * TODO Replace with matrix-vector multiply
 */
template < size_t R, size_t C, typename M1, typename M2 >
struct matrix_cell_multiply;

template < size_t R, size_t C, typename T, typename U, size_t R1, size_t C1, size_t C2 >
struct matrix_cell_multiply< R, C, matrix< T, R1, C1 >, matrix< U, C1, C2 > > {
	typedef matrix< T, R1, C1 > left_side_type;
	typedef typename left_side_type::row_type left_side_row;

	typedef matrix< U, C1, C2 > right_side_type;
	typedef matrix< T, R1, C2 > result_type;

	typedef typename right_side_type::transposed_type right_side_transposed;

	void
	operator()(result_type& res, left_side_row const& lhs, right_side_transposed const& rhs)
	{
		matrix_cell_multiply< R, C - 1, left_side_type, right_side_type >()(res, lhs, rhs);
		res.template at<R>().template at<C>() = lhs * rhs.template at<C>();
	}
};

template < size_t R, typename T, typename U, size_t R1, size_t C1, size_t C2 >
struct matrix_cell_multiply< R, 0, matrix< T, R1, C1 >, matrix< U, C1, C2 > > {
	typedef matrix< T, R1, C1 > left_side_type;
	typedef typename left_side_type::row_type left_side_row;

	typedef matrix< U, C1, C2 > right_side_type;
	typedef matrix< T, R1, C2 > result_type;

	typedef typename right_side_type::transposed_type right_side_transposed;

	void
	operator()(result_type& res, left_side_row const& lhs, right_side_transposed const& rhs)
	{
		res.template at<R>().template at<0>() = lhs * rhs.template at<0>();
	}
};

/**
 * Matrix multiplication metafunction
 */
template < size_t R, typename M1, typename M2 >
struct matrix_row_multiply;

template < size_t R, typename T, typename U, size_t R1, size_t C1, size_t C2 >
struct matrix_row_multiply< R, matrix< T, R1, C1 >, matrix< U, C1, C2 > > {
	typedef matrix< T, R1, C1 > left_side_type;
	typedef matrix< U, C1, C2 > right_side_type;
	typedef matrix< T, R1, C2 > result_type;

	typedef typename right_side_type::transposed_type right_side_transposed;

	void
	operator()(result_type& res, left_side_type const& lhs, right_side_transposed const& rhs)
	{
		matrix_row_multiply< R - 1, left_side_type, right_side_type >()(res, lhs, rhs);
		// TODO Replace with matrix-vector multiply
		matrix_cell_multiply< R, C2 - 1, left_side_type, right_side_type >()(res, lhs.template at<R>(), rhs);
	}
};

template < typename T, typename U, size_t R1, size_t C1, size_t C2 >
struct matrix_row_multiply< 0, matrix< T, R1, C1 >, matrix< U, C1, C2 > > {
	typedef matrix< T, R1, C1 > left_side_type;
	typedef matrix< U, C1, C2 > right_side_type;
	typedef matrix< T, R1, C2 > result_type;

	typedef typename right_side_type::transposed_type right_side_transposed;

	void
	operator()(result_type& res, left_side_type const& lhs, right_side_transposed const& rhs)
	{
		// TODO Replace with matrix-vector multiply
		matrix_cell_multiply< 0, C2 - 1, left_side_type, right_side_type >()(res, lhs.template at<0>(), rhs);
	}

};

/**
 * Cofactor sign metafunction
 */
template < size_t R, size_t C >
struct cofactor_sign {
	static const int value = ((R + C) % 2) ? -1 : 1;
};

/**
 * Matrix minor calculation metafunction
 */
template < typename T >
struct matrix_minor;



/**
 * Matrix cofactor metafunction
 */
template < size_t R, size_t C, typename T, size_t Size >
struct matrix_cofactor;

template < size_t R, size_t C, typename T, size_t Size >
struct matrix_cofactor < R, C, matrix< T, Size, Size >, Size > {

};

template < typename T >
struct matrix_cofactor < 0, 0, matrix< T, 1, 1 >, 1 > {
	typedef matrix<T, 1, 1> matrix_type;
	T
	operator()( matrix_type const& )
	{
		return 0;
	}
};

template < size_t N, typename T >
	struct identity_matrix;

/**
 * Top specialization
 */
template < size_t N, typename T>
	struct identity_matrix< N, matrix< T, N + 1, N + 1> > {
		typedef matrix<T, N + 1, N + 1> matrix_type;

		static void
		fill_matrix(matrix_type& m)
		{
			m.template at<N>().zero();
			m.template at<N>().template at<N>() = 1;
			identity_matrix< N -1, matrix_type >::fill_matrix(m);
		}

		static matrix_type
		build_matrix()
		{
			matrix_type m;
			fill_matrix(m);
			return m;
		}
	};

template < size_t N, typename T, size_t Size >
	struct identity_matrix< N, matrix< T, Size, Size > > {
		typedef matrix<T, Size, Size> matrix_type;

		static void
		fill_matrix(matrix_type& m)
		{
			m.template at<N>().zero();
			m.template at<N>().template at<N>() = 1;
			identity_matrix< N -1, matrix_type >::fill_matrix(m);
		}
	};

template < typename T, size_t Size >
	struct identity_matrix< 0, matrix< T, Size, Size > > {
		typedef matrix<T, Size, Size> matrix_type;

		static void
		fill_matrix(matrix_type& m)
		{
			m.template at<0>().zero();
			m.template at<0>().template at<0>() = 1;
		}
	};

} // namespace detail

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
			detail::matrix_addition< RC - 1, matrix< T, RC, CC > >()(*this, rhs);
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
		identity()
		{
			// TODO add concept checking that the matrix is square
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

#endif /* OPENGAMES_MATRIX_HPP_ */
