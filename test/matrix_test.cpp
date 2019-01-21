/*
 * matrix_test.cpp
 *
 *  Created on: Dec 21, 2018
 *      Author: ser-fedorov
 */

#include "test_printing.hpp"
#include <psst/math/matrix.hpp>
#include <psst/math/vector.hpp>

#include <gtest/gtest.h>

#include <iostream>

namespace psst {
namespace math {
namespace test {

using vector3d  = vector<double, 3>;
using vector3df = vector<float, 3>;
using matrix3x3 = matrix<double, 3, 3>;
using matrix2x2 = matrix<double, 2, 2>;
using matrix3x4 = matrix<double, 3, 4>;
using matrix4x3 = matrix<double, 4, 3>;

TEST(Matrix, ConstructDefault)
{
    matrix3x3 m1;
    EXPECT_EQ(sizeof(double) * 9, sizeof(m1));
    for (auto const& v : m1) {
        EXPECT_EQ(0, v);
    }
}

TEST(Matrix, ConstructInitList)
{
    // clang-format off
    matrix3x3 m1 {
        { 11, 12, 13 },
        { 21, 22, 23 },
        { 31, 32, 33 }
    };
    // clang-format on

    EXPECT_EQ(11, m1[0][0]);
    EXPECT_EQ(12, m1[0][1]);
    EXPECT_EQ(13, m1[0][2]);
    EXPECT_EQ(11, m1[0].x());
    EXPECT_EQ(12, m1[0].y());
    EXPECT_EQ(13, m1[0].z());
    //    EXPECT_EQ(11, m1.x().x());
    //    EXPECT_EQ(12, m1.x().y());
    //    EXPECT_EQ(13, m1.x().z());

    EXPECT_EQ(21, m1[1][0]);
    EXPECT_EQ(22, m1[1][1]);
    EXPECT_EQ(23, m1[1][2]);
    EXPECT_EQ(21, m1[1].x());
    EXPECT_EQ(22, m1[1].y());
    EXPECT_EQ(23, m1[1].z());
    //    EXPECT_EQ(21, m1.y().x());
    //    EXPECT_EQ(22, m1.y().y());
    //    EXPECT_EQ(23, m1.y().z());

    EXPECT_EQ(31, m1[2][0]);
    EXPECT_EQ(32, m1[2][1]);
    EXPECT_EQ(33, m1[2][2]);
    EXPECT_EQ(31, m1[2].x());
    EXPECT_EQ(32, m1[2].y());
    EXPECT_EQ(33, m1[2].z());
    //    EXPECT_EQ(31, m1.z().x());
    //    EXPECT_EQ(32, m1.z().y());
    //    EXPECT_EQ(33, m1.z().z());
}

TEST(Matrix, ConstructPlainArray)
{
    // clang-format off
    double vals[]{
        11, 12, 13,
        21, 22, 23,
        31, 32, 33
    };
    // clang-format on

    matrix3x3 m1{vals};
    EXPECT_EQ(11, m1[0][0]);
    EXPECT_EQ(12, m1[0][1]);
    EXPECT_EQ(13, m1[0][2]);

    EXPECT_EQ(21, m1[1][0]);
    EXPECT_EQ(22, m1[1][1]);
    EXPECT_EQ(23, m1[1][2]);

    EXPECT_EQ(31, m1[2][0]);
    EXPECT_EQ(32, m1[2][1]);
    EXPECT_EQ(33, m1[2][2]);
}

TEST(Matrix, Construct2DimArray)
{
    // clang-format off
    double vals[][3]{
        { 11, 12, 13 },
        { 21, 22, 23 },
        { 31, 32, 33 }
    };
    // clang-format on
    matrix3x3 m1{vals};
    EXPECT_EQ(11, m1[0][0]);
    EXPECT_EQ(12, m1[0][1]);
    EXPECT_EQ(13, m1[0][2]);

    EXPECT_EQ(21, m1[1][0]);
    EXPECT_EQ(22, m1[1][1]);
    EXPECT_EQ(23, m1[1][2]);

    EXPECT_EQ(31, m1[2][0]);
    EXPECT_EQ(32, m1[2][1]);
    EXPECT_EQ(33, m1[2][2]);
}

TEST(Matrix, ConstructSingleValue)
{
    matrix3x3 m1{42.0};
    EXPECT_EQ(42, m1[0][0]);
    EXPECT_EQ(42, m1[0][1]);
    EXPECT_EQ(42, m1[0][2]);

    EXPECT_EQ(42, m1[1][0]);
    EXPECT_EQ(42, m1[1][1]);
    EXPECT_EQ(42, m1[1][2]);

    EXPECT_EQ(42, m1[2][0]);
    EXPECT_EQ(42, m1[2][1]);
    EXPECT_EQ(42, m1[2][2]);
}

TEST(Matrix, IO)
{
    // clang-format off
    matrix3x3 m1 {
       { 1.1, 1.2, 1.3 },
       { 2.1, 2.2, 3.3 },
       { 3.1, 3.2, 3.3 }
    };
    // clang-format on

    matrix3x3 m2;

    std::ostringstream os;
    os << m1;
    std::istringstream is(os.str());
    is >> m2;
    EXPECT_TRUE(is.good());
    EXPECT_EQ(m1, m2) << "Failed to read value from " << os.str();

    os.str("");
    os << io::pretty << m1 << io::ugly;
    m2 = matrix3x3::identity();

    EXPECT_NE(m1, m2);
    is.str(os.str());
    is >> m2;
    EXPECT_TRUE(is.good());
    EXPECT_EQ(m1, m2) << "Failed to read value from " << os.str();
}

TEST(Matrix, Iteration)
{
    // clang-format off
    matrix3x3 m1 {
        { 11, 12, 13 },
        { 21, 22, 23 },
        { 31, 32, 33 }
    };
    // clang-format on
    auto it = m1.begin();
    auto e  = m1.end();
    for (int i = 1; i < 4 && it != e; ++i) {
        for (int j = 1; j < 4 && it != e; ++j) {
            EXPECT_EQ(i * 10 + j, *it);
            ++it;
        }
    }
}

TEST(Matrix, Add)
{
    // clang-format off
    matrix3x3 m1{
        { 11, 12, 13 },
        { 21, 22, 23 },
        { 31, 32, 33 }
    };
    matrix3x3 m2{
        { 11, 12, 13 },
        { 21, 22, 23 },
        { 31, 32, 33 }
    };
    EXPECT_EQ(
    (matrix3x3{
        { 22, 24, 26 },
        { 42, 44, 46 },
        { 62, 64, 66 }
    }), m1 + m2);
    // clang-format on
    EXPECT_EQ(matrix3x3{}, m1 - m2);
}

TEST(Matrix, ScalarMultiply)
{
    // clang-format off
    matrix3x3 initial{
        { 11, 12, 13 },
        { 21, 22, 23 },
        { 31, 32, 33 }
    };
    matrix3x3 expected{
        { 22, 24, 26 },
        { 42, 44, 46 },
        { 62, 64, 66 }
    };
    // clang-format on
    EXPECT_EQ(expected, initial * 2) << "Not equal " << expected << " and " << initial * 2;
    EXPECT_EQ(0, cmp(expected, initial * 2));
    EXPECT_EQ(0, cmp(expr::row<0>(expected), expr::row<0>(initial * 2)))
        << "Not equal " << expr::row<0>(expected) << " and " << expr::row<0>(initial * 2);
    EXPECT_EQ(0, cmp(expr::row<1>(expected), expr::row<1>(initial * 2)))
        << "Not equal " << expr::row<1>(expected) << " and " << expr::row<1>(initial * 2);
    EXPECT_EQ(0, cmp(expr::row<2>(expected), expr::row<2>(initial * 2)))
        << "Not equal " << expr::row<2>(expected) << " and " << expr::row<2>(initial * 2);

    EXPECT_EQ(initial, expected / 2) << "Not equal " << initial << " and " << expected / 2;
    EXPECT_EQ(0, cmp(initial, expected / 2));
}

TEST(Matrix, VectorMultiply)
{
    // clang-format off
    matrix3x3 initial{
        { 11, 12, 13 },
        { 21, 22, 23 },
        { 31, 32, 33 }
    };
    // clang-format on
    vector3d expected{74, 134, 194};
    vector3d vec{1, 2, 3};
    EXPECT_EQ(as_col_matrix(expected), initial * vec)
        << "Not equal " << as_col_matrix(expected) << " and " << initial * vec;
}

TEST(Matrix, MatrixMultiply)
{
    // clang-format off
    matrix3x3 initial{
        { 11, 12, 13 },
        { 21, 22, 23 },
        { 31, 32, 33 }
    };
    matrix3x3 expected{
        {  776,  812,  848 },
        { 1406, 1472, 1538 },
        { 2036, 2132, 2228 }
    };
    // clang-format on
    auto mul = initial * initial;
    EXPECT_EQ(expected, mul) << "Invalid result " << mul;
}

TEST(Matrix, RectMatrixAdd)
{
    // clang-format off
    matrix<float, 3, 4> initial{
        { 11, 12, 13, 14 },
        { 21, 22, 23, 24 },
        { 31, 32, 33, 34 }
    };
    matrix<float, 3, 4> expected{
        { 22, 24, 26, 28 },
        { 42, 44, 46, 48 },
        { 62, 64, 66, 68 }
    };
    // clang-format on
    EXPECT_EQ(expected, initial + initial);
}

TEST(Matrix, Minor)
{
    // clang-format off
    matrix3x3 init{
        { 11, 12, 13 },
        { 21, 22, 23 },
        { 31, 32, 33 }
    };
    // clang-format on
    EXPECT_EQ((matrix2x2{{22, 23}, {32, 33}}), (expr::minor<0, 0>(init)));
    EXPECT_EQ((matrix2x2{{21, 23}, {31, 33}}), (expr::minor<0, 1>(init)));
    EXPECT_EQ((matrix2x2{{21, 22}, {31, 32}}), (expr::minor<0, 2>(init)));
}

TEST(Matrix, Determinant)
{
    {
        // clang-format off
        matrix2x2 m{
            {1, 2},
            {3, 4}
        };
        // clang-format on
        EXPECT_EQ(-2, det(m));
    }
    {
        // clang-format off
        matrix3x3 m{
            { 11, 12, 13 },
            { 21, 22, 23 },
            { 31, 32, 33 }
        };
        // clang-format on
        EXPECT_EQ(0, det(m));
    }
}

TEST(Matrix, Mutate)
{
    // clang-format off
    matrix3x4 m{
        { 11, 21, 31, 41 },
        { 12, 22, 32, 42 },
        { 13, 23, 33, 43 }
    };
    // Transposed
    matrix4x3 t{
        { 11, 12, 13 },
        { 21, 22, 23 },
        { 31, 32, 33 },
        { 41, 42, 43 }
    };
    // Flipped around secondary diagonal
    matrix4x3 f_2nd {
        { 43, 42, 41 },
        { 33, 32, 31 },
        { 23, 22, 21 },
        { 13, 12, 11 }
    };
    // Flipped horizontally
    matrix3x4 f_h {
        { 41, 31, 21, 11 },
        { 42, 32, 22, 12 },
        { 43, 33, 23, 13 },
    };
    // Flipped vertically
    matrix3x4 f_v{
        { 13, 23, 33, 43 },
        { 12, 22, 32, 42 },
        { 11, 21, 31, 41 }
    };
    // Rotated cw
    matrix4x3 cw{
        { 13, 12, 11 },
        { 23, 22, 21 },
        { 33, 32, 31 },
        { 43, 42, 41 }
    };
    // Rotated ccw
    matrix4x3 ccw{
        { 41, 42, 43 },
        { 31, 32, 33 },
        { 21, 22, 23 },
        { 11, 12, 13 },
    };
    // Rotated 180°
    matrix3x4 r_180{
        { 43, 33, 23, 13 },
        { 42, 32, 22, 12 },
        { 41, 31, 21, 11 }
    };
    // clang-format on

    EXPECT_EQ(t, transpose(m)) << "Transpose: " << transpose(m);
    EXPECT_EQ(f_2nd, flip_secondary(m)) << "Flip 2nd diagonal: " << flip_secondary(m);
    EXPECT_EQ(f_h, flip_horizontally(m)) << "Flip horizontally: " << flip_horizontally(m);
    EXPECT_EQ(f_v, flip_vertically(m)) << "Flip vertically: " << flip_vertically(m);
    EXPECT_EQ(cw, rotate_cw(m)) << "Rotate CW: " << rotate_cw(m);
    EXPECT_EQ(ccw, rotate_ccw(m)) << "Rotate CCW: " << rotate_ccw(m);
    EXPECT_EQ(r_180, rotate_180(m)) << "Rotate 180: " << rotate_180(m);
}

TEST(Matrix, BinaryIO)
{
    // clang-format off
    matrix3x4 src{
        { 11, 21, 31, 41 },
        { 12, 22, 32, 42 },
        { 13, 23, 33, 43 }
    };
    // clang-format on
    matrix3x4          tgt;
    std::ostringstream os;
    os << io::binmode(true) << src;
    EXPECT_FALSE(os.str().empty());
    EXPECT_EQ(sizeof(std::size_t) * 4 + sizeof(double) * matrix3x4::size, os.str().size());
    {
        std::istringstream is;
        is >> io::binmode(true) >> tgt;
        EXPECT_EQ(matrix3x4{}, tgt) << "Nothing should be read from an empty stream";
    }
    std::istringstream is(os.str());
    is >> io::binmode(true) >> tgt;
    EXPECT_EQ(src, tgt) << "Invalid data read from stream";
}

} /* namespace test */
} /* namespace math */
} /* namespace psst */
