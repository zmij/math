/**
 * Copyright 2019 Sergei A. Fedorov
 * coordinate_conversion.hpp
 *
 *  Created on: Jan 13, 2019
 *      Author: ser-fedorov
 */

#ifndef PSST_MATH_COORDINATE_CONVERSION_HPP_
#define PSST_MATH_COORDINATE_CONVERSION_HPP_

#include <psst/math/cylindrical_coord.hpp>
#include <psst/math/detail/conversion.hpp>
#include <psst/math/polar_coord.hpp>
#include <psst/math/spherical_coord.hpp>
#include <psst/math/vector.hpp>

namespace psst::math {
namespace expr {

inline namespace v {

//@{
/** @name Polar to XYZW conversion */
template <typename T, typename U, std::size_t Cartesian, typename Expression>
struct conversion<vector<T, 2, components::polar>, vector<U, Cartesian, components::xyzw>,
                  Expression> : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        using std::cos;
        using std::sin;
        return vector<U, Cartesian, components::xyzw>{this->arg_.rho() * cos(this->arg_.phi()),
                                                      this->arg_.rho() * sin(this->arg_.phi())};
    }
};
//@}

//@{
/** @name XYZW to polar conversion */
template <typename T, typename U, std::size_t Cartesian, typename Expression>
struct conversion<vector<U, Cartesian, components::xyzw>, vector<T, 2, components::polar>,
                  Expression> : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        using std::atan2;
        using std::sqrt;
        auto mgt = sqrt(sum_of_squares(this->arg_.x(), this->arg_.y()));
        return vector<T, 2, components::polar>{mgt, atan2(this->arg_.y(), this->arg_.x())};
    }
};
//@}

//@{
/** @name Spherical to XYZW conversion */
template <typename T, typename U, std::size_t Cartesian, typename Expression>
struct conversion<vector<T, 3, components::spherical>, vector<U, Cartesian, components::xyzw>,
                  Expression> : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        using std::cos;
        using std::sin;
        auto projection_len = this->arg_.rho() * cos(this->arg_.phi());
        return vector<U, Cartesian, components::xyzw>{projection_len * cos(this->arg_.theta()),
                                                      projection_len * sin(this->arg_.theta()),
                                                      this->arg_.rho() * sin(this->arg_.phi())};
    }
};
//@}

//@{
/** @name XYZW to spherical conversion */
template <typename T, typename U, std::size_t Cartesian, typename Expression>
struct conversion<vector<U, Cartesian, components::xyzw>, vector<T, 3, components::spherical>,
                  Expression> : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        using std::asin;
        using std::atan2;
        using std::sqrt;
        auto mgt = sqrt(sum_of_squares(this->arg_.x(), this->arg_.y(), this->arg_.z()));
        T    inclination{0};
        if (this->arg_.z() != 0) {
            inclination = asin(this->arg_.z() / mgt);
        }
        return vector<T, 3, components::spherical>{mgt, inclination,
                                                   atan2(this->arg_.y(), this->arg_.x())};
    }
};
//@}

//@{
/** @name Spherical to polar conversion */
template <typename T, typename U, typename Expression>
struct conversion<vector<T, 3, components::spherical>, vector<U, 2, components::polar>, Expression>
    : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        using std::cos;
        return vector<U, 2, components::polar>{this->arg_.rho() * cos(this->arg_.phi()),
                                               this->arg_.azimuth()};
    }
};
//@}

//@{
/** @name Polar to spherical conversion */
template <typename T, typename U, typename Expression>
struct conversion<vector<T, 2, components::polar>, vector<U, 3, components::spherical>, Expression>
    : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        return vector<U, 3, components::spherical>{this->arg_.rho(), 0, this->arg_.azimuth()};
    }
};
//@}

//@{
// TODO Make all coord conversions function overloads instead of expressions
/** @name Cylindrical to XYZW conversion */
template <typename T, typename U, std::size_t Cartesian, typename Expression>
struct conversion<vector<T, 3, components::cylindrical>, vector<U, Cartesian, components::xyzw>,
                  Expression> : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        using std::cos;
        using std::sin;

        return vector<U, Cartesian, components::xyzw>{this->arg_.rho() * cos(this->arg_.phi()),
                                                      this->arg_.rho() * sin(this->arg_.phi()),
                                                      this->arg_.z()};
    }
};
//@}

//@{
/** @name XYZW to cylindrical conversion */
template <typename T, typename U, std::size_t Cartesian, typename Expression>
struct conversion<vector<U, Cartesian, components::xyzw>, vector<T, 3, components::cylindrical>,
                  Expression> : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        using std::atan2;
        using std::sqrt;
        return vector<T, 3, components::cylindrical>{
            sqrt(sum_of_squares(this->arg_.x(), this->arg_.y())),
            atan2(this->arg_.y(), this->arg_.x()), this->arg_.z()};
    }
};
//@}

//@{
/** @name Cylindrical to polar conversion */
template <typename T, typename U, typename Expression>
struct conversion<vector<T, 3, components::cylindrical>, vector<U, 2, components::polar>,
                  Expression> : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        return vector<U, 2, components::polar>{this->arg_.rho(), this->arg_.azimuth()};
    }
};
//@}

//@{
/** @name Polar to cylindrical conversion */
template <typename T, typename U, typename Expression>
struct conversion<vector<T, 2, components::polar>, vector<U, 3, components::cylindrical>,
                  Expression> : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        return vector<U, 3, components::cylindrical>{this->arg_.rho(), this->arg_.azimuth(), 0};
    }
};
//@}

//@{
/** @name Cylindrical to spherical conversion */
template <typename T, typename U, typename Expression>
struct conversion<vector<U, 3, components::cylindrical>, vector<T, 3, components::spherical>,
                  Expression> : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        using std::asin;
        using std::atan2;
        T mgt = magnitude(this->arg_);
        T inclination{0};
        if (mgt != 0) {
            inclination = asin(this->arg_.z() / mgt);
        }
        return vector<T, 3, components::spherical>{mgt, inclination, this->arg_.azimuth()};
    }
};
//@}

//@{
/** @name Spherical to cylindrical conversion */
template <typename T, typename U, typename Expression>
struct conversion<vector<T, 3, components::spherical>, vector<U, 3, components::cylindrical>,
                  Expression> : unary_expression<Expression> {
    using expression_base = unary_expression<Expression>;
    using expression_base::expression_base;

    constexpr auto
    result() const
    {
        using std::cos;
        using std::sin;
        return vector<U, 3, components::cylindrical>{this->arg_.rho() * cos(this->arg_.phi()),
                                                     this->arg_.azimuth(),
                                                     this->arg_.rho() * sin(this->arg_.phi())};
    }
};
//@}

}    // namespace v
}    // namespace expr
}    // namespace psst::math

#endif /* PSST_MATH_COORDINATE_CONVERSION_HPP_ */
