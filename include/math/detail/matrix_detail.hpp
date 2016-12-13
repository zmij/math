/*
 * matrix_detail.hpp
 *
 *  Created on: Oct 29, 2014
 *      Author: zmij
 */

#ifndef ZMIJ_MATRIX_DETAIL_HPP_
#define ZMIJ_MATRIX_DETAIL_HPP_

#include <math/vector.hpp>

namespace math {

namespace detail {

template < typename RowIndexTuple, size_t ColCount, typename T >
struct matrix_builder;

template < size_t ... RowIndexes, size_t CC, typename T >
struct matrix_builder < detail::indexes_tuple< RowIndexes ... >, CC, T > :
    detail::data_holder< RowIndexes, vector< T, CC > > ... {

    using row_index_tuple       = detail::indexes_tuple< RowIndexes ... >;
    using this_type             = matrix_builder < row_index_tuple, CC, T >;

    using col_index_tuple       = typename detail::index_builder< CC >::type;
    using transposed_type       = matrix_builder< col_index_tuple, row_index_tuple::size, T >;

    using row_type              = vector< T, CC >;

    using iterator              = row_type*;
    using const_iterator        = row_type const*;

    using element_type          = typename row_type::element_type;
    using value_type            = typename row_type::value_type;
    using lvalue_reference      = typename row_type::lvalue_reference;
    using const_reference       = typename row_type::const_reference;

    using lvalue_row_reference  = typename ::std::add_lvalue_reference< row_type >::type;
    using const_row_reference   = typename ::std::add_lvalue_reference<
                                    typename ::std::add_const<row_type>::type>::type;

    matrix_builder() :
        detail::data_holder< RowIndexes, row_type >() ...
    {
    }

    template < typename ... E >
    matrix_builder( vector<E, CC> const& ... args ) :
        detail::data_holder< RowIndexes, row_type >(args) ...
    {
    }

    matrix_builder( std::initializer_list< std::initializer_list< value_type > > const& args) :
        detail::data_holder< RowIndexes, row_type >( row_type(*(args.begin() + RowIndexes)) ) ...
    {
    }

    template < size_t N >
    lvalue_row_reference
    at()
    {
        return this->detail::data_holder< N, row_type >::value;
    }

    template < size_t N >
    const_row_reference
    at() const
    {
        return this->detail::data_holder< N, row_type >::value;
    }

    iterator
    begin()
    {
        return &this->detail::data_holder< 0, row_type >::value;
    }

    const_iterator
    begin() const
    {
        return &this->detail::data_holder< 0, row_type >::value;
    }

    iterator
    end()
    {
        return begin() + row_index_tuple::size;
    }

    const_iterator
    end() const
    {
        return begin() + row_index_tuple::size;
    }

    lvalue_row_reference
    operator[] (size_t idx)
    {
        assert(idx < row_index_tuple::size);
        return begin()[idx];
    }

    const_row_reference
    operator[] (size_t idx) const
    {
        assert(idx < row_index_tuple::size);
        return begin()[idx];
    }
};

} // namespace detail

// Forward decl
template < typename T, size_t RC, size_t CC >
struct matrix;

namespace detail {

/**
 * Matrix scalar multiplication metafunction
 */
template < size_t N, typename T >
struct matrix_scalar_multiplication;

template < size_t N, typename T, size_t RC, size_t CC >
struct matrix_scalar_multiplication< N, matrix< T, RC, CC > > {
    using matrix_type = matrix< T, RC, CC >;
    template < typename U >
    void
    operator()( matrix_type& m, U s )
    {
        matrix_scalar_multiplication< N - 1, matrix_type >()(m, s);
        m.template at<N>() *= s;
    }
};

template < typename T, size_t RC, size_t CC >
struct matrix_scalar_multiplication< 0, matrix< T, RC, CC > > {
    using matrix_type = matrix< T, RC, CC >;
    template < typename U >
    void
    operator()( matrix_type& m, U s )
    {
        m.template at<0>() *= s;
    }
};

/**
 * Matrix scalar multiplication metafunction
 */
template < size_t N, typename T >
struct matrix_scalar_division;

template < size_t N, typename T, size_t RC, size_t CC >
struct matrix_scalar_division< N, matrix< T, RC, CC > > {
    using matrix_type = matrix< T, RC, CC >;
    template < typename U >
    void
    operator()( matrix_type& m, U s )
    {
        matrix_scalar_division< N - 1, matrix_type >()(m, s);
        m.template at<N>() *= s;
    }
};

template < typename T, size_t RC, size_t CC >
struct matrix_scalar_division< 0, matrix< T, RC, CC > > {
    using matrix_type = matrix< T, RC, CC >;
    template < typename U >
    void
    operator()( matrix_type& m, U s )
    {
        m.template at<0>() *= s;
    }
};

/**
 * Matrix addition metafunction
 */
template < size_t N, typename T >
struct matrix_addition;

template < size_t N, typename T, size_t RC, size_t CC >
struct matrix_addition< N, matrix < T, RC, CC > > {
    template < typename U >
    void
    operator()(matrix<T, RC, CC>& lhs, matrix<U, RC, CC> const& rhs)
    {
        matrix_addition< N - 1, matrix< T, RC, CC > >()(lhs, rhs);
        lhs.template at<N>() += rhs.template at<N>();
    }
};

template < typename T, size_t RC, size_t CC >
struct matrix_addition< 0, matrix < T, RC, CC > > {
    template < typename U >
    void
    operator()(matrix<T, RC, CC>& lhs, matrix<U, RC, CC> const& rhs)
    {
        lhs.template at<0>() += rhs.template at<0>();
    }
};

/**
 * Matrix transposition row loop metafunction
 */
template < size_t R, size_t C, typename T >
struct matrix_row_transpose;

template < size_t R, size_t C, typename T, size_t RC, size_t CC >
struct matrix_row_transpose< R, C, matrix<T, RC, CC > > {
    using source_type = matrix< T, RC, CC >;
    using source_row = typename source_type::row_type;
    using transposed_type = typename source_type::transposed_type;

    void
    operator()(transposed_type& t, source_row const& r)
    {
        matrix_row_transpose< R, C - 1, source_type >()(t, r);
        t.template at<C>().template at<R>() = r.template at<C>();
    }
};

template < size_t R, typename T, size_t RC, size_t CC >
struct matrix_row_transpose< R, 0, matrix<T, RC, CC > > {
    using source_type = matrix< T, RC, CC >;
    using source_row = typename source_type::row_type;
    using transposed_type = typename source_type::transposed_type;

    void
    operator()(transposed_type& t, source_row const& r)
    {
        t.template at<0>().template at<R>() = r.template at<0>();
    }
};

/**
 * Matrix transposition metafunction
 */
template < size_t R, typename T >
struct matrix_transpose;

template < size_t R, typename T, size_t RC, size_t CC >
struct matrix_transpose < R, matrix< T, RC, CC > > {
    using source_type = matrix< T, RC, CC >;
    using transposed_type = typename source_type::transposed_type;

    void
    operator()(transposed_type& t, source_type const& s)
    {
        matrix_transpose< R - 1, source_type >()(t, s);
        matrix_row_transpose< R, CC - 1, source_type >()(t, s.template at<R>());
    }
};

template < typename T, size_t RC, size_t CC >
struct matrix_transpose < 0, matrix< T, RC, CC > > {
    using source_type = matrix< T, RC, CC >;
    using transposed_type = typename source_type::transposed_type;

    void
    operator()(transposed_type& t, source_type const& s)
    {
        matrix_row_transpose< 0, CC - 1, source_type >()(t, s.template at<0>());
    }
};

/**
 * Matrix by vector multiplication metafunction.
 * Vector is considered a column matrix
 * @tparam C Vector column index / matrix row index
 * @tparam M Matrix type
 * @tparam V Vector type
 */
template < size_t C, typename M, typename V >
struct matrix_vector_multiply;

template < size_t C, typename T, typename U, size_t RC, size_t CC >
struct matrix_vector_multiply< C, matrix< T, RC, CC >, vector< U, CC > > {
    using matrix_type = matrix< T, RC, CC >;
    using vector_type = vector< U, CC >;
    using result_type = vector< T, CC >;

    void
    operator()(result_type& res, matrix_type const& m, vector_type const& v)
    {
        matrix_vector_multiply< C - 1, matrix_type, vector_type >()(res, m, v);
        res.template at<C>() = v * m.template at<C>();
    }
};

template < typename T, typename U, size_t RC, size_t CC >
struct matrix_vector_multiply< 0, matrix< T, RC, CC >, vector< U, CC > > {
    using matrix_type = matrix< T, RC, CC >;
    using vector_type = vector< U, CC >;
    using result_type = vector< T, CC >;

    void
    operator()(result_type& res, matrix_type const& m, vector_type const& v)
    {
        res.template at<0>() = v * m.template at<0>();
    }
};

/**
 * Matrix multiplication row loop metafunction
 * TODO Replace with matrix-vector multiply
 */
template < size_t R, size_t C, typename M1, typename M2 >
struct matrix_cell_multiply;

template < size_t R, size_t C, typename T, typename U, size_t R1, size_t C1, size_t C2 >
struct matrix_cell_multiply< R, C, matrix< T, R1, C1 >, matrix< U, C1, C2 > > {
    using left_side_type = matrix< T, R1, C1 >;
    using left_side_row = typename left_side_type::row_type;

    using right_side_type = matrix< U, C1, C2 >;
    using result_type = matrix< T, R1, C2 >;

    using right_side_transposed = typename right_side_type::transposed_type;

    void
    operator()(result_type& res, left_side_row const& lhs, right_side_transposed const& rhs)
    {
        matrix_cell_multiply< R, C - 1, left_side_type, right_side_type >()(res, lhs, rhs);
        res.template at<R>().template at<C>() = lhs * rhs.template at<C>();
    }
};

template < size_t R, typename T, typename U, size_t R1, size_t C1, size_t C2 >
struct matrix_cell_multiply< R, 0, matrix< T, R1, C1 >, matrix< U, C1, C2 > > {
    using left_side_type = matrix< T, R1, C1 >;
    using left_side_row = typename left_side_type::row_type;

    using right_side_type = matrix< U, C1, C2 >;
    using result_type = matrix< T, R1, C2 >;

    using right_side_transposed = typename right_side_type::transposed_type;

    void
    operator()(result_type& res, left_side_row const& lhs, right_side_transposed const& rhs)
    {
        res.template at<R>().template at<0>() = lhs * rhs.template at<0>();
    }
};

/**
 * Matrix multiplication metafunction
 */
template < size_t R, typename M1, typename M2 >
struct matrix_row_multiply;

template < size_t R, typename T, typename U, size_t R1, size_t C1, size_t C2 >
struct matrix_row_multiply< R, matrix< T, R1, C1 >, matrix< U, C1, C2 > > {
    using left_side_type = matrix< T, R1, C1 >;
    using right_side_type = matrix< U, C1, C2 >;
    using result_type = matrix< T, R1, C2 >;

    using right_side_transposed = typename right_side_type::transposed_type;

    void
    operator()(result_type& res, left_side_type const& lhs, right_side_transposed const& rhs)
    {
        matrix_row_multiply< R - 1, left_side_type, right_side_type >()(res, lhs, rhs);
        // TODO Replace with matrix-vector multiply
        matrix_cell_multiply< R, C2 - 1, left_side_type, right_side_type >()(res, lhs.template at<R>(), rhs);
    }
};

template < typename T, typename U, size_t R1, size_t C1, size_t C2 >
struct matrix_row_multiply< 0, matrix< T, R1, C1 >, matrix< U, C1, C2 > > {
    using left_side_type = matrix< T, R1, C1 >;
    using right_side_type = matrix< U, C1, C2 >;
    using result_type = matrix< T, R1, C2 >;

    using right_side_transposed = typename right_side_type::transposed_type;

    void
    operator()(result_type& res, left_side_type const& lhs, right_side_transposed const& rhs)
    {
        // TODO Replace with matrix-vector multiply
        matrix_cell_multiply< 0, C2 - 1, left_side_type, right_side_type >()(res, lhs.template at<0>(), rhs);
    }

};

/**
 * Cofactor sign metafunction
 */
template < size_t R, size_t C >
struct cofactor_sign {
    static const int value = ((R + C) % 2) ? -1 : 1;
};

/**
 * Matrix minor calculation metafunction
 */
template < typename T >
struct matrix_minor;



/**
 * Matrix cofactor metafunction
 */
template < size_t R, size_t C, typename T, size_t Size >
struct matrix_cofactor;

template < size_t R, size_t C, typename T, size_t Size >
struct matrix_cofactor < R, C, matrix< T, Size, Size >, Size > {

};

template < typename T >
struct matrix_cofactor < 0, 0, matrix< T, 1, 1 >, 1 > {
    using matrix_type = matrix<T, 1, 1>;
    T
    operator()( matrix_type const& )
    {
        return 0;
    }
};

template < size_t N, typename T >
    struct identity_matrix;

/**
 * Top specialization
 */
template < size_t N, typename T>
    struct identity_matrix< N, matrix< T, N + 1, N + 1> > {
        using matrix_type = matrix<T, N + 1, N + 1>;

        static void
        fill_matrix(matrix_type& m)
        {
            m.template at<N>().zero();
            m.template at<N>().template at<N>() = 1;
            identity_matrix< N -1, matrix_type >::fill_matrix(m);
        }

        static matrix_type
        build_matrix()
        {
            matrix_type m;
            fill_matrix(m);
            return m;
        }
    };

template < size_t N, typename T, size_t Size >
    struct identity_matrix< N, matrix< T, Size, Size > > {
        using matrix_type = matrix<T, Size, Size>;

        static void
        fill_matrix(matrix_type& m)
        {
            m.template at<N>().zero();
            m.template at<N>().template at<N>() = 1;
            identity_matrix< N -1, matrix_type >::fill_matrix(m);
        }
    };

template < typename T, size_t Size >
    struct identity_matrix< 0, matrix< T, Size, Size > > {
        using matrix_type = matrix<T, Size, Size>;

        static void
        fill_matrix(matrix_type& m)
        {
            m.template at<0>().zero();
            m.template at<0>().template at<0>() = 1;
        }
    };

} // namespace detail

}  // namespace math

#endif /* ZMIJ_MATRIX_DETAIL_HPP_ */
