/**
 * Copyright 2019 Sergei A. Fedorov
 * vector_view.hpp
 *
 *  Created on: Jan 17, 2019
 *      Author: ser-fedorov
 */

#ifndef PUSHKIN_MATH_VECTOR_VIEW_HPP_
#define PUSHKIN_MATH_VECTOR_VIEW_HPP_

#include <pushkin/math/detail/vector_expressions.hpp>

namespace psst {
namespace math {

// Mutating vector_view
template <typename T, std::size_t Size, typename Axes>
struct vector_view<T*, Size, Axes>
    : expr::vector_expression<vector_view<T*, Size, Axes>, vector<T, Size, Axes>> {

    using this_type = vector_view<T*, Size, Axes>;
    using base_expression_type
        = expr::vector_expression<vector_view<T*, Size, Axes>, vector<T, Size, Axes>>;

    using traits              = vector_traits<typename base_expression_type::result_type>;
    using value_type          = typename traits::value_type;
    using lvalue_reference    = typename traits::lvalue_reference;
    using const_reference     = typename traits::const_reference;
    using magnitude_type      = typename traits::magnitude_type;
    using pointer             = typename traits::pointer;
    using const_pointer       = typename traits::const_pointer;
    using index_sequence_type = typename traits::index_sequence_type;
    using iterator            = typename traits::iterator;
    using const_iterator      = typename traits::const_iterator;
    using axis_access         = typename base_expression_type::axis_access;
    template <std::size_t N>
    using value_policy = typename axis_access::template value_policy<N>;

    static constexpr auto size = traits::size;

    constexpr explicit vector_view(pointer p) : data_{p} {}

    pointer
    data()
    {
        return data_;
    }

    constexpr const_pointer
    data() const
    {
        return data_;
    }

    template <std::size_t N>
    typename value_policy<N>::accessor_type
    at()
    {
        static_assert(N < size, "Invalid component index in vector_view");
        return data_[N];
    }

    template <std::size_t N>
    constexpr const_reference
    at() const
    {
        static_assert(N < size, "Invalid component index in vector_view");
        return data_[N];
    }
    iterator
    begin()
    {
        return data_;
    }

    constexpr const_iterator
    begin() const
    {
        return cbegin();
    }
    constexpr const_iterator
    cbegin() const
    {
        return data_;
    }

    iterator
    end()
    {
        return data_ + size;
    }

    constexpr const_iterator
    end() const
    {
        return cend();
    }
    constexpr const_iterator
    cend() const
    {
        return data_ + size;
    }

    // FIXME Apply accessors
    lvalue_reference operator[](std::size_t idx)
    {
        assert(idx < size);
        return data_[idx];
    }

    constexpr const_reference operator[](std::size_t idx) const
    {
        assert(idx < size);
        return data_[idx];
    }
    // TODO Make converter a CRTP base
    template <typename U>
    U
    convert() const
    {
        return math::convert<U>(*this);
    }
    /**
     * Implicit conversion to pointer to element
     */
    operator pointer() { return data(); }
    /**
     * Implicit conversion to const pointer to element
     */
    operator const_pointer() const { return data(); }

private:
    pointer data_;
};

// Constant vector_view
template <typename T, std::size_t Size, typename Axes>
struct vector_view<T const*, Size, Axes>
    : expr::vector_expression<vector_view<T const*, Size, Axes>, vector<T, Size, Axes>> {

    using this_type = vector_view<T const*, Size, Axes>;
    using base_expression_type
        = expr::vector_expression<vector_view<T const*, Size, Axes>, vector<T, Size, Axes>>;

    using traits              = vector_traits<typename base_expression_type::result_type>;
    using value_type          = typename traits::value_type;
    using lvalue_reference    = typename traits::lvalue_reference;
    using const_reference     = typename traits::const_reference;
    using magnitude_type      = typename traits::magnitude_type;
    using pointer             = typename traits::pointer;
    using const_pointer       = typename traits::const_pointer;
    using index_sequence_type = typename traits::index_sequence_type;
    using iterator            = typename traits::iterator;
    using const_iterator      = typename traits::const_iterator;
    using axis_access         = typename base_expression_type::axis_access;
    template <std::size_t N>
    using value_policy = typename axis_access::template value_policy<N>;

    static constexpr auto size = traits::size;

    constexpr explicit vector_view(pointer p) : data_{p} {}
    constexpr explicit vector_view(const_pointer p) : data_{p} {}

    constexpr const_pointer
    data() const
    {
        return data_;
    }

    template <std::size_t N>
    constexpr const_reference
    at() const
    {
        static_assert(N < size, "Invalid component index in vector_view");
        return data_[N];
    }

    constexpr const_iterator
    begin() const
    {
        return cbegin();
    }
    constexpr const_iterator
    cbegin() const
    {
        return data_;
    }

    constexpr const_iterator
    end() const
    {
        return cend();
    }
    constexpr const_iterator
    cend() const
    {
        return data_ + size;
    }

    constexpr const_reference operator[](std::size_t idx) const
    {
        assert(idx < size);
        return data_[idx];
    }
    // TODO Make converter a CRTP base
    template <typename U>
    U
    convert() const
    {
        return math::convert<U>(*this);
    }
    /**
     * Implicit conversion to const pointer to element
     */
    operator const_pointer() const { return data(); }

private:
    const_pointer data_;
};

template <std::size_t Size, typename Axes = axes::default_axes_t<Size>, typename T>
constexpr auto
make_vector_view(T* val)
{
    return vector_view<T*, Size, Axes>(val);
}
template <std::size_t Size, typename Axes = axes::default_axes_t<Size>, typename T>
constexpr auto
make_vector_view(T const* val)
{
    return vector_view<T const*, Size, Axes>(val);
}

}    // namespace math
}    // namespace psst

#ifdef __METASHELL

#    include <pushkin/math/colors.hpp>

using namespace psst::math;

using vec3f_view = vector_view<float*, 3>;
using vec3d_view = vector_view<double*, 3>;

float*  fp;
double* dp;

vec3f_view v3f_v{fp};
vec3d_view v3d_v{dp};

using vec3f_const_view = vector_view<float const*, 3>;
using vec3d_const_view = vector_view<double const*, 3>;

vec3f_const_view v3f_cv{fp};
vec3d_const_view v3d_cv{dp};

using rgba_view_f       = vector_view<float*, 4, axes::rgba>;
using rgba_const_view_f = vector_view<float const*, 4, axes::rgba>;

rgba_view_f       rgba_v{fp};
rgba_const_view_f rgba_cv{fp};

#endif

#endif /* PUSHKIN_MATH_VECTOR_VIEW_HPP_ */
