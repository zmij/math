/**
 * Copyright 2019 Sergei A. Fedorov
 * cylindrical_coord.hpp
 *
 *  Created on: Jan 5, 2019
 *      Author: ser-fedorov
 */

#ifndef PSST_MATH_CYLINDRICAL_COORD_HPP_
#define PSST_MATH_CYLINDRICAL_COORD_HPP_

#include <psst/math/angles.hpp>
#include <psst/math/detail/conversion.hpp>
#include <psst/math/detail/vector_expressions.hpp>

namespace psst {
namespace math {
namespace components {

struct cylindrical {
    static constexpr std::size_t min_components = 3;
    static constexpr std::size_t max_components = 3;
    static constexpr std::size_t rho            = 0;
    static constexpr std::size_t phi            = 1;
    static constexpr std::size_t z              = 2;
    static constexpr std::size_t r              = rho;
    static constexpr std::size_t azimuth        = phi;
    static constexpr std::size_t elevation      = z;

    using value_policies = utils::template_tuple<math::value_policy::no_change,
                                                 math::value_policy::clamp_zero_to_two_pi,
                                                 math::value_policy::no_change>;
};
}    // namespace components

namespace component_access {
//@{
/** @name Cylindrical coordinates */
template <typename VectorType, typename T>
struct component_access<3, components::cylindrical, VectorType, T>
    : basic_component_access<VectorType, T, components::cylindrical> {
    using base_type = basic_component_access<VectorType, T, components::cylindrical>;

    PSST_MATH_COMPONENT_ACCESS(r);
    PSST_MATH_COMPONENT_ACCESS(rho);
    PSST_MATH_COMPONENT_ACCESS(phi);
    PSST_MATH_COMPONENT_ACCESS(azimuth);
    PSST_MATH_COMPONENT_ACCESS(z);
    PSST_MATH_COMPONENT_ACCESS(elevation);
};
//@}

}    // namespace component_access

namespace expr {
inline namespace v {
//@{
/** @name Vector scalar muliply for spherical coordinates */
template <typename LHS, typename RHS>
struct vector_scalar_multiply<components::cylindrical, LHS, RHS>
    : binary_vector_expression_components<vector_scalar_multiply, components::cylindrical, LHS, RHS>,
      binary_expression<LHS, RHS> {
    using base_type
        = binary_vector_expression_components<vector_scalar_multiply, components::cylindrical, LHS, RHS>;
    using value_type = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr value_type
    at() const
    {
        static_assert(N < base_type::size, "Vector multiply element index is out of range");
        if constexpr (N == components::cylindrical::rho || N == components::cylindrical::elevation) {
            // In cylindrical coordinates only rho and elevation is multiplied
            return this->lhs_.template at<N>() * this->rhs_;
        } else {
            return this->lhs_.template at<N>();
        }
    }
};

//@}
//@{
/** @name Vector scalar division for spherical coordinates */
template <typename LHS, typename RHS>
struct vector_scalar_divide<components::cylindrical, LHS, RHS>
    : binary_vector_expression_components<vector_scalar_divide, components::cylindrical, LHS, RHS>,
      binary_expression<LHS, RHS> {
    using base_type
        = binary_vector_expression_components<vector_scalar_divide, components::cylindrical, LHS, RHS>;
    using value_type = typename base_type::value_type;

    using expression_base = binary_expression<LHS, RHS>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr value_type
    at() const
    {
        static_assert(N < base_type::size, "Vector divide element index is out of range");
        if constexpr (N == components::cylindrical::rho || N == components::cylindrical::elevation) {
            // In cylindrical coordinates only rho and elevation is divided
            return this->lhs_.template at<N>() / this->rhs_;
        } else {
            return this->lhs_.template at<N>();
        }
    }
};
//@}
//@{
/** @name Magnitude squared for spherical coordinates */
template <typename Expr>
struct vector_magnitude_squared<components::cylindrical, Expr>
    : scalar_expression<vector_magnitude_squared<components::cylindrical, Expr>,
                        traits::scalar_expression_result_t<Expr>>,
      unary_expression<Expr> {
    static_assert(traits::is_vector_expression_v<Expr>, "Argument to magnitude must be a vector");

    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    constexpr auto
    value() const
    {
        return this->arg_.rho() * this->arg_.rho()
               + this->arg_.elevation() * this->arg_.elevation();
    }
};

//@}

//@{
template <typename Expr>
struct vector_normalize<components::cylindrical, Expr>
    : unary_vector_expression_components<vector_normalize, components::cylindrical, Expr>,
      unary_expression<Expr> {
    using base_type       = unary_vector_expression_components<vector_normalize, components::cylindrical, Expr>;
    using value_type      = typename base_type::value_type;
    using expression_base = unary_expression<Expr>;
    using expression_base::expression_base;

    template <std::size_t N>
    constexpr auto
    at() const
    {
        static_assert(N < base_type::size, "Vector normalize component index is out of range");
        if constexpr (N == components::cylindrical::rho || N == components::cylindrical::elevation) {
            return this->arg_.template at<N>() / magnitude(this->arg_);
        } else {
            return this->arg_.template at<N>();
        }
    }
};
//@}

}    // namespace v
}    // namespace expr

template <typename T>
using cylindrical_coord = vector<T, 3, components::cylindrical>;

}    // namespace math
}    // namespace psst

#endif /* PSST_MATH_CYLINDRICAL_COORD_HPP_ */
