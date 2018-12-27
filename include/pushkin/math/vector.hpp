/*
 * vector.hpp
 *
 *  Created on: 19.01.2012
 *      Author: zmij
 */

#ifndef PUSHKIN_MATH_VECTOR_HPP_
#define PUSHKIN_MATH_VECTOR_HPP_

#include <array>
#include <algorithm>
#include <stdexcept>
#include <assert.h>
#include <cmath>

#include <pushkin/math/detail/vector_expressions.hpp>
#include <pushkin/math/detail/axis_names.hpp>
#include <pushkin/math/detail/conversion.hpp>
#include <pushkin/math/detail/calculus.hpp>

namespace psst {
namespace math {

template < typename T, ::std::size_t Size, typename Axes >
struct vector
    : expr::vector_expression< vector<T, Size, Axes> >,
      detail::calculus_selector<T, Size, Axes>,
      detail::axes_names_t<Size, Axes, vector<T, Size, Axes>, T> {

    using this_type             = vector< T, Size, Axes >;
    using traits                = vector_traits<this_type>;
    using base_expression_type  = expr::vector_expression< vector<T, Size, Axes> >;

    using value_type            = typename traits::value_type;
    using lvalue_reference      = typename traits::lvalue_reference;
    using const_reference       = typename traits::const_reference;
    using magnitude_type        = typename traits::magnitude_type;

    using pointer               = typename traits::pointer;
    using const_pointer         = typename traits::const_pointer;
    using index_sequence_type   = typename traits::index_sequence_type;

    using iterator              = typename traits::iterator;
    using const_iterator        = typename traits::const_iterator;
    using init_list             = ::std::initializer_list< value_type >;

    static constexpr auto size  = traits::size;

    constexpr vector() : vector{T{0}, index_sequence_type{}} {}

    /**
     * Single value construction, initialize all components to the
     * same value. To precede initializer list constructor, should
     * be called with round parenthesis.
     * @param val
     */
    constexpr explicit
    vector(value_type val) : vector(val, index_sequence_type{}) {}

    constexpr vector(init_list const& args)
        : vector(args.begin(), index_sequence_type{}) {}

    constexpr vector(const_pointer p) : vector(p, index_sequence_type{}) {}

    template < typename U, ::std::size_t SizeR, typename AxesR >
    constexpr explicit vector( vector<U, SizeR, AxesR> const& rhs )
        : vector(rhs, utils::make_min_index_sequence<Size, SizeR>{}) {}
    template < typename Expression, typename Result >
    constexpr vector(expr::vector_expression<Expression, Result> const& rhs)
        : vector(rhs,
            utils::make_min_index_sequence<Size, expr::vector_expression<Expression, Result>::size>{}){}

    pointer
    data() { return data_.data(); }

    constexpr const_pointer
    data() const { return data_.data(); }

    template < ::std::size_t N >
    lvalue_reference
    at()
    {
        static_assert(N < size, "Invalid value index in vector");
        return ::std::get<N>(data_);
    }

    template < ::std::size_t N >
    constexpr const_reference
    at() const
    {
        static_assert(N < size, "Invalid value index in vector");
        return ::std::get<N>(data_);
    }

    iterator
    begin() { return data_.begin(); }

    constexpr const_iterator
    begin() const { return cbegin(); }
    constexpr const_iterator
    cbegin() const { return data_.cbegin(); }

    iterator
    end() { return data_.end(); }

    constexpr const_iterator
    end() const { return cend(); }
    constexpr const_iterator
    cend() const { return data_.end(); }

    lvalue_reference
    operator[](::std::size_t idx)
    {
        assert(idx < size);
        return data_[idx];
    }

    constexpr const_reference
    operator[](::std::size_t idx) const
    {
        assert(idx < size);
        return data_[idx];
    }
    // TODO Make converter a CRTP base
    template < typename TAxes >
    vector<value_type, Size, TAxes>
    convert() const
    {
        return math::convert<vector<value_type, Size, TAxes>>(*this);
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
private:
    template <::std::size_t... Indexes>
    constexpr vector(value_type val, ::std::index_sequence<Indexes...>)
      : data_({ utils::value_fill<Indexes, T>{val}.value ... }){}
    template <::std::size_t... Indexes>
    constexpr vector(const_pointer p, ::std::index_sequence<Indexes...>)
      : data_({ *(p + Indexes)... }) {}
    template < typename U, ::std::size_t SizeR, typename AxesR, ::std::size_t... Indexes >
    constexpr vector(vector<U, SizeR, AxesR> const& rhs, ::std::index_sequence<Indexes...>)
      : data_({rhs.template at<Indexes>()...}) {}
    template <typename Expr, typename Result, ::std::size_t... Indexes>
    constexpr vector(expr::vector_expression<Expr, Result> const& rhs, ::std::index_sequence<Indexes...>)
      : data_({ (value_type)rhs.template at<Indexes>()... }) {}
private:
    using data_type = ::std::array<T, size>;
    data_type data_;
};

template <std::size_t N, typename T, std::size_t Size, typename Axes>
constexpr auto&
get(vector<T, Size, Axes>& v)
{
    return v.template at<N>();
}

/**
 * Dot product operator
 * @param lhs
 * @param rhs
 * @return
 */
template < typename T, size_t Size, typename Axes >
typename vector< T, Size, Axes >::magnitude_type
operator | (vector< T, Size, Axes > const& lhs,  vector< T, Size, Axes > const& rhs)
{
    return dot_product(lhs, rhs);
}

/**
 * Cross product for 3D vectors
 * @param a
 * @param b
 * @return
 */
template < typename T, typename U, typename Axes >
vector< typename vector<T, 3, Axes>::value_type, 3, Axes >
cross(vector<T, 3, Axes> const& lhs, vector<U, 3, Axes> const& rhs)
{
    vector< T, 3, Axes > res {
        lhs.template at<1>() * rhs.template at<2>() - lhs.template at<2>() * rhs.template at<1>(),
        lhs.template at<2>() * rhs.template at<0>() - lhs.template at<0>() * rhs.template at<2>(),
        lhs.template at<0>() * rhs.template at<1>() - lhs.template at<1>() * rhs.template at<0>()
    };
    return res;
}

template < typename T, typename U, typename Axes >
vector< typename vector<T, 3, Axes>::value_type, 3, Axes >
operator ^ (vector<T, 3, Axes> const& lhs, vector<U, 3, Axes> const& rhs)
{
    return cross(lhs, rhs);
}

/**
 * Cross product for homogenous 3D vectors
 * @param a
 * @param b
 * @return
 */
template < typename T, typename U >
vector< typename vector<T, 4, axes::xyzw>::value_type, 4, axes::xyzw >
cross(vector<T, 4, axes::xyzw> const& lhs, vector<U, 4, axes::xyzw> const& rhs)
{
    vector< T, 4, axes::xyzw > res {
        lhs.template at<1>() * rhs.template at<2>() - lhs.template at<2>() * rhs.template at<1>(),
        lhs.template at<2>() * rhs.template at<0>() - lhs.template at<0>() * rhs.template at<2>(),
        lhs.template at<0>() * rhs.template at<1>() - lhs.template at<1>() * rhs.template at<0>(),
        typename vector<T, 4, axes::xyzw>::value_type(1)
    };
    return res;
}
template < typename T, typename U >
vector< typename vector<T, 4, axes::xyzw>::value_type, 4, axes::xyzw >
operator ^ (vector<T, 4, axes::xyzw> const& lhs, vector<U, 4, axes::xyzw> const& rhs)
{
    return cross(lhs, rhs);
}

/**
 * Projection of vector v onto vector n
 * @param n Target vector
 * @param v Source vector
 * @return Vector that is parallel to n
 */
template < typename T, size_t Size, typename Axes >
vector< typename vector<T, Size, Axes>::value_type, Size, Axes >
projection( vector<T, Size, Axes> const& n, vector<T, Size, Axes> const& v )
{
    return n * ( v * n / n.magnitude_square() );
}

/**
 * Vector that is perpendicular to n, such as vǁ + vⱶ = v
 * @param n
 * @param v
 * @return
 */
template < typename T, size_t Size, typename Axes >
vector< typename vector<T, Size, Axes>::value_type, Size, Axes >
perpendicular( vector<T, Size, Axes> const& n, vector<T, Size, Axes> const& v )
{
    return v - projection(n, v);
}

/**
 * Project vector v onto vector n
 * @param n
 * @param v
 * @return Pair of vectors vǁ, vⱶ. vǁ is parallel to n, vǁ + vⱶ = v
 */
template < typename T, size_t Size, typename Axes >
::std::pair<
    vector< typename vector<T, Size, Axes>::value_type, Size, Axes >,
    vector< typename vector<T, Size, Axes>::value_type, Size, Axes > >
project( vector<T, Size, Axes> const& n, vector<T, Size, Axes> const& v )
{
    auto p = projection(n, v);
    return ::std::make_pair(p, v - p);
}


} // namespace math
}  /* namespace psst */

#ifdef __METASHELL
// Some definitions for debugging templates, not to input them
// every time the metashell starts
using namespace psst::math;

using vec_3f = vector<float, 3>;
using vec_3d = vector<double, 3>;

using vec_4f = vector<float, 4>;
using vec_4d = vector<double, 4>;

vec_3f v3f_1, v3f_2;
vec_4f v4f_1, v4f_2;

vec_3d v3d_1, v3d_2;
vec_4d v4d_1, v4d_2;

#endif

#endif /* PUSHKIN_MATH_VECTOR_HPP_ */
