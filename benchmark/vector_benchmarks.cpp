/*
 * benchmarks.cpp
 *
 *  Created on: 13 дек. 2016 г.
 *      Author: sergey.fedorov
 */

#include "make_test_data.hpp"
#include <pushkin/math/matrix.hpp>
#include <pushkin/math/matrix_io.hpp>
#include <pushkin/math/vector.hpp>
#include <pushkin/math/vector_io.hpp>

#include <benchmark/benchmark.h>

namespace psst {
namespace math {
namespace bench {

//----------------------------------------------------------------------------
//  Cmp ops
//----------------------------------------------------------------------------
template <typename V>
void
Compare(benchmark::State& state)
{
    V lhs{0};
    V rhs{0};
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(lhs == rhs);
    }
}

//----------------------------------------------------------------------------
//  Vector
//----------------------------------------------------------------------------
template <typename Vector>
void
VectorEq(benchmark::State& state)
{
    while (state.KeepRunning()) {
        auto v1 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});
        auto v2 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});
        benchmark::DoNotOptimize(v1 == v2);
    }
}
template <typename Vector>
void
VectorCmp(benchmark::State& state)
{
    while (state.KeepRunning()) {
        auto v1 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});
        auto v2 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});
        benchmark::DoNotOptimize(v1 < v2);
    }
}
template <typename Vector>
void
VectorAdd(benchmark::State& state)
{
    auto v1 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});
    auto v2 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});

    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(v2 += v1);
    }
}
template <typename Vector>
void
VectorAddAssign(benchmark::State& state)
{
    auto v1 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});
    auto v2 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});

    while (state.KeepRunning()) {
        decltype(v1) v3;
        benchmark::DoNotOptimize(v3 = v1 + v2);
    }
}
template <typename Vector>
void
VectorSum(benchmark::State& state)
{
    auto v1 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});
    auto v2 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});
    auto v3 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});

    while (state.KeepRunning()) {
        decltype(v1) v4;
        benchmark::DoNotOptimize(v4 = v1 + v2 + v3);
    }
}
template <typename Vector>
void
VectorSub(benchmark::State& state)
{
    auto v1 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});
    auto v2 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});

    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(v2 -= v1);
    }
}
template <typename Vector>
void
VectorSubAssign(benchmark::State& state)
{
    auto v1 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});
    auto v2 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});

    while (state.KeepRunning()) {
        decltype(v1) v3;
        benchmark::DoNotOptimize(v3 = v1 - v2);
    }
}
template <typename Vector>
void
VectorScalarMul(benchmark::State& state)
{
    while (state.KeepRunning()) {
        auto v1 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});
        benchmark::DoNotOptimize(v1 *= 100500);
    }
}
template <typename Vector>
void
VectorScalarDiv(benchmark::State& state)
{
    while (state.KeepRunning()) {
        auto v1 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});
        benchmark::DoNotOptimize(v1 /= 100500);
    }
}
template <typename Vector>
void
VectorDot(benchmark::State& state)
{
    auto v1 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});
    auto v2 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});

    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(dot_product(v1, v2));
    }
}
template <typename Vector>
void
VectorCross(benchmark::State& state)
{
    auto v1 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});
    auto v2 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});

    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(cross(v1, v2));
    }
}
template <typename Vector>
void
VectorMagSQ(benchmark::State& state)
{
    while (state.KeepRunning()) {
        auto v1 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});
        benchmark::DoNotOptimize(v1.magnitude_square());
    }
}
template <typename Vector>
void
VectorMag(benchmark::State& state)
{
    while (state.KeepRunning()) {
        auto v1 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});
        benchmark::DoNotOptimize(v1.magnitude());
    }
}
template <typename Vector>
void
VectorNorm(benchmark::State& state)
{
    while (state.KeepRunning()) {
        auto v1 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});
        benchmark::DoNotOptimize(v1.normalize());
    }
}

template <typename Vector>
void
VectorLerp(benchmark::State& state)
{
    while (state.KeepRunning()) {
        auto v1 = make_test_vector<typename Vector::value_type>(dimension_count<Vector::size>{});
        decltype(v1) v2 = v1 * 2;
        benchmark::DoNotOptimize(lerp(v1, v2, 0.5));
    }
}

template <typename Vector>
void
VectorSlerp(benchmark::State& state)
{
    using value_type = typename Vector::value_type;
    while (state.KeepRunning()) {
        auto         v1 = make_test_vector<value_type>(dimension_count<Vector::size>{});
        decltype(v1) v2 = v1 * 2;
        benchmark::DoNotOptimize(slerp(v1, v2, value_type{0.5}));
    }
}

//----------------------------------------------------------------------------
// clang-format off
BENCHMARK_TEMPLATE(Compare,             float);
BENCHMARK_TEMPLATE(Compare,             double);

BENCHMARK_TEMPLATE(VectorEq,            vector<float,   3>);
BENCHMARK_TEMPLATE(VectorEq,            vector<double,  3>);
BENCHMARK_TEMPLATE(VectorCmp,           vector<float,   3>);
BENCHMARK_TEMPLATE(VectorCmp,           vector<double,  3>);
BENCHMARK_TEMPLATE(VectorAdd,           vector<float,   3>);
BENCHMARK_TEMPLATE(VectorAdd,           vector<double,  3>);
BENCHMARK_TEMPLATE(VectorAddAssign,     vector<float,   3>);
BENCHMARK_TEMPLATE(VectorAddAssign,     vector<double,  3>);
BENCHMARK_TEMPLATE(VectorSum,           vector<float,   3>);
BENCHMARK_TEMPLATE(VectorSum,           vector<double,  3>);
BENCHMARK_TEMPLATE(VectorSub,           vector<float,   3>);
BENCHMARK_TEMPLATE(VectorSub,           vector<double,  3>);
BENCHMARK_TEMPLATE(VectorSubAssign,     vector<float,   3>);
BENCHMARK_TEMPLATE(VectorSubAssign,     vector<double,  3>);
BENCHMARK_TEMPLATE(VectorScalarMul,     vector<float,   3>);
BENCHMARK_TEMPLATE(VectorScalarMul,     vector<double,  3>);
BENCHMARK_TEMPLATE(VectorScalarDiv,     vector<float,   3>);
BENCHMARK_TEMPLATE(VectorScalarDiv,     vector<double,  3>);
BENCHMARK_TEMPLATE(VectorDot,           vector<float,   3>);
BENCHMARK_TEMPLATE(VectorDot,           vector<double,  3>);
BENCHMARK_TEMPLATE(VectorCross,         vector<float,   3>);
BENCHMARK_TEMPLATE(VectorCross,         vector<double,  3>);
BENCHMARK_TEMPLATE(VectorMagSQ,         vector<float,   3>);
BENCHMARK_TEMPLATE(VectorMagSQ,         vector<double,  3>);
BENCHMARK_TEMPLATE(VectorMag,           vector<float,   3>);
BENCHMARK_TEMPLATE(VectorMag,           vector<double,  3>);
BENCHMARK_TEMPLATE(VectorNorm,          vector<float,   3>);
BENCHMARK_TEMPLATE(VectorNorm,          vector<double,  3>);
BENCHMARK_TEMPLATE(VectorLerp,          vector<float,   3>);
BENCHMARK_TEMPLATE(VectorLerp,          vector<double,  3>);
BENCHMARK_TEMPLATE(VectorSlerp,         vector<float,   3>);
BENCHMARK_TEMPLATE(VectorSlerp,         vector<double,  3>);

BENCHMARK_TEMPLATE(VectorEq,            vector<float,   4>);
BENCHMARK_TEMPLATE(VectorEq,            vector<double,  4>);
BENCHMARK_TEMPLATE(VectorCmp,           vector<float,   4>);
BENCHMARK_TEMPLATE(VectorCmp,           vector<double,  4>);
BENCHMARK_TEMPLATE(VectorAdd,           vector<float,   4>);
BENCHMARK_TEMPLATE(VectorAdd,           vector<double,  4>);
BENCHMARK_TEMPLATE(VectorAddAssign,     vector<float,   4>);
BENCHMARK_TEMPLATE(VectorAddAssign,     vector<double,  4>);
BENCHMARK_TEMPLATE(VectorSum,           vector<float,   4>);
BENCHMARK_TEMPLATE(VectorSum,           vector<double,  4>);
BENCHMARK_TEMPLATE(VectorSub,           vector<float,   4>);
BENCHMARK_TEMPLATE(VectorSub,           vector<double,  4>);
BENCHMARK_TEMPLATE(VectorSubAssign,     vector<float,   4>);
BENCHMARK_TEMPLATE(VectorSubAssign,     vector<double,  4>);
BENCHMARK_TEMPLATE(VectorScalarMul,     vector<float,   4>);
BENCHMARK_TEMPLATE(VectorScalarMul,     vector<double,  4>);
BENCHMARK_TEMPLATE(VectorScalarDiv,     vector<float,   4>);
BENCHMARK_TEMPLATE(VectorScalarDiv,     vector<double,  4>);
BENCHMARK_TEMPLATE(VectorDot,           vector<float,   4>);
BENCHMARK_TEMPLATE(VectorDot,           vector<double,  4>);
BENCHMARK_TEMPLATE(VectorCross,         vector<float,   4>);
BENCHMARK_TEMPLATE(VectorCross,         vector<double,  4>);
BENCHMARK_TEMPLATE(VectorMagSQ,         vector<float,   4>);
BENCHMARK_TEMPLATE(VectorMagSQ,         vector<double,  4>);
BENCHMARK_TEMPLATE(VectorMag,           vector<float,   4>);
BENCHMARK_TEMPLATE(VectorMag,           vector<double,  4>);
BENCHMARK_TEMPLATE(VectorNorm,          vector<float,   4>);
BENCHMARK_TEMPLATE(VectorNorm,          vector<double,  4>);
BENCHMARK_TEMPLATE(VectorLerp,          vector<float,   4>);
BENCHMARK_TEMPLATE(VectorLerp,          vector<double,  4>);
BENCHMARK_TEMPLATE(VectorSlerp,         vector<float,   4>);
BENCHMARK_TEMPLATE(VectorSlerp,         vector<double,  4>);

BENCHMARK_TEMPLATE(VectorEq,            vector<float,   10>);
BENCHMARK_TEMPLATE(VectorCmp,           vector<float,   10>);
BENCHMARK_TEMPLATE(VectorAdd,           vector<float,   10>);
BENCHMARK_TEMPLATE(VectorAddAssign,     vector<float,   10>);
BENCHMARK_TEMPLATE(VectorSum,           vector<float,   10>);
BENCHMARK_TEMPLATE(VectorSub,           vector<float,   10>);
BENCHMARK_TEMPLATE(VectorSubAssign,     vector<float,   10>);
BENCHMARK_TEMPLATE(VectorScalarMul,     vector<float,   10>);
BENCHMARK_TEMPLATE(VectorScalarDiv,     vector<float,   10>);
BENCHMARK_TEMPLATE(VectorDot,           vector<float,   10>);
BENCHMARK_TEMPLATE(VectorMagSQ,         vector<float,   10>);
BENCHMARK_TEMPLATE(VectorMag,           vector<float,   10>);
BENCHMARK_TEMPLATE(VectorNorm,          vector<float,   10>);
// clang-format on

} /* namespace bench */
} /* namespace math */
} /* namespace psst */

BENCHMARK_MAIN();
