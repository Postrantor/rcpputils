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

#ifndef RCPPMATH__ROLLING_MEAN_ACCUMULATOR_HPP_
#define RCPPMATH__ROLLING_MEAN_ACCUMULATOR_HPP_

#include "rcpputils/rolling_mean_accumulator.hpp"

// 警告: rcppmath 命名空间已被弃用，请改为包含 rcpputils/rolling_mean_accumulator.hpp
// Warning: the rcppmath namespace is deprecated, include rcpputils/rolling_mean_accumulator.hpp instead
#warning "the rcppmath namespace is deprecated, include rcpputils/rolling_mean_accumulator.hpp instead"

// 声明 rcppmath 命名空间
// Declare the rcppmath namespace
namespace rcppmath
{

// 使用模板类定义 RollingMeanAccumulator 类型，将其标记为已弃用，并建议使用 rcpputils::RollingMeanAccumulator 替代
// Define the RollingMeanAccumulator type using a template class, mark it as deprecated and suggest using rcpputils::RollingMeanAccumulator instead
template <typename T>
using RollingMeanAccumulator [[deprecated("use rcpputils::RollingMeanAccumulator instead")]] =
  rcpputils::RollingMeanAccumulator<T>;

} // 结束 rcppmath 命名空间
  // End of the rcppmath namespace

#endif // RCPPMATH__ROLLING_MEAN_ACCUMULATOR_HPP_
