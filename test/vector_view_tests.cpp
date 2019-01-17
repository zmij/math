/**
 * Copyright 2019 Sergei A. Fedorov
 * vector_view_tests.cpp
 *
 *  Created on: Jan 17, 2019
 *      Author: ser-fedorov
 */

#include "test_printing.hpp"
#include <pushkin/math/vector.hpp>
#include <pushkin/math/vector_view.hpp>

#include <gtest/gtest.h>

#include <sstream>

namespace psst {
namespace math {
namespace test {

using vector3d            = vector<double, 3>;
using vector3d_view       = vector_view<double*, 3>;
using vector3d_const_view = vector_view<double const*, 3>;

using vector3f            = vector<float, 3>;
using vector3f_view       = vector_view<float*, 3>;
using vector3f_const_view = vector_view<float const*, 3>;

TEST(VectorView, ConstantView)
{
    {
        vector3f            v{1, 2, 3};
        vector3f_const_view vv{v.data()};

        EXPECT_EQ(v[0], vv[0]);
        EXPECT_EQ(v[1], vv[1]);
        EXPECT_EQ(v[2], vv[2]);

        EXPECT_EQ(v.x(), vv.x());
        EXPECT_EQ(v.y(), vv.y());
        EXPECT_EQ(v.z(), vv.z());

        EXPECT_EQ(v, vv);
    }
    {
        vector3d            v{1, 2, 3};
        vector3d_const_view vv{v.data()};

        EXPECT_EQ(v[0], vv[0]);
        EXPECT_EQ(v[1], vv[1]);
        EXPECT_EQ(v[2], vv[2]);

        EXPECT_EQ(v.x(), vv.x());
        EXPECT_EQ(v.y(), vv.y());
        EXPECT_EQ(v.z(), vv.z());

        EXPECT_EQ(v, vv);
    }
}

TEST(VectorView, View)
{
    {
        vector3f      v{1, 2, 3};
        vector3f_view vv{v.data()};

        EXPECT_EQ(v[0], vv[0]);
        EXPECT_EQ(v[1], vv[1]);
        EXPECT_EQ(v[2], vv[2]);

        EXPECT_EQ(v.x(), vv.x());
        EXPECT_EQ(v.y(), vv.y());
        EXPECT_EQ(v.z(), vv.z());

        EXPECT_EQ(v, vv);
    }
    {
        vector3d      v{1, 2, 3};
        vector3d_view vv{v.data()};

        EXPECT_EQ(v[0], vv[0]);
        EXPECT_EQ(v[1], vv[1]);
        EXPECT_EQ(v[2], vv[2]);

        EXPECT_EQ(v.x(), vv.x());
        EXPECT_EQ(v.y(), vv.y());
        EXPECT_EQ(v.z(), vv.z());

        EXPECT_EQ(v, vv);
    }
}

TEST(VectorView, ViewModification)
{
    {
        vector3f      v{1, 2, 3};
        vector3f_view vv{v.data()};

        vv[0] = 2;
        EXPECT_EQ(2, v[0]);
        vv.at<0>() = 1;
        EXPECT_EQ(1, v.at<0>());
        vv.x() = 3;
        EXPECT_EQ(3, v.x());
    }
}

}    // namespace test
}    // namespace math
}    // namespace psst
