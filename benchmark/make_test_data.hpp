/*
 * make_test_data.hpp
 *
 *  Created on: Dec 14, 2016
 *      Author: zmij
 */

#ifndef BENCHMARK_MAKE_TEST_DATA_HPP_
#define BENCHMARK_MAKE_TEST_DATA_HPP_

#include <psst/math/matrix.hpp>
#include <psst/math/vector.hpp>

namespace psst {
namespace math {
namespace bench {

template <std::size_t N>
using dimension_count = std::integral_constant<std::size_t, N>;

//----------------------------------------------------------------------------
//  Vector 3
//----------------------------------------------------------------------------
template <typename T>
constexpr vector<T, 3, components::xyzw> make_test_vector(dimension_count<3> const&)
{
    return {1, 2, 3};
}

//----------------------------------------------------------------------------
//  Vector 4
//----------------------------------------------------------------------------
template <typename T>
constexpr vector<T, 4, components::xyzw> make_test_vector(dimension_count<4> const&)
{
    return {1, 2, 3, 4};
}

//----------------------------------------------------------------------------
//  Vector 10
//----------------------------------------------------------------------------
template <typename T>
constexpr vector<T, 10, components::none> make_test_vector(dimension_count<10> const&)
{
    return {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
}

//----------------------------------------------------------------------------
//  Matrix 3x3
//----------------------------------------------------------------------------
template <typename T>
constexpr matrix<T, 3, 3> make_test_matrix(traits::matrix_size<3, 3> const&)
{
    // clang-format off
    return
    {
        { 11, 12, 13 },
        { 21, 22, 23 },
        { 31, 32, 33 }
    };
    // clang-format on
}

//----------------------------------------------------------------------------
//  Matrix 4x4
//----------------------------------------------------------------------------
template <typename T>
constexpr matrix<T, 4, 4> make_test_matrix(traits::matrix_size<4, 4> const&)
{
    // clang-format off
    return
    {
        { 11, 12, 13, 14 },
        { 21, 22, 23, 24 },
        { 31, 32, 33, 34 },
        { 41, 42, 43, 44 }
    };
    // clang-format on
}

//----------------------------------------------------------------------------
//  Matrix 3x4
//----------------------------------------------------------------------------
template <typename T>
constexpr matrix<T, 3, 4> make_test_matrix(traits::matrix_size<3, 4> const&)
{
    // clang-format off
    return
    {
        { 11, 12, 13, 14 },
        { 21, 22, 23, 24 },
        { 31, 32, 33, 34 }
    };
    // clang-format on
}

//----------------------------------------------------------------------------
//  Matrix 4x3
//----------------------------------------------------------------------------
template <typename T>
constexpr matrix<T, 4, 3> make_test_matrix(traits::matrix_size<4, 3> const&)
{
    // clang-format off
    return
    {
        { 11, 12, 13 },
        { 21, 22, 23 },
        { 31, 32, 33 },
        { 41, 42, 43 }
    };
    // clang-format on
}

//----------------------------------------------------------------------------
//  Matrix 10x10
//----------------------------------------------------------------------------
template <typename T>
constexpr matrix<T, 10, 10, components::none> make_test_matrix(traits::matrix_size<10, 10> const&)
{
    // clang-format off
    return
    {
        {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9 },
        { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 },
        { 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 },
        { 30, 31, 32, 33, 34, 35, 36, 37, 38, 39 },
        { 40, 41, 42, 43, 44, 45, 46, 47, 48, 49 },
        { 50, 51, 52, 53, 54, 55, 56, 57, 58, 59 },
        { 60, 61, 62, 63, 64, 65, 66, 67, 68, 69 },
        { 70, 71, 72, 73, 74, 75, 76, 77, 78, 79 },
        { 80, 81, 82, 83, 84, 85, 86, 87, 88, 89 },
        { 90, 91, 92, 93, 94, 95, 96, 97, 98, 99 },
    };
    // clang-format on
}

} /* namespace bench */
} /* namespace math */
} /* namespace psst */

#endif /* BENCHMARK_MAKE_TEST_DATA_HPP_ */
