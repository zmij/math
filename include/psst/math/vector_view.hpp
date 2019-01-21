/**
 * Copyright 2019 Sergei A. Fedorov
 * vector_view.hpp
 *
 *  Created on: Jan 17, 2019
 *      Author: ser-fedorov
 */

#ifndef PSST_MATH_VECTOR_VIEW_HPP_
#define PSST_MATH_VECTOR_VIEW_HPP_

#include <psst/math/detail/vector_expressions.hpp>

#include <iterator>

namespace psst {
namespace math {

// TODO Deal with alignment stuff

// Mutating vector_view
template <typename T, std::size_t Size, typename Axes>
struct vector_view<T*, Size, Axes>
    : expr::vector_expression<vector_view<T*, Size, Axes>, vector<T, Size, Axes>> {

    using this_type = vector_view<T*, Size, Axes>;
    using base_expression_type
        = expr::vector_expression<vector_view<T*, Size, Axes>, vector<T, Size, Axes>>;

    using traits              = traits::vector_traits<typename base_expression_type::result_type>;
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

    template <typename Expression, typename = math::traits::enable_if_vector_expression<Expression>,
              typename = math::traits::enable_for_compatible_axes<this_type, Expression>>
    vector_view&
    operator=(Expression const& rhs)
    {
        return assign(rhs, utils::make_min_index_sequence<
                               Size, math::traits::vector_expression_size_v<Expression>>{});
    }

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
    template <typename Expr, std::size_t... Indexes>
    vector_view&
    assign(Expr&& rhs, std::index_sequence<Indexes...>)
    {
        ((this->template at<Indexes>() = rhs.template at<Indexes>()), ...);
        return *this;
    }

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

    using traits              = traits::vector_traits<typename base_expression_type::result_type>;
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

/**
 * Utility to treat a region of memory as a 'container' of vectors of certain type
 */
template <typename T, std::size_t Size, typename Axes = axes::default_axes_t<Size>>
struct memory_vector_view;

template <typename T, std::size_t Size, typename Axes>
struct memory_vector_view<T*, Size, Axes> {
    using pointer_type       = T*;
    using const_pointer_type = T const*;
    using view_type          = vector_view<T*, Size, Axes>;

    static constexpr std::size_t component_count = Size;
    static constexpr std::size_t element_size    = sizeof(T) * component_count;

    template <typename P>
    struct base_iterator : std::iterator<std::random_access_iterator_tag, view_type, std::ptrdiff_t,
                                         view_type, view_type> {
        using base_type = std::iterator<std::random_access_iterator_tag, view_type, std::ptrdiff_t,
                                        view_type, view_type>;
        using difference_type = typename base_type::difference_type;
        using value_type      = typename base_type::value_type;

        base_iterator(P p) : p_{p} {}

        bool
        operator==(base_iterator const& rhs) const
        {
            return p_ == rhs.p_;
        }
        bool
        operator!=(base_iterator const& rhs) const
        {
            return p_ != rhs.p_;
        }

        base_iterator&
        operator++()
        {
            p_ += component_count;
            return *this;
        }
        base_iterator
        operator++(int)
        {
            base_iterator i{p_};
            p_ += component_count;
            return i;
        }
        base_iterator
        operator+(difference_type d) const
        {
            return base_iterator{p_ + d * component_count};
        }
        base_iterator&
        operator+=(difference_type d)
        {
            p_ += d * component_count;
            return *this;
        }

        base_iterator&
        operator--()
        {
            p_ -= component_count;
            return *this;
        }
        base_iterator
        operator--(int)
        {
            base_iterator i{p_};
            p_ -= component_count;
            return i;
        }
        base_iterator
        operator-(difference_type d) const
        {
            return base_iterator{p_ - d * component_count};
        }
        difference_type
        operator-(base_iterator const& rhs) const
        {
            return (p_ - rhs.p_) / component_count;
        }
        base_iterator&
        operator-=(difference_type d)
        {
            p_ -= d * component_count;
            return *this;
        }

        base_iterator operator[](difference_type index) const
        {
            return base_iterator{p_ + index * component_count};
        }

        value_type operator*() const { return value_type{p_}; }
        value_type operator->() const { return value_type{p_}; }

    private:
        P p_;
    };

    using iterator       = base_iterator<pointer_type>;
    using const_iterator = base_iterator<const_pointer_type>;

    constexpr memory_vector_view(pointer_type buffer, std::size_t buffer_size)
        : buffer_{buffer}, buffer_size_{buffer_ ? buffer_size : 0}
    {
        if (buffer_size % Size != 0)
            throw std::runtime_error{"The size of buffer is not a multiple of components"};
    }

    /**
     * Is the memory empty
     * @return
     */
    constexpr bool
    empty() const
    {
        return buffer_size_ == 0;
    }

    /**
     * Number of vector_view elements available
     * @return
     */
    constexpr std::size_t
    size() const
    {
        return buffer_size_ / Size;
    }

    constexpr view_type operator[](std::size_t index) const
    {
        return view_type{buffer_ + index * element_size};
    }

    constexpr iterator
    begin()
    {
        return iterator{buffer_};
    }
    constexpr const_iterator
    begin() const
    {
        return cbegin();
    }
    constexpr const_iterator
    cbegin() const
    {
        return const_iterator{buffer_};
    }

    constexpr iterator
    end()
    {
        return iterator{buffer_ + buffer_size_};
    }
    constexpr const_iterator
    end() const
    {
        return cend();
    }
    constexpr const_iterator
    cend() const
    {
        return const_iterator{buffer_ + buffer_size_};
    }

private:
    pointer_type buffer_;
    std::size_t  buffer_size_;
};

//----------------------------------------------------------------------------
template <typename T, typename U, typename = traits::enable_if_vector<T>>
constexpr auto
make_vector_view(U* buffer)
{
    using value_type    = traits::scalar_expression_result_t<T>;
    using axes_type     = traits::axes_names_t<T>;
    constexpr auto size = traits::vector_expression_size_v<T>;
    static_assert((std::is_same<std::decay_t<U>, value_type>{}), "Incompatible pointer type");
    return vector_view<U*, size, axes_type>(buffer);
}

template <typename T, typename = traits::enable_if_vector<T>>
constexpr auto
make_vector_view(char* buffer)
{
    using value_type = traits::scalar_expression_result_t<T>;
    return make_vector_view<T>(reinterpret_cast<value_type*>(buffer));
}

template <typename T, typename = traits::enable_if_vector<T>>
constexpr auto
make_vector_view(char const* buffer)
{
    using value_type = traits::scalar_expression_result_t<T>;
    return make_vector_view<T>(reinterpret_cast<value_type const*>(buffer));
}

template <typename T, typename U, typename = traits::enable_if_vector<T>>
constexpr auto
make_memory_vector_view(U* val, std::size_t buffer_size)
{
    using value_type    = traits::scalar_expression_result_t<T>;
    using axes_type     = traits::axes_names_t<T>;
    constexpr auto size = traits::vector_expression_size_v<T>;
    static_assert((std::is_same<std::decay_t<U>, value_type>{}), "Incompatible pointer type");
    return memory_vector_view<U*, size, axes_type>(val, buffer_size);
}

template <typename T, typename = traits::enable_if_vector<T>>
constexpr auto
make_memory_vector_view(char* buffer, std::size_t buffer_size)
{
    using value_type = traits::scalar_expression_result_t<T>;
    return make_memory_vector_view<T>(reinterpret_cast<value_type*>(buffer),
                                      buffer_size / sizeof(value_type));
}

template <typename T, typename = traits::enable_if_vector<T>>
constexpr auto
make_memory_vector_view(char const* buffer, std::size_t buffer_size)
{
    using value_type = traits::scalar_expression_result_t<T>;
    return make_memory_vector_view<T>(reinterpret_cast<value_type const*>(buffer),
                                      buffer_size / sizeof(value_type));
}

}    // namespace math
}    // namespace psst

#ifdef __METASHELL

#    include <psst/math/colors.hpp>

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

#endif /* PSST_MATH_VECTOR_VIEW_HPP_ */
