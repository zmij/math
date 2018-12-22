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

#include <type_traits>
#include <utility>
#include <array>

namespace psst {
namespace math {

namespace detail {

template <::std::size_t Index, typename T>
struct value_fill {
  T value;
};

template < ::std::size_t N, typename T, typename U >
struct vector_cmp;

template < ::std::size_t N, typename T, typename U,
    ::std::size_t TSize, ::std::size_t USize,
    typename Axes >
struct vector_cmp<N, vector<T, TSize, Axes>, vector<U, USize, Axes>> {
    using left_side     = vector<T, TSize, Axes>;
    using traits_type   = typename left_side::value_traits;
    using right_side    = vector<U, USize, Axes>;
    using prev_elem     = vector_cmp<N - 1, left_side, right_side>;

    constexpr static bool
    eq(left_side const& lhs, right_side const& rhs)
    {
        return prev_elem::eq(lhs, rhs) &&
                traits_type::eq(lhs.template at<N>(),
                                rhs.template at<N>());
    }

    constexpr static bool
    less(left_side const& lhs, right_side const& rhs)
    {
        auto p = prev_elem::cmp(lhs, rhs);
        return p < 0 ? true : traits_type::less(
                                lhs.template at<N>(),
                                rhs.template at<N>());
    }
    constexpr static int
    cmp(left_side const& lhs, right_side const& rhs)
    {
        auto p = prev_elem::cmp(lhs, rhs);
        if (p == 0) {
            return traits_type::cmp(lhs.template at<N>(),
                    rhs.template at<N>());
        }
        return p;
    }
};

template < typename T, typename U,
        ::std::size_t TSize, ::std::size_t USize,
        typename Axes >
struct vector_cmp<0, vector<T, TSize, Axes>, vector<U, USize, Axes>> {
    using left_side     = vector<T, TSize, Axes>;
    using right_side    = vector<U, USize, Axes>;
    using traits_type   = typename left_side::value_traits;

    constexpr static bool
    eq(left_side const& lhs, right_side const& rhs)
    {
        return traits_type::eq(lhs.template at<0>(), rhs.template at<0>());
    }

    constexpr static bool
    less(left_side const& lhs, right_side const& rhs)
    {
        return traits_type::less(lhs.template at<0>(), rhs.template at<0>());
    }

    constexpr static int
    cmp(left_side const& lhs, right_side const& rhs)
    {
        return traits_type::cmp(lhs.template at<0>(), rhs.template at<0>());
    }
};

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
struct vector_scalar_multiplication;

template < ::std::size_t N, typename T, ::std::size_t Size, typename Axes >
struct vector_scalar_multiplication< N, vector< T, Size, Axes > > {
    using vector_type   = vector<T, Size, Axes>;

    template < typename U >
    void
    operator()( vector_type& v, U s )
    {
        vector_scalar_multiplication< N - 1, vector<T, Size, Axes> >{}(v, s);
        v.template at<N>() *= s;
    }
};

template < typename T, ::std::size_t Size, typename Axes >
struct vector_scalar_multiplication< 0, vector< T, Size, Axes > > {
    using vector_type   = vector<T, Size, Axes>;

    template < typename U >
    void
    operator()( vector_type& v, U s )
    {
        v.template at<0>() *= s;
    }
};

template < ::std::size_t N, typename T >
struct vector_scalar_division;

template < ::std::size_t N, typename T, ::std::size_t Size, typename Axes >
struct vector_scalar_division< N, vector< T, Size, Axes > > {
    using vector_type   = vector<T, Size, Axes>;

    template < typename U >
    void
    operator()( vector_type& v, U s )
    {
        vector_scalar_division< N - 1, vector<T, Size, Axes> >{}(v, s);
        v.template at<N>() /= s;
    }
};

template < typename T, ::std::size_t Size, typename Axes >
struct vector_scalar_division< 0, vector< T, Size, Axes > > {
    using vector_type   = vector<T, Size, Axes>;

    template < typename U >
    void
    operator()( vector_type& v, U s )
    {
        v.template at<0>() /= s;
    }
};

template < ::std::size_t N, typename T >
struct vector_addition;

template < ::std::size_t N, typename T, ::std::size_t Size, typename Axes >
struct vector_addition< N, vector< T, Size, Axes > > {
    template < typename U >
    void
    operator()( vector<T, Size, Axes>& lhs, vector<U, Size, Axes> const& rhs)
    {
        vector_addition<N - 1, vector<T, Size, Axes> >{}(lhs, rhs);
        lhs.template at<N>() += rhs.template at<N>();
    }
};

template < typename T, ::std::size_t Size, typename Axes >
struct vector_addition< 0, vector< T, Size, Axes > > {
    template < typename U >
    void
    operator()( vector<T, Size, Axes>& lhs, vector<U, Size, Axes> const& rhs)
    {
        lhs.template at<0>() += rhs.template at<0>();
    }
};

template < ::std::size_t N, typename T >
struct vector_substraction;

template < ::std::size_t N, typename T, ::std::size_t Size, typename Axes >
struct vector_substraction< N, vector< T, Size, Axes > > {
    template < typename U >
    void
    operator()( vector<T, Size, Axes>& lhs, vector<U, Size, Axes> const& rhs)
    {
        vector_substraction<N - 1, vector<T, Size, Axes> >{}(lhs, rhs);
        lhs.template at<N>() -= rhs.template at<N>();
    }
};

template < typename T, ::std::size_t Size, typename Axes >
struct vector_substraction< 0, vector< T, Size, Axes > > {
    template < typename U >
    void
    operator()( vector<T, Size, Axes>& lhs, vector<U, Size, Axes> const& rhs)
    {
        lhs.template at<0>() -= rhs.template at<0>();
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
