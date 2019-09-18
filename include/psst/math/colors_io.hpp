/**
 * Copyright 2019 Sergei A. Fedorov
 * colors_io.hpp
 *
 *  Created on: Sep 18, 2019
 *      Author: ser-fedorov
 */

#ifndef PSST_MATH_INCLUDE_PSST_MATH_COLORS_IO_HPP_
#define PSST_MATH_INCLUDE_PSST_MATH_COLORS_IO_HPP_

#include <psst/math/colors.hpp>
#include <psst/math/vector_io.hpp>

namespace psst::math {

inline std::ostream&
operator<<(std::ostream& os, color::rgba_hex const& val)
{
    // clang-format off
        os << '#' << std::hex << std::setfill('0')
           << std::setw(2) << (unsigned int)val.red()
           << std::setw(2) << (unsigned int)val.green()
           << std::setw(2) << (unsigned int)val.blue()
           << std::setw(2) << (unsigned int)val.alpha()
           << std::dec
        ;
    // clang-format on
    return os;
}

inline std::ostream&
operator<<(std::ostream& os, color::rgb_hex const& val)
{
    // clang-format off
        os << '#' << std::hex << std::setfill('0')
           << std::setw(2) << (unsigned int)val.red()
           << std::setw(2) << (unsigned int)val.green()
           << std::setw(2) << (unsigned int)val.blue()
           << std::dec
        ;
    // clang-format on
    return os;
}

// TODO Generalize input for colors
inline std::istream&
operator>>(std::istream& is, color::rgba_hex& val)
{
    if (is.peek() != '#') {
        is.setstate(std::ios_base::failbit);
        return is;
    }
    char c;
    is.get(c);
    color::rgba_hex tmp;
    for (auto i = 0; i < color::rgba_hex::size; ++i) {
        char hi, lo;
        if (is.get(hi) && is.get(lo)) {
            if (!std::isxdigit(hi) || !std::isxdigit(lo)) {
                is.setstate(std::ios_base::failbit);
                return is;
            }
            int h  = (hi >= 'A' ? (hi >= 'a') ? (hi - 'a' + 10) : (hi - 'A' + 10) : (hi - '0'));
            int l  = (lo >= 'A' ? (lo >= 'a') ? (lo - 'a' + 10) : (lo - 'A' + 10) : (lo - '0'));
            tmp[i] = (h << 4) | l;
        }
    }
    val = tmp;
    return is;
}

inline std::istream&
operator>>(std::istream& is, color::rgb_hex& val)
{
    if (is.peek() != '#') {
        is.setstate(std::ios_base::failbit);
        return is;
    }
    char c;
    is.get(c);
    color::rgb_hex tmp;
    for (auto i = 0; i < color::rgb_hex::size; ++i) {
        char hi, lo;
        if (is.get(hi) && is.get(lo)) {
            if (!std::isxdigit(hi) || !std::isxdigit(lo)) {
                is.setstate(std::ios_base::failbit);
                return is;
            }
            int h  = (hi >= 'A' ? (hi >= 'a') ? (hi - 'a' + 10) : (hi - 'A' + 10) : (hi - '0'));
            int l  = (lo >= 'A' ? (lo >= 'a') ? (lo - 'a' + 10) : (lo - 'A' + 10) : (lo - '0'));
            tmp[i] = (h << 4) | l;
        }
    }
    val = tmp;
    return is;
}

}    // namespace psst::math

#endif /* PSST_MATH_INCLUDE_PSST_MATH_COLORS_IO_HPP_ */
