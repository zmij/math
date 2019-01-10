/**
 * Copyright 2019 Sergei A. Fedorov
 * random.hpp
 *
 *  Created on: Jan 10, 2019
 *      Author: ser-fedorov
 */

#ifndef PUSHKIN_MATH_RANDOM_HPP_
#define PUSHKIN_MATH_RANDOM_HPP_

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
    engine_type mutable gen_;
    distribution_type mutable dist_;
};

}    // namespace v
}    // namespace expr

template <typename T, typename Distribution, typename Engine = std::mt19937_64>
constexpr auto
random_vector_data(Distribution const& d)
{
    return expr::v::random_vector_generator<T, Distribution, Engine>{d};
}

}    // namespace math
}    // namespace psst

#endif /* PUSHKIN_MATH_RANDOM_HPP_ */
