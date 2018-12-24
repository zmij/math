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

#include <pushkin/math/detail/vector_detail.hpp>
#include <pushkin/math/detail/conversion.hpp>
#include <pushkin/math/detail/calculus.hpp>

namespace psst {
namespace math {

template < typename T, ::std::size_t Size, typename Axes >
struct vector
    : detail::calculus_selector<T, Size, Axes>,
      detail::axes_names_t<Size, Axes, vector<T, Size, Axes>, T> {

    using this_type             = vector< T, Size, Axes >;

    using element_type          = T;
    using value_traits          = detail::scalar_value_traits<T>;
    using value_type            = typename value_traits::value_type;
    using lvalue_reference      = typename value_traits::lvalue_reference;
    using const_reference       = typename value_traits::const_reference;
    using magnitude_type        = typename value_traits::magnitude_type;

    using pointer               = typename value_traits::pointer;
    using const_pointer         = typename value_traits::const_pointer;

    using iterator              = pointer;
    using const_iterator        = const_pointer;

    using index_sequence_type   = typename ::std::make_index_sequence<Size>;
    using init_list             = ::std::initializer_list< value_type >;

    static constexpr auto size  = Size;

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
        : vector(rhs,
            ::std::make_index_sequence< detail::min<Size, SizeR >::value >{}) {}

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
      : data_({ detail::value_fill<Indexes, T>{val}.value ... }){}
    template <::std::size_t... Indexes>
    constexpr vector(const_pointer p, ::std::index_sequence<Indexes...>)
      : data_({ *(p + Indexes)... }) {}
    template < typename U, ::std::size_t SizeR, typename AxesR, ::std::size_t... Indexes >
    constexpr vector(vector<U, SizeR, AxesR> const& rhs, ::std::index_sequence<Indexes...>)
      : data_({rhs.template at<Indexes>()...}) {}
private:
    using data_type = ::std::array<T, size>;
    data_type data_;
};

template < typename T, typename U,
    ::std::size_t TSize, ::std::size_t USize,
    typename Axes >
constexpr bool
operator == (vector< T, TSize, Axes > const& lhs, vector < U, USize, Axes > const& rhs)
{
    using left_side = vector<T, TSize, Axes>;
    using right_side = vector<U, USize, Axes>;
    using min_type = detail::min<TSize, USize>;
    return detail::vector_cmp<min_type::value - 1, left_side, right_side>::eq(lhs, rhs);
}

template < typename T, typename U,
    ::std::size_t TSize, ::std::size_t USize,
    typename Axes >
constexpr bool
operator != (vector< T, TSize, Axes > const& lhs, vector < U, USize, Axes > const& rhs)
{
    return !(lhs == rhs);
}

template < typename T, typename U,
    ::std::size_t TSize, ::std::size_t USize,
    typename Axes >
constexpr bool
operator < (vector< T, TSize, Axes > const& lhs, vector < U, USize, Axes > const& rhs)
{
    using left_side = vector<T, TSize, Axes>;
    using right_side = vector<U, USize, Axes>;
    using min_type = detail::min<TSize, USize>;
    return detail::vector_cmp<min_type::value - 1, left_side, right_side>::less(lhs, rhs);
}

template < typename T, typename U,
    ::std::size_t TSize, ::std::size_t USize,
    typename Axes >
constexpr bool
operator > (vector< T, TSize, Axes > const& lhs, vector < U, USize, Axes > const& rhs)
{
    return rhs < lhs;
}

template < typename T, typename U,
    ::std::size_t TSize, ::std::size_t USize,
    typename Axes >
constexpr bool
operator <= (vector< T, TSize, Axes > const& lhs, vector < U, USize, Axes > const& rhs)
{
    return !(rhs < lhs);
}

template < typename T, typename U,
    ::std::size_t TSize, ::std::size_t USize,
    typename Axes >
constexpr bool
operator >= (vector< T, TSize, Axes > const& lhs, vector < U, USize, Axes > const& rhs)
{
    return !(lhs < rhs);
}

template < typename T, size_t Size, typename Axes >
typename vector< T, Size, Axes >::magnitude_type
dot_product(vector< T, Size, Axes > const& lhs,  vector< T, Size, Axes > const& rhs)
{
    return detail::dot_product< Size - 1, vector<T, Size, Axes> >()(lhs, rhs);
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
    return detail::dot_product< Size - 1, vector<T, Size, Axes> >()(lhs, rhs);
}

template < typename T, size_t Size, typename Axes, typename U >
vector< T, Size, Axes >
operator * (vector< T, Size, Axes> const& v, U s)
{
    vector< T, Size, Axes> res(v);
    res *= s;
    return res;
}

template < typename T, size_t Size, typename Axes, typename U >
vector< T, Size, Axes >
operator * (U s, vector< T, Size, Axes > const& v)
{
    vector< T, Size, Axes > res(v);
    res *= s;
    return res;
}

template < typename T, size_t Size, typename Axes, typename U >
vector< T, Size, Axes >
operator / (vector< T, Size, Axes > const& v, U s)
{
    vector< T, Size, Axes > res(v);
    res /= s;
    return res;
}

template < typename T, typename U, size_t Size, typename Axes >
vector< T, Size, Axes >
operator + ( vector< T, Size, Axes > const& lhs, vector< U, Size, Axes > const& rhs )
{
    vector< T, Size, Axes > res(lhs);
    res += rhs;
    return res;
}

template < typename T, typename U, size_t Size, typename Axes >
vector< T, Size, Axes >
operator - ( vector< T, Size, Axes > const& lhs, vector< U, Size, Axes > const& rhs )
{
    vector< T, Size, Axes > res(lhs);
    res -= rhs;
    return res;
}

template < typename T, size_t Size, typename Axes >
typename vector<T, Size, Axes>::magnitude_type
magnitude_square(vector<T, Size, Axes> const& v)
{
    return v.magnitude_square();
}

template < typename T, size_t Size, typename Axes >
typename vector<T, Size, Axes>::magnitude_type
magnitude(vector<T, Size, Axes> const& v)
{
    return v.magnitude();
}

template< typename T, typename U, size_t Size, typename Axes >
typename vector<T, Size, Axes>::magnitude_type
distance_square(vector<T, Size, Axes> const& a, vector<U, Size, Axes> const& b)
{
    return magnitude_square(a - b);
}

template< typename T, typename U, size_t Size, typename Axes >
typename vector<T, Size, Axes>::magnitude_type
distance(vector<T, Size, Axes> const& a, vector<U, Size, Axes> const& b)
{
    return magnitude(a - b);
}

template < typename T, size_t Size, typename Axes >
vector< typename vector<T, Size, Axes>::value_type, Size, Axes >
normalize(vector<T, Size, Axes> const& v)
{
    return v.normalize();
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

template < typename T, ::std::size_t Size, typename Axes, typename U >
vector< typename vector<T, Size, Axes>::value_type, Size, Axes >
lerp( vector<T, Size, Axes> const& start, vector<T, Size, Axes> const& end, U percent )
{
    return start + (end - start) * percent;
}

template < typename T, ::std::size_t Size, typename Axes, typename U >
vector< typename vector<T, Size, Axes>::value_type, Size, Axes >
slerp( vector<T, Size, Axes> const& start, vector<T, Size, Axes> const& end, U percent )
{
    using vector_type = vector<T, Size, Axes>;
    using value_traits = typename vector_type::value_traits;
    using ::std::sin;
    using ::std::cos;
    using ::std::acos;

    vector_type s{start};
    auto s_mag = s.magnitude();
    s /= s_mag; // Normalize
    vector_type e{end};
    auto e_mag = e.magnitude();
    e /= e_mag; // Normalize
    // Lerp magnitude
    auto res_mag = s_mag + (e_mag - s_mag) * percent;

    auto dot = dot_product(s, e);
    if (value_traits::eq(dot, 0)) {
        // Perpendicular vectors
        auto theta = acos(dot) * percent;
        auto res = s * cos(theta) + e * sin(theta);
        return res * res_mag;
    } else if (value_traits::eq(dot, 1)) {
        // Collinear vectors same direction
        return lerp(start, end, percent);
    } else if (value_traits::eq(dot, -1)) {
        // Collinear vectors opposite direction
        throw ::std::runtime_error("Slerp for opposite vectors is undefined");
    } else {
        // Generic formula
        auto omega = acos(dot);
        auto sin_o = sin(omega);
        auto res = (s * sin((1-percent) * omega) +
                e * sin(percent * omega)) / sin_o;
        return res * res_mag;
    }
}

} // namespace math
}  /* namespace psst */

#endif /* PUSHKIN_MATH_VECTOR_HPP_ */
