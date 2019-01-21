/*
 * matrix_benchmarks.cpp
 *
 *  Created on: Dec 14, 2016
 *      Author: zmij
 */

#include "make_test_data.hpp"
#include <psst/math/matrix.hpp>
#include <psst/math/matrix_io.hpp>
#include <psst/math/vector.hpp>
#include <psst/math/vector_io.hpp>

#include <benchmark/benchmark.h>

namespace psst {
namespace math {
namespace bench {

//----------------------------------------------------------------------------
//  Benchmarks
//----------------------------------------------------------------------------
template <typename Matrix>
void
MatrixEq(benchmark::State& state)
{
    using traits_type = matrix_traits<Matrix>;
    while (state.KeepRunning()) {
        Matrix m1
            = make_test_matrix<typename traits_type::value_type>(typename traits_type::size_type{});
        Matrix m2
            = make_test_matrix<typename traits_type::value_type>(typename traits_type::size_type{});
        benchmark::DoNotOptimize(m1 == m2);
    }
    state.SetComplexityN(traits_type::size);
}
template <typename Matrix>
void
MatrixCmp(benchmark::State& state)
{
    using traits_type = matrix_traits<Matrix>;
    while (state.KeepRunning()) {
        Matrix m1
            = make_test_matrix<typename traits_type::value_type>(typename traits_type::size_type{});
        Matrix m2
            = make_test_matrix<typename traits_type::value_type>(typename traits_type::size_type{});
        benchmark::DoNotOptimize(m1 < m2);
    }
    state.SetComplexityN(traits_type::size);
}
template <typename Matrix>
void
MatrixAdd(benchmark::State& state)
{
    using traits_type = matrix_traits<Matrix>;
    while (state.KeepRunning()) {
        Matrix m1
            = make_test_matrix<typename traits_type::value_type>(typename traits_type::size_type{});
        benchmark::DoNotOptimize(m1 += m1);
    }
    state.SetComplexityN(traits_type::size);
}
template <typename Matrix>
void
MatrixSub(benchmark::State& state)
{
    using traits_type = matrix_traits<Matrix>;
    while (state.KeepRunning()) {
        Matrix m1
            = make_test_matrix<typename traits_type::value_type>(typename traits_type::size_type{});
        benchmark::DoNotOptimize(m1 -= m1);
    }
    state.SetComplexityN(traits_type::size);
}
template <typename Matrix>
void
MatrixNeg(benchmark::State& state)
{
    using traits_type = matrix_traits<Matrix>;
    while (state.KeepRunning()) {
        Matrix m1
            = make_test_matrix<typename traits_type::value_type>(typename traits_type::size_type{});
        benchmark::DoNotOptimize(m1 = -m1);
    }
    state.SetComplexityN(traits_type::size);
}
template <typename Matrix>
void
MatrixScalarMul(benchmark::State& state)
{
    using traits_type = matrix_traits<Matrix>;
    while (state.KeepRunning()) {
        Matrix m1
            = make_test_matrix<typename traits_type::value_type>(typename traits_type::size_type{});
        benchmark::DoNotOptimize(m1 *= 100500);
    }
    state.SetComplexityN(traits_type::size);
}
template <typename Matrix>
void
MatrixScalarDiv(benchmark::State& state)
{
    using traits_type = matrix_traits<Matrix>;
    while (state.KeepRunning()) {
        Matrix m1
            = make_test_matrix<typename traits_type::value_type>(typename traits_type::size_type{});
        benchmark::DoNotOptimize(m1 /= 100500);
    }
    state.SetComplexityN(traits_type::size);
}

template <typename Matrix>
void
MatrixTranspose(benchmark::State& state)
{
    using traits_type = matrix_traits<Matrix>;
    while (state.KeepRunning()) {
        Matrix m1
            = make_test_matrix<typename traits_type::value_type>(typename traits_type::size_type{});
        benchmark::DoNotOptimize(m1.transpose());
    }
    state.SetComplexityN(traits_type::size);
}

template <typename Matrix>
void
MatrixColMultiply(benchmark::State& state)
{
    using traits_type = matrix_traits<Matrix>;
    while (state.KeepRunning()) {
        Matrix m
            = make_test_matrix<typename traits_type::value_type>(typename traits_type::size_type{});
        auto v = make_test_vector<typename traits_type::value_type>(
            dimension_count<traits_type::rows>{});

        benchmark::DoNotOptimize(m * v);
    }
}
template <typename Matrix>
void
MatrixRowMultiply(benchmark::State& state)
{
    using traits_type = matrix_traits<Matrix>;
    while (state.KeepRunning()) {
        Matrix m
            = make_test_matrix<typename traits_type::value_type>(typename traits_type::size_type{});
        auto v = make_test_vector<typename traits_type::value_type>(
            dimension_count<traits_type::cols>{});

        benchmark::DoNotOptimize(v * m);
    }
}

template <typename LMatrix, typename RMatrix = LMatrix>
void
MatrixMultiply(benchmark::State& state)
{
    using left_traits  = matrix_traits<LMatrix>;
    using right_traits = matrix_traits<RMatrix>;
    while (state.KeepRunning()) {
        LMatrix lhs
            = make_test_matrix<typename left_traits::value_type>(typename left_traits::size_type{});
        RMatrix rhs = make_test_matrix<typename right_traits::value_type>(
            typename right_traits::size_type{});
        benchmark::DoNotOptimize(lhs * rhs);
    }
    state.SetComplexityN(left_traits::size * right_traits::size);
}

//----------------------------------------------------------------------------
// clang-format off
BENCHMARK_TEMPLATE(MatrixEq,                    matrix<float,   3, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixEq,                    matrix<double,  3, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixCmp,                   matrix<float,   3, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixCmp,                   matrix<double,  3, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixAdd,                   matrix<float,   3, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixAdd,                   matrix<double,  3, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixSub,                   matrix<float,   3, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixSub,                   matrix<double,  3, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixNeg,                   matrix<float,   3, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixNeg,                   matrix<double,  3, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixScalarMul,             matrix<float,   3, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixScalarMul,             matrix<double,  3, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixScalarDiv,             matrix<float,   3, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixScalarDiv,             matrix<double,  3, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixTranspose,             matrix<float,   3, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixTranspose,             matrix<double,  3, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixColMultiply,           matrix<float,   3, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixColMultiply,           matrix<double,  3, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixRowMultiply,           matrix<float,   3, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixRowMultiply,           matrix<double,  3, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixMultiply,              matrix<float,   3, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixMultiply,              matrix<double,  3, 3>)->Complexity();

BENCHMARK_TEMPLATE(MatrixEq,                    matrix<float,   4, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixEq,                    matrix<double,  4, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixCmp,                   matrix<float,   4, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixCmp,                   matrix<double,  4, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixAdd,                   matrix<float,   4, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixAdd,                   matrix<double,  4, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixSub,                   matrix<float,   4, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixSub,                   matrix<double,  4, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixNeg,                   matrix<float,   4, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixNeg,                   matrix<double,  4, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixScalarMul,             matrix<float,   4, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixScalarMul,             matrix<double,  4, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixScalarDiv,             matrix<float,   4, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixScalarDiv,             matrix<double,  4, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixTranspose,             matrix<float,   4, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixTranspose,             matrix<double,  4, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixColMultiply,           matrix<float,   4, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixColMultiply,           matrix<double,  4, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixRowMultiply,           matrix<float,   4, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixRowMultiply,           matrix<double,  4, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixMultiply,              matrix<float,   4, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixMultiply,              matrix<double,  4, 4>)->Complexity();

BENCHMARK_TEMPLATE(MatrixEq,                    matrix<float,   3, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixEq,                    matrix<double,  3, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixCmp,                   matrix<float,   3, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixCmp,                   matrix<double,  3, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixAdd,                   matrix<float,   3, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixAdd,                   matrix<double,  3, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixSub,                   matrix<float,   3, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixSub,                   matrix<double,  3, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixNeg,                   matrix<float,   3, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixNeg,                   matrix<double,  3, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixScalarMul,             matrix<float,   3, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixScalarMul,             matrix<double,  3, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixScalarDiv,             matrix<float,   3, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixScalarDiv,             matrix<double,  3, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixTranspose,             matrix<float,   3, 4>)->Complexity();
BENCHMARK_TEMPLATE(MatrixTranspose,             matrix<double,  3, 4>)->Complexity();

BENCHMARK_TEMPLATE(MatrixEq,                    matrix<float,   4, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixEq,                    matrix<double,  4, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixCmp,                   matrix<float,   4, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixCmp,                   matrix<double,  4, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixAdd,                   matrix<float,   4, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixAdd,                   matrix<double,  4, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixSub,                   matrix<float,   4, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixSub,                   matrix<double,  4, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixNeg,                   matrix<float,   4, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixNeg,                   matrix<double,  4, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixScalarMul,             matrix<float,   4, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixScalarMul,             matrix<double,  4, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixScalarDiv,             matrix<float,   4, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixScalarDiv,             matrix<double,  4, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixTranspose,             matrix<float,   4, 3>)->Complexity();
BENCHMARK_TEMPLATE(MatrixTranspose,             matrix<double,  4, 3>)->Complexity();

BENCHMARK_TEMPLATE(MatrixMultiply,              matrix<float,   3, 4>, matrix<float,  4, 3>);
BENCHMARK_TEMPLATE(MatrixMultiply,              matrix<double,  3, 4>, matrix<double, 4, 3>);
BENCHMARK_TEMPLATE(MatrixMultiply,              matrix<float,   4, 3>, matrix<float,  3, 4>);
BENCHMARK_TEMPLATE(MatrixMultiply,              matrix<double,  4, 3>, matrix<double, 3, 4>);

BENCHMARK_TEMPLATE(MatrixEq,                    matrix<float,   10, 10>)->Complexity();
BENCHMARK_TEMPLATE(MatrixCmp,                   matrix<float,   10, 10>)->Complexity();
BENCHMARK_TEMPLATE(MatrixAdd,                   matrix<float,   10, 10>)->Complexity();
BENCHMARK_TEMPLATE(MatrixSub,                   matrix<float,   10, 10>)->Complexity();
BENCHMARK_TEMPLATE(MatrixNeg,                   matrix<float,   10, 10>)->Complexity();
BENCHMARK_TEMPLATE(MatrixScalarMul,             matrix<float,   10, 10>)->Complexity();
BENCHMARK_TEMPLATE(MatrixScalarDiv,             matrix<float,   10, 10>)->Complexity();
BENCHMARK_TEMPLATE(MatrixTranspose,             matrix<float,   10, 10>)->Complexity();
BENCHMARK_TEMPLATE(MatrixColMultiply,           matrix<float,   10, 10>)->Complexity();
BENCHMARK_TEMPLATE(MatrixRowMultiply,           matrix<float,   10, 10>)->Complexity();
BENCHMARK_TEMPLATE(MatrixMultiply,              matrix<float,   10, 10>)->Complexity();
// clang-format on

} /* namespace bench */
} /* namespace math */
} /* namespace psst */
