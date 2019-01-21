/*
 * matrix_fwd.hpp
 *
 *  Created on: 18 дек. 2016 г.
 *      Author: sergey.fedorov
 */

#ifndef PSST_MATH_MATRIX_FWD_HPP_
#define PSST_MATH_MATRIX_FWD_HPP_

#include <psst/math/components.hpp>

namespace psst {
namespace math {

template <typename T, size_t RC, size_t CC,
          typename Components = components::default_components_t<(CC > RC) ? CC : RC>>
struct matrix;

} /* namespace math */
} /* namespace psst */

#endif /* PSST_MATH_MATRIX_FWD_HPP_ */
