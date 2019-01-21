/**
 * Copyright 2019 Sergei A. Fedorov
 * color_tests.cpp
 *
 *  Created on: Jan 8, 2019
 *      Author: ser-fedorov
 */

#include "test_printing.hpp"
#include <psst/math/colors.hpp>

#include <gtest/gtest.h>

#include <sstream>

namespace psst {
namespace math {
namespace test {

using rgba     = color::rgba<float>;
using argb     = color::argb<float>;
using hsla     = color::hsla<float>;
using hsva     = color::hsva<float>;
using rgba_hex = color::rgba_hex;
using color::operator""_rgba;

TEST(Vector, ConstructRGB)
{
    {
        rgba c{0.1, 0.2, 0.3, 0.4};
        EXPECT_FLOAT_EQ(0.1, c.r());
        EXPECT_FLOAT_EQ(0.2, c.g());
        EXPECT_FLOAT_EQ(0.3, c.b());
        EXPECT_FLOAT_EQ(0.4, c.a());
    }
    {
        argb c{0.1, 0.2, 0.3, 0.4};
        EXPECT_FLOAT_EQ(0.1, c.a());
        EXPECT_FLOAT_EQ(0.2, c.r());
        EXPECT_FLOAT_EQ(0.3, c.g());
        EXPECT_FLOAT_EQ(0.4, c.b());
    }
}

TEST(Color, Hex)
{
    color::rgba_hex c_hex = 0xff0000ff_rgba;
    EXPECT_EQ(0xff, c_hex.red());
    EXPECT_EQ(0, c_hex.green());
    EXPECT_EQ(0, c_hex.blue());
    EXPECT_EQ(0xff, c_hex.alpha());
    rgba c1 = convert<rgba>(c_hex);
    std::cout << c_hex << " " << c1 << "\n";
    EXPECT_EQ(1.0, c1.red());
    EXPECT_EQ(0, c1.green());
    EXPECT_EQ(0, c1.blue());
    EXPECT_EQ(1.0, c1.alpha());
}

class HslToRgb : public ::testing::TestWithParam<std::pair<hsla, rgba_hex>> {};

TEST_P(HslToRgb, Convert)
{
    auto [hsl, rgb_hex] = GetParam();
    rgba rgb            = convert<rgba>(rgb_hex);
    rgba rgb_cvt        = convert<rgba>(hsl);
    hsla hsl_cvt        = convert<hsla>(rgb);
    std::cout << "HSLA color " << hsl << "\n"
              << "RGBA color " << rgb_hex << " (" << rgb << ")\n"
              << "HSLA -> RGBA = " << rgb_cvt << "\n"
              << "RGBA -> HSLA = " << hsl_cvt << "\n";

    EXPECT_NEAR(rgb.r(), rgb_cvt.r(), 0.005);
    EXPECT_NEAR(rgb.g(), rgb_cvt.g(), 0.005);
    EXPECT_NEAR(rgb.b(), rgb_cvt.b(), 0.005);
    EXPECT_EQ(rgb.a(), rgb_cvt.a());

    EXPECT_NEAR(hsl.h(), hsl_cvt.h(), 0.005);
    EXPECT_NEAR(hsl.s(), hsl_cvt.s(), 0.005);
    EXPECT_NEAR(hsl.l(), hsl_cvt.l(), 0.005);
    EXPECT_EQ(hsl.a(), hsl_cvt.a());
}

// clang-format off
INSTANTIATE_TEST_CASE_P(Colors, HslToRgb,
    ::testing::Values(
        std::make_pair(hsla{},                          0x0_rgba),
        std::make_pair(hsla{0,              0,  0, 1},  0x000000ff_rgba),
        std::make_pair(hsla{(float)0_deg,   1, .5, 1},  0xff0000ff_rgba),
        std::make_pair(hsla{(float)15_deg,  1, .5, 1},  0xff4000ff_rgba),
        std::make_pair(hsla{(float)30_deg,  1, .5, 1},  0xff8000ff_rgba),
        std::make_pair(hsla{(float)45_deg,  1, .5, 1},  0xffbf00ff_rgba),
        std::make_pair(hsla{(float)60_deg,  1, .5, 1},  0xffff00ff_rgba),
        std::make_pair(hsla{(float)75_deg,  1, .5, 1},  0xbfff00ff_rgba),
        std::make_pair(hsla{(float)90_deg,  1, .5, 1},  0x80ff00ff_rgba),
        std::make_pair(hsla{(float)105_deg, 1, .5, 1},  0x40ff00ff_rgba),
        std::make_pair(hsla{(float)120_deg, 1, .5, 1},  0x00ff00ff_rgba),
        std::make_pair(hsla{(float)135_deg, 1, .5, 1},  0x00ff40ff_rgba),
        std::make_pair(hsla{(float)150_deg, 1, .5, 1},  0x00ff80ff_rgba),
        std::make_pair(hsla{(float)165_deg, 1, .5, 1},  0x00ffbfff_rgba),
        std::make_pair(hsla{(float)180_deg, 1, .5, 1},  0x00ffffff_rgba),
        std::make_pair(hsla{(float)195_deg, 1, .5, 1},  0x00bfffff_rgba),
        std::make_pair(hsla{(float)210_deg, 1, .5, 1},  0x007fffff_rgba),
        std::make_pair(hsla{(float)225_deg, 1, .5, 1},  0x0040ffff_rgba),
        std::make_pair(hsla{(float)240_deg, 1, .5, 1},  0x0000ffff_rgba),
        std::make_pair(hsla{(float)255_deg, 1, .5, 1},  0x4000ffff_rgba),
        std::make_pair(hsla{(float)270_deg, 1, .5, 1},  0x7f00ffff_rgba),
        std::make_pair(hsla{(float)285_deg, 1, .5, 1},  0xbf00ffff_rgba),
        std::make_pair(hsla{(float)300_deg, 1, .5, 1},  0xff00ffff_rgba),
        std::make_pair(hsla{(float)315_deg, 1, .5, 1},  0xff00bfff_rgba),
        std::make_pair(hsla{(float)330_deg, 1, .5, 1},  0xff0080ff_rgba),
        std::make_pair(hsla{(float)345_deg, 1, .5, 1},  0xff0040ff_rgba),
        std::make_pair(hsla{(float)360_deg, 1, .5, 1},  0xff0000ff_rgba)
    ), /**/);
// clang-format on

class HsvToRgb : public ::testing::TestWithParam<std::pair<hsva, rgba_hex>> {};

TEST_P(HsvToRgb, Convert)
{
    auto [hsv, rgb_hex] = GetParam();
    rgba rgb            = convert<rgba>(rgb_hex);
    rgba rgb_cvt        = convert<rgba>(hsv);
    hsva hsv_cvt        = convert<hsva>(rgb);
    std::cout << "HSVA color " << hsv << "\n"
              << "RGBA color " << rgb_hex << " (" << rgb << ")\n"
              << "HSVA -> RGBA = " << rgb_cvt << "\n"
              << "RGBA -> HSVA = " << hsv_cvt << "\n";

    EXPECT_NEAR(rgb.r(), rgb_cvt.r(), 0.005);
    EXPECT_NEAR(rgb.g(), rgb_cvt.g(), 0.005);
    EXPECT_NEAR(rgb.b(), rgb_cvt.b(), 0.005);
    EXPECT_EQ(rgb.a(), rgb_cvt.a());

    EXPECT_NEAR(hsv.h(), hsv_cvt.h(), 0.005);
    EXPECT_NEAR(hsv.s(), hsv_cvt.s(), 0.005);
    EXPECT_NEAR(hsv.v(), hsv_cvt.v(), 0.005);
    EXPECT_EQ(hsv.a(), hsv_cvt.a());
}

// clang-format off
INSTANTIATE_TEST_CASE_P(Colors, HsvToRgb,
    ::testing::Values(
        std::make_pair(hsva{},                          0x0_rgba),
        std::make_pair(hsva{0,              0,  0, 1},  0x000000ff_rgba),
        std::make_pair(hsva{(float)0_deg,   1,  1, 1},  0xff0000ff_rgba),
        std::make_pair(hsva{(float)15_deg,  1,  1, 1},  0xff4000ff_rgba),
        std::make_pair(hsva{(float)30_deg,  1,  1, 1},  0xff8000ff_rgba),
        std::make_pair(hsva{(float)45_deg,  1,  1, 1},  0xffbf00ff_rgba),
        std::make_pair(hsva{(float)60_deg,  1,  1, 1},  0xffff00ff_rgba),
        std::make_pair(hsva{(float)75_deg,  1,  1, 1},  0xbfff00ff_rgba),
        std::make_pair(hsva{(float)90_deg,  1,  1, 1},  0x80ff00ff_rgba),
        std::make_pair(hsva{(float)105_deg, 1,  1, 1},  0x40ff00ff_rgba),
        std::make_pair(hsva{(float)120_deg, 1,  1, 1},  0x00ff00ff_rgba),
        std::make_pair(hsva{(float)135_deg, 1,  1, 1},  0x00ff40ff_rgba),
        std::make_pair(hsva{(float)150_deg, 1,  1, 1},  0x00ff80ff_rgba),
        std::make_pair(hsva{(float)165_deg, 1,  1, 1},  0x00ffbfff_rgba),
        std::make_pair(hsva{(float)180_deg, 1,  1, 1},  0x00ffffff_rgba),
        std::make_pair(hsva{(float)195_deg, 1,  1, 1},  0x00bfffff_rgba),
        std::make_pair(hsva{(float)210_deg, 1,  1, 1},  0x007fffff_rgba),
        std::make_pair(hsva{(float)225_deg, 1,  1, 1},  0x0040ffff_rgba),
        std::make_pair(hsva{(float)240_deg, 1,  1, 1},  0x0000ffff_rgba),
        std::make_pair(hsva{(float)255_deg, 1,  1, 1},  0x4000ffff_rgba),
        std::make_pair(hsva{(float)270_deg, 1,  1, 1},  0x7f00ffff_rgba),
        std::make_pair(hsva{(float)285_deg, 1,  1, 1},  0xbf00ffff_rgba),
        std::make_pair(hsva{(float)300_deg, 1,  1, 1},  0xff00ffff_rgba),
        std::make_pair(hsva{(float)315_deg, 1,  1, 1},  0xff00bfff_rgba),
        std::make_pair(hsva{(float)330_deg, 1,  1, 1},  0xff0080ff_rgba),
        std::make_pair(hsva{(float)345_deg, 1,  1, 1},  0xff0040ff_rgba),
        std::make_pair(hsva{(float)360_deg, 1,  1, 1},  0xff0000ff_rgba)
    ), /**/);
// clang-format on

} /* namespace test */
} /* namespace math */
} /* namespace psst */
