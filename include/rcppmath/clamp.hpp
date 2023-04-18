// Copyright 2020 PAL Robotics S.L.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*! \file clamp.hpp
 *  \brief Restrict a value between two bounds.
 */

#ifndef RCPPMATH__CLAMP_HPP_
#define RCPPMATH__CLAMP_HPP_

#include <cassert>

namespace rcppmath
{
/**
 * 如果v小于lo，则返回lo；否则，如果hi小于v，则返回hi；否则，返回v。
 * 使用operator <比较值。
 * If v compares less than lo, returns lo; otherwise if hi compares less
 * than v, returns hi; otherwise returns v. Uses operator< to compare the values.
 *
 * \param[in] v 要限制的值。The value to clamp.
 * \param[in] lo 下界。The lower boundary.
 * \param[in] hi 上界。The higher boundary.
 * \return 如果v小于lo，则返回对lo的引用；如果hi小于v，则返回对hi的引用；否则，返回对v的引用。
 *         Reference to lo if v is less than lo, reference to hi if hi is less than v, otherwise
 *         reference to v.
 * \note 实现来自https://en.cppreference.com/w/cpp/algorithm/clamp。
 *       Implementation from https://en.cppreference.com/w/cpp/algorithm/clamp.
 * \warning 如果其中一个参数是右值，并通过引用捕获clamp的结果，如果返回该参数，则会产生悬空引用。
 *          Capturing the result of clamp by reference if one of the parameters is rvalue produces
 *          a dangling reference if that parameter is returned.
 */
template <class T>
[[deprecated("use std::clamp instead, introduced in C++17")]] constexpr const T &
clamp(const T & v, const T & lo, const T & hi)
{
  // 断言hi不小于lo，确保有效的边界条件。
  // Assert that hi is not less than lo, ensuring valid boundary conditions.
  assert(!(hi < lo));

  // 使用三元操作符返回适当的边界值。
  // Use ternary operators to return the appropriate boundary value.
  return (v < lo) ? lo : (hi < v) ? hi : v;
}

/**
 * 使用提供的 Comparison 对象 (comp) 进行夹紧。
 * Performs clamping with a provided Comparison object (comp).
 *
 * \param[in] v 要夹紧的值。The value to clamp.
 * \param[in] lo 较低边界。The lower boundary.
 * \param[in] hi 较高边界。The higher boundary.
 * \param[in] comp 比较对象，如果第一个参数小于第二个参数，则返回 true。Comparison object that returns true if the first argument is less than the second.
 * \return 如果 v 小于 lo，则返回 lo 的引用；如果 hi 小于 v，则返回 hi 的引用；否则返回 v 的引用。"小于"语义由比较对象确定。Reference to lo if v is less than lo, reference to hi if hi is less than v, otherwise reference to v. "Less than" semantics determined by Comparison object.
 * \warning 如果捕获 clamp 的结果为引用，并且其中一个参数为 rvalue，则在返回该参数时会产生悬空引用。Capturing the result of clamp by reference if one of the parameters is rvalue produces a dangling reference if that parameter is returned.
 * \sa rcppmath::clamp(const T&, const T&, const T&)
 */
template <class T, class Compare>
[[deprecated("use std::clamp instead, introduced in C++17")]] constexpr const T &
clamp(const T & v, const T & lo, const T & hi, Compare comp)
{
  // 断言：hi 不小于 lo。Assert that hi is not less than lo.
  assert(!comp(hi, lo));

  // 如果 v 小于 lo，则返回 lo 的引用；如果 hi 小于 v，则返回 hi 的引用；否则返回 v 的引用。"小于"语义由比较对象确定。
  // Return reference to lo if v is less than lo, reference to hi if hi is less than v, otherwise reference to v. "Less than" semantics determined by Comparison object.
  return comp(v, lo) ? lo : comp(hi, v) ? hi : v;
}

} // namespace rcppmath

#endif // RCPPMATH__CLAMP_HPP_
