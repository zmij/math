/*
 * Matrix.hpp
 *
 *  Created on: 19.01.2012
 *      Author: zmij
 */

#ifndef PUSHKIN_MATH_MATRIX_HPP_
#define PUSHKIN_MATH_MATRIX_HPP_

#include <pushkin/math/detail/matrix_expressions.hpp>

namespace psst {
namespace math {

/**
 * Matrix template.
 * @tparam T type of value in a matrix cell
 * @tparam RC row count
 * @tparam CC column count;
 */
template < typename T, ::std::size_t RC, ::std::size_t CC, typename Axes >
struct matrix : expr::matrix_expression<matrix<T, RC, CC, Axes>>,
            detail::axes_names_t<RC, Axes, matrix<T, RC, CC, Axes>, vector<T, CC, Axes>> {

    using this_type             = matrix< T, RC, CC, Axes >;
    using transposed_type       = matrix< T, CC, RC, Axes >;
    using traits                = matrix_traits<this_type>;

    using row_type              = typename traits::row_type;
    using row_indexes_type      = typename traits::row_indexes_type;
    using col_indexes_type      = typename traits::col_indexes_type;

    using value_type            = typename traits::value_type;
    using lvalue_reference      = typename traits::lvalue_reference;
    using const_reference       = typename traits::const_reference;
    using pointer               = typename traits::pointer;
    using const_pointer         = typename traits::const_pointer;
    using iterator              = typename traits::iterator;
    using const_iterator        = typename traits::const_iterator;

    static constexpr ::std::size_t rows   = traits::rows;
    static constexpr ::std::size_t cols   = traits::cols;
    static constexpr ::std::size_t size   = traits::size;

    using row_iterator          = row_type*;
    using const_row_iterator    = row_type const*;
    using lvalue_row_reference  = ::std::add_lvalue_reference_t< row_type >;
    using const_row_reference   = ::std::add_lvalue_reference_t<::std::add_const_t<row_type>>;

    using multi_dim_type        = value_type[cols];
    using const_multi_dim_type  = value_type const[cols];
    using multi_dim_ptr         = multi_dim_type*;
    using const_multi_dim_ptr   = const_multi_dim_type const*;

    using init_list             = ::std::initializer_list< typename row_type::init_list >;

    constexpr matrix() = default;

    constexpr explicit matrix(value_type val)
        : matrix(val, col_indexes_type{}) {}

    constexpr explicit matrix(const_pointer p)
        : matrix(p, row_indexes_type{}) {}

    constexpr explicit matrix(const_multi_dim_ptr p)
        : matrix(p, row_indexes_type{}) {}

    constexpr matrix(init_list const& args)
        : matrix(args, row_indexes_type{}) {}

    template <typename Expression, typename Result>
    constexpr matrix(expr::matrix_expression<Expression, Result> const& rhs)
        : matrix(rhs,
            utils::make_min_index_sequence<rows, expr::matrix_expression<Expression, Result>::rows>{}) {}

    pointer
    data()
    { return std::get<0>(data_).data(); }
    const_pointer
    data() const
    { return std::get<0>(data_).data(); }

    template < ::std::size_t R >
    lvalue_row_reference
    at()
    {
        static_assert(R < rows, "Invalid matrix row index");
        return ::std::get<R>(data_);
    }

    template < ::std::size_t R >
    const_row_reference
    at() const
    {
        static_assert(R < rows, "Invalid matrix row index");
        return ::std::get<R>(data_);
    }

    template < ::std::size_t R, ::std::size_t C >
    lvalue_reference
    element()
    {
        static_assert(C < cols, "Invalid matrix column index");
        return at<R>().template at<C>();
    }
    template < ::std::size_t R, ::std::size_t C >
    const_reference
    element() const
    {
        static_assert(C < cols, "Invalid matrix column index");
        return at<R>().template at<C>();
    }

    iterator
    begin()
    { return data_.front().begin(); }
    const_iterator
    begin() const { return cbegin(); }
    const_iterator
    cbegin() const
    { return data_.front().cbegin(); }

    iterator
    end()
    { return data_.back().end(); }
    const_iterator
    end() const
    { return cend(); }
    const_iterator
    cend() const
    { return data_.back().end(); }

    row_iterator
    row_begin()
    { return data_.begin(); }
    const_row_iterator
    row_begin() const
    { return data_.begin(); }

    row_iterator
    row_end()
    { return data_.end(); }
    const_row_iterator
    row_end() const
    { return data_.end(); }

    lvalue_row_reference
    operator[] (::std::size_t idx)
    {
        assert(idx < rows);
        return data_[idx];
    }
    const_row_reference
    operator[] (::std::size_t idx) const
    {
        assert(idx < rows);
        return data_[idx];
    }

    // TODO Make it an expression
    this_type
    operator - ()
    {
        this_type res(*this);
        res *= -1;
        return res;
    }

    template < typename U >
    this_type&
    operator += (matrix<U, RC, CC, Axes> const& rhs)
    {
        return *this = *this + rhs;
    }

    template < typename U >
    this_type&
    operator -= (matrix<U, RC, CC, Axes> const& rhs)
    {
        return *this = *this - rhs;
    }

    template < typename U >
    this_type&
    operator *= (U s)
    {
        return *this = *this * s;
    }

    template < typename U >
    this_type&
    operator /= (U s)
    {
        return *this = *this / s;
    }

    transposed_type
    transpose() const
    {
        return expr::transpose(*this);
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

    template < typename U = T >
    constexpr static typename ::std::enable_if< RC == CC, matrix<U, RC, CC, Axes> >::type const&
    identity()
    {
        return expr::identity<this_type>();
    }
private:
    template < ::std::size_t... RI >
    constexpr matrix(value_type val, ::std::index_sequence<RI...>)
        : data_({ utils::value_fill<RI, row_type>{row_type(val)}.value... }) {}
    template < ::std::size_t... RI >
    constexpr matrix(init_list const& args, ::std::index_sequence<RI...>)
        : data_({ row_type( *(args.begin() + RI) ) ... }) {}
    template < ::std::size_t... RI >
    constexpr matrix(const_pointer p, ::std::index_sequence<RI...>)
        : data_({ row_type(p + RI * cols) ... }) {}
    template < ::std::size_t... RI >
    constexpr matrix(const_multi_dim_ptr p, ::std::index_sequence<RI...>)
        : data_({ row_type(p[RI]) ... }) {}
    template < typename Expr, typename Result, std::size_t... RI >
    constexpr matrix(expr::matrix_expression<Expr, Result> const& rhs, std::index_sequence<RI...>)
        : data_( { expr::row<RI>(rhs)... } ) {}
private:
    using data_type     = ::std::array<row_type, rows>;
    data_type data_;
};

} // namespace math
}  /* namespace psst */

#endif /* PUSHKIN_MATRIX_HPP_ */
