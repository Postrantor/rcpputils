// Copyright 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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

/*! \file asserts.hpp
 * \brief Assertion-like exceptions for halting tests.
 */

#ifndef RCPPUTILS__ASSERTS_HPP_
#define RCPPUTILS__ASSERTS_HPP_

#include <exception>
#include <stdexcept>
#include <string>

#include "rcpputils/visibility_control.hpp"

// Needed to disable compiler warning for exporting a class that extends a
// non-DLL-interface class.
// This should be fine since its extending an STL class.
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4251)
#pragma warning(disable : 4275)
#endif

namespace rcpputils
{

/**
 * \brief 一个类似断言的异常，用于在条件不满足时暂停测试。
 * \brief An assertion-like exception for halting tests if conditions are not met.
 */
class RCPPUTILS_PUBLIC AssertionException : public std::exception
{
  // 用于存储异常消息的字符串
  // A string used to store the exception message
  std::string msg_;

public:
  /**
   * \brief AssertionException 构造函数
   * \brief Constructor for AssertionException
   *
   * \param[in] msg 当抛出此异常时要显示的消息。
   * \param[in] msg The message to display when this exception is thrown.
   */
  explicit AssertionException(const char * msg);

  /**
   * \brief 获取抛出此异常的原因的消息描述。
   * \brief Get the message description of why this exception was thrown.
   *
   * \return 字符串消息
   * \return The string message
   */
  virtual const char * what() const noexcept;
};

/**
 * \brief 一个在状态检查失败时抛出的异常。
 * \brief An exception to be thrown when a state check fails.
 */
class RCPPUTILS_PUBLIC IllegalStateException : public std::exception
{
  // 存储异常消息的字符串变量
  // A string variable to store the exception message
  std::string msg_;

public:
  /**
   * \brief IllegalStateException 的构造函数
   * \brief Constructor for IllegalStateException
   *
   * \param[in] msg 当此异常被抛出时显示的消息。
   * \param[in] msg The message to display when this exception is thrown.
   */
  explicit IllegalStateException(const char * msg);

  /**
   * \brief 获取抛出此异常的原因的消息描述。
   * \brief Get the message description of why this exception was thrown.
   *
   * \return 字符串消息
   * \return The string message
   */
  virtual const char * what() const noexcept;
};

/**
 * \brief 检查参数条件是否通过。
 * \brief Check that an argument condition passes.
 *
 * \param[in] condition 断言为真的条件
 * \param[in] condition condition that is asserted to be true
 * \param[in] msg 当条件为假时传递给异常的消息
 * \param[in] msg message to pass to exception when condition is false
 * \throw std::invalid_argument 如果条件不满足。
 * \throw std::invalid_argument if the condition is not met.
 */
inline void require_true(bool condition, const std::string & msg = "invalid argument passed")
{
  // 如果条件不满足，抛出 std::invalid_argument 异常
  // If the condition is not met, throw a std::invalid_argument exception
  if (!condition) {
    throw std::invalid_argument{msg};
  }
}

/**
 * \brief 检查状态条件是否通过。
 * \brief Check that a state condition passes.
 *
 * \param[in] condition 要检查的条件，判断其是否为真
 * \param[in] condition condition to check whether it is true or not
 * \param[in] msg 当条件为假时传递给异常的消息
 * \param[in] msg message to pass to exception when condition is false
 * \throw rcpputils::IllegalStateException 如果条件不满足，则抛出此异常。
 * \throw rcpputils::IllegalStateException if the condition is not met.
 */
inline void check_true(bool condition, const std::string & msg = "check reported invalid state")
{
  // 如果条件不满足，抛出带有 msg 的 IllegalStateException 异常
  // If the condition is not met, throw an IllegalStateException with the provided msg
  if (!condition) {
    throw rcpputils::IllegalStateException{msg.c_str()};
  }
}

/**
 * \brief 断言条件通过。
 * \brief Assert that a condition passes.
 *
 * 与 assert 类似，但是它会抛出异常而不是调用 abort()。
 * This function behaves similar to assert, except that it throws instead of invoking abort().
 * 只有在未定义 NDEBUG 时才启用它
 * It is only enabled when NDEBUG is not defined
 *
 * \param[in] condition 要检查的条件，判断其是否为真
 * \param[in] condition condition to check whether it's true or not
 * \param[in] msg 当条件不满足时传递给异常的消息
 * \param[in] msg message to pass to exception when condition is not met.
 * \throw rcpputils::AssertionException 如果 NDEBUG 宏未设置且条件不满足，则抛出此异常。
 * \throw rcpputils::AssertionException if the macro NDEBUG is not set and the condition is not met.
 */
inline void assert_true(bool condition, const std::string & msg = "assertion failed")
{
// 使用与 cassert 相同的宏定义
// Same macro definition used by cassert
#ifndef NDEBUG
  // 如果条件不满足，抛出带有 msg 的 AssertionException 异常
  // If the condition is not met, throw an AssertionException with the provided msg
  if (!condition) {
    throw rcpputils::AssertionException{msg.c_str()};
  }
#else
  // 当 NDEBUG 定义时，忽略条件和消息参数
  // Ignore the condition and msg parameters when NDEBUG is defined
  (void)condition;
  (void)msg;
#endif
}
} // namespace rcpputils

#ifdef _WIN32
#pragma warning(pop)
#endif

#endif // RCPPUTILS__ASSERTS_HPP_
