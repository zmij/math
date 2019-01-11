/*
 * matrix_expressions.hpp
 *
 *  Created on: Dec 26, 2018
 *      Author: ser-fedorov
 */

#ifndef PUSHKIN_MATH_DETAIL_MATRIX_EXPRESSIONS_HPP_
#define PUSHKIN_MATH_DETAIL_MATRIX_EXPRESSIONS_HPP_

#include <pushkin/math/detail/vector_expressions.hpp>

namespace psst {
namespace math {
namespace expr {

inline namespace m {

//----------------------------------------------------------------------------
template <typename Expression, typename Result = Expression>
struct matrix_expression {
    static_assert(is_matrix_v<Result>, "Result of matrix expression must be a matrix");
    using expression_type  = Expression;
    using result_type      = Result;
    using traits           = value_traits_t<Result>;
    using value_type       = typename traits::value_type;
    using value_tag        = typename traits::value_tag;
    using axes_names       = typename traits::axes_names;
    using matrix_type      = typename traits::matrix_type;
    using transposed_type  = typename traits::transposed_type;
    using row_type         = typename traits::row_type;
    using col_type         = typename traits::col_type;
    using row_indexes_type = typename traits::row_indexes_type;
    using col_indexes_type = typename traits::col_indexes_type;

    static constexpr auto rows = traits::rows;
    static constexpr auto cols = traits::cols;
    static constexpr auto size = traits::size;
};

namespace detail {

template <typename T>
constexpr std::size_t
matrix_row_count()
{
    static_assert(is_matrix_expression_v<T>, "The expression is not matrix type");
    return std::decay_t<T>::rows;
}

}    // namespace detail

template <typename T>
struct matrix_row_count : utils::size_constant<detail::matrix_row_count<T>()> {};
template <typename T>
using matrix_row_count_t = typename matrix_row_count<T>::type;
template <typename T>
constexpr std::size_t matrix_row_count_v = matrix_row_count_t<T>::value;

//----------------------------------------------------------------------------
template <typename Matrix>
struct identity_matrix : matrix_expression<identity_matrix<Matrix>, Matrix> {
    using base_type  = matrix_expression<identity_matrix<Matrix>, Matrix>;
    using value_type = typename base_type::value_type;

    static_assert(base_type::cols == base_type::rows,
                  "Identity matrix is defined only for square matrices");

    template <std::size_t R, std::size_t C>
    constexpr value_type
    element() const
    {
        static_assert(R < base_type::rows, "Invalid matrix expression row index");
        static_assert(C < base_type::cols, "Invalid matrix expression col index");
        if constexpr (R == C) {
            return 1;
        } else {
            return 0;
        }
    }
};

template <typename Matrix, typename = enable_if_matrix_expression<Matrix>>
constexpr auto
identity()
{
    return identity_matrix<std::decay_t<Matrix>>{};
}

template <typename Matrix, typename = enable_if_matrix_expression<Matrix>>
constexpr auto
identity(Matrix&& m)
{
    return identity_matrix<std::decay_t<Matrix>>{};
}

//----------------------------------------------------------------------------
//@{
/** @name Vector as row matrix */
template <typename Vector>
struct vector_to_row_matrix {
    static_assert(is_vector_expression_v<Vector>, "Argument to the expression must be a vector");
    using traits = typename std::decay_t<Vector>::traits;
    using type = matrix<typename traits::value_type, 1, traits::size, typename traits::axes_names>;
};
template <typename Vector>
using vector_to_row_matrix_t = typename vector_to_row_matrix<Vector>::type;

template <typename Vector>
struct vector_as_row_matrix
    : matrix_expression<vector_as_row_matrix<Vector>, vector_to_row_matrix_t<Vector>>,
      unary_expression<Vector> {

    using base_type
        = matrix_expression<vector_as_row_matrix<Vector>, vector_to_row_matrix_t<Vector>>;

    using expression_base = unary_expression<Vector>;
    using expression_base::expression_base;

    template <std::size_t R, std::size_t C>
    constexpr auto
    element() const
    {
        static_assert(R < base_type::rows, "Invalid matrix expression row index");
        static_assert(C < base_type::cols, "Invalid matrix expression col index");
        return get<C>(this->arg_);
    }
};

template <typename Vector, typename = enable_if_vector_expression<Vector>>
constexpr auto
as_row_matrix(Vector&& vec)
{
    return make_unary_expression<vector_as_row_matrix>(std::forward<Vector>(vec));
}
//@}

//----------------------------------------------------------------------------
//@{
/** @name Vector as column matrix */
template <typename Vector>
struct vector_to_col_matrix {
    static_assert(is_vector_expression_v<Vector>, "Argument to the expression must be a vector");
    using traits = typename std::decay_t<Vector>::traits;
    using type = matrix<typename traits::value_type, traits::size, 1, typename traits::axes_names>;
};
template <typename Vector>
using vector_to_col_matrix_t = typename vector_to_col_matrix<Vector>::type;

template <typename Vector>
struct vector_as_col_matrix
    : matrix_expression<vector_as_col_matrix<Vector>, vector_to_col_matrix_t<Vector>>,
      unary_expression<Vector> {

    using base_type
        = matrix_expression<vector_as_col_matrix<Vector>, vector_to_col_matrix_t<Vector>>;

    using expression_base = unary_expression<Vector>;
    using expression_base::expression_base;

    template <std::size_t R, std::size_t C>
    constexpr auto
    element() const
    {
        static_assert(R < base_type::rows, "Invalid matrix expression row index");
        static_assert(C < base_type::cols, "Invalid matrix expression col index");
        return get<R>(this->arg_);
    }
};

template <typename Vector, typename = enable_if_vector_expression<Vector>>
constexpr auto
as_col_matrix(Vector&& vec)
{
    return make_unary_expression<vector_as_col_matrix>(std::forward<Vector>(vec));
}
//@}

//----------------------------------------------------------------------------
template <typename Matrix, std::size_t RN>
struct nth_row : vector_expression<nth_row<Matrix, RN>, typename std::decay_t<Matrix>::row_type>,
                 unary_expression<Matrix> {

    using matrix_traits = typename std::decay_t<Matrix>::traits;
    static_assert(RN < matrix_traits::rows, "Invalid row index");

    using base_type
        = vector_expression<nth_row<Matrix, RN>, typename std::decay_t<Matrix>::row_type>;
    using value_type = typename base_type::value_type;

    using expression_base = unary_expression<Matrix>;
    using expression_base::expression_base;

    template <std::size_t CN>
    constexpr auto
    at() const
    {
        static_assert(CN < matrix_traits::cols, "Invalid column index");
        return this->arg_.template element<RN, CN>();
    }
};

template <std::size_t R, typename Matrix, typename = enable_if_matrix_expression<Matrix>>
constexpr auto
row(Matrix&& mtx)
{
    return make_unary_expression<nth_row, R>(std::forward<Matrix>(mtx));
}

//----------------------------------------------------------------------------
template <typename Matrix, std::size_t CN>
struct nth_col : vector_expression<nth_col<Matrix, CN>, typename std::decay_t<Matrix>::col_type>,
                 unary_expression<Matrix> {

    using matrix_traits = typename std::decay_t<Matrix>::traits;
    static_assert(CN < matrix_traits::cols, "Invalid col index");

    using base_type
        = vector_expression<nth_row<Matrix, CN>, typename std::decay_t<Matrix>::col_type>;
    using value_type = typename base_type::value_type;

    using expression_base = unary_expression<Matrix>;
    using expression_base::expression_base;

    template <std::size_t RN>
    constexpr auto
    at() const
    {
        static_assert(RN < matrix_traits::rows, "Invalid row index");
        return this->arg_.template element<RN, CN>();
    }
};

template <std::size_t C, typename Matrix, typename = enable_if_matrix_expression<Matrix>>
constexpr auto
col(Matrix&& mtx)
{
    return make_unary_expression<nth_col, C>(std::forward<Matrix>(mtx));
}

//----------------------------------------------------------------------------
//@{
template <typename Matrix>
struct matrix_as_row_result {
    using original_matrix_type = typename std::decay_t<Matrix>::matrix_type;
    using value_type           = typename original_matrix_type::value_type;
    using axes_names           = typename original_matrix_type::axes_names;
    static constexpr auto size = original_matrix_type::size;
    using type                 = vector<value_type, size, axes_names>;
};
template <typename Matrix>
using matrix_as_row_result_t = typename matrix_as_row_result<Matrix>::type;
template <typename Expr>
struct matrix_as_row : vector_expression<matrix_as_row<Expr>, matrix_as_row_result_t<Expr>>,
                       unary_expression<Expr> {
    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    using original_matrix_type = typename std::decay_t<Expr>::matrix_type;

    template <std::size_t N>
    constexpr auto
    at() const
    {
        return this->arg_
            .template element<N / original_matrix_type::rows, N % original_matrix_type::rows>();
    }
};

template <typename Matrix, typename = enable_if_matrix_expression<Matrix>>
constexpr auto
as_vector(Matrix&& mtx)
{
    using matrix_type = std::decay_t<Matrix>;
    if constexpr (matrix_type::rows == 1) {
        return row<0>(std::forward<Matrix>(mtx));
    } else if constexpr (matrix_type::cols == 1) {
        return col<0>(std::forward<Matrix>(mtx));
    } else {
        return make_unary_expression<matrix_as_row>(std::forward<Matrix>(mtx));
    }
}
//@}

//----------------------------------------------------------------------------
//@{
/** @name Remove Nth matrix row */
template <typename Matrix>
struct remove_row_result {
    using original_matrix_type = typename std::decay_t<Matrix>::matrix_type;
    using value_type           = typename original_matrix_type::value_type;
    using axes_names           = typename original_matrix_type::axes_names;
    static_assert(original_matrix_type::rows > 0, "Matrix is empty");
    static constexpr auto rows = original_matrix_type::rows - 1;
    static constexpr auto cols = original_matrix_type::cols;
    using type                 = matrix<value_type, rows, cols, axes_names>;
};

template <typename Matrix>
using remove_row_result_t = typename remove_row_result<Matrix>::type;

template <typename Matrix, std::size_t RN>
struct remove_nth_row : matrix_expression<remove_nth_row<Matrix, RN>, remove_row_result_t<Matrix>>,
                        unary_expression<Matrix> {
    using base_type = matrix_expression<remove_nth_row<Matrix, RN>, remove_row_result_t<Matrix>>;
    using expression_base = unary_expression<Matrix>;
    using expression_base::expression_base;

    template <std::size_t R, std::size_t C>
    constexpr auto
    element() const
    {
        static_assert(R < base_type::rows, "Invalid matrix expression row index");
        static_assert(C < base_type::cols, "Invalid matrix expression col index");
        if constexpr (R < RN) {
            return this->arg_.template element<R, C>();
        } else {
            return this->arg_.template element<R + 1, C>();
        }
    }
};

template <std::size_t R, typename Matrix, typename = enable_if_matrix_expression<Matrix>>
constexpr auto
remove_row(Matrix&& mtx)
{
    return make_unary_expression<remove_nth_row, R>(std::forward<Matrix>(mtx));
}
//@}

//----------------------------------------------------------------------------
//@{
/** @name Remove Nth matrix column */
template <typename Matrix>
struct remove_col_result {
    using original_matrix_type = typename std::decay_t<Matrix>::matrix_type;
    using value_type           = typename original_matrix_type::value_type;
    using axes_names           = typename original_matrix_type::axes_names;
    static_assert(original_matrix_type::cols > 0, "Matrix is empty");
    static constexpr auto rows = original_matrix_type::rows;
    static constexpr auto cols = original_matrix_type::cols - 1;
    using type                 = matrix<value_type, rows, cols, axes_names>;
};

template <typename Matrix>
using remove_col_result_t = typename remove_col_result<Matrix>::type;

template <typename Matrix, std::size_t CN>
struct remove_nth_col : matrix_expression<remove_nth_col<Matrix, CN>, remove_col_result_t<Matrix>>,
                        unary_expression<Matrix> {
    using base_type = matrix_expression<remove_nth_col<Matrix, CN>, remove_col_result_t<Matrix>>;
    using expression_base = unary_expression<Matrix>;
    using expression_base::expression_base;

    template <std::size_t R, std::size_t C>
    constexpr auto
    element() const
    {
        static_assert(R < base_type::rows, "Invalid matrix expression row index");
        static_assert(C < base_type::cols, "Invalid matrix expression col index");
        if constexpr (C < CN) {
            return this->arg_.template element<R, C>();
        } else {
            return this->arg_.template element<R, C + 1>();
        }
    }
};

template <std::size_t C, typename Matrix, typename = enable_if_matrix_expression<Matrix>>
constexpr auto
remove_col(Matrix&& mtx)
{
    return make_unary_expression<remove_nth_col, C>(std::forward<Matrix>(mtx));
}
//@}

//@{
/** @name Matrix minor */
template <typename Matrix, std::size_t RN, std::size_t CN>
struct matrix_minor : matrix_expression<matrix_minor<Matrix, RN, CN>,
                                        remove_col_result_t<remove_row_result_t<Matrix>>>,
                      unary_expression<Matrix> {
    using base_type = matrix_expression<matrix_minor<Matrix, RN, CN>,
                                        remove_col_result_t<remove_row_result_t<Matrix>>>;

    using expression_base = unary_expression<Matrix>;
    using expression_base::expression_base;

    template <std::size_t R, std::size_t C>
    constexpr auto
    element() const
    {
        static_assert(R < base_type::rows, "Invalid matrix expression row index");
        static_assert(C < base_type::cols, "Invalid matrix expression col index");
        constexpr std::size_t row = R < RN ? R : R + 1;
        constexpr std::size_t col = C < CN ? C : C + 1;
        return this->arg_.template element<row, col>();
    }
};

template <std::size_t R, std::size_t C, typename Matrix,
          typename = enable_if_matrix_expression<Matrix>>
constexpr auto
minor(Matrix&& mtx)
{
    return make_unary_expression<matrix_minor, R, C>(std::forward<Matrix>(mtx));
}
//@}

//----------------------------------------------------------------------------
//@{
/** @name Compare two matrix expressions */
namespace detail {

template <std::size_t R, typename LHS, typename RHS>
struct matrix_expression_cmp;

template <typename LHS, typename RHS>
struct matrix_expression_cmp<0, LHS, RHS> {
    static_assert((is_matrix_expression_v<LHS> && is_matrix_expression_v<RHS>),
                  "Both sides to the comparison must be matrix expressions");
    using lhs_type = std::decay_t<LHS>;
    using rhs_type = std::decay_t<RHS>;
    constexpr static int
    cmp(lhs_type const& lhs, rhs_type const& rhs)
    {
        return v::cmp(row<0>(lhs), row<0>(rhs));
    }
};

template <std::size_t R, typename LHS, typename RHS>
struct matrix_expression_cmp {
    static_assert((is_matrix_expression_v<LHS> && is_matrix_expression_v<RHS>),
                  "Both sides to the comparison must be matrix expressions");
    using prev_element = matrix_expression_cmp<R - 1, LHS, RHS>;
    using lhs_type     = std::decay_t<LHS>;
    using rhs_type     = std::decay_t<RHS>;
    constexpr static int
    cmp(lhs_type const& lhs, rhs_type const& rhs)
    {
        auto res = prev_element::cmp(lhs, rhs);
        if (res == 0) {
            return v::cmp(row<R>(lhs), row<R>(rhs));
        }
        return res;
    }
};

}    // namespace detail

template <typename LHS, typename RHS>
struct matrix_cmp : binary_scalar_expression<matrix_cmp, LHS, RHS, int>,
                    binary_expression<LHS, RHS> {
    static_assert((is_matrix_expression_v<LHS> && is_matrix_expression_v<RHS>),
                  "Both sides to the comparison must be matrix expressions");
    using lhs_type                        = std::decay_t<LHS>;
    using rhs_type                        = std::decay_t<RHS>;
    static constexpr std::size_t cmp_size = utils::min_v<lhs_type::rows, rhs_type::rows>;
    using cmp_type                        = detail::matrix_expression_cmp<cmp_size - 1, LHS, RHS>;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    constexpr int
    value() const
    {
        return cmp_type::cmp(this->lhs_, this->rhs_);
    }
};

template <typename LHS, typename RHS,
          typename = std::enable_if_t<is_matrix_expression_v<LHS> && is_matrix_expression_v<RHS>>>
constexpr auto
cmp(LHS&& lhs, RHS&& rhs)
{
    return make_binary_expression<matrix_cmp>(std::forward<LHS>(lhs), std::forward<RHS>(rhs));
}
//@}

//----------------------------------------------------------------------------
//@{
/** @name Matrix equality */
template <typename LHS, typename RHS>
struct matrix_eq : binary_scalar_expression<matrix_eq, LHS, RHS, bool>,
                   binary_expression<LHS, RHS> {
    static_assert((is_matrix_expression_v<LHS> && is_matrix_expression_v<RHS>),
                  "Both sides to the comparison must be matrix expressions");
    static constexpr std::size_t cmp_size = utils::min_v<LHS::rows, RHS::rows>;
    using cmp_type                        = detail::matrix_expression_cmp<cmp_size - 1, LHS, RHS>;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    constexpr bool
    value() const
    {
        return cmp_type::cmp(this->lhs_, this->rhs_) == 0;
    }
};

template <typename LHS, typename RHS, typename = enable_if_matrix_expressions<LHS, RHS>>
constexpr auto
operator==(LHS&& lhs, RHS&& rhs)
{
    return make_binary_expression<matrix_eq>(std::forward<LHS>(lhs), std::forward<RHS>(rhs));
}

template <typename LHS, typename RHS, typename = enable_if_matrix_expressions<LHS, RHS>>
constexpr auto
operator!=(LHS&& lhs, RHS&& rhs)
{
    return !(std::forward<LHS>(lhs) == std::forward<RHS>(rhs));
}
//@}

//----------------------------------------------------------------------------
//@{
/** @name Matrix inequality */
template <typename LHS, typename RHS>
struct matrix_less : binary_scalar_expression<matrix_less, LHS, RHS, bool>,
                     binary_expression<LHS, RHS> {
    static_assert((is_matrix_expression_v<LHS> && is_matrix_expression_v<RHS>),
                  "Both sides to the comparison must be matrix expressions");
    static constexpr std::size_t cmp_size = utils::min_v<LHS::rows, RHS::rows>;
    using cmp_type                        = detail::matrix_expression_cmp<cmp_size - 1, LHS, RHS>;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    constexpr bool
    value() const
    {
        return cmp_type::cmp(this->lhs_, this->rhs_) < 0;
    }
};

template <typename LHS, typename RHS, typename = enable_if_matrix_expressions<LHS, RHS>>
constexpr auto
operator<(LHS&& lhs, RHS&& rhs)
{
    return make_binary_expression<matrix_less>(std::forward<LHS>(lhs), std::forward<RHS>(rhs));
}

template <typename LHS, typename RHS, typename = enable_if_matrix_expressions<LHS, RHS>>
constexpr auto
operator<=(LHS&& lhs, RHS&& rhs)
{
    return !(std::forward<RHS>(rhs) < std::forward<LHS>(lhs));
}

template <typename LHS, typename RHS, typename = enable_if_matrix_expressions<LHS, RHS>>
constexpr auto
operator>(LHS&& lhs, RHS&& rhs)
{
    return make_binary_expression<matrix_less>(std::forward<RHS>(rhs), std::forward<LHS>(lhs));
}

template <typename LHS, typename RHS, typename = enable_if_matrix_expressions<LHS, RHS>>
constexpr auto
operator>=(LHS&& lhs, RHS&& rhs)
{
    return !(std::forward<LHS>(lhs) < std::forward<RHS>(rhs));
}
//@}

//----------------------------------------------------------------------------
//@{
/** @name Matrix transposition */
/**
 * Matrix transposition is a "flip" around the main diagonal
 */
template <typename Expr>
struct matrix_transpose
    : matrix_expression<matrix_transpose<Expr>, typename std::decay_t<Expr>::transposed_type>,
      unary_expression<Expr> {

    using base_type
        = matrix_expression<matrix_transpose<Expr>, typename std::decay_t<Expr>::transposed_type>;

    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    template <std::size_t R, std::size_t C>
    constexpr auto
    element() const
    {
        static_assert(R < base_type::rows, "Invalid matrix expression row index");
        static_assert(C < base_type::cols, "Invalid matrix expression col index");
        return this->arg_.template element<C, R>();
    }
};

template <typename Expr, typename = enable_if_matrix_expression<Expr>>
constexpr auto
transpose(Expr&& expr)
{
    return make_unary_expression<matrix_transpose>(std::forward<Expr>(expr));
}
//@}

//@{
/** @name Flip matrix around the secondary diagonal */
template <typename Expr>
struct matrix_secondary_flip
    : matrix_expression<matrix_secondary_flip<Expr>, typename std::decay_t<Expr>::transposed_type>,
      unary_expression<Expr> {
    using base_type = matrix_expression<matrix_secondary_flip<Expr>,
                                        typename std::decay_t<Expr>::transposed_type>;

    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    template <std::size_t R, std::size_t C>
    constexpr auto
    element() const
    {
        static_assert(R < base_type::rows, "Invalid matrix expression row index");
        static_assert(C < base_type::cols, "Invalid matrix expression col index");
        return this->arg_.template element<base_type::cols - C - 1, base_type::rows - R - 1>();
    }
};

template <typename Expr, typename = enable_if_matrix_expression<Expr>>
constexpr auto
flip_secondary(Expr&& expr)
{
    return make_unary_expression<matrix_secondary_flip>(std::forward<Expr>(expr));
}
//@}

//@{
/** @name Flip matrix horizontally */
template <typename Expr>
struct matrix_horizontal_flip
    : matrix_expression<matrix_horizontal_flip<Expr>, typename std::decay_t<Expr>::matrix_type>,
      unary_expression<Expr> {
    using base_type
        = matrix_expression<matrix_horizontal_flip<Expr>, typename std::decay_t<Expr>::matrix_type>;

    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    template <std::size_t R, std::size_t C>
    constexpr auto
    element() const
    {
        static_assert(R < base_type::rows, "Invalid matrix expression row index");
        static_assert(C < base_type::cols, "Invalid matrix expression col index");
        return this->arg_.template element<R, base_type::cols - C - 1>();
    }
};

template <typename Expr, typename = enable_if_matrix_expression<Expr>>
constexpr auto
flip_horizontally(Expr&& expr)
{
    return make_unary_expression<matrix_horizontal_flip>(std::forward<Expr>(expr));
}
//@}

//@{
/** @name Flip matrix vertically */
template <typename Expr>
struct matrix_vertical_flip
    : matrix_expression<matrix_vertical_flip<Expr>, typename std::decay_t<Expr>::matrix_type>,
      unary_expression<Expr> {
    using base_type
        = matrix_expression<matrix_vertical_flip<Expr>, typename std::decay_t<Expr>::matrix_type>;

    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    template <std::size_t R, std::size_t C>
    constexpr auto
    element() const
    {
        static_assert(R < base_type::rows, "Invalid matrix expression row index");
        static_assert(C < base_type::cols, "Invalid matrix expression col index");
        return this->arg_.template element<base_type::rows - R - 1, C>();
    }
};

template <typename Expr, typename = enable_if_matrix_expression<Expr>>
constexpr auto
flip_vertically(Expr&& expr)
{
    return make_unary_expression<matrix_vertical_flip>(std::forward<Expr>(expr));
}
//@}

//@{
/** @name Rotate matrix clockwise */
template <typename Expr>
struct matrix_cw_rotation
    : matrix_expression<matrix_cw_rotation<Expr>, typename std::decay_t<Expr>::transposed_type>,
      unary_expression<Expr> {
    using base_type
        = matrix_expression<matrix_cw_rotation<Expr>, typename std::decay_t<Expr>::transposed_type>;

    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    template <std::size_t R, std::size_t C>
    constexpr auto
    element() const
    {
        static_assert(R < base_type::rows, "Invalid matrix expression row index");
        static_assert(C < base_type::cols, "Invalid matrix expression col index");
        return this->arg_.template element<base_type::cols - C - 1, R>();
    }
};

template <typename Expr, typename = enable_if_matrix_expression<Expr>>
constexpr auto
rotate_cw(Expr&& expr)
{
    return make_unary_expression<matrix_cw_rotation>(std::forward<Expr>(expr));
}
//@}

//@{
/** @name Rotate matrix counter-clockwise */
template <typename Expr>
struct matrix_ccw_rotation
    : matrix_expression<matrix_ccw_rotation<Expr>, typename std::decay_t<Expr>::transposed_type>,
      unary_expression<Expr> {
    using base_type = matrix_expression<matrix_ccw_rotation<Expr>,
                                        typename std::decay_t<Expr>::transposed_type>;

    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    template <std::size_t R, std::size_t C>
    constexpr auto
    element() const
    {
        static_assert(R < base_type::rows, "Invalid matrix expression row index");
        static_assert(C < base_type::cols, "Invalid matrix expression col index");
        return this->arg_.template element<C, base_type::rows - R - 1>();
    }
};

template <typename Expr, typename = enable_if_matrix_expression<Expr>>
constexpr auto
rotate_ccw(Expr&& expr)
{
    return make_unary_expression<matrix_ccw_rotation>(std::forward<Expr>(expr));
}
//@}

//@{
/** @name Flip matrix vertically */
template <typename Expr>
struct matrix_180_rotate
    : matrix_expression<matrix_180_rotate<Expr>, typename std::decay_t<Expr>::matrix_type>,
      unary_expression<Expr> {
    using base_type
        = matrix_expression<matrix_180_rotate<Expr>, typename std::decay_t<Expr>::matrix_type>;

    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    template <std::size_t R, std::size_t C>
    constexpr auto
    element() const
    {
        static_assert(R < base_type::rows, "Invalid matrix expression row index");
        static_assert(C < base_type::cols, "Invalid matrix expression col index");
        return this->arg_.template element<base_type::rows - R - 1, base_type::cols - C - 1>();
    }
};

template <typename Expr, typename = enable_if_matrix_expression<Expr>>
constexpr auto
rotate_180(Expr&& expr)
{
    return make_unary_expression<matrix_180_rotate>(std::forward<Expr>(expr));
}
//@}

//----------------------------------------------------------------------------
//@{
/** @name Matrices sum */
template <typename LHS, typename RHS>
struct matrix_sum_result {
    static_assert((is_matrix_expression_v<LHS> && is_matrix_expression_v<RHS>),
                  "Both sides to the expession must be matrix expressions");
    using lhs_type = std::decay_t<LHS>;
    using rhs_type = std::decay_t<RHS>;
    static_assert(lhs_type::cols == rhs_type::cols, "Matrices must be of equal size");
    static_assert(lhs_type::rows == rhs_type::rows, "Matrices must be of equal size");
    static_assert(
        (std::is_same<typename lhs_type::axes_names, typename rhs_type::axes_names>::value),
        "Matrices must have the same axes");
    using value_type = scalar_expression_result_t<lhs_type, rhs_type>;
    using type = matrix<value_type, lhs_type::rows, lhs_type::cols, typename lhs_type::axes_names>;
};
template <typename LHS, typename RHS>
using matrix_sum_result_t = typename matrix_sum_result<LHS, RHS>::type;

template <typename LHS, typename RHS>
struct matrix_sum : matrix_expression<matrix_sum<LHS, RHS>, matrix_sum_result_t<LHS, RHS>>,
                    binary_expression<LHS, RHS> {
    using base_type  = matrix_expression<matrix_sum<LHS, RHS>, matrix_sum_result_t<LHS, RHS>>;
    using value_type = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template <std::size_t R, std::size_t C>
    constexpr auto
    element() const
    {
        static_assert(R < base_type::rows, "Invalid matrix expression row index");
        static_assert(C < base_type::cols, "Invalid matrix expression col index");
        return this->lhs_.template element<R, C>() + this->rhs_.template element<R, C>();
    }
};

template <typename LHS, typename RHS, typename = enable_if_matrix_expressions<LHS, RHS>>
constexpr auto
operator+(LHS&& lhs, RHS&& rhs)
{
    return make_binary_expression<matrix_sum>(std::forward<LHS>(lhs), std::forward<RHS>(rhs));
}
//@}

//----------------------------------------------------------------------------
//@{
/** @name Matrix difference */
template <typename LHS, typename RHS>
struct matrix_diff : matrix_expression<matrix_diff<LHS, RHS>, matrix_sum_result_t<LHS, RHS>>,
                     binary_expression<LHS, RHS> {
    using base_type  = matrix_expression<matrix_diff<LHS, RHS>, matrix_sum_result_t<LHS, RHS>>;
    using value_type = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template <std::size_t R, std::size_t C>
    constexpr auto
    element() const
    {
        static_assert(R < base_type::rows, "Invalid matrix expression row index");
        static_assert(C < base_type::cols, "Invalid matrix expression col index");
        return this->lhs_.template element<R, C>() - this->rhs_.template element<R, C>();
    }
};

template <typename LHS, typename RHS, typename = enable_if_matrix_expressions<LHS, RHS>>
constexpr auto
operator-(LHS&& lhs, RHS&& rhs)
{
    return make_binary_expression<matrix_diff>(std::forward<LHS>(lhs), std::forward<RHS>(rhs));
}
//@}

//----------------------------------------------------------------------------
//@{
/** @name Matrix by scalar multiplication */
template <typename LHS, typename RHS>
struct matrix_scalar_mul_result {
    static_assert((is_matrix_expression_v<LHS>),
                  "Left side to the expession must be matrix expressions");
    static_assert(is_scalar_v<RHS>, "Right side to the expression must be a scalar expression");
    using lhs_type   = std::decay_t<LHS>;
    using rhs_type   = std::decay_t<RHS>;
    using value_type = scalar_expression_result_t<lhs_type, rhs_type>;
    using type = matrix<value_type, lhs_type::rows, lhs_type::cols, typename lhs_type::axes_names>;
};
template <typename LHS, typename RHS>
using matrix_scalar_mul_result_t = typename matrix_scalar_mul_result<LHS, RHS>::type;

template <typename LHS, typename RHS>
struct matrix_scalar_multiply
    : matrix_expression<matrix_scalar_multiply<LHS, RHS>, matrix_scalar_mul_result_t<LHS, RHS>>,
      binary_expression<LHS, RHS> {
    using base_type
        = matrix_expression<matrix_scalar_multiply<LHS, RHS>, matrix_scalar_mul_result_t<LHS, RHS>>;
    using value_type = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template <std::size_t R, std::size_t C>
    constexpr auto
    element() const
    {
        static_assert(R < base_type::rows, "Invalid matrix expression row index");
        static_assert(C < base_type::cols, "Invalid matrix expression col index");
        return this->lhs_.template element<R, C>() * this->rhs_;
    }
};
//@}

//----------------------------------------------------------------------------
//@{
/** @name Matrix by scalar division */
template <typename LHS, typename RHS>
struct matrix_scalar_divide
    : matrix_expression<matrix_scalar_divide<LHS, RHS>, matrix_scalar_mul_result_t<LHS, RHS>>,
      binary_expression<LHS, RHS> {
    using base_type
        = matrix_expression<matrix_scalar_divide<LHS, RHS>, matrix_scalar_mul_result_t<LHS, RHS>>;
    using value_type = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template <std::size_t R, std::size_t C>
    constexpr auto
    element() const
    {
        static_assert(R < base_type::rows, "Invalid matrix expression row index");
        static_assert(C < base_type::cols, "Invalid matrix expression col index");
        return this->lhs_.template element<R, C>() / this->rhs_;
    }
};

template <typename LHS, typename RHS,
          typename = std::enable_if_t<is_matrix_expression_v<LHS> && is_scalar_v<RHS>>>
constexpr auto
operator/(LHS&& lhs, RHS&& rhs)
{
    return s::detail::wrap_non_expression_args<matrix_scalar_divide>(std::forward<LHS>(lhs),
                                                                     std::forward<RHS>(rhs));
}
//@}

//----------------------------------------------------------------------------
//@{
/** @name Matrix-matrix multiplication */
template <typename LHS, typename RHS>
struct matrix_matrix_mul_result {
    static_assert((is_matrix_expression_v<LHS> && is_matrix_expression_v<RHS>),
                  "Both sides to the expession must be matrix expressions");
    using lhs_type = std::decay_t<LHS>;
    using rhs_type = std::decay_t<RHS>;
    static_assert(lhs_type::cols == rhs_type::rows,
                  "Left hand columns must be equal to right hand rows");
    static_assert((compatible_axes_v<LHS, RHS>), "Matrices must have the same axes");
    using value_type = scalar_expression_result_t<lhs_type, rhs_type>;
    using axes_names = axes_names_for_t<LHS, RHS>;
    using type       = matrix<value_type, lhs_type::rows, rhs_type::cols, axes_names>;
};

template <typename LHS, typename RHS>
using matrix_matrix_mul_result_t = typename matrix_matrix_mul_result<LHS, RHS>::type;

template <typename LHS, typename RHS>
struct matrix_matrix_multiply
    : matrix_expression<matrix_matrix_multiply<LHS, RHS>, matrix_matrix_mul_result_t<LHS, RHS>>,
      binary_expression<LHS, RHS> {

    using base_type
        = matrix_expression<matrix_matrix_multiply<LHS, RHS>, matrix_matrix_mul_result_t<LHS, RHS>>;
    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template <std::size_t R, std::size_t C>
    constexpr auto
    element() const
    {
        static_assert(R < base_type::rows, "Invalid matrix expression row index");
        static_assert(C < base_type::cols, "Invalid matrix expression col index");
        return dot_product(row<R>(this->lhs_), col<C>(this->rhs_));
    }
};

//----------------------------------------------------------------------------
template <typename LHS, typename RHS,
          typename
          = std::enable_if_t<(is_matrix_expression_v<LHS> && is_scalar_v<RHS>)
                             || (is_scalar_v<LHS> && is_matrix_expression_v<RHS>)
                             || (is_matrix_expression_v<LHS> && is_matrix_expression_v<RHS>)
                             || (is_matrix_expression_v<LHS> && is_vector_expression_v<RHS>)
                             || (is_vector_expression_v<LHS> && is_matrix_expression_v<RHS>)>>
constexpr auto operator*(LHS&& lhs, RHS&& rhs)
{
    if constexpr (is_matrix_expression_v<LHS> && is_scalar_v<RHS>) {
        return s::detail::wrap_non_expression_args<matrix_scalar_multiply>(std::forward<LHS>(lhs),
                                                                           std::forward<RHS>(rhs));
    } else if constexpr (is_scalar_v<LHS> && is_matrix_expression_v<RHS>) {
        return s::detail::wrap_non_expression_args<matrix_scalar_multiply>(std::forward<RHS>(rhs),
                                                                           std::forward<LHS>(lhs));
    } else if constexpr (is_matrix_expression_v<LHS> && is_matrix_expression_v<RHS>) {
        return make_binary_expression<matrix_matrix_multiply>(std::forward<LHS>(lhs),
                                                              std::forward<RHS>(rhs));
    } else if constexpr (is_matrix_expression_v<LHS> && is_vector_expression_v<RHS>) {
        return std::forward<LHS>(lhs) * as_col_matrix(std::forward<RHS>(rhs));
    } else if constexpr (is_vector_expression_v<LHS> && is_matrix_expression_v<RHS>) {
        return as_row_matrix(std::forward<LHS>(lhs)) * std::forward<RHS>(rhs);
    }
}
//@}

//@{
/** @name Matrix determinant */
template <typename Expr, typename = enable_if_matrix_expression<Expr>>
constexpr auto
det(Expr&& mtx);

template <typename Expr>
struct matrix_determinant
    : scalar_expression<matrix_determinant<Expr>, typename std::decay_t<Expr>::value_type>,
      unary_expression<Expr> {

    using base_type
        = scalar_expression<matrix_determinant<Expr>, typename std::decay_t<Expr>::value_type>;
    using matrix_type = typename std::decay_t<Expr>::matrix_type;
    static_assert(matrix_type::cols == matrix_type::rows,
                  "Identity matrix is defined only for square matrices");
    using value_type       = typename base_type::value_type;
    using col_indexes_type = typename matrix_type::col_indexes_type;

    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    constexpr auto
    value() const
    {
        if constexpr (matrix_type::rows > 1) {
            return sum(col_indexes_type{});
        } else if constexpr (matrix_type::rows == 1) {
            return this->arg_.template element<0, 0>();
        } else {
            return value_type{};
        }
    }

private:
    template <std::size_t N>
    constexpr auto
    nth_element() const
    {
        if constexpr (N % 2 == 0) {
            return this->arg_.template element<0, N>() * det(minor<0, N>(this->arg_));
        } else {
            return s::product(this->arg_.template element<0, N>(), det(minor<0, N>(this->arg_)),
                              -1);
        }
    }
    template <std::size_t... CI>
    constexpr auto
    sum(std::index_sequence<CI...>) const
    {
        return s::sum(this->template nth_element<CI>()...);
    }
};

template <typename Expr, typename>
constexpr auto
det(Expr&& expr)
{
    return make_unary_expression<matrix_determinant>(std::forward<Expr>(expr));
}
//@}

}    // namespace m

}    // namespace expr
}    // namespace math
}    // namespace psst

#endif /* PUSHKIN_MATH_DETAIL_MATRIX_EXPRESSIONS_HPP_ */
