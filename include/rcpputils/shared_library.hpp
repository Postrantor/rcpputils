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

#ifndef RCPPUTILS__SHARED_LIBRARY_HPP_
#define RCPPUTILS__SHARED_LIBRARY_HPP_

#include <stdexcept>
#include <string>

#include "rcpputils/visibility_control.hpp"
#include "rcutils/shared_library.h"

namespace rcpputils
{

/**
 * 这个类是 rcutils 共享库的抽象，以便能够在现代 C++ 中使用它。
 * This class is an abstraction of rcutils shared library to be able to use it
 * with modern C++.
 **/
class SharedLibrary
{
public:
  /// 构造函数中加载库。
  /// The library is loaded in the constructor.
  /**
   * \param[in] library_path 库的字符串路径。
   * \throws std::bad_alloc 如果为回调分配存储空间失败
   * \throws std::runtime_error 如果存在一些无效参数或者库没有正确加载
   *
   * \param[in] library_path The library string path.
   * \throws std::bad_alloc if allocating storage for the callback fails
   * \throws std::runtime_error if there are some invalid arguments or the library
   * was not load properly
   */
  RCPPUTILS_PUBLIC
  explicit SharedLibrary(const std::string & library_path);

  /// 析构函数中卸载库。
  /// The library is unloaded in the destructor.
  RCPPUTILS_PUBLIC
  virtual ~SharedLibrary();

  /// 卸载库
  /// Unload library
  /**
  * \throws std::runtime_error 如果库没有正确卸载
  *
  * \throws std::runtime_error if the library is not unloaded properly
   */
  RCPPUTILS_PUBLIC
  void unload_library();

  /// 如果共享库包含特定的符号名称，则返回 true，否则返回 false。
  /// Return true if the shared library contains a specific symbol name, otherwise returns false.
  /**
   * \param[in] symbol_name 共享库内的符号名称
   * \return 如果符号存在则返回 true，否则返回 false。
   *
   * \param[in] symbol_name name of the symbol inside the shared library
   * \return if symbols exists returns true, otherwise returns false.
   */
  RCPPUTILS_PUBLIC
  bool has_symbol(const char * symbol_name);

  /**
   * \param[in] symbol_name 共享库内的符号名称
   * \return 如果符号存在则返回 true，否则返回 false。
   *
   * \param[in] symbol_name name of the symbol inside the shared library
   * \return if symbols exists returns true, otherwise returns false.
   */
  RCPPUTILS_PUBLIC
  bool has_symbol(const std::string & symbol_name);

  /// 返回共享库符号指针。
  /// Return shared library symbol pointer.
  /**
   * \param[in] symbol_name 共享库内的符号名称
   * \return 共享库符号指针，如果符号不存在，则抛出 runtime_error 异常
   * \throws std::runtime_error 如果共享库中不存在该符号
   *
   * \param[in] symbol_name name of the symbol inside the shared library
   * \return shared library symbol pointer, if the symbol doesn't exist then throws a
   * runtime_error exception
   * \throws std::runtime_error if the symbol doesn't exist in the shared library
   */
  RCPPUTILS_PUBLIC
  void * get_symbol(const char * symbol_name);

  /// 返回共享库符号指针。
  /// Return shared library symbol pointer.
  /**
   * \param[in] symbol_name 共享库内的符号名称
   * \return 共享库符号指针，如果符号不存在，则抛出 runtime_error 异常
   * \throws std::runtime_error 如果共享库中不存在该符号
   *
   * \param[in] symbol_name name of the symbol inside the shared library
   * \return shared library symbol pointer, if the symbol doesn't exist then throws a
   * runtime_error exception
   * \throws std::runtime_error if the symbol doesn't exist in the shared library
   */
  RCPPUTILS_PUBLIC
  void * get_symbol(const std::string & symbol_name);

  /// 返回共享库路径
  /// Return shared library path
  /**
   * \return 共享库路径，如果未定义，则抛出 std::runtime_error
   * \throws std::runtime_error 如果路径为 NULL
   *
   * \return shared library path or it throws an std::runtime_error if it's not defined
   * \throws std::runtime_error if the path is NULL
   */
  RCPPUTILS_PUBLIC
  std::string get_library_path();

private:
  // rcutils_shared_library_t 类型的私有成员变量 lib
  // Private member variable 'lib' of type rcutils_shared_library_t
  rcutils_shared_library_t lib;
};

/// 获取平台特定的库名称 (Get the platform specific library name)
/**
 * 最大文件名大小为1024个字符，如果输入的 library_name 大于此值，则方法会抛出异常。
 * (The maximum file name size is 1024 characters, if the input library_name is bigger than
 * this value then the method throws an exception.)
 *
 * \param[in] library_name 库基本名称（无前缀和扩展名）(library base name (without prefix and extension))
 * \param[in] debug 如果为 true，则库将返回调试库名称，否则返回普通库路径
 *                 (if true the library will return a debug library name, otherwise
 *                  it returns a normal library path)
 * \return 平台特定的库名称 (platform specific library name)
 * \throws std::runtime_error 如果无法创建库名称 (if it's not able to create the library name)
 */
RCPPUTILS_PUBLIC
std::string get_platform_library_name(std::string library_name, bool debug = false);

} // namespace rcpputils

#endif // RCPPUTILS__SHARED_LIBRARY_HPP_
