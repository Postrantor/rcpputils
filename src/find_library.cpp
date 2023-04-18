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

#include "rcpputils/find_library.hpp"

#include <cassert>
#include <cstddef>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "rcutils/filesystem.h"

#include "rcpputils/env.hpp"
#include "rcpputils/filesystem_helper.hpp"
#include "rcpputils/split.hpp"

namespace rcpputils
{

namespace
{

#ifdef _WIN32
static constexpr char kPathVar[] = "PATH";
static constexpr char kPathSeparator = ';';
static constexpr char kSolibPrefix[] = "";
static constexpr char kSolibExtension[] = ".dll";
#elif __APPLE__
static constexpr char kPathVar[] = "DYLD_LIBRARY_PATH";
static constexpr char kPathSeparator = ':';
static constexpr char kSolibPrefix[] = "lib";
static constexpr char kSolibExtension[] = ".dylib";
#else
static constexpr char kPathVar[] = "LD_LIBRARY_PATH";
static constexpr char kPathSeparator = ':';
static constexpr char kSolibPrefix[] = "lib";
static constexpr char kSolibExtension[] = ".so";
#endif

} // namespace

/**
 * @brief 在给定的搜索路径中查找库文件的路径 (Find the path of the library file in the given search paths)
 *
 * @param library_name 库文件名 (Library name)
 * @return std::string 如果找到库文件，则返回其路径，否则返回空字符串 (Returns the path of the library if found, otherwise returns an empty string)
 */
std::string find_library_path(const std::string & library_name)
{
  // 获取环境变量 kPathVar 的值 (Get the value of the environment variable kPathVar)
  std::string search_path = get_env_var(kPathVar);

  // 使用 kPathSeparator 分割 search_path，得到一个包含各个搜索路径的向量 (Split search_path using kPathSeparator to get a vector containing individual search paths)
  std::vector<std::string> search_paths = rcpputils::split(search_path, kPathSeparator);

  // 根据库名称生成库文件名 (Generate the library filename based on the library name)
  std::string filename = filename_for_library(library_name);

  // 遍历所有搜索路径 (Iterate through all search paths)
  for (const auto & search_path : search_paths) {
    // 拼接搜索路径和库文件名以获取完整路径 (Concatenate the search path and library filename to get the full path)
    std::string path = search_path + "/" + filename;

    // 检查路径是否指向一个文件 (Check if the path points to a file)
    if (rcutils_is_file(path.c_str())) {
      return path;
    }
  }

  // 如果未找到库文件，返回空字符串 (Return an empty string if the library is not found)
  return "";
}

/**
 * @brief 在给定的目录中查找库文件的路径 (Find the path of the library file in the given directory)
 *
 * @param directory 搜索库文件的目录 (Directory to search for the library)
 * @param library_name 库文件名 (Library name)
 * @return std::string 如果找到库文件，则返回其路径，否则返回空字符串 (Returns the path of the library if found, otherwise returns an empty string)
 */
std::string path_for_library(const std::string & directory, const std::string & library_name)
{
  // 使用给定的目录和库名称生成库文件的完整路径 (Generate the full path of the library using the given directory and library name)
  auto path = rcpputils::fs::path(directory) / filename_for_library(library_name);

  // 检查路径是否指向一个常规文件 (Check if the path points to a regular file)
  if (path.is_regular_file()) {
    return path.string();
  }

  // 如果未找到库文件，返回空字符串 (Return an empty string if the library is not found)
  return "";
}

/**
 * @brief 根据库名称生成库文件名 (Generate the library filename based on the library name)
 *
 * @param library_name 库文件名 (Library name)
 * @return std::string 库文件名，包括前缀和扩展名 (Library filename, including prefix and extension)
 */
std::string filename_for_library(const std::string & library_name)
{
  // 将 kSolibPrefix 和 kSolibExtension 添加到库名称以生成库文件名 (Add kSolibPrefix and kSolibExtension to the library name to generate the library filename)
  return kSolibPrefix + library_name + kSolibExtension;
}

} // namespace rcpputils
