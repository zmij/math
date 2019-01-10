/**
 * Copyright 2019 Sergei A. Fedorov
 * random.hpp
 *
 *  Created on: Jan 10, 2019
 *      Author: ser-fedorov
 */

#ifndef PUSHKIN_MATH_RANDOM_HPP_
#define PUSHKIN_MATH_RANDOM_HPP_

#include <pushkin/math/detail/matrix_expressions.hpp>
#include <pushkin/math/detail/vector_expressions.hpp>

#include <chrono>
#include <random>

namespace psst {
namespace math {

namespace expr {

inline namespace v {

template <typename T, typename Distribution, typename Engine = std::mt19937_64>
struct random_vector_generator : vector_expression<random_vector_generator<T, Distribution, Engine>,
                                                   vector<T, 0, axes::none>> {
    using base_type  = vector_expression<random_vector_generator<T, Distribution, Engine>,
                                        vector<T, 0, axes::none>>;
    using value_type = typename base_type::value_type;

    static constexpr std::size_t size = utils::npos_v;

    using engine_type       = Engine;
    using distribution_type = Distribution;

    random_vector_generator(distribution_type const& d) : gen_{std::random_device()()}, dist_{d} {}

    template <std::size_t N>
    constexpr value_type
    at() const
    {
        return dist_(gen_);
    }

private:
    mutable engine_type       gen_;
    mutable distribution_type dist_;
};

}    // namespace v

inline namespace m {

template <typename T, typename Distribution, typename Engine = std::mt19937_64>
struct random_matrix_generator : matrix_expression<random_matrix_generator<T, Distribution, Engine>,
                                                   matrix<T, 0, 0, axes::none>> {
    using base_type  = matrix_expression<random_matrix_generator<T, Distribution, Engine>,
                                        matrix<T, 0, 0, axes::none>>;
    using value_type = typename base_type::value_type;

    static constexpr auto rows = utils::npos_v;
    static constexpr auto cols = utils::npos_v;
    static constexpr auto size = utils::npos_v;

    using engine_type       = Engine;
    using distribution_type = Distribution;

    random_matrix_generator(distribution_type const& d) : gen_{std::random_device()()}, dist_{d} {}

    template <std::size_t R, std::size_t C>
    constexpr value_type
    element() const
    {
        return dist_(gen_);
    }

private:
    mutable engine_type       gen_;
    mutable distribution_type dist_;
};

template <std::size_t RN, typename T, typename Distribution, typename Engine>
struct nth_row<random_matrix_generator<T, Distribution, Engine>, RN>
    : vector_expression<nth_row<random_matrix_generator<T, Distribution, Engine>, RN>,
                        vector<T, 0, axes::none>>,
      unary_expression<random_matrix_generator<T, Distribution, Engine>> {
    using base_type
        = vector_expression<nth_row<random_matrix_generator<T, Distribution, Engine>, RN>,
                            vector<T, 0, axes::none>>;
    using value_type = typename base_type::value_type;

    static constexpr std::size_t size = utils::npos_v;

    using expression_base = unary_expression<random_matrix_generator<T, Distribution, Engine>>;
    using expression_base::expression_base;

    template <std::size_t CN>
    constexpr auto
    at() const
    {
        return this->arg_.template element<RN, CN>();
    }
};

}    // namespace m
}    // namespace expr

template <typename T, typename Distribution, typename Engine = std::mt19937_64>
constexpr auto
random_vector_data(Distribution const& d)
{
    return expr::v::random_vector_generator<T, Distribution, Engine>{d};
}

template <typename T, typename Distribution, typename Engine = std::mt19937_64>
constexpr auto
random_matrix_data(Distribution const& d)
{
    return expr::m::random_matrix_generator<T, Distribution, Engine>{d};
}

}    // namespace math
}    // namespace psst

#endif /* PUSHKIN_MATH_RANDOM_HPP_ */
