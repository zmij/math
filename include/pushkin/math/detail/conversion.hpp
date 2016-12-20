/*
 * conversion.hpp
 *
 *  Created on: 20 дек. 2016 г.
 *      Author: sergey.fedorov
 */

#ifndef PUSHKIN_MATH_DETAIL_CONVERSION_HPP_
#define PUSHKIN_MATH_DETAIL_CONVERSION_HPP_

#include <pushkin/math/vector_fwd.hpp>

namespace psst {
namespace math {

namespace cvt {

template < typename T, typename U >
struct conversion;

template < typename T, ::std::size_t Cartesian >
struct conversion< vector<T, 2, axes::polar>, vector<T, Cartesian, axes::xyzw> > {
    using source_type = vector<T, 2, axes::polar>;
    using target_type = vector<T, Cartesian, axes::xyzw >;

    static_assert(target_type::size >= 2,
        "Target vector for conversion from polar coordinates must have at least 2 components");

    static target_type
    apply(source_type const& src)
    {
        auto const& phi = src.phi();
        auto const& rho = src.rho();
        return {rho * ::std::cos(phi), rho * ::std::sin(phi)};
    }
};

template < typename T, ::std::size_t Cartesian >
struct conversion< vector<T, Cartesian, axes::xyzw>, vector<T, 2, axes::polar> > {
    using source_type = vector<T, Cartesian, axes::xyzw >;
    using target_type = vector<T, 2, axes::polar>;

    static_assert(source_type::size >= 2,
        "Target vector for conversion to polar coordinates must have at lease 2 components");

    static target_type
    apply(source_type const& src)
    {
        return {src.magnitude(), ::std::atan2(src.y(), src.x())};
    }
};

}  /* namespace convert */

template < typename Target, typename Source >
Target
convert(Source const& src)
{
    return cvt::conversion<Source, Target>::apply(src);
}

}  /* namespace math */
}  /* namespace psst */

#endif /* PUSHKIN_MATH_DETAIL_CONVERSION_HPP_ */
