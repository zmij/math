/*
 * vector_io.hpp
 *
 *  Created on: 13.02.2012
 *      Author: zmij
 */

#ifndef OPENGAMES_MATH_VECTOR_IO_HPP_
#define OPENGAMES_MATH_VECTOR_IO_HPP_

#include "math/vector.hpp"
#include <iostream>

namespace math {

namespace detail {

template< size_t, typename T >
	struct vector_output;

template< size_t N, typename T, size_t Size >
	struct vector_output< N, vector< T, Size > > {
		typedef vector< T, Size > vector_type;
		static void
		output(std::ostream& out, vector_type const& v)
		{
			vector_output< N - 1, vector_type >::output(out, v);
			out << "," << v.template at< N >();
		}
	};

template< typename T, size_t Size >
	struct vector_output< 0, vector< T, Size > > {
		typedef vector< T, Size > vector_type;
		static void
		output(std::ostream& out, vector_type const& v)
		{
			out << v.template at< 0 >();
		}
	};

} // namespace detail

template < typename T, size_t Size >
std::ostream&
operator << (std::ostream& out, vector<T, Size> const& v)
{
	std::ostream::sentry s(out);
	if (s) {
		out << "{";
		detail::vector_output< Size - 1, vector<T, Size> >::output(out, v);
		out << "}";
	}
	return out;
}

} // namespace math

#endif /* OPENGAMES_MATH_VECTOR_IO_HPP_ */
