// Copyright 2015-2020 Open Source Robotics Foundation, Inc.
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

#ifndef RCPPUTILS__SCOPE_EXIT_HPP_
#define RCPPUTILS__SCOPE_EXIT_HPP_

#include <utility>

#include "rcutils/macros.h"

namespace rcpputils
{

/**
 * @brief 一个用于在作用域结束时自动调用指定函数的模板类 (A template class for automatically calling a specified function when the scope ends)
 *
 * @tparam CallableT 可调用对象类型 (Callable object type)
 */
template <typename CallableT>
struct scope_exit final
{
  /**
   * @brief 构造函数，接收可调用对象 (Constructor, accepts callable object)
   *
   * @param callable 可调用对象 (Callable object)
   */
  explicit scope_exit(CallableT && callable) : callable_(std::forward<CallableT>(callable)) {}

  // 禁止拷贝构造 (Disallow copy construction)
  scope_exit(const scope_exit &) = delete;
  // 允许移动构造 (Allow move construction)
  scope_exit(scope_exit &&) = default;

  // 禁止拷贝赋值 (Disallow copy assignment)
  scope_exit & operator=(const scope_exit &) = delete;
  // 允许移动赋值 (Allow move assignment)
  scope_exit & operator=(scope_exit &&) = default;

  /**
   * @brief 析构函数，在析构时调用可调用对象 (Destructor, calls the callable object upon destruction)
   */
  ~scope_exit()
  {
    if (!cancelled_) {
      callable_();
    }
  }

  /**
   * @brief 取消在作用域结束时调用可调用对象 (Cancel calling the callable object when the scope ends)
   */
  void cancel() { cancelled_ = true; }

private:
  CallableT callable_;           // 可调用对象 (Callable object)
  bool cancelled_{false};        // 是否取消调用标志 (Whether to cancel the call flag)
};

/**
 * @brief 用于创建 scope_exit 实例的工厂函数 (A factory function for creating scope_exit instances)
 *
 * @tparam CallableT 可调用对象类型 (Callable object type)
 * @param callable 可调用对象 (Callable object)
 * @return scope_exit<CallableT> 返回一个 scope_exit 实例 (Returns a scope_exit instance)
 */
template <typename CallableT>
scope_exit<CallableT> make_scope_exit(CallableT && callable)
{
  return scope_exit<CallableT>(std::forward<CallableT>(callable));
}

} // namespace rcpputils

#define RCPPUTILS_SCOPE_EXIT(code)                                                                 \
  auto RCUTILS_JOIN(scope_exit_, __LINE__) = rcpputils::make_scope_exit([&]() { code; })

#endif // RCPPUTILS__SCOPE_EXIT_HPP_
