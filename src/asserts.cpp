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

#include "rcpputils/asserts.hpp"

namespace rcpputils
{
/**
 * @brief AssertionException 构造函数
 * @param msg 异常消息
 *
 * @brief AssertionException constructor
 * @param msg Exception message
 */
AssertionException::AssertionException(const char * msg)
{
  // 将输入的异常消息赋值给成员变量 msg_
  // Assign the input exception message to the member variable msg_
  msg_ = msg;
}

/**
 * @brief 获取异常信息
 * @return 异常信息字符串
 *
 * @brief Get exception information
 * @return Exception information string
 */
const char * AssertionException::what() const noexcept
{
  // 返回成员变量 msg_ 的 C 风格字符串
  // Return the C-style string of the member variable msg_
  return msg_.c_str();
}

/**
 * @brief IllegalStateException 构造函数
 * @param msg 异常消息
 *
 * @brief IllegalStateException constructor
 * @param msg Exception message
 */
IllegalStateException::IllegalStateException(const char * msg)
{
  // 将输入的异常消息赋值给成员变量 msg_
  // Assign the input exception message to the member variable msg_
  msg_ = msg;
}

/**
 * @brief 获取异常信息
 * @return 异常信息字符串
 *
 * @brief Get exception information
 * @return Exception information string
 */
const char * IllegalStateException::what() const noexcept
{
  // 返回成员变量 msg_ 的 C 风格字符串
  // Return the C-style string of the member variable msg_
  return msg_.c_str();
}
} // namespace rcpputils
