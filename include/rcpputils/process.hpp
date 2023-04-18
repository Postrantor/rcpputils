// Copyright 2020 Open Source Robotics Foundation, Inc.
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

#ifndef RCPPUTILS__PROCESS_HPP_
#define RCPPUTILS__PROCESS_HPP_

#include <rcutils/process.h>

#include <string>

namespace rcpputils
{

/// 获取当前可执行文件的名称。
/// Retrieve the current executable name.
/**
 * 此函数便携式地获取当前程序名称并返回其副本。
 * This function portably retrieves the current program name and returns
 * a copy of it.
 *
 * 此函数是线程安全的。
 * This function is thread-safe.
 *
 * \return 程序名称。
 * \return The program name.
 * \throws std::runtime_error 发生错误时抛出
 * \throws std::runtime_error on error
 */
std::string get_executable_name()
{
  // 获取默认分配器
  // Get the default allocator
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  // 使用分配器获取可执行文件名
  // Get the executable name using the allocator
  char * executable_name = rcutils_get_executable_name(allocator);

  // 如果获取失败，抛出运行时错误
  // If retrieval fails, throw a runtime error
  if (nullptr == executable_name) {
    throw std::runtime_error("Failed to get executable name");
  }

  // 将可执行文件名转换为 std::string 类型
  // Convert the executable name to an std::string type
  std::string ret(executable_name);

  // 释放分配的内存
  // Deallocate the allocated memory
  allocator.deallocate(executable_name, allocator.state);

  // 返回可执行文件名
  // Return the executable name
  return ret;
}

} // namespace rcpputils

#endif // RCPPUTILS__PROCESS_HPP_
