/**
 * Copyright 2019 Sergei A. Fedorov
 * quaternion_tests.cpp
 *
 *  Created on: Jan 7, 2019
 *      Author: ser-fedorov
 */

#include "test_printing.hpp"
#include <psst/math/quaternion.hpp>

#include <gtest/gtest.h>

#include <sstream>

namespace psst {
namespace math {
namespace test {

using quaternion_d = quaternion<double>;
using vector3d     = vector<double, 3>;

TEST(Quat, Components)
{
    quaternion_d q{1, 2, 3, 4};

    EXPECT_EQ(1, q.w());
    EXPECT_EQ(2, q.x());
    EXPECT_EQ(3, q.y());
    EXPECT_EQ(4, q.z());

    EXPECT_EQ(1, q.scalar_part());
    EXPECT_EQ((vector3d{2, 3, 4}), q.vector_part());
}

TEST(Quat, Add)
{
    quaternion_d q1{1, 2, 3, 4}, q2{5, 6, 7, 8};

    EXPECT_EQ((quaternion_d{6, 8, 10, 12}), q1 + q2);
    EXPECT_EQ((quaternion_d{-4, -4, -4, -4}), q1 - q2);
}

TEST(Quat, Magnitude)
{
    quaternion_d q1{2, 2, 2, 2};
    EXPECT_EQ(4, magnitude(q1));
    EXPECT_EQ(16, magnitude_square(q1));
}

TEST(Quat, Normalize)
{
    quaternion_d q1{1, 1, 1, 1};
    EXPECT_EQ((quaternion_d{0.5, 0.5, 0.5, 0.5}), normalize(q1))
        << "Normalized quat " << normalize(q1);
    EXPECT_EQ((quaternion_d{0, -1, 0, 0}), normalize(quaternion_d{0, 1, 0, 0}));
}

TEST(Quat, ScalarMultiply)
{
    quaternion_d q1{1, 2, 3, 4};

    EXPECT_EQ((quaternion_d{2, 4, 6, 8}), q1 * 2);
    EXPECT_EQ((quaternion_d{0.5, 1, 1.5, 2}), q1 / 2);
}

TEST(Quat, QuatMultuply)
{
    quaternion_d q1{1, 2, 3, 4}, q2{5, 6, 7, 8};

    EXPECT_EQ((quaternion_d{-60, 12, 30, 24}), q1 * q2) << "Quat multiply result " << q1 * q2;
}

TEST(Quat, Conjugate)
{
    quaternion_d q{1, 2, 3, 4};

    EXPECT_EQ((quaternion_d{1, -2, -3, -4}), conjugate(q));
    EXPECT_EQ(q, conjugate(conjugate(q)));
}

}    // namespace test
}    // namespace math
}    // namespace psst
