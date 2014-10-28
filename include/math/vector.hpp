/*
 * vector.hpp
 *
 *  Created on: 19.01.2012
 *      Author: zmij
 */

#ifndef OPENGAMES_MATH_VECTOR_HPP_
#define OPENGAMES_MATH_VECTOR_HPP_

#include <array>
#include <algorithm>
#include <stdexcept>
#include <assert.h>

#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_lvalue_reference.hpp>

#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/utility/enable_if.hpp>

namespace math {

namespace detail {

template< size_t Index, typename T >
	struct data_holder {
		enum {
			index = Index
		};
		typedef T type;

		type value;

		data_holder() :
				value()
		{
		}

		data_holder(type const& value) :
				value(value)
		{
		}

		template< typename Y >
			data_holder(Y const& value) :
					value(value)
			{
			}

		void
		swap(data_holder<Index, T>& rhs)
		{
			using std::swap;
			swap(value, rhs.value);
		}

		template< size_t idx1, typename Y >
			data_holder&
			operator =(const data_holder< idx1, Y >& rhs)
			{
				value = rhs.value;
				return *this;
			}
	};

template< size_t ... Indexes >
	struct indexes_tuple {
		enum {
			size = sizeof ... (Indexes)
		};
	};

template< size_t num, typename tp = indexes_tuple< > >
	struct index_builder;

template< size_t num, size_t ... Indexes >
	struct index_builder< num, indexes_tuple< Indexes ... > > : index_builder<
			num - 1, indexes_tuple< Indexes ..., sizeof ... (Indexes) > > {
	};

template< size_t ... Indexes >
	struct index_builder< 0, indexes_tuple< Indexes ... > > {
		typedef indexes_tuple< Indexes ... > type;
		enum {
			size = sizeof ... (Indexes)
		};
	};

template < typename IndexTuple, typename T >
	struct vector_builder;

template < size_t ... Indexes, typename T >
	struct vector_builder< indexes_tuple< Indexes ... >, T > :
		data_holder< Indexes, T > ... {

		typedef indexes_tuple< Indexes ... > indexes_tuple_type;
		typedef vector_builder< indexes_tuple_type, T > this_type;

		typedef T element_type;
		typedef typename boost::remove_reference<
				typename boost::remove_cv< element_type >::type >::type value_type;
		typedef typename boost::add_lvalue_reference< value_type >::type lvalue_reference;
		typedef typename boost::add_reference<
				typename boost::add_const< value_type >::type >::type const_reference;

		typedef value_type* pointer;
		typedef value_type const* const_pointer;

		typedef value_type* iterator;
		typedef value_type const* const_iterator;

		vector_builder() :
			data_holder< Indexes, T >() ...
		{
		}

//		vector_builder(vector_builder const& rhs) :
//			data_holder< Indexes, T >( rhs.template at< Indexes >() ) ...
//		{
//		}
		/**
		 * Construct vector from a vector of larger size
		 * @param rhs
		 * @param
		 */
		template < size_t ... IndexesR, typename U >
		vector_builder(
				vector_builder< indexes_tuple< IndexesR ... >, U > const& rhs,
				typename boost::enable_if_c< sizeof ... (Indexes) <= sizeof ... (IndexesR) >::type* = 0 ) :
			data_holder< Indexes, T >( rhs.template at< Indexes >() ) ...
		{
		}

		template < typename ... E >
		vector_builder(E const& ... args,
				typename boost::enable_if_c< sizeof ... (Indexes) == sizeof ... (E) >::type* = 0) :
			data_holder< Indexes, T >(args) ...
		{
		}

		template < typename ... E >
		vector_builder(E&& ... args,
				typename boost::enable_if_c< sizeof ... (Indexes) == sizeof ... (E) >::type* = 0) :
			data_holder< Indexes, T >( std::forward<E>(args) ) ...
		{
		}

		vector_builder(std::initializer_list<value_type> const& args) :
			data_holder< Indexes, T >(*(args.begin() + Indexes))...
		{
		}

		vector_builder(const_pointer p) :
			data_holder< Indexes, T >(*(p + Indexes))...
		{
		}

		pointer
		data()
		{
			return &this->data_holder< 0, T >::value;
		}

		const_pointer
		data() const
		{
			return &this->data_holder< 0, T >::value;
		}

		template < size_t N >
		lvalue_reference
		at()
		{
			return this->data_holder< N, T >::value;
		}

		template < size_t N >
		const_reference
		at() const
		{
			return this->data_holder< N, T >::value;
		}

		iterator
		begin()
		{
			return &this->data_holder< 0, T >::value;
		}

		const_iterator
		begin() const
		{
			return &this->data_holder< 0, T >::value;
		}

		iterator
		end()
		{
			return begin() + indexes_tuple_type::size;
		}

		const_iterator
		end() const
		{
			return begin() + indexes_tuple_type::size;
		}

		lvalue_reference
		operator[](size_t idx)
		{
			assert(idx < indexes_tuple_type::size);
			return begin()[idx];
		}

		const_reference
		operator[](size_t idx) const
		{
			assert(idx < indexes_tuple_type::size);
			return begin()[idx];
		}
	};
} // namespace detail

template < typename T, size_t Size >
	struct vector;

namespace detail {

template < size_t N, typename T >
struct dot_product;

template < size_t N, typename T, size_t Size >
struct dot_product< N, vector<T, Size> > {

	typename vector<T, Size>::value_type
	operator()( vector<T, Size> const& lhs, vector<T, Size> const& rhs )
	{
		return dot_product< N -1, vector<T, Size> >()(lhs, rhs) +
				lhs.template at<N>() * rhs.template at<N>();
	}
};

template < typename T, size_t Size >
struct dot_product< 0, vector<T, Size> > {
	typename vector<T, Size>::value_type
	operator()(vector<T, Size> const& lhs, vector<T, Size> const& rhs)
	{
		return lhs.template at< 0 >() * rhs.template at< 0 >();
	}
};

template < size_t N, typename T >
struct scalar_multiplication;

template < size_t N, typename T, size_t Size >
struct scalar_multiplication< N, vector< T, Size > > {
	template < typename U >
	void
	operator()( vector<T, Size>& v, U s )
	{
		scalar_multiplication< N - 1, vector<T, Size> >()(v, s);
		v.template at<N>() *= s;
	}
};

template < typename T, size_t Size >
struct scalar_multiplication< 0, vector< T, Size > > {
	template < typename U >
	void
	operator()( vector<T, Size>& v, U s )
	{
		v.template at<0>() *= s;
	}
};

template < size_t N, typename T >
struct scalar_division;

template < size_t N, typename T, size_t Size >
struct scalar_division< N, vector< T, Size > > {
	template < typename U >
	void
	operator()( vector<T, Size>& v, U s )
	{
		scalar_division< N - 1, vector<T, Size> >()(v, s);
		v.template at<N>() /= s;
	}
};

template < typename T, size_t Size >
struct scalar_division< 0, vector< T, Size > > {
	template < typename U >
	void
	operator()( vector<T, Size>& v, U s )
	{
		v.template at<0>() /= s;
	}
};

template < size_t N, typename T >
struct vector_addition;

template < size_t N, typename T, size_t Size >
struct vector_addition< N, vector< T, Size > > {
	template < typename U >
	void
	operator()( vector<T, Size>& lhs, vector<U, Size> const& rhs)
	{
		vector_addition<N - 1, vector<T, Size> >()(lhs, rhs);
		lhs.template at<N>() += rhs.template at<N>();
	}
};

template < typename T, size_t Size >
struct vector_addition< 0, vector< T, Size > > {
	template < typename U >
	void
	operator()( vector<T, Size>& lhs, vector<U, Size> const& rhs)
	{
		lhs.template at<0>() += rhs.template at<0>();
	}
};

template < size_t N, typename T >
struct vector_substraction;

template < size_t N, typename T, size_t Size >
struct vector_substraction< N, vector< T, Size > > {
	template < typename U >
	void
	operator()( vector<T, Size>& lhs, vector<U, Size> const& rhs)
	{
		vector_substraction<N - 1, vector<T, Size> >()(lhs, rhs);
		lhs.template at<N>() -= rhs.template at<N>();
	}
};

template < typename T, size_t Size >
struct vector_substraction< 0, vector< T, Size > > {
	template < typename U >
	void
	operator()( vector<T, Size>& lhs, vector<U, Size> const& rhs)
	{
		lhs.template at<0>() -= rhs.template at<0>();
	}
};

template < size_t N, typename T >
struct set_all_elements;

template < size_t N, typename T, size_t Size >
struct set_all_elements< N, vector< T, Size > > {
	template < typename U >
	void
	operator()( vector<T, Size>& v, U s )
	{
		set_all_elements< N - 1, vector< T, Size > >()(v, s);
		v.template at<N>() = s;
	}
};

template < typename T, size_t Size >
struct set_all_elements< 0, vector< T, Size > > {
	template < typename U >
	void
	operator()( vector<T, Size>& v, U s )
	{
		v.template at<0>() = s;
	}
};

} // namespace detail

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
			detail::scalar_multiplication< Size - 1, this_type >()(*this, val);
			return *this;
		}

		template < typename U >
		this_type&
		operator /= (U val)
		{
			detail::scalar_division< Size - 1, this_type >()(*this, val);
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

#endif /* OPENGAMES_VECTOR_HPP_ */
