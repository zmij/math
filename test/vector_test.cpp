/*
 * test.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: zmij
 */

#include "test_printing.hpp"
#include <psst/math/coordinate_conversion.hpp>
#include <psst/math/cylindrical_coord.hpp>
#include <psst/math/polar_coord.hpp>
#include <psst/math/spherical_coord.hpp>
#include <psst/math/vector.hpp>

#include <gtest/gtest.h>

#include <sstream>

namespace psst {
namespace math {
namespace test {

using vector3d  = vector<double, 3>;
using vector3df = vector<float, 3>;

TEST(Scalar, Compare)
{
    using traits = traits::scalar_value_traits<float>;
    auto i_val   = 42;
    auto d_val   = 42.0;
    auto ds_val  = expr::make_scalar_constant(42.0);

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
    EXPECT_TRUE(std::is_standard_layout<vector3d>::value);
    EXPECT_TRUE(std::is_trivially_copyable<vector3d>::value);
    // EXPECT_TRUE(std::is_trivially_default_constructible<vector3d>::value);
    // EXPECT_TRUE(std::is_trivial<vector3d>::value);
    EXPECT_TRUE(std::is_trivially_copy_constructible<vector3d>::value);
    // EXPECT_TRUE(std::is_pod<vector3d>::value);
}

TEST(Vector, ConstructDefault)
{
    vector3d v1{};
    EXPECT_EQ(0, v1[0]);
    EXPECT_EQ(0, v1[1]);
    EXPECT_EQ(0, v1[2]);
    EXPECT_TRUE(v1.is_zero());

    EXPECT_EQ(0, v1.x());
    EXPECT_EQ(0, v1.y());
    EXPECT_EQ(0, v1.z());

    EXPECT_EQ(sizeof(double) * 3, sizeof(v1))
        << "Size of vector is equal to sizes of it's components";

    vector3d::base_expression_type const& be = v1;
    EXPECT_EQ(0, get<0>(be));
    EXPECT_EQ(0, get<1>(be));
    EXPECT_EQ(0, get<2>(be));
    vector3d const& v2 = be;
    EXPECT_EQ(0, v2[0]);
    EXPECT_EQ(0, v2[1]);
    EXPECT_EQ(0, v2[2]);
}

TEST(Vector, ConstructSingleValue)
{
    // Single value
    vector3d v1(42);
    EXPECT_EQ(42, v1[0]);
    EXPECT_EQ(42, v1[1]);
    EXPECT_EQ(42, v1[2]);
    EXPECT_EQ(sizeof(float) * 3, sizeof(vector3df))
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

TEST(Vector, IO)
{
    vector3d           v1{0.1, 0.2, 0.3, 0.4}, v2{};
    std::ostringstream os;
    os << v1;
    std::istringstream is(os.str());
    is >> v2;
    EXPECT_TRUE(is.good());
    EXPECT_EQ(v1, v2) << "Failed to read value from " << os.str();

    os.str("");
    os << io::pretty << v1 << io::ugly;
    v2.zero();
    EXPECT_NE(v1, v2);
    is.str(os.str());
    is >> v2;
    EXPECT_TRUE(is.good());
    EXPECT_EQ(v1, v2) << "Failed to read value from " << os.str();
}

TEST(Vector, BinaryIO)
{
    {
        // First just some values
        double             src{42.0};
        std::ostringstream os;
        io::write_binary(os, src);
        double             tgt{0};
        std::istringstream is;
        io::read_binary(is, tgt);
        EXPECT_EQ(0, tgt) << "No value read from an empty stream";
        is.str(os.str());
        io::read_binary(is, tgt);
        EXPECT_EQ(src, tgt) << "Correct value read from stream";
    }
    {
        vector3d           src{1, 2, 3}, tgt{};
        std::ostringstream os;
        os << io::binmode(true) << src;
        EXPECT_FALSE(os.str().empty());
        EXPECT_EQ(sizeof(std::size_t) + sizeof(double) * vector3d::size, os.str().size());
        {
            std::istringstream is;
            is >> io::binmode(true) >> tgt;
            EXPECT_EQ(vector3d{}, tgt) << "No value read from an empty stream";
        }
        std::istringstream is{os.str()};
        is >> io::binmode(true) >> tgt;
        EXPECT_EQ(src, tgt) << "Incorrect value read from stream";
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
    vector3d v1{2, 5, 8}, v2{4, 6, 9};
    {
        vector3d expected{6, 11, 17};
        auto     res = v1 + v2;
        EXPECT_EQ(expected, res) << "Unexpected result " << res << "\n";
    }
    {
        auto     res = v1 - v2;
        vector3d expected{-2, -1, -1};
        EXPECT_EQ(expected, res) << "Unexpected result " << res << "\n";
    }
}

TEST(Vector, Multiply)
{
    vector3d initial{1, 2, 3}, expected{5, 10, 15};
    auto     res1 = initial * 5;
    EXPECT_EQ(expected, res1) << "Unexpected result " << res1 << "\n";
    auto res2 = expected / 5;
    EXPECT_EQ(initial, res2) << "Unexpected result " << res2 << "\n";
}

TEST(Vector, Cross)
{
    vector3d v1{1, 2, 3}, v2{4, 5, 6};
    EXPECT_EQ((vector3d{-3, 6, -3}), v1 * v2) << "Cross product " << v1 * v2;
}

TEST(Vector, Magnitude)
{
    vector3d v1{1, 1, 1};
    EXPECT_EQ(3, v1.magnitude_square());
    EXPECT_FALSE(v1.is_unit());
    EXPECT_FALSE(v1.is_zero());
}

TEST(Vector, Expression)
{
    vector3df v1{1, 1, 1}, v2{1, 0, 1}, expected{2.5, 1, 2.5};
    vector3df res = (v1 * 2 + v2 * 3) / 4 * 2;
    EXPECT_EQ(expected, res);
}

TEST(Vector, Normalize)
{
    vector3d v1{10, 0, 0};
    EXPECT_FALSE(v1.is_zero());
    EXPECT_FALSE(v1.is_unit());
    v1.normalize();
    EXPECT_TRUE(v1.is_unit()) << "Expected magnitude == 1 " << v1
                              << " mag_sq=" << v1.magnitude_square();
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
    int      idx = 0;
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
    double   v = 1 / std::sqrt(2);
    EXPECT_EQ((vector3d{v, v, 0}), slerp(v1, v2, 0.5))
        << "Unexpected lerp result " << slerp(v1, v2, 0.5);
}

TEST(Polar, Clamp)
{
    polar_coord<double> pc{1, 360.0_deg};
    EXPECT_EQ(0, pc.azimuth());
    pc.azimuth() = -180.0_deg;
    EXPECT_EQ(180.0_deg, pc.azimuth());
}

TEST(Polar, IO)
{
    polar_coord<double> pc{1, 360.0_deg};
    {
        // Text
        std::ostringstream os;
        os << pc;
        polar_coord<double> tgt;
        std::istringstream  is(os.str());
        is >> tgt;
        EXPECT_EQ(pc, tgt);
    }
    {
        // Binary
        std::ostringstream os;
        os << io::binmode(true) << pc;
        polar_coord<double> tgt;
        std::istringstream  is(os.str());
        is >> io::binmode(true) >> tgt;
        EXPECT_EQ(pc, tgt);
    }
}

TEST(Polar, Multiply)
{
    polar_coord<double> pc{1, 180.0_deg};
    EXPECT_EQ(1, pc.rho());
    EXPECT_EQ(180.0_deg, pc.azimuth());
    auto m = pc * 2;
    EXPECT_EQ(2, m.rho());
    EXPECT_EQ(180.0_deg, m.azimuth());

    auto d = m / 2;
    EXPECT_EQ(1, d.rho());
    EXPECT_EQ(180.0_deg, d.azimuth());
}

TEST(Polar, Magnitude)
{
    polar_coord<double> pc{-1, 180.0_deg};
    EXPECT_EQ(1, magnitude(pc));
    EXPECT_EQ(1, magnitude_square(pc));
}

TEST(Polar, Normalize)
{
    {
        polar_coord<double> pc{-1, 180.0_deg};
        polar_coord<double> n = normalize(pc);
        EXPECT_EQ(1, n.rho());
        EXPECT_EQ(0, n.azimuth());
    }
    {
        polar_coord<double> pc{10, 180.0_deg};
        polar_coord<double> n = normalize(pc);
        EXPECT_EQ(1, n.rho());
        EXPECT_EQ(180_deg, n.azimuth());
    }
}

TEST(Polar, XYZConversion)
{
    using vector2d    = vector<double, 2, components::xyzw>;
    using polar_coord = vector<double, 2, components::polar>;

    vector2d    v{1, 0};
    polar_coord pc{1, 0};

    EXPECT_EQ(pc, convert<polar_coord>(v));
    EXPECT_EQ(v, convert<vector2d>(pc));

    EXPECT_EQ(pc, v.convert<polar_coord>());
    EXPECT_EQ(v, pc.convert<vector2d>());

    EXPECT_EQ(convert<polar_coord>(v) * 3, convert<polar_coord>(v * 3));
}

TEST(Spherical, Clamp)
{
    spherical_coord<double> sc{10, 180_deg, 360_deg};
    EXPECT_EQ(10, sc.rho());
    EXPECT_EQ(90_deg, sc.inclination());
    EXPECT_EQ(0, sc.azimuth());
}

TEST(Spherical, Multiply)
{
    spherical_coord<double> sc{10, 45_deg, 60_deg};
    EXPECT_EQ(10, sc.rho());
    EXPECT_EQ(45_deg, sc.inclination());
    EXPECT_EQ(60_deg, sc.azimuth());

    auto m = sc * 2;
    EXPECT_EQ(20, m.rho());
    EXPECT_EQ(45_deg, m.inclination());
    EXPECT_EQ(60_deg, m.azimuth());

    auto d = m / 2;
    EXPECT_EQ(10, d.rho());
    EXPECT_EQ(45_deg, d.inclination());
    EXPECT_EQ(60_deg, d.azimuth());
}

TEST(Spherical, Magnitude)
{
    spherical_coord<double> sc{-10, 45_deg, 60_deg};
    EXPECT_EQ(10, magnitude(sc));
    EXPECT_EQ(100, magnitude_square(sc));
}

TEST(Spherical, Normalize)
{
    {
        spherical_coord<double> sc{-10, 45_deg, 60_deg};
        spherical_coord<double> n = normalize(sc);
        EXPECT_EQ(1, n.rho());
        EXPECT_EQ(-45_deg, n.inclination());
        EXPECT_EQ(240_deg, n.azimuth());
    }
    {
        spherical_coord<double> sc{10, 45_deg, 60_deg};
        spherical_coord<double> n = normalize(sc);
        EXPECT_EQ(1, n.rho());
        EXPECT_EQ(45_deg, n.inclination());
        EXPECT_EQ(60_deg, n.azimuth());
    }
}

TEST(Spherical, XYZConversion)
{
    using vector3d    = vector<double, 3, components::xyzw>;
    using spherical_d = spherical_coord<double>;

    {
        vector3d    v{1, 0, 0};
        spherical_d sc{1, 0, 0};

        EXPECT_EQ(sc, convert<spherical_d>(v)) << "Conversion result " << convert<spherical_d>(v);
        EXPECT_EQ(v, convert<vector3d>(sc)) << "Conversion result " << convert<vector3d>(sc);

        EXPECT_EQ(convert<spherical_d>(v) * 3, convert<spherical_d>(v * 3));
    }
    {
        vector3d    v{-1, 0, 0};
        spherical_d sc{1, 0, 180_deg};

        EXPECT_EQ(sc, convert<spherical_d>(v)) << "Conversion result " << convert<spherical_d>(v);
        EXPECT_EQ(v, convert<vector3d>(sc)) << "Conversion result " << convert<vector3d>(sc);

        EXPECT_EQ(convert<spherical_d>(v) * 3, convert<spherical_d>(v * 3));
    }
    {
        vector3d    v{0, 1, 0};
        spherical_d sc{1, 0, 90_deg};

        EXPECT_EQ(sc, convert<spherical_d>(v)) << "Conversion result " << convert<spherical_d>(v);
        EXPECT_EQ(v, convert<vector3d>(sc)) << "Conversion result " << convert<vector3d>(sc);

        EXPECT_EQ(convert<spherical_d>(v) * 3, convert<spherical_d>(v * 3));
    }
    {
        vector3d    v{0, -1, 0};
        spherical_d sc{1, 0, 270_deg};

        EXPECT_EQ(sc, convert<spherical_d>(v)) << "Conversion result " << convert<spherical_d>(v);
        EXPECT_EQ(v, convert<vector3d>(sc)) << "Conversion result " << convert<vector3d>(sc);

        EXPECT_EQ(convert<spherical_d>(v) * 3, convert<spherical_d>(v * 3));
    }
    {
        vector3d    v{0, 0, 1};
        spherical_d sc{1, 90_deg, 0};

        EXPECT_EQ(sc, convert<spherical_d>(v)) << "Conversion result " << convert<spherical_d>(v);
        EXPECT_EQ(v, convert<vector3d>(sc)) << "Conversion result " << convert<vector3d>(sc);

        EXPECT_EQ(convert<spherical_d>(v) * 3, convert<spherical_d>(v * 3));
    }
    {
        vector3d    v{0, 0, -1};
        spherical_d sc{1, -90_deg, 0};

        EXPECT_EQ(sc, convert<spherical_d>(v)) << "Conversion result " << convert<spherical_d>(v);
        EXPECT_EQ(v, convert<vector3d>(sc)) << "Conversion result " << convert<vector3d>(sc);

        EXPECT_EQ(convert<spherical_d>(v) * 3, convert<spherical_d>(v * 3));
    }
    {
        vector3d    v{3, 4, 0};
        spherical_d sc{5, 0, 0.92729521800161219};

        EXPECT_EQ(sc, convert<spherical_d>(v)) << "Conversion result " << convert<spherical_d>(v);
        EXPECT_EQ(v, convert<vector3d>(sc)) << "Conversion result " << convert<vector3d>(sc);
    }
    {
        spherical_d sc{1, 45_deg, 45_deg};
        vector3d    v{.5, .5, sqrt(2) / 2};

        EXPECT_EQ(sc, convert<spherical_d>(v)) << "Conversion result " << convert<spherical_d>(v);
        EXPECT_EQ(v, convert<vector3d>(sc)) << "Conversion result " << convert<vector3d>(sc);

        EXPECT_EQ(convert<spherical_d>(v) * 3, convert<spherical_d>(v * 3));
    }
}

TEST(Spherical, PolarConversion)
{
    using polar_d     = polar_coord<double>;
    using spherical_d = spherical_coord<double>;
    {
        polar_d     pc{1, 45_deg};
        spherical_d sc{1, 0, 45_deg};
        EXPECT_EQ(sc, convert<spherical_d>(pc));
        EXPECT_EQ(pc, convert<polar_d>(sc));
    }
    {
        spherical_d sc{1, 90_deg, 45_deg};
        EXPECT_EQ((polar_d{0, 45_deg}), convert<polar_d>(sc))
            << "Conversion result " << convert<polar_d>(sc);
    }
    {
        spherical_d sc{1, 45_deg, 0};
        EXPECT_EQ((polar_d{sqrt(2) / 2, 0}), convert<polar_d>(sc))
            << "Conversion result " << convert<polar_d>(sc);
    }
}

TEST(Cylindrical, Clamp)
{
    cylindrical_coord<double> cc{1, 360_deg, 1};
    EXPECT_EQ(0, cc.azimuth());
    cc.azimuth() = -180.0_deg;
    EXPECT_EQ(180.0_deg, cc.azimuth());
}

TEST(Cylindrical, Multiply)
{
    cylindrical_coord<double> cc{1, 180_deg, 1};
    EXPECT_EQ(1, cc.rho());
    EXPECT_EQ(180_deg, cc.azimuth());
    EXPECT_EQ(1, cc.z());
    auto m = cc * 2;
    EXPECT_EQ(2, m.rho());
    EXPECT_EQ(180_deg, m.azimuth());
    EXPECT_EQ(2, m.z());

    auto d = m / 2;
    EXPECT_EQ(1, d.rho());
    EXPECT_EQ(180_deg, d.azimuth());
    EXPECT_EQ(1, d.z());
}

TEST(Cylindrical, Magnitude)
{
    cylindrical_coord<double> cc{3, 360_deg, 4};
    EXPECT_EQ(5, magnitude(cc));
    EXPECT_EQ(25, magnitude_square(cc));
}

TEST(Cylindrical, Normalize)
{
    cylindrical_coord<double> cc{3, 360_deg, 4};
    cylindrical_coord<double> n = normalize(cc);
    EXPECT_EQ(0.6, n.rho());
    EXPECT_EQ(0, n.azimuth());
    EXPECT_EQ(0.8, n.z());
}

TEST(Cylindrical, XYZConversion)
{
    using vector3d      = vector<double, 3, components::xyzw>;
    using cylindrical_d = cylindrical_coord<double>;

    {
        vector3d      v{1, 0, 0};
        cylindrical_d c{1, 0, 0};

        EXPECT_EQ(c, convert<cylindrical_d>(v))
            << "Conversion result " << convert<cylindrical_d>(v);
        EXPECT_EQ(v, convert<vector3d>(c)) << "Conversion result " << convert<vector3d>(c);

        EXPECT_EQ(convert<cylindrical_d>(v) * 3, convert<cylindrical_d>(v * 3));
    }
    {
        vector3d      v{-1, 0, 0};
        cylindrical_d c{1, 180_deg, 0};

        EXPECT_EQ(c, convert<cylindrical_d>(v))
            << "Conversion result " << convert<cylindrical_d>(v);
        EXPECT_EQ(v, convert<vector3d>(c)) << "Conversion result " << convert<vector3d>(c);

        EXPECT_EQ(convert<cylindrical_d>(v) * 3, convert<cylindrical_d>(v * 3));
    }
    {
        vector3d      v{0, 1, 0};
        cylindrical_d c{1, 90_deg, 0};

        EXPECT_EQ(c, convert<cylindrical_d>(v))
            << "Conversion result " << convert<cylindrical_d>(v);
        EXPECT_EQ(v, convert<vector3d>(c)) << "Conversion result " << convert<vector3d>(c);

        EXPECT_EQ(convert<cylindrical_d>(v) * 3, convert<cylindrical_d>(v * 3));
    }
    {
        vector3d      v{0, -1, 0};
        cylindrical_d c{1, 270_deg, 0};

        EXPECT_EQ(c, convert<cylindrical_d>(v))
            << "Conversion result " << convert<cylindrical_d>(v);
        EXPECT_EQ(v, convert<vector3d>(c)) << "Conversion result " << convert<vector3d>(c);

        EXPECT_EQ(convert<cylindrical_d>(v) * 3, convert<cylindrical_d>(v * 3));
    }
    {
        vector3d      v{0, 0, 1};
        cylindrical_d c{0, 0, 1};

        EXPECT_EQ(c, convert<cylindrical_d>(v))
            << "Conversion result " << convert<cylindrical_d>(v);
        EXPECT_EQ(v, convert<vector3d>(c)) << "Conversion result " << convert<vector3d>(c);

        EXPECT_EQ(convert<cylindrical_d>(v) * 3, convert<cylindrical_d>(v * 3));
    }
    {
        vector3d      v{0, 0, -1};
        cylindrical_d c{0, 0, -1};

        EXPECT_EQ(c, convert<cylindrical_d>(v))
            << "Conversion result " << convert<cylindrical_d>(v);
        EXPECT_EQ(v, convert<vector3d>(c)) << "Conversion result " << convert<vector3d>(c);

        EXPECT_EQ(convert<cylindrical_d>(v) * 3, convert<cylindrical_d>(v * 3));
    }
    {
        vector3d      v{2, 2, 2};
        cylindrical_d c{sqrt(8), 45_deg, 2};

        EXPECT_EQ(c, convert<cylindrical_d>(v))
            << "Conversion result " << convert<cylindrical_d>(v);
        EXPECT_EQ(v, convert<vector3d>(c)) << "Conversion result " << convert<vector3d>(c);
    }
}

TEST(Cylindrical, PolarConversion)
{
    using polar_d       = polar_coord<double>;
    using cylindrical_d = cylindrical_coord<double>;

    {
        polar_d       p{1, 45_deg};
        cylindrical_d c{1, 45_deg, 0};

        EXPECT_EQ(c, convert<cylindrical_d>(p));
        EXPECT_EQ(p, convert<polar_d>(c));
    }
}

TEST(Cylindrical, SphericalConversion)
{
    using cylindrical_d = cylindrical_coord<double>;
    using spherical_d   = spherical_coord<double>;

    {
        cylindrical_d c{sqrt(2) / 2, 45_deg, sqrt(2) / 2};
        spherical_d   s{1, 45_deg, 45_deg};

        EXPECT_EQ(c, convert<cylindrical_d>(s))
            << "Conversion result " << convert<cylindrical_d>(s);
        EXPECT_EQ(s, convert<spherical_d>(c)) << "Conversion result " << convert<spherical_d>(c);
    }
    {
        cylindrical_d c{sqrt(2) / 2, 45_deg, -sqrt(2) / 2};
        spherical_d   s{1, -45_deg, 45_deg};

        EXPECT_EQ(c, convert<cylindrical_d>(s))
            << "Conversion result " << convert<cylindrical_d>(s);
        EXPECT_EQ(s, convert<spherical_d>(c)) << "Conversion result " << convert<spherical_d>(c);
    }
}

} /* namespace test */
} /* namespace math */
} /* namespace psst */
