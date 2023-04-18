// Copyright 2019 Open Source Robotics Foundation, Inc.
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

/*! \file endian.hpp
 * \brief If std::endian is not available, the necessary functions are emulated.
 *
 * Note: Once std::endian is supported on all ROS2 platforms, this class
 * can be deprecated in favor of the built-in functionality.
 *
 * Note: std::endian is targeted for C++20
 */

#ifndef RCPPUTILS__ENDIAN_HPP_
#define RCPPUTILS__ENDIAN_HPP_

// TODO(anyone) replace this macro when the appropriate C++20 value lands.
#if !defined(RCPPUTILS_NO_STD_ENDIAN) && (__cplusplus <= 201703L)
#define RCPPUTILS_NO_STD_ENDIAN
#endif

#if !defined(RCPPUTILS_NO_STD_ENDIAN)
#include <type_traits>
namespace rcpputils
{
using std::endian;
}  // namespace rcpputils
#define RCPPUTILS_HAVE_STD_ENDIAN 1
#endif  // __cplusplus > 201703L

#ifndef RCPPUTILS_HAVE_STD_ENDIAN
#ifdef __has_include
#  if __has_include(<endian.h>)
#    include <endian.h>
#  endif
#endif

namespace rcpputils
{
/*!
 * \enum endian
 * \brief 定义编译时字节序的类型特征。
 * \brief Type traits for defining the endianness at compile-time.
 *
 * [cppreference.com 文档](https://en.cppreference.com/w/cpp/types/endian#Possible_implementation)
 * [cppreference.com documentation](https://en.cppreference.com/w/cpp/types/endian#Possible_implementation)
 *
 *  来源：https://en.cppreference.com/w/Cppreference:FAQ，此代码已获得 Creative Commons
 * Attribution-Sharealike 3.0 Unported License (CC-BY-SA) 许可。
 *  From: https://en.cppreference.com/w/Cppreference:FAQ, this is licensed Creative Commons
 * Attribution-Sharealike 3.0 Unported License (CC-BY-SA)
 */
enum class endian
{
#ifdef _WIN32
  // 在 Windows 平台上，定义 little 为 0
  // On Windows platform, define little as 0
  little = 0,
  // 在 Windows 平台上，定义 big 为 1
  // On Windows platform, define big as 1
  big    = 1,
  // 在 Windows 平台上，定义 native 为 little（默认小端字节序）
  // On Windows platform, define native as little (default to little-endian)
  native = little
#else
  // 在非 Windows 平台上，通过预处理器宏 __ORDER_LITTLE_ENDIAN__ 定义 little
  // On non-Windows platforms, define little by preprocessor macro __ORDER_LITTLE_ENDIAN__
  little = __ORDER_LITTLE_ENDIAN__,
  // 在非 Windows 平台上，通过预处理器宏 __ORDER_BIG_ENDIAN__ 定义 big
  // On non-Windows platforms, define big by preprocessor macro __ORDER_BIG_ENDIAN__
  big    = __ORDER_BIG_ENDIAN__,
  // 在非 Windows 平台上，通过预处理器宏 __BYTE_ORDER__ 定义 native
  // On non-Windows platforms, define native by preprocessor macro __BYTE_ORDER__
  native = __BYTE_ORDER__
#endif
};
}  // namespace rcpputils
#endif  // RCPPUTILS_HAVE_STD_ENDIAN
#endif  // RCPPUTILS__ENDIAN_HPP_
