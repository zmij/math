/*
 * vector_detail.hpp
 *
 *  Created on: Oct 29, 2014
 *      Author: zmij
 */

#ifndef ZMIJ_VECTOR_DETAIL_HPP_
#define ZMIJ_VECTOR_DETAIL_HPP_

#include <type_traits>

namespace math {

namespace detail {
/** Variadic template arguments unwrapping */

template< ::std::size_t Index, typename T >
struct data_holder {
    static constexpr ::std::size_t index = Index;
    using type = T;

    type value;

    data_holder() = default;
    data_holder(data_holder const&) = default;
    data_holder(data_holder&&) = default;
    data_holder&
    operator = (data_holder const&) = default;
    data_holder&
    operator = (data_holder&&) = default;

    data_holder(type const& value)
        : value(value)
    {
    }

    template< typename Y >
    data_holder(Y const& value)
        : value(value)
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

template< ::std::size_t ... Indexes >
struct indexes_tuple {
    static constexpr ::std::size_t size = sizeof ... (Indexes);
};

template< ::std::size_t num, typename tp = indexes_tuple< > >
struct index_builder;

template< ::std::size_t num, size_t ... Indexes >
struct index_builder< num, indexes_tuple< Indexes ... > > : index_builder<
        num - 1, indexes_tuple< Indexes ..., sizeof ... (Indexes) > > {
};

template< ::std::size_t ... Indexes >
struct index_builder< 0, indexes_tuple< Indexes ... > > {
    using type = indexes_tuple< Indexes ... >;
    static constexpr ::std::size_t size = sizeof ... (Indexes);
};

template < typename Builder >
struct __base_coord_access {
    using builder_type          = Builder;
protected:
    builder_type&
    rebind()
    {
        return static_cast<builder_type&>(*this);
    }
    builder_type const&
    rebind() const
    {
        return static_cast<builder_type const&>(*this);
    }
};

template < ::std::size_t AxesCount, typename Builder, typename T >
struct __coord_access : __coord_access< AxesCount - 1, Builder, T >{};

template < typename Builder, typename T >
struct __coord_access<0, Builder, T> {};
template < typename Builder, typename T >
struct __coord_access<1, Builder, T> : __base_coord_access<Builder> {
    using value_type            = typename ::std::decay<T>::type;
    using lvalue_reference      = typename ::std::add_lvalue_reference<value_type>::type;
    using const_reference       = typename ::std::add_lvalue_reference<
                                    typename ::std::add_const<value_type>::type>::type;
    using base_type             = __base_coord_access<Builder>;

    lvalue_reference
    x()
    {
        return base_type::rebind().template at<0>();
    }
    const_reference
    x() const
    {
        return base_type::rebind().template at<0>();
    }
};
template < typename Builder, typename T >
struct __coord_access<2, Builder, T> : __coord_access<1, Builder, T> {
    using value_type            = typename ::std::decay<T>::type;
    using lvalue_reference      = typename ::std::add_lvalue_reference<value_type>::type;
    using const_reference       = typename ::std::add_lvalue_reference<
                                    typename ::std::add_const<value_type>::type>::type;
    using base_type             = __base_coord_access<Builder>;

    lvalue_reference
    y()
    {
        return base_type::rebind().template at<1>();
    }
    const_reference
    y() const
    {
        return base_type::rebind().template at<1>();
    }
};
template < typename Builder, typename T >
struct __coord_access<3, Builder, T> : __coord_access<2, Builder, T> {
    using value_type            = typename ::std::decay<T>::type;
    using lvalue_reference      = typename ::std::add_lvalue_reference<value_type>::type;
    using const_reference       = typename ::std::add_lvalue_reference<
                                    typename ::std::add_const<value_type>::type>::type;
    using base_type             = __base_coord_access<Builder>;

    lvalue_reference
    z()
    {
        return base_type::rebind().template at<2>();
    }
    const_reference
    z() const
    {
        return base_type::rebind().template at<2>();
    }
};

template < typename IndexTuple, typename T >
struct vector_builder;

template < ::std::size_t ... Indexes, typename T >
struct vector_builder< indexes_tuple< Indexes ... >, T > :
    data_holder< Indexes, T > ...,
    __coord_access<sizeof ... (Indexes),
        vector_builder<indexes_tuple< Indexes ... >, T>, T> {

    static constexpr ::std::size_t size = sizeof ... (Indexes);
    using indexes_tuple_type    = indexes_tuple< Indexes ... >;
    using this_type             = vector_builder< indexes_tuple_type, T >;

    using element_type          = T;
    using value_type            = typename ::std::decay<T>::type;
    using lvalue_reference      = typename ::std::add_lvalue_reference<value_type>::type;
    using const_reference       = typename ::std::add_lvalue_reference<
                                    typename ::std::add_const<value_type>::type>::type;

    using pointer               = value_type*;
    using const_pointer         = value_type const*;

    using iterator              = value_type*;
    using const_iterator        = value_type const*;

    vector_builder() = default;

    /**
     * Construct vector from a vector of larger size
     * @param rhs
     * @param
     */
    template < size_t ... IndexesR, typename U >
    vector_builder(
            vector_builder< indexes_tuple< IndexesR ... >, U > const& rhs,
            typename ::std::enable_if< size <= sizeof ... (IndexesR) >::type* = 0 )
        : data_holder< Indexes, T >( rhs.template at< Indexes >() ) ...
    {
    }

    template < typename ... E >
    vector_builder(E const& ... args,
            typename ::std::enable_if< size == sizeof ... (E) >::type* = 0)
        : data_holder< Indexes, T >(args) ...
    {
    }

    template < typename ... E >
    vector_builder(E&& ... args,
            typename ::std::enable_if< size == sizeof ... (E) >::type* = 0)
        : data_holder< Indexes, T >( std::forward<E>(args) ) ...
    {
    }

    vector_builder(::std::initializer_list<value_type> const& args)
        : data_holder< Indexes, T >(*(args.begin() + Indexes))...
    {
    }

    vector_builder(const_pointer p)
        : data_holder< Indexes, T >(*(p + Indexes))...
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
        return cbegin();
    }
    const_iterator
    cbegin() const
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
        return cend();
    }
    const_iterator
    cend() const
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

template < typename T, ::std::size_t Size >
struct vector;

namespace detail {

template < ::std::size_t N, typename T >
struct dot_product;

template < ::std::size_t N, typename T, ::std::size_t Size >
struct dot_product< N, vector<T, Size> > {

    typename vector<T, Size>::value_type
    operator()( vector<T, Size> const& lhs, vector<T, Size> const& rhs )
    {
        return dot_product< N -1, vector<T, Size> >()(lhs, rhs) +
                lhs.template at<N>() * rhs.template at<N>();
    }
};

template < typename T, ::std::size_t Size >
struct dot_product< 0, vector<T, Size> > {
    typename vector<T, Size>::value_type
    operator()(vector<T, Size> const& lhs, vector<T, Size> const& rhs)
    {
        return lhs.template at< 0 >() * rhs.template at< 0 >();
    }
};

template < ::std::size_t N, typename T >
struct vector_scalar_multiplication;

template < ::std::size_t N, typename T, ::std::size_t Size >
struct vector_scalar_multiplication< N, vector< T, Size > > {
    template < typename U >
    void
    operator()( vector<T, Size>& v, U s )
    {
        vector_scalar_multiplication< N - 1, vector<T, Size> >{}(v, s);
        v.template at<N>() *= s;
    }
};

template < typename T, ::std::size_t Size >
struct vector_scalar_multiplication< 0, vector< T, Size > > {
    template < typename U >
    void
    operator()( vector<T, Size>& v, U s )
    {
        v.template at<0>() *= s;
    }
};

template < ::std::size_t N, typename T >
struct vector_scalar_division;

template < ::std::size_t N, typename T, ::std::size_t Size >
struct vector_scalar_division< N, vector< T, Size > > {
    template < typename U >
    void
    operator()( vector<T, Size>& v, U s )
    {
        vector_scalar_division< N - 1, vector<T, Size> >{}(v, s);
        v.template at<N>() /= s;
    }
};

template < typename T, ::std::size_t Size >
struct vector_scalar_division< 0, vector< T, Size > > {
    template < typename U >
    void
    operator()( vector<T, Size>& v, U s )
    {
        v.template at<0>() /= s;
    }
};

template < ::std::size_t N, typename T >
struct vector_addition;

template < ::std::size_t N, typename T, ::std::size_t Size >
struct vector_addition< N, vector< T, Size > > {
    template < typename U >
    void
    operator()( vector<T, Size>& lhs, vector<U, Size> const& rhs)
    {
        vector_addition<N - 1, vector<T, Size> >{}(lhs, rhs);
        lhs.template at<N>() += rhs.template at<N>();
    }
};

template < typename T, ::std::size_t Size >
struct vector_addition< 0, vector< T, Size > > {
    template < typename U >
    void
    operator()( vector<T, Size>& lhs, vector<U, Size> const& rhs)
    {
        lhs.template at<0>() += rhs.template at<0>();
    }
};

template < ::std::size_t N, typename T >
struct vector_substraction;

template < ::std::size_t N, typename T, ::std::size_t Size >
struct vector_substraction< N, vector< T, Size > > {
    template < typename U >
    void
    operator()( vector<T, Size>& lhs, vector<U, Size> const& rhs)
    {
        vector_substraction<N - 1, vector<T, Size> >{}(lhs, rhs);
        lhs.template at<N>() -= rhs.template at<N>();
    }
};

template < typename T, ::std::size_t Size >
struct vector_substraction< 0, vector< T, Size > > {
    template < typename U >
    void
    operator()( vector<T, Size>& lhs, vector<U, Size> const& rhs)
    {
        lhs.template at<0>() -= rhs.template at<0>();
    }
};

template < ::std::size_t N, typename T >
struct set_all_elements;

template < ::std::size_t N, typename T, ::std::size_t Size >
struct set_all_elements< N, vector< T, Size > > {
    template < typename U >
    void
    operator()( vector<T, Size>& v, U s )
    {
        set_all_elements< N - 1, vector< T, Size > >{}(v, s);
        v.template at<N>() = s;
    }
};

template < typename T, ::std::size_t Size >
struct set_all_elements< 0, vector< T, Size > > {
    template < typename U >
    void
    operator()( vector<T, Size>& v, U s )
    {
        v.template at<0>() = s;
    }
};

} // namespace detail

}  // namespace math



#endif /* ZMIJ_VECTOR_DETAIL_HPP_ */
