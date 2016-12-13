/*
 * benchmarks.cpp
 *
 *  Created on: 13 дек. 2016 г.
 *      Author: sergey.fedorov
 */

#include <benchmark/benchmark_api.h>
#include <math/vector.hpp>
#include <math/matrix.hpp>

#include <math/vector_io.hpp>
#include <math/matrix_io.hpp>

namespace psst {
namespace math {
namespace bench {

using vector3d = math::vector<double, 3>;
using vector3f = math::vector<float, 3>;
using vector4d = math::vector<double, 4>;
using vector4f = math::vector<float, 3>;

//----------------------------------------------------------------------------
//  Cmp ops
//----------------------------------------------------------------------------
template < typename V >
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
//  Vector3
//----------------------------------------------------------------------------
template < typename Vector >
void
Vector3Add( benchmark::State& state )
{
    Vector v1{1, 2, 3};
    Vector v2{3, 2, 1};

    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(v2 += v1);
    }
}
template < typename Vector >
void
Vector3Sub( benchmark::State& state )
{
    Vector v1{1, 2, 3};
    Vector v2{3, 2, 1};

    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(v2 -= v1);
    }
}
template < typename Vector >
void
Vector3ScalarMul( benchmark::State& state )
{
    while (state.KeepRunning()) {
        Vector v1{1, 2, 3};
        benchmark::DoNotOptimize(v1 *= 100500);
    }
}
template < typename Vector >
void
Vector3ScalarDiv( benchmark::State& state )
{
    while (state.KeepRunning()) {
        Vector v1{1, 2, 3};
        benchmark::DoNotOptimize(v1 /= 100500);
    }
}
template < typename Vector >
void
Vector3Dot( benchmark::State& state )
{
    Vector v1{1, 2, 3};
    Vector v2{3, 2, 1};

    while (state.KeepRunning()) {
        benchmark::DoNotOptimize( dot_product(v1, v2) );
    }
}
template < typename Vector >
void
Vector3Cross( benchmark::State& state )
{
    Vector v1{1, 2, 3};
    Vector v2{3, 2, 1};

    while (state.KeepRunning()) {
        benchmark::DoNotOptimize( cross(v1, v2) );
    }
}
template < typename Vector >
void
Vector3MagSQ( benchmark::State& state )
{
    while (state.KeepRunning()) {
        Vector v1{1, 2, 3};
        benchmark::DoNotOptimize(v1.magnitude_square());
    }
}
template < typename Vector >
void
Vector3Mag( benchmark::State& state )
{
    while (state.KeepRunning()) {
        Vector v1{1, 2, 3};
        benchmark::DoNotOptimize(v1.magnitude());
    }
}
template < typename Vector >
void
Vector3Norm( benchmark::State& state )
{
    while (state.KeepRunning()) {
        Vector v1{1, 2, 3};
        benchmark::DoNotOptimize(v1.normalize());
    }
}


//----------------------------------------------------------------------------
//  Vector4
//----------------------------------------------------------------------------
template < typename Vector >
void
Vector4Add( benchmark::State& state )
{
    Vector v1{1, 2, 3, 4};
    Vector v2{4, 3, 2, 1};

    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(v2 += v1);
    }
}
template < typename Vector >
void
Vector4Sub( benchmark::State& state )
{
    Vector v1{1, 2, 3, 4};
    Vector v2{4, 3, 2, 1};

    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(v2 -= v1);
    }
}
template < typename Vector >
void
Vector4ScalarMul( benchmark::State& state )
{
    while (state.KeepRunning()) {
        Vector v1{1, 2, 3, 4};
        benchmark::DoNotOptimize(v1 *= 100500);
    }
}
template < typename Vector >
void
Vector4ScalarDiv( benchmark::State& state )
{
    while (state.KeepRunning()) {
        Vector v1{1, 2, 3, 4};
        benchmark::DoNotOptimize(v1 /= 100500);
    }
}
template < typename Vector >
void
Vector4Dot( benchmark::State& state )
{
    Vector v1{1, 2, 3, 4};
    Vector v2{4, 3, 2, 1};

    while (state.KeepRunning()) {
        benchmark::DoNotOptimize( dot_product(v1, v2) );
    }
}
template < typename Vector >
void
Vector4Cross( benchmark::State& state )
{
    Vector v1{1, 2, 3, 4};
    Vector v2{4, 3, 2, 1};

    while (state.KeepRunning()) {
        benchmark::DoNotOptimize( cross(v1, v2) );
    }
}
template < typename Vector >
void
Vector4MagSQ( benchmark::State& state )
{
    while (state.KeepRunning()) {
        Vector v1{1, 2, 3, 4};
        benchmark::DoNotOptimize(v1.magnitude_square());
    }
}
template < typename Vector >
void
Vector4Mag( benchmark::State& state )
{
    while (state.KeepRunning()) {
        Vector v1{1, 2, 3, 4};
        benchmark::DoNotOptimize(v1.magnitude());
    }
}
template < typename Vector >
void
Vector4Norm( benchmark::State& state )
{
    while (state.KeepRunning()) {
        Vector v1{1, 2, 3, 4};
        benchmark::DoNotOptimize(v1.normalize());
    }
}
//----------------------------------------------------------------------------
BENCHMARK_TEMPLATE(Compare,             float);
BENCHMARK_TEMPLATE(Compare,             double);

BENCHMARK_TEMPLATE(Vector3Add,          vector<float,   3>);
BENCHMARK_TEMPLATE(Vector3Add,          vector<double,  3>);
BENCHMARK_TEMPLATE(Vector3Sub,          vector<float,   3>);
BENCHMARK_TEMPLATE(Vector3Sub,          vector<double,  3>);
BENCHMARK_TEMPLATE(Vector3ScalarMul,    vector<float,   3>);
BENCHMARK_TEMPLATE(Vector3ScalarMul,    vector<double,  3>);
BENCHMARK_TEMPLATE(Vector3ScalarDiv,    vector<float,   3>);
BENCHMARK_TEMPLATE(Vector3ScalarDiv,    vector<double,  3>);
BENCHMARK_TEMPLATE(Vector3Dot,          vector<float,   3>);
BENCHMARK_TEMPLATE(Vector3Dot,          vector<double,  3>);
BENCHMARK_TEMPLATE(Vector3Cross,        vector<float,   3>);
BENCHMARK_TEMPLATE(Vector3Cross,        vector<double,  3>);
BENCHMARK_TEMPLATE(Vector3MagSQ,        vector<float,   3>);
BENCHMARK_TEMPLATE(Vector3MagSQ,        vector<double,  3>);
BENCHMARK_TEMPLATE(Vector3Mag,          vector<float,   3>);
BENCHMARK_TEMPLATE(Vector3Mag,          vector<double,  3>);
BENCHMARK_TEMPLATE(Vector3Norm,         vector<float,   3>);
BENCHMARK_TEMPLATE(Vector3Norm,         vector<double,  3>);

BENCHMARK_TEMPLATE(Vector4Add,          vector<float,   4>);
BENCHMARK_TEMPLATE(Vector4Add,          vector<double,  4>);
BENCHMARK_TEMPLATE(Vector4Sub,          vector<float,   4>);
BENCHMARK_TEMPLATE(Vector4Sub,          vector<double,  4>);
BENCHMARK_TEMPLATE(Vector4ScalarMul,    vector<float,   4>);
BENCHMARK_TEMPLATE(Vector4ScalarMul,    vector<double,  4>);
BENCHMARK_TEMPLATE(Vector4ScalarDiv,    vector<float,   4>);
BENCHMARK_TEMPLATE(Vector4ScalarDiv,    vector<double,  4>);
BENCHMARK_TEMPLATE(Vector4Dot,          vector<float,   4>);
BENCHMARK_TEMPLATE(Vector4Dot,          vector<double,  4>);
BENCHMARK_TEMPLATE(Vector4Cross,        vector<float,   4>);
BENCHMARK_TEMPLATE(Vector4Cross,        vector<double,  4>);
BENCHMARK_TEMPLATE(Vector4MagSQ,        vector<float,   4>);
BENCHMARK_TEMPLATE(Vector4MagSQ,        vector<double,  4>);
BENCHMARK_TEMPLATE(Vector4Mag,          vector<float,   4>);
BENCHMARK_TEMPLATE(Vector4Mag,          vector<double,  4>);
BENCHMARK_TEMPLATE(Vector4Norm,         vector<float,   4>);
BENCHMARK_TEMPLATE(Vector4Norm,         vector<double,  4>);

}  /* namespace bench */
}  /* namespace math */
}  /* namespace psst */

BENCHMARK_MAIN()
