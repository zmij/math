/*
 * vector_detail.hpp
 *
 *  Created on: Oct 29, 2014
 *      Author: zmij
 */

#ifndef PUSHKIN_VECTOR_DETAIL_HPP_
#define PUSHKIN_VECTOR_DETAIL_HPP_

#include <pushkin/math/detail/axis_names.hpp>
#include <pushkin/math/detail/value_traits.hpp>
#include <pushkin/math/detail/vector_expressions.hpp>

#include <type_traits>
#include <utility>
#include <array>

namespace psst {
namespace math {

namespace detail {

template < ::std::size_t N, typename T >
struct dot_product;

template < ::std::size_t N, typename T, ::std::size_t Size, typename Axes >
struct dot_product< N, vector<T, Size, Axes> > {
    using vector_type   = vector<T, Size, Axes>;
    using value_type    = typename vector_type::magnitude_type;

    value_type
    operator()( vector_type const& lhs, vector_type const& rhs )
    {
        return dot_product< N -1, vector<T, Size, Axes> >()(lhs, rhs) +
                lhs.template at<N>() * rhs.template at<N>();
    }
};

template < typename T, ::std::size_t Size, typename Axes >
struct dot_product< 0, vector<T, Size, Axes> > {
    using vector_type   = vector<T, Size, Axes>;
    using value_type    = typename vector_type::magnitude_type;

    value_type
    operator()(vector_type const& lhs, vector_type const& rhs)
    {
        return lhs.template at< 0 >() * rhs.template at< 0 >();
    }
};


template < ::std::size_t N, typename T >
struct set_all_elements;

template < ::std::size_t N, typename T, ::std::size_t Size, typename Axes >
struct set_all_elements< N, vector< T, Size, Axes > > {
    template < typename U >
    void
    operator()( vector<T, Size, Axes>& v, U s )
    {
        set_all_elements< N - 1, vector< T, Size, Axes > >{}(v, s);
        v.template at<N>() = s;
    }
};

template < typename T, ::std::size_t Size, typename Axes >
struct set_all_elements< 0, vector< T, Size, Axes > > {
    template < typename U >
    void
    operator()( vector<T, Size, Axes>& v, U s )
    {
        v.template at<0>() = s;
    }
};

} // namespace detail

}  // namespace math
}  /* namespace psst */

#endif /* PUSHKIN_VECTOR_DETAIL_HPP_ */
