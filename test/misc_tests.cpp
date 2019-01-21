/**
 * Copyright 2019 Sergei A. Fedorov
 * misc_tests.cpp
 *
 *  Created on: Jan 5, 2019
 *      Author: ser-fedorov
 */

#include <psst/math/detail/value_policy.hpp>

#include <gtest/gtest.h>

#include <sstream>

namespace psst {
namespace math {
namespace test {

TEST(Misc, Clamp)
{
    {
        using clamper = value_policy::clamp_zero_to_one<double>;
        double  val{0};
        clamper cl{val};
        cl = 100;
        EXPECT_EQ(1, cl);
        EXPECT_EQ(1, val);
        cl -= 100;
        EXPECT_EQ(0, cl);
        EXPECT_EQ(0, val);
    }
}

}    // namespace test
}    // namespace math
}    // namespace psst
