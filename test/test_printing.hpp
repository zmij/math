/*
 * test_printing.hpp
 *
 *  Created on: Dec 27, 2018
 *      Author: ser-fedorov
 */

#ifndef TEST_PRINTING_HPP_
#define TEST_PRINTING_HPP_

#include <psst/math/matrix_io.hpp>
#include <psst/math/vector_io.hpp>

namespace psst {
namespace math {
namespace expr {

inline namespace v {

template <typename E, typename R>
void
PrintTo(vector_expression<E, R> const& exp, std::ostream* os)
{
    *os << io::pretty << exp << io::ugly;
}

}    // namespace v

inline namespace m {

template <typename E, typename R>
void
PrintTo(matrix_expression<E, R> const& mtx, std::ostream* os)
{
    *os << "\n" << io::pretty << mtx << io::ugly;
}

}    // namespace m

}    // namespace expr
}    // namespace math
}    // namespace psst

#endif /* TEST_PRINTING_HPP_ */
