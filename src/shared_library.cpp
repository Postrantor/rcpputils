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

#include <iostream>
#include <string>

#include "rcutils/error_handling.h"

#include "rcpputils/shared_library.hpp"

namespace rcpputils
{
/**
 * @brief 构造一个 SharedLibrary 对象 (Construct a SharedLibrary object)
 *
 * @param library_path 动态库的路径 (Path to the shared library)
 */
SharedLibrary::SharedLibrary(const std::string & library_path)
{
  // 初始化 lib 结构体为零值 (Initialize the lib structure with zero values)
  lib = rcutils_get_zero_initialized_shared_library();

  // 使用默认分配器加载动态库 (Load the shared library using the default allocator)
  rcutils_ret_t ret =
    rcutils_load_shared_library(&lib, library_path.c_str(), rcutils_get_default_allocator());

  // 检查返回值，处理错误情况 (Check the return value and handle error cases)
  if (ret != RCUTILS_RET_OK) {
    if (ret == RCUTILS_RET_BAD_ALLOC) {
      throw std::bad_alloc();
    } else {
      std::string rcutils_error_str(rcutils_get_error_string().str);
      rcutils_reset_error();
      throw std::runtime_error{rcutils_error_str};
    }
  }
}

/**
 * @brief 销毁 SharedLibrary 对象 (Destroy a SharedLibrary object)
 */
SharedLibrary::~SharedLibrary()
{
  // 如果动态库已加载，则卸载它 (If the shared library is loaded, unload it)
  if (rcutils_is_shared_library_loaded(&lib)) {
    rcutils_ret_t ret = rcutils_unload_shared_library(&lib);

    // 如果卸载失败，输出错误信息 (If unloading fails, output the error message)
    if (ret != RCUTILS_RET_OK) {
      std::cerr << rcutils_get_error_string().str << std::endl;
      rcutils_reset_error();
    }
  }
}

/**
 * @brief 卸载动态库 (Unload the shared library)
 */
void SharedLibrary::unload_library()
{
  rcutils_ret_t ret = rcutils_unload_shared_library(&lib);

  // 如果卸载失败，抛出运行时错误 (If unloading fails, throw a runtime error)
  if (ret != RCUTILS_RET_OK) {
    std::string rcutils_error_str(rcutils_get_error_string().str);
    rcutils_reset_error();
    throw std::runtime_error{rcutils_error_str};
  }
}

/**
 * @brief 获取动态库中的符号 (Get a symbol from the shared library)
 *
 * @param symbol_name 要获取的符号名称 (Name of the symbol to get)
 * @return void* 指向符号的指针 (Pointer to the symbol)
 */
void * SharedLibrary::get_symbol(const char * symbol_name)
{
  void * lib_symbol = rcutils_get_symbol(&lib, symbol_name);

  // 如果没有找到符号，抛出运行时错误 (If the symbol is not found, throw a runtime error)
  if (!lib_symbol) {
    std::string rcutils_error_str(rcutils_get_error_string().str);
    rcutils_reset_error();
    throw std::runtime_error{rcutils_error_str};
  }

  return lib_symbol;
}

/**
 * @brief 获取动态库中的符号 (Get a symbol from the shared library)
 *
 * @param symbol_name 要获取的符号名称 (Name of the symbol to get)
 * @return void* 指向符号的指针 (Pointer to the symbol)
 */
void * SharedLibrary::get_symbol(const std::string & symbol_name)
{
  return get_symbol(symbol_name.c_str());
}

/**
 * @brief 检查动态库中是否存在符号 (Check if a symbol exists in the shared library)
 *
 * @param symbol_name 要检查的符号名称 (Name of the symbol to check)
 * @return bool 符号是否存在 (Whether the symbol exists)
 */
bool SharedLibrary::has_symbol(const char * symbol_name)
{
  return rcutils_has_symbol(&lib, symbol_name);
}

/**
 * @brief 检查动态库中是否存在符号 (Check if a symbol exists in the shared library)
 *
 * @param symbol_name 要检查的符号名称 (Name of the symbol to check)
 * @return bool 符号是否存在 (Whether the symbol exists)
 */
bool SharedLibrary::has_symbol(const std::string & symbol_name)
{
  return has_symbol(symbol_name.c_str());
}

/**
 * @brief 获取共享库的路径 (Get the path of the shared library)
 *
 * @return std::string 返回共享库路径字符串 (Return the shared library path string)
 * @throws std::runtime_error 当库路径未定义时抛出异常 (Throws an exception when the library path is not defined)
 */
std::string SharedLibrary::get_library_path()
{
  // 如果库路径不为空 (If the library path is not null)
  if (lib.library_path != nullptr) {
    // 返回库路径字符串 (Return the library path string)
    return std::string(lib.library_path);
  }
  // 抛出运行时错误，表示库路径未定义 (Throw a runtime error indicating that the library path is not defined)
  throw std::runtime_error{"Library path is not defined"};
}

/**
 * @brief 获取平台特定的库名称 (Get the platform-specific library name)
 *
 * @param library_name 原始库名称 (The original library name)
 * @param debug 是否为调试模式 (Whether it is in debug mode)
 * @return std::string 返回平台特定的库名称字符串 (Return the platform-specific library name string)
 * @throws std::runtime_error 当获取平台库名称失败时抛出异常 (Throws an exception when getting the platform library name fails)
 */
std::string get_platform_library_name(std::string library_name, bool debug)
{
  // 定义一个字符数组用于存储平台库名称 (Define a character array to store the platform library name)
  char library_name_platform[1024]{};

  // 调用 rcutils_get_platform_library_name 函数获取平台库名称 (Call the rcutils_get_platform_library_name function to get the platform library name)
  rcutils_ret_t ret =
    rcutils_get_platform_library_name(library_name.c_str(), library_name_platform, 1024, debug);

  // 检查返回值是否为 RCUTILS_RET_OK (Check if the return value is RCUTILS_RET_OK)
  if (ret != RCUTILS_RET_OK) {
    // 获取 rcutils 错误字符串 (Get the rcutils error string)
    std::string rcutils_error_str(rcutils_get_error_string().str);
    // 重置 rcutils 错误 (Reset the rcutils error)
    rcutils_reset_error();
    // 抛出运行时错误，表示获取平台库名称失败 (Throw a runtime error indicating that getting the platform library name failed)
    throw std::runtime_error{rcutils_error_str};
  }
  // 返回平台特定的库名称字符串 (Return the platform-specific library name string)
  return std::string(library_name_platform);
}

} // namespace rcpputils
