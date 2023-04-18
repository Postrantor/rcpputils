// Copyright 2021 Open Source Robotics Foundation, Inc.
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

#ifndef RCPPUTILS__TIME_HPP_
#define RCPPUTILS__TIME_HPP_

#include <chrono>

#include "rcutils/time.h"

namespace rcpputils
{

/// 转换为 std::chrono::nanoseconds。
/// Convert to std::chrono::nanoseconds.
/**
 * 此函数用于将 std::chrono::duration 转换为 std::chrono::nanoseconds，如果在转换过程中发生溢出，则抛出异常。
 * This function helps to convert from std::chrono::duration to std::chrono::nanoseconds and throw
 * exception if overflow occurs while converting.
 *
 * \param[in] time 需要转换为 std::chrono::nanoseconds 的时间。
 * \param[in] time The time to be converted to std::chrono::nanoseconds.
 * \return std::chrono::nanoseconds。
 * \return std::chrono::nanoseconds.
 * \throws std::invalid_argument 如果时间大于 std::chrono::nanoseconds::max() 或小于 std::chrono::nanoseconds::min()。
 * \throws std::invalid_argument if time is bigger than std::chrono::nanoseconds::max() or less than
 * std::chrono::nanoseconds::min().
 */
template <typename DurationRepT, typename DurationT>
std::chrono::nanoseconds
convert_to_nanoseconds(const std::chrono::duration<DurationRepT, DurationT> & time)
{
  // 定义一个常量 ns_max_as_double，表示 std::chrono::nanoseconds 最大值的双精度浮点数形式。
  // Define a constant ns_max_as_double, which represents the double-precision floating-point form of the maximum value of std::chrono::nanoseconds.
  constexpr auto ns_max_as_double =
    std::chrono::duration_cast<std::chrono::duration<double, std::chrono::nanoseconds::period>>(
      std::chrono::nanoseconds::max());

  // 如果输入的时间大于 ns_max_as_double，则抛出 std::invalid_argument 异常。
  // If the input time is greater than ns_max_as_double, throw a std::invalid_argument exception.
  if (time > ns_max_as_double) {
    throw std::invalid_argument{"time must be less than std::chrono::nanoseconds::max()"};
  }

  // 定义一个常量 ns_min_as_double，表示 std::chrono::nanoseconds 最小值的双精度浮点数形式。
  // Define a constant ns_min_as_double, which represents the double-precision floating-point form of the minimum value of std::chrono::nanoseconds.
  constexpr auto ns_min_as_double =
    std::chrono::duration_cast<std::chrono::duration<double, std::chrono::nanoseconds::period>>(
      std::chrono::nanoseconds::min());

  // 如果输入的时间小于 ns_min_as_double，则抛出 std::invalid_argument 异常。
  // If the input time is less than ns_min_as_double, throw a std::invalid_argument exception.
  if (time < ns_min_as_double) {
    throw std::invalid_argument{"time must be bigger than std::chrono::nanoseconds::min()"};
  }

  // 将输入的时间转换为 std::chrono::nanoseconds 类型，并返回结果。
  // Convert the input time to std::chrono::nanoseconds type and return the result.
  return std::chrono::duration_cast<std::chrono::nanoseconds>(time);
}

} // namespace rcpputils

#endif // RCPPUTILS__TIME_HPP_
