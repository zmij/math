/**
 * Copyright 2019 Sergei A. Fedorov
 * vector_view_tests.cpp
 *
 *  Created on: Jan 17, 2019
 *      Author: ser-fedorov
 */

#include "test_printing.hpp"
#include <psst/math/vector.hpp>
#include <psst/math/vector_view.hpp>

#include <gtest/gtest.h>

#include <sstream>
#include <vector>

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
    vector3f      v1{1, 2, 3};
    vector3f      v2{4, 5, 6};
    vector3f_view vv{v1.data()};

    EXPECT_NE(v2, v1);
    EXPECT_NE(v2, vv);

    vv[0] = 2;
    EXPECT_EQ(2, v1[0]);
    vv.at<0>() = 1;
    EXPECT_EQ(1, v1.at<0>());
    vv.x() = 3;
    EXPECT_EQ(3, v1.x());

    vv = v2;
    EXPECT_EQ(v2, v1);
    EXPECT_EQ(v2, vv);

    vv = v2 + v2;
    EXPECT_EQ(v2 * 2, v1);
    EXPECT_EQ(v2 * 2, vv);
}

TEST(VectorView, AccessBuffers)
{
    {
        vector3f v1{1, 2, 3};
        auto     vv = make_vector_view<vector3f>(v1.data());
        EXPECT_EQ(v1, vv);
    }
    {
        std::vector<vector3f> vectors{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

        float const* float_const_buf = vectors.data()->data();
        auto const   float_buf_size  = vector3f::size * vectors.size();
        auto mem_const_view = make_memory_vector_view<vector3f>(float_const_buf, float_buf_size);

        EXPECT_EQ(vectors.size(), mem_const_view.size());
        std::size_t i = 0;
        for (auto v : mem_const_view) {
            EXPECT_EQ(vectors[i], v);
            ++i;
        }
        float* float_buf = vectors.data()->data();
        auto   mem_view  = make_memory_vector_view<vector3f>(float_buf, float_buf_size);
        EXPECT_EQ(vectors.size(), mem_view.size());
        i = 0;
        for (auto v : mem_view) {
            EXPECT_EQ(vectors[i], v);
            v.z() = 42;
            EXPECT_EQ(42, vectors[i].z());
            ++i;
        }

        char const* char_const_buf = reinterpret_cast<char const*>(float_const_buf);
        auto const  char_buf_size  = float_buf_size * sizeof(float);
        auto char_mem_const_view = make_memory_vector_view<vector3f>(char_const_buf, char_buf_size);
        EXPECT_EQ(vectors.size(), char_mem_const_view.size());
        i = 0;
        for (auto v : char_mem_const_view) {
            EXPECT_EQ(vectors[i], v);
            ++i;
        }

        char* char_buf      = reinterpret_cast<char*>(float_buf);
        auto  char_mem_view = make_memory_vector_view<vector3f>(char_buf, char_buf_size);
        EXPECT_EQ(vectors.size(), char_mem_view.size());
        i = 0;
        for (auto v : char_mem_view) {
            EXPECT_EQ(vectors[i], v);
            EXPECT_EQ(42, v.z());
            v.y() = 42;
            EXPECT_EQ(42, vectors[i].y());
            ++i;
        }
    }
}

}    // namespace test
}    // namespace math
}    // namespace psst
