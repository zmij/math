/*
 * test.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: zmij
 */

#include <pushkin/math/vector.hpp>
#include <pushkin/math/matrix.hpp>

#include "test_printing.hpp"

#include <gtest/gtest.h>

namespace psst {
namespace math {
namespace test {

using vector3d = vector<double, 3>;
using vector3df = vector<float, 3>;

using rgba_col = vector<float, 4, axes::rgba>;
using argb_col = vector<float, 4, axes::argb>;

TEST(Scalar, Compare)
{
    using traits = scalar_value_traits<float>;
    auto i_val = 42;
    auto d_val = 42.0;
    auto ds_val = expr::make_scalar_constant(42.0);

    EXPECT_EQ(i_val, d_val);
    EXPECT_EQ(i_val, ds_val);
    EXPECT_EQ(ds_val, d_val);

    EXPECT_EQ(0, traits::cmp(i_val, d_val));

    EXPECT_EQ(0, traits::cmp(ds_val, i_val));
    EXPECT_EQ(0, traits::cmp(i_val, ds_val));
    EXPECT_EQ(0, traits::cmp(i_val, ds_val.value()));

    EXPECT_EQ(0, traits::cmp(ds_val, d_val));
    EXPECT_EQ(0, traits::cmp(d_val, ds_val));
    EXPECT_EQ(0, traits::cmp(d_val, ds_val.value()));

    EXPECT_EQ(0, traits::cmp(ds_val, ds_val));
    EXPECT_EQ(0, traits::cmp(ds_val, ds_val.value()));
}

TEST(Vector, TypeTraits)
{
  EXPECT_TRUE(::std::is_standard_layout<vector3d>::value);
  EXPECT_TRUE(::std::is_trivially_copyable<vector3d>::value);
  //EXPECT_TRUE(::std::is_trivially_default_constructible<vector3d>::value);
  //EXPECT_TRUE(::std::is_trivial<vector3d>::value);
  EXPECT_TRUE(::std::is_trivially_copy_constructible<vector3d>::value);
  //EXPECT_TRUE(::std::is_pod<vector3d>::value);
}

TEST(Vector, ConstructDefault)
{
    vector3d v1 {};
    EXPECT_EQ(0, v1[0]);
    EXPECT_EQ(0, v1[1]);
    EXPECT_EQ(0, v1[2]);
    EXPECT_TRUE(v1.is_zero());

    EXPECT_EQ(0, v1.x());
    EXPECT_EQ(0, v1.y());
    EXPECT_EQ(0, v1.z());

    EXPECT_EQ(sizeof(double)*3, sizeof(v1))
        << "Size of vector is equal to sizes of it's components";

    vector3d::base_expression_type const& be = v1;
    EXPECT_EQ(0, be.at<0>());
    EXPECT_EQ(0, be.at<1>());
    EXPECT_EQ(0, be.at<2>());
    vector3d const& v2 = be;
    EXPECT_EQ(0, v2[0]);
    EXPECT_EQ(0, v2[1]);
    EXPECT_EQ(0, v2[2]);

}

TEST(Vector, ConstructSingleValue)
{
    // Single value
    vector3d v1 (42);
    EXPECT_EQ(42, v1[0]);
    EXPECT_EQ(42, v1[1]);
    EXPECT_EQ(42, v1[2]);
    EXPECT_EQ(sizeof(float)*3, sizeof(vector3df))
        << "Size of vector is equal to sizes of it's components";
}

TEST(Vector, ConstructInitList)
{
    // Initializer list
    vector3d v1{1, 2, 3};
    EXPECT_EQ(1, v1[0]);
    EXPECT_EQ(2, v1[1]);
    EXPECT_EQ(3, v1[2]);
}

TEST(Vector, ConstructXYZ)
{
    // Initializer list
    vector3d v1{1, 2, 3};
    EXPECT_EQ(1, v1[0]);
    EXPECT_EQ(2, v1[1]);
    EXPECT_EQ(3, v1[2]);

    EXPECT_EQ((vector3d{1, 1, 1}), v1.xxx());
    EXPECT_EQ((vector3d{2, 2, 2}), v1.yyy());
    EXPECT_EQ((vector3d{3, 3, 3}), v1.zzz());
    EXPECT_EQ((vector3d{3, 2, 1}), v1.zyx());
    EXPECT_EQ((vector3d{2, 1, 3}), v1.yxz());
}

TEST(Vector, ConstructPoiter)
{
    // Pointer to double
    double vals[]{1, 2, 3};
    {
        vector3d v1{vals};
        EXPECT_EQ(1, v1[0]);
        EXPECT_EQ(2, v1[1]);
        EXPECT_EQ(3, v1[2]);
    }

    double* p = vals;
    {
        vector3d v1{p};
        EXPECT_EQ(1, v1[0]);
        EXPECT_EQ(2, v1[1]);
        EXPECT_EQ(3, v1[2]);
    }
}

TEST(Vector, ConstructRGB)
{
    {
        rgba_col c{0.1, 0.2, 0.3, 0.4};
        EXPECT_FLOAT_EQ(0.1, c.r());
        EXPECT_FLOAT_EQ(0.2, c.g());
        EXPECT_FLOAT_EQ(0.3, c.b());
        EXPECT_FLOAT_EQ(0.4, c.a());
    }
    {
        argb_col c{0.1, 0.2, 0.3, 0.4};
        EXPECT_FLOAT_EQ(0.1, c.a());
        EXPECT_FLOAT_EQ(0.2, c.r());
        EXPECT_FLOAT_EQ(0.3, c.g());
        EXPECT_FLOAT_EQ(0.4, c.b());
    }
}

TEST(Vector, Modify)
{
    vector3d v1{};
    EXPECT_TRUE(v1.is_zero());

    v1[0] = 1;
    EXPECT_FALSE(v1.is_zero());
    EXPECT_EQ((vector3d{1, 0, 0}), v1);
    v1[1] = 2;
    EXPECT_EQ((vector3d{1, 2, 0}), v1);
    v1[2] = 3;
    EXPECT_EQ((vector3d{1, 2, 3}), v1);

    v1.x() *= 2;
    EXPECT_EQ((vector3d{2, 2, 3}), v1);
    v1.y() *= 2;
    EXPECT_EQ((vector3d{2, 4, 3}), v1);
    v1.z() *= 2;
    EXPECT_EQ((vector3d{2, 4, 6}), v1);
}

TEST(Vector, Add)
{
    vector3d v1{2,5,8}, v2{4,6,9};
    {
        vector3d expected{6, 11, 17};
        auto res = v1 + v2;
        EXPECT_EQ(expected, res) << "Unexpected result " << res << "\n";
    }
    {
        auto res = v1 - v2;
        vector3d expected{-2, -1, -1};
        EXPECT_EQ(expected, res) << "Unexpected result " << res << "\n";
    }
}

TEST(Vector, Multiply)
{
    vector3d initial{1,2,3}, expected{5, 10, 15};
    auto res1 = initial * 5;
    EXPECT_EQ(expected, res1) << "Unexpected result " << res1 << "\n";
    auto res2 = expected / 5;
    EXPECT_EQ(initial, res2) << "Unexpected result " << res2 << "\n";
}

TEST(Vector, Magnitude)
{
    vector3d v1 {1, 1, 1};
    EXPECT_EQ(3, v1.magnitude_square());
    EXPECT_FALSE(v1.is_unit());
    EXPECT_FALSE(v1.is_zero());
}

TEST(Vector, Expression)
{
  vector3df v1 {1, 1, 1}, v2 {1, 0, 1}, expected { 2.5, 1, 2.5 };
  vector3df res = (v1 * 2 + v2 * 3) / 4 * 2;
  EXPECT_EQ(expected, res);
}

TEST(Vector, Normalize)
{
    vector3d v1 {10, 0, 0};
    EXPECT_FALSE(v1.is_zero());
    EXPECT_FALSE(v1.is_unit());
    v1.normalize();
    EXPECT_TRUE(v1.is_unit()) << "Expected magnitude == 1 " << v1 << " mag_sq=" << v1.magnitude_square();
}

TEST(Vector, Unit)
{
    {
        vector3d v1{1, 0, 0};
        EXPECT_TRUE(v1.is_unit());
    }
    {
        vector3d v1{0, 1, 0};
        EXPECT_TRUE(v1.is_unit());
    }
    {
        vector3d v1{0, 0, 1};
        EXPECT_TRUE(v1.is_unit());
    }
}

TEST(Vector, Iteration)
{
    vector3d v1{1, 2, 3};
    int idx = 0;
    for (auto v : v1) {
        EXPECT_EQ(idx + 1, v);
        ++idx;
    }
}

TEST(Vector, Lerp)
{
    vector3d v1{1, 1, 1};
    vector3d v2{3, 3, 3};
    EXPECT_EQ((vector3d{2, 2, 2}), lerp(v1, v2, 0.5))
        << "Unexpected lerp result " << lerp(v1, v2, 0.5);
}

TEST(Vector, Slerp)
{
    vector3d v1{1, 0, 0};
    vector3d v2{0, 1, 0};
    double v = 1 / ::std::sqrt(2);
    EXPECT_EQ((vector3d{v, v, 0}), slerp(v1, v2, 0.5))
        << "Unexpected lerp result " << slerp(v1, v2, 0.5);
}

TEST(Vector, PolarCvt)
{
    using vector2d    = vector<double, 2, axes::xyzw>;
    using polar_coord = vector<double, 2, axes::polar>;

    vector2d v{1, 0};
    polar_coord pc{1, 0};

    EXPECT_EQ(pc, convert<polar_coord>(v));
    EXPECT_EQ(v, convert<vector2d>(pc));

    EXPECT_EQ(pc, v.convert<axes::polar>());
    EXPECT_EQ(v, pc.convert<axes::xyzw>());
}

}  /* namespace test */
}  /* namespace math */
}  /* namespace psst */


