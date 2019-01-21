/**
 * Copyright 2019 Sergei A. Fedorov
 * random_tests.cpp
 *
 *  Created on: Jan 10, 2019
 *      Author: ser-fedorov
 */

#include "test_printing.hpp"
#include <psst/math/random.hpp>
#include <psst/math/vector.hpp>

#include <gtest/gtest.h>

#include <sstream>

namespace psst {
namespace math {
namespace test {

using vector3d = vector<double, 3>;
using vector3f = vector<float, 3>;
using vector4d = vector<double, 4>;
using vector4f = vector<float, 4>;

using matrix3x3d = matrix<double, 3, 3>;
using matrix3x3f = matrix<float, 3, 3>;
using matrix4x3d = matrix<double, 4, 3>;
using matrix4x3f = matrix<float, 4, 3>;
using matrix3x4d = matrix<double, 3, 4>;
using matrix3x4f = matrix<float, 3, 4>;

TEST(Vector, RandomVector)
{
    auto gen_d = random_vector_data<double>(std::uniform_real_distribution<double>{0, 1});
    auto gen_f = random_vector_data<float>(std::normal_distribution<float>{0, 1});

    vector3d v1 = gen_d;
    std::cout << "Random vector " << v1 << "\n";
    EXPECT_NE(0, magnitude_square(v1));
    v1 = gen_d;
    std::cout << "Random vector " << v1 << "\n";
    EXPECT_NE(0, magnitude_square(v1));

    vector4d v2 = gen_d;
    std::cout << "Random vector " << v2 << "\n";
    EXPECT_NE(0, magnitude_square(v2));

    vector3f v3 = gen_f;
    std::cout << "Random vector " << v3 << "\n";
    EXPECT_NE(0, magnitude_square(v3));

    vector4f v4 = gen_f;
    std::cout << "Random vector " << v4 << "\n";
    EXPECT_NE(0, magnitude_square(v4));
}

TEST(Matrix, RandomMatrix)
{
    std::cout << io::pretty;
    auto gen_d = random_matrix_data<double>(std::uniform_real_distribution<double>{0, 1});
    auto gen_f = random_matrix_data<float>(std::normal_distribution<float>{0, 1});

    matrix3x3d m1 = gen_d;
    std::cout << "Random matrix " << m1 << "\n";

    matrix3x3f m2 = gen_f;
    std::cout << "Random matrix " << m2 << "\n";

    std::cout << io::ugly;
}

}    // namespace test
}    // namespace math
}    // namespace psst
