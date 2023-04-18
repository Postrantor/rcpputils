// Copyright (c) 2020, Open Source Robotics Foundation, Inc.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the copyright holder nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <stdexcept>
#include <string>

#include "rcutils/env.h"
#include "rcutils/error_handling.h"

#include "rcpputils/env.hpp"

namespace rcpputils
{

/**
 * @brief 获取环境变量的值 (Get the value of an environment variable)
 * 
 * @param env_var 环境变量的名称 (Name of the environment variable)
 * @return std::string 环境变量的值 (Value of the environment variable)
 */
std::string get_env_var(const char * env_var)
{
  const char *
    value{}; // 初始化一个指向常量字符的指针，用于存储环境变量的值 (Initialize a pointer to constant character for storing the value of the environment variable)
  const char * err = rcutils_get_env(
    env_var,
    &value); // 调用 rcutils_get_env 函数，获取环境变量的值 (Call rcutils_get_env function to get the value of the environment variable)
  if (err) { // 如果发生错误 (If an error occurs)
    throw std::runtime_error(err); // 抛出运行时错误异常 (Throw a runtime error exception)
  }
  return value
           ? value
           : ""; // 返回环境变量的值，如果值为空，则返回空字符串 (Return the value of the environment variable, if the value is empty, return an empty string)
}

/**
 * @brief 设置环境变量的值 (Set the value of an environment variable)
 * 
 * @param env_var 环境变量的名称 (Name of the environment variable)
 * @param env_value 环境变量的新值 (New value of the environment variable)
 * @return bool 设置成功返回 true，否则抛出异常 (Return true if the setting is successful, otherwise throw an exception)
 */
bool set_env_var(const char * env_var, const char * env_value)
{
  if (
    !rcutils_set_env(
      env_var,
      env_value)) { // 调用 rcutils_set_env 函数，设置环境变量的值 (Call rcutils_set_env function to set the value of the environment variable)
    std::string err =
      rcutils_get_error_string().str; // 获取错误信息字符串 (Get the error message string)
    // 由于已经提取了错误字符串，所以重置错误状态 (Resetting the error state since error string has been extracted)
    rcutils_reset_error();
    throw std::runtime_error(err); // 抛出运行时错误异常 (Throw a runtime error exception)
  }
  return true; // 设置成功，返回 true (Setting successful, return true)
}

} // namespace rcpputils
