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

/*! \file find_library.hpp
 * \brief Find library located in OS's library paths environment variable.
 */

#ifndef RCPPUTILS__FIND_LIBRARY_HPP_
#define RCPPUTILS__FIND_LIBRARY_HPP_

#include <string>

#include "rcpputils/visibility_control.hpp"

namespace rcpputils
{

/// 在操作系统指定的库路径环境变量中查找一个库。
/// Find a library located in the OS's specified environment variable for library paths.
/**
 *
 * 平台的环境变量和文件格式：
 * The environment variable and file format per platform:
 *  * Linux: `${LD_LIBRARY_PATH}`, `lib{}.so`
 *  * Apple: `${DYLD_LIBRARY_PATH}`, `lib{}.dyld`
 *  * Windows: `%PATH%`, `{}.dll`
 *
 * \param[in] library_name 要查找的库的名称。
 * \param[in] library_name Name of the library to find.
 * \return 包括适当的前缀和扩展名的绝对文件系统路径，如果未找到库，则为空字符串。
 * \return The absolute filesystem path, including the appropriate prefix and extension, or the
 * empty string when the library was not found.
 * \throws std::runtime_error 如果在访问环境变量时遇到错误。
 * \throws std::runtime_error if an error is encountered when accessing environment variables.
 */
RCPPUTILS_PUBLIC
std::string find_library_path(const std::string & library_name);

/// 根据库的目录构造文件路径，并检查其是否存在。
/// Construct the filepath for a library given its directory, and checks that it exists.
/**
 * \param[in] directory 包含库的目录。
 * \param[in] directory The directory that contains the library.
 * \param[in] library_name 要查找的库的名称。
 * \param[in] library_name Name of the library to find.
 * \return 包括适当的前缀和扩展名的绝对文件系统路径，如果库不存在，则为空字符串。
 * \return The absolute filesystem path, including the appropriate prefix and extension, or the
 * empty string when the library does not exist.
 */
RCPPUTILS_PUBLIC
std::string path_for_library(const std::string & directory, const std::string & library_name);

/// 根据库名创建对应的文件名。
/// Create the filename corresponding to the library name.
/**
 *
 * 有关平台特定文件名的信息，请参阅 find_library_path()。
 * \sa find_library_path() for information about the platform-specific filenames.
 *
 * \param[in] library_name 库名。
 * \param[in] library_name Name of the library.
 * \return 库名对应的文件名。
 * \return The filename for the library.
 */
RCPPUTILS_PUBLIC
std::string filename_for_library(const std::string & library_name);

} // namespace rcpputils

#endif // RCPPUTILS__FIND_LIBRARY_HPP_
