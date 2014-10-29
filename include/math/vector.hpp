/*
 * vector.hpp
 *
 *  Created on: 19.01.2012
 *      Author: zmij
 */

#ifndef ZMIJ_MATH_VECTOR_HPP_
#define ZMIJ_MATH_VECTOR_HPP_

#include <array>
#include <algorithm>
#include <stdexcept>
#include <assert.h>

#include "math/detail/vector_detail.hpp"

namespace math {

template < typename T, size_t Size >
	struct vector : detail::vector_builder<
		typename detail::index_builder< Size >::type, T > {

		typedef detail::vector_builder<
				typename detail::index_builder< Size >::type, T > super_type;
		typedef vector< T, Size > this_type;

		typedef typename super_type::value_type value_type;
		typedef typename super_type::lvalue_reference lvalue_reference;
		typedef typename super_type::const_reference const_reference;
		typedef typename super_type::pointer pointer;
		typedef typename super_type::const_pointer const_pointer;

		vector() :
			super_type()
		{
		}

		template < typename ... E >
		vector(E const& ... args) :
			super_type(args ... )
		{
		}

		vector(std::initializer_list< value_type > const& args) :
			super_type(args)
		{
		}

		vector(const_pointer p) :
			super_type(p)
		{
		}

		template < typename U, size_t SizeR >
		vector( vector<U, SizeR> const& rhs ) :
			super_type(rhs)
		{
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
		operator *= (U val)
		{
			detail::vector_scalar_multiplication< Size - 1, this_type >()(*this, val);
			return *this;
		}

		template < typename U >
		this_type&
		operator /= (U val)
		{
			detail::vector_scalar_division< Size - 1, this_type >()(*this, val);
			return *this;
		}

		template < typename U >
		this_type&
		operator += (vector<U, Size> const& rhs)
		{
			detail::vector_addition< Size - 1, this_type >()(*this, rhs);
			return *this;
		}

		template < typename U >
		this_type&
		operator -= (vector<U, Size> const& rhs)
		{
			detail::vector_substraction< Size - 1, this_type >()(*this, rhs);
			return *this;
		}

		value_type
		magnitude_square() const
		{
			return detail::dot_product< Size - 1, this_type >()(*this, *this);
		}

		value_type
		magnitude() const
		{
			return sqrt(magnitude_square());
		}

		bool
		is_zero() const
		{
			return magnitude_square() == 0;
		}

		bool
		is_unit() const
		{
			return magnitude_square() == 1;
		}

		this_type&
		zero()
		{
			detail::set_all_elements< Size - 1, this_type >()(*this, 0);
			return *this;
		}

		this_type&
		normalize()
		{
			value_type m = magnitude();
			if (m != 0) {
				if (m != 1)
					(*this) /= m;
			} else {
				throw std::runtime_error("Cannot normalize a zero vector");
			}
			return *this;
		}

		this_type
		normalize() const
		{
			this_type v(*this);
			v.normalize();
			return v;
		}
	};

template < typename T, typename U, size_t Size >
bool
operator == (vector< T, Size > const& lhs, vector < U, Size > const& rhs)
{
	return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template < typename T, typename U, size_t Size >
bool
operator != (vector< T, Size > const& lhs, vector < U, Size > const& rhs)
{
	return !(lhs == rhs);
}

template < typename T, typename U, size_t Size >
bool
operator < (vector< T, Size > const& lhs, vector < U, Size > const& rhs)
{
	return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template < typename T, typename U, size_t Size >
bool
operator > (vector< T, Size > const& lhs, vector < U, Size > const& rhs)
{
	return rhs < lhs;
}

template < typename T, typename U, size_t Size >
bool
operator <= (vector< T, Size > const& lhs, vector < U, Size > const& rhs)
{
	return !(rhs < lhs);
}

template < typename T, typename U, size_t Size >
bool
operator >= (vector< T, Size > const& lhs, vector < U, Size > const& rhs)
{
	return !(lhs < rhs);
}

template < typename T, size_t Size >
typename vector< T, Size>::value_type
operator * (vector< T, Size> const& lhs,  vector< T, Size> const& rhs)
{
	return detail::dot_product< Size - 1, vector<T, Size> >()(lhs, rhs);
}

template < typename T, size_t Size, typename U >
vector< T, Size>
operator * (vector< T, Size> const& v, U s)
{
	vector< T, Size> res(v);
	res *= s;
	return res;
}

template < typename T, size_t Size, typename U >
vector< T, Size>
operator * (U s, vector< T, Size> const& v)
{
	vector< T, Size> res(v);
	res *= s;
	return res;
}

template < typename T, size_t Size, typename U >
vector< T, Size>
operator / (vector< T, Size> const& v, U s)
{
	vector< T, Size> res(v);
	res /= s;
	return res;
}

template < typename T, typename U, size_t Size >
vector< T, Size >
operator + ( vector< T, Size > const& lhs, vector< U, Size > const& rhs )
{
	vector< T, Size > res(lhs);
	res += rhs;
	return res;
}

template < typename T, typename U, size_t Size >
vector< T, Size >
operator - ( vector< T, Size > const& lhs, vector< U, Size > const& rhs )
{
	vector< T, Size > res(lhs);
	res -= rhs;
	return res;
}

template < typename T, size_t Size >
typename vector<T, Size>::value_type
magnitude_square(vector<T, Size> const& v)
{
	return v.magnitude_square();
}

template < typename T, size_t Size >
typename vector<T, Size>::value_type
magnitude(vector<T, Size> const& v)
{
	return v.magnitude();
}

template< typename T, typename U, size_t Size >
typename vector<T, Size>::value_type
distance_square(vector<T, Size> const& a, vector<U, Size> const& b)
{
	return magnitude_square(a - b);
}

template< typename T, typename U, size_t Size >
typename vector<T, Size>::value_type
distance(vector<T, Size> const& a, vector<U, Size> const& b)
{
	return magnitude(a - b);
}

template < typename T, size_t Size >
vector< typename vector<T, Size>::value_type, Size >
normalize(vector<T, Size> const& v)
{
	return v.normalize();
}

/**
 * Cross product for 3D vectors
 * @param a
 * @param b
 * @return
 */
template < typename T, typename U >
vector< typename vector<T, 3>::value_type, 3 >
cross(vector<T, 3> const& a, vector<U, 3> const& b)
{
	vector< T, 3 > res {
		a.template at<1>() * b.template at<2>() - a.template at<2>() * b.template at<1>(),
		a.template at<2>() * b.template at<0>() - a.template at<0>() * b.template at<2>(),
		a.template at<0>() * b.template at<1>() - a.template at<1>() * b.template at<0>()
	};
	return res;
}

/**
 * Cross product for homogenous 3D vectors
 * @param a
 * @param b
 * @return
 */
template < typename T, typename U >
vector< typename vector<T, 4>::value_type, 4 >
cross(vector<T, 4> const& a, vector<U, 4> const& b)
{
	vector< T, 4 > res {
		a.template at<1>() * b.template at<2>() - a.template at<2>() * b.template at<1>(),
		a.template at<2>() * b.template at<0>() - a.template at<0>() * b.template at<2>(),
		a.template at<0>() * b.template at<1>() - a.template at<1>() * b.template at<0>(),
		typename vector<T, 4>::value_type(1)
	};
	return res;
}

/**
 * Projection of vector v onto vector n
 * @param n Target vector
 * @param v Source vector
 * @return Vector that is parallel to n
 */
template < typename T, size_t Size >
vector< typename vector<T, Size>::value_type, Size >
projection( vector<T, Size> const& n, vector<T, Size> const& v )
{
	return n * ( v * n / n.magnitude_square() );
}

/**
 * Vector that is perpendicular to n, such as vǁ + vⱶ = v
 * @param n
 * @param v
 * @return
 */
template < typename T, size_t Size >
vector< typename vector<T, Size>::value_type, Size >
perpendicular( vector<T, Size> const& n, vector<T, Size> const& v )
{
	return v - projection(n, v);
}

/**
 * Project vector v onto vector n
 * @param n
 * @param v
 * @return Pair of vectors vǁ, vⱶ. vǁ is parallel to n, vǁ + vⱶ = v
 */
template < typename T, size_t Size >
std::pair<
	vector< typename vector<T, Size>::value_type, Size >,
	vector< typename vector<T, Size>::value_type, Size > >
project( vector<T, Size> const& n, vector<T, Size> const& v )
{
	auto p = projection(n, v);
	return std::make_pair(p, v - p);
}

} // namespace math

#endif /* ZMIJ_MATH_VECTOR_HPP_ */
