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

/*
 * Author: Víctor López
 */

#ifndef RCPPUTILS__ROLLING_MEAN_ACCUMULATOR_HPP_
#define RCPPUTILS__ROLLING_MEAN_ACCUMULATOR_HPP_

#include <cassert>
#include <cstddef>
#include <vector>

namespace rcpputils
{

/// 计算最后累积元素的平均值。
/// Computes the mean of the last accumulated elements.
/**
 * 这是一个简化版的 boost 滚动平均累加器，为了避免引入 boost 依赖而编写。
 * This is a simplified version of boost's rolling mean accumulator,
 * written to avoid dragging in boost dependencies.
 */
template <typename T> class RollingMeanAccumulator
{
public:
  /// 构造具有指定窗口大小的滚动平均累加器。
  /// Constructs the rolling mean accumulator with a specified window size.
  /**
   * \param[in] rolling_window_size 累加器窗口长度的无符号整数。
   * \param[in] rolling_window_size The unsigned integral length of the accumulator's window length.
   */
  explicit RollingMeanAccumulator(size_t rolling_window_size)
  : buffer_(rolling_window_size, 0.0), next_insert_(0), sum_(0.0), buffer_filled_(false)
  {
  }

  /// 在累加器的缓冲区中收集提供的值。
  /// Collects the provided value in the accumulator's buffer.
  /**
   * \param[in] val 要累积的值。
   * \param[in] val The value to accumulate.
   */
  void accumulate(T val)
  {
    sum_ -= buffer_[next_insert_];
    sum_ += val;
    buffer_[next_insert_] = val;
    next_insert_++;
    buffer_filled_ |= next_insert_ >= buffer_.size();
    next_insert_ = next_insert_ % buffer_.size();
  }

  /// 计算迄今为止累积的滚动平均值。
  /// Calculates the rolling mean accumulated insofar.
  /**
   * \return 累积值的滚动平均值。
   * \return Rolling mean of the accumulated values.
   */
  T getRollingMean() const
  {
    size_t valid_data_count = buffer_filled_ * buffer_.size() + !buffer_filled_ * next_insert_;
    assert(valid_data_count > 0);
    return sum_ / valid_data_count;
  }

private:
  std::vector<T>
    buffer_; // 缓冲区，用于存储要累积的值。Buffer for storing the values to be accumulated.
  size_t next_insert_; // 下一个要插入的缓冲区位置。Next position in the buffer to insert.
  T sum_;              // 累加器的总和。Sum of the accumulator.
  bool
    buffer_filled_; // 标志，表示缓冲区是否已填满。Flag indicating whether the buffer is filled or not.
};

} // namespace rcpputils

#endif // RCPPUTILS__ROLLING_MEAN_ACCUMULATOR_HPP_
