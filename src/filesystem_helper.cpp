// Copyright (c) 2019, Open Source Robotics Foundation, Inc.
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

// This file is originally from:
// https://github.com/ros/pluginlib/blob/1a4de29fa55173e9b897ca8ff57ebc88c047e0b3/pluginlib/include/pluginlib/impl/filesystem_helper.hpp

/*! \file filesystem_helper.hpp
 * \brief Cross-platform filesystem helper functions and additional emulation of [std::filesystem](https://en.cppreference.com/w/cpp/filesystem).
 *
 * Note: Once std::filesystem is supported on all ROS2 platforms, this class
 * can be deprecated/removed in favor of the built-in functionality.
 */

#include "rcpputils/filesystem_helper.hpp"

#include <sys/stat.h>

#include <algorithm>
#include <cstring>
#include <string>
#include <system_error>
#include <vector>

#ifdef _WIN32
#define NOMINMAX
#define NOGDI
#include <direct.h>
#include <fileapi.h>
#include <io.h>
#include <windows.h>
#define access _access_s
#else
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include "rcpputils/split.hpp"
#include "rcutils/env.h"

namespace rcpputils
{
namespace fs
{

/// \internal 返回一个布尔值，当路径为Windows上带有驱动器字母的绝对路径时返回 true。
/// \internal Returns true if the path is an absolute path with a drive letter on Windows.
static bool is_absolute_with_drive_letter(const std::string & path);

// 构造函数，将字符串转换为 path 对象
// Constructor that converts a string to a path object
path::path(const std::string & p) // NOLINT(runtime/explicit): this is a conversion constructor
: path_(p)
{
  // 将路径中的反斜杠替换为首选分隔符
  // Replace backslashes in the path with the preferred separator
  std::replace(path_.begin(), path_.end(), '\\', kPreferredSeparator);

  // 将路径中的正斜杠替换为首选分隔符
  // Replace forward slashes in the path with the preferred separator
  std::replace(path_.begin(), path_.end(), '/', kPreferredSeparator);

  // 使用首选分隔符将路径拆分为一个字符串向量
  // Split the path into a vector of strings using the preferred separator
  path_as_vector_ = split(path_, kPreferredSeparator);
}

// 返回 path 对象的字符串表示形式
// Returns the string representation of the path object
std::string path::string() const { return path_; }

// 检查路径是否存在
// Checks if the path exists
bool path::exists() const { return access(path_.c_str(), 0) == 0; }

// 检查路径是否是目录
// Checks if the path is a directory
bool path::is_directory() const noexcept
{
  struct stat stat_buffer;
  const auto rc = stat(path_.c_str(), &stat_buffer);

  // 如果 stat 函数调用失败，返回 false
  // If the stat function call fails, return false
  if (rc != 0) {
    return false;
  }

#ifdef _WIN32
  // 在 Windows 上检查是否为目录
  // Check if it's a directory on Windows
  return (stat_buffer.st_mode & S_IFDIR) == S_IFDIR;
#else
  // 在其他平台上检查是否为目录
  // Check if it's a directory on other platforms
  return S_ISDIR(stat_buffer.st_mode);
#endif
}

// 检查路径是否是常规文件
// Checks if the path is a regular file
bool path::is_regular_file() const noexcept
{
  struct stat stat_buffer;
  const auto rc = stat(path_.c_str(), &stat_buffer);

  // 如果 stat 函数调用失败，返回 false
  // If the stat function call fails, return false
  if (rc != 0) {
    return false;
  }

#ifdef _WIN32
  // 在 Windows 上检查是否为常规文件
  // Check if it's a regular file on Windows
  return (stat_buffer.st_mode & S_IFREG) == S_IFREG;
#else
  // 在其他平台上检查是否为常规文件
  // Check if it's a regular file on other platforms
  return S_ISREG(stat_buffer.st_mode);
#endif
}

/**
 * @brief 获取文件的大小 (Get the size of a file)
 *
 * @return uint64_t 文件的大小，以字节为单位 (The size of the file in bytes)
 */
uint64_t path::file_size() const
{
  // 如果路径是一个目录，抛出异常 (If the path is a directory, throw an exception)
  if (this->is_directory()) {
    auto ec = std::make_error_code(std::errc::is_a_directory);
    throw std::system_error{ec, "cannot get file size"};
  }

  // 定义一个 stat 结构体来存储文件信息 (Define a stat structure to store file information)
  struct stat stat_buffer;
  // 调用 stat 函数获取文件信息，并检查返回值 (Call the stat function to get file information and check the return value)
  const auto rc = stat(path_.c_str(), &stat_buffer);

  // 如果 stat 函数返回非零值，表示发生错误，抛出异常 (If the stat function returns a non-zero value, an error has occurred, and an exception is thrown)
  if (rc != 0) {
    std::error_code ec{errno, std::system_category()};
    errno = 0;
    throw std::system_error{ec, "cannot get file size"};
  } else {
    // 返回文件大小 (Return the file size)
    return static_cast<uint64_t>(stat_buffer.st_size);
  }
}

/**
 * @brief 检查路径是否为空 (Check if the path is empty)
 *
 * @return bool 如果路径为空，则返回 true，否则返回 false (Returns true if the path is empty, false otherwise)
 */
bool path::empty() const { return path_.empty(); }

/**
 * @brief 检查路径是否是绝对路径 (Check if the path is an absolute path)
 *
 * @return bool 如果路径是绝对路径，则返回 true，否则返回 false (Returns true if the path is an absolute path, false otherwise)
 */
bool path::is_absolute() const
{
  return path_.size() > 0 &&
         (path_[0] == kPreferredSeparator || is_absolute_with_drive_letter(path_));
}

/**
 * @brief 获取路径向量的常量起始迭代器 (Get the constant begin iterator of the path vector)
 *
 * @return std::vector<std::string>::const_iterator 路径向量的常量起始迭代器 (Constant begin iterator of the path vector)
 */
std::vector<std::string>::const_iterator path::cbegin() const { return path_as_vector_.cbegin(); }

/**
 * @brief 获取路径向量的常量结束迭代器 (Get the constant end iterator of the path vector)
 *
 * @return std::vector<std::string>::const_iterator 路径向量的常量结束迭代器 (Constant end iterator of the path vector)
 */
std::vector<std::string>::const_iterator path::cend() const { return path_as_vector_.cend(); }

/**
 * @brief 返回当前路径的父路径 (Return the parent path of the current path)
 *
 * @return path 父路径 (Parent path)
 */
path path::parent_path() const
{
  // 边缘情况：空路径 (Edge case: empty path)
  if (this->empty()) {
    return path("");
  }

  // 边缘情况：如果路径仅由一个部分组成，则返回 '.' 或 '/'，具体取决于路径是否为绝对路径
  //            (Edge case: if path only consists of one part, then return '.' or '/'
  //            depending if the path is absolute or not)
  if (1u == path_as_vector_.size()) {
    if (this->is_absolute()) {
      // Windows 操作系统比较棘手，因为绝对路径可能以 'C:\' 或 '\' 开头
      // (Windows is tricky, since an absolute path may start with 'C:\' or '\')
      if (is_absolute_with_drive_letter(path_)) {
        return path(path_as_vector_[0] + kPreferredSeparator);
      }
      return path(std::string(1, kPreferredSeparator));
    }
    return path(".");
  }

  // 边缘情况：对于路径 'C:\foo'，我们希望返回 'C:\' 而不是 'C:'
  // 不要从以字母驱动器开头的 Windows 上的绝对路径中删除根目录
  // (Edge case: with a path 'C:\foo' we want to return 'C:\' not 'C:'
  // Don't drop the root directory from an absolute path on Windows starting with a letter drive)
  if (2u == path_as_vector_.size() && is_absolute_with_drive_letter(path_)) {
    return path(path_as_vector_[0] + kPreferredSeparator);
  }

  path parent;
  for (auto it = this->cbegin(); it != --this->cend(); ++it) {
    if (parent.empty() && (!this->is_absolute() || is_absolute_with_drive_letter(path_))) {
      // 这个处理相对路径或 Windows 驱动器字母的情况；在这两种情况下，我们都不希望在开头有分隔符，所以只需直接复制片段
      // (This handles the case where we are dealing with a relative path or
      // the Windows drive letter; in both cases we don't want a separator at
      // the beginning, so just copy the piece directly.)
      parent = *it;
    } else {
      parent /= *it;
    }
  }
  return parent;
}

/**
 * @brief 返回当前路径的文件名 (Return the filename of the current path)
 *
 * @return path 文件名 (Filename)
 */
path path::filename() const { return path_.empty() ? path() : *--this->cend(); }

/**
 * @brief 返回当前路径的扩展名 (Return the extension of the current path)
 *
 * @return path 扩展名 (Extension)
 */
path path::extension() const
{
  const char * delimiter = ".";
  // 将当前路径字符串按照分隔符进行拆分 (Split the current path string by the delimiter)
  auto split_fname = rcpputils::split(this->string(), *delimiter);
  // 如果拆分后只有一个部分，则返回空路径；否则返回以 '.' 开头的扩展名部分
  // (Return an empty path if there's only one part after splitting, otherwise return the extension part starting with '.')
  return split_fname.size() == 1 ? path("") : path("." + split_fname.back());
}

/**
 * @brief 重载 / 运算符，用于连接两个路径字符串
 * @param other 另一个要连接的路径字符串
 * @return 返回新的路径对象
 *
 * @brief Overloads the / operator to concatenate two path strings
 * @param other The other path string to be concatenated
 * @return Returns a new path object
 */
path path::operator/(const std::string & other) const { return this->operator/(path(other)); }

/**
 * @brief 重载 /= 运算符，用于连接两个路径字符串，并更新当前路径对象
 * @param other 另一个要连接的路径字符串
 * @return 返回当前路径对象的引用
 *
 * @brief Overloads the /= operator to concatenate two path strings and update the current path object
 * @param other The other path string to be concatenated
 * @return Returns a reference to the current path object
 */
path & path::operator/=(const std::string & other)
{
  this->operator/=(path(other));
  return *this;
}

/**
 * @brief 重载 / 运算符，用于连接两个路径对象
 * @param other 另一个要连接的路径对象
 * @return 返回新的路径对象
 *
 * @brief Overloads the / operator to concatenate two path objects
 * @param other The other path object to be concatenated
 * @return Returns a new path object
 */
path path::operator/(const path & other) const { return path(*this).operator/=(other); }

/**
 * @brief 重载 /= 运算符，用于连接两个路径对象，并更新当前路径对象
 * @param other 另一个要连接的路径对象
 * @return 返回当前路径对象的引用
 *
 * @brief Overloads the /= operator to concatenate two path objects and update the current path object
 * @param other The other path object to be concatenated
 * @return Returns a reference to the current path object
 */
path & path::operator/=(const path & other)
{
  if (other.is_absolute()) {
    this->path_ = other.path_;
    this->path_as_vector_ = other.path_as_vector_;
  } else {
    if (this->path_.empty() || this->path_[this->path_.length() - 1] != kPreferredSeparator) {
      // 确保不会在路径中添加重复的分隔符；例如，在绝对路径中，向量的第一项为空字符串时可能发生这种情况。
      // This ensures that we don't put duplicate separators into the path;
      // this can happen, for instance, on absolute paths where the first
      // item in the vector is the empty string.
      this->path_ += kPreferredSeparator;
    }
    this->path_ += other.string();
    this->path_as_vector_.insert(
      std::end(this->path_as_vector_), std::begin(other.path_as_vector_),
      std::end(other.path_as_vector_));
  }
  return *this;
}

/**
 * @brief 检查给定路径是否是以驱动器字母开头的绝对路径（仅适用于 Windows）
 * @param path 要检查的路径字符串
 * @return 如果是绝对路径，则返回 true；否则返回 false
 *
 * @brief Checks if the given path is an absolute path starting with a drive letter (Windows only)
 * @param path The path string to check
 * @return Returns true if it is an absolute path; otherwise returns false
 */
static bool is_absolute_with_drive_letter(const std::string & path)
{
  (void)path; // Maybe unused
#ifdef _WIN32
  if (path.empty()) {
    return false;
  }
  return 0 == path.compare(1, 2, ":\\");
#else
  return false; // only Windows contains absolute paths starting with drive letters
#endif
}

/**
 * @brief 检查路径是否指向常规文件
 * @param p 要检查的路径对象
 * @return 如果是常规文件，则返回 true；否则返回 false
 *
 * @brief Checks if the path points to a regular file
 * @param p The path object to check
 * @return Returns true if it's a regular file; otherwise returns false
 */
bool is_regular_file(const path & p) noexcept { return p.is_regular_file(); }

/**
 * @brief 检查路径是否指向目录
 * @param p 要检查的路径对象
 * @return 如果是目录，则返回 true；否则返回 false
 *
 * @brief Checks if the path points to a directory
 * @param p The path object to check
 * @return Returns true if it's a directory; otherwise returns false
 */
bool is_directory(const path & p) noexcept { return p.is_directory(); }

/**
 * @brief 获取路径指向的文件大小
 * @param p 要获取文件大小的路径对象
 * @return 返回文件大小（以字节为单位）
 *
 * @brief Gets the file size of the file pointed to by the path
 * @param p The path object for which to get the file size
 * @return Returns the file size in bytes
 */
uint64_t file_size(const path & p) { return p.file_size(); }

/**
 * @brief 检查给定路径是否存在
 * @param path_to_check 要检查的路径对象
 * @return 如果存在，则返回 true；否则返回 false
 *
 * @brief Checks if the given path exists
 * @param path_to_check The path object to check
 * @return Returns true if it exists; otherwise returns false
 */
bool exists(const path & path_to_check) { return path_to_check.exists(); }

/**
 * @brief 获取临时目录路径 (Get the temporary directory path)
 *
 * @return path 返回临时目录路径对象 (Return a temporary directory path object)
 */
path temp_directory_path()
{
#ifdef _WIN32
  // 使用宽字符时，不支持Unicode路径 (Not supporting Unicode paths when using wide characters)
#ifdef UNICODE
#error "rcpputils::fs does not support Unicode paths"
#endif
  TCHAR temp_path
    [MAX_PATH]; // 定义一个字符数组用于存储临时路径 (Define a character array to store the temporary path)
  DWORD size = GetTempPathA(
    MAX_PATH,
    temp_path); // 获取临时路径，并将其长度存储在size中 (Get the temporary path and store its length in 'size')
  if (size > MAX_PATH || size == 0) {
    std::error_code ec(static_cast<int>(GetLastError()), std::system_category());
    throw std::system_error(ec, "cannot get temporary directory path");
  }
  temp_path[size] = '\0';
#else
  const char * temp_path =
    NULL; // 初始化一个指向临时路径的指针 (Initialize a pointer to the temporary path)
  const char * ret_str = rcutils_get_env(
    "TMPDIR",
    &temp_path); // 获取环境变量"TMPDIR"的值 (Get the value of the environment variable "TMPDIR")
  if (NULL != ret_str || *temp_path == '\0') {
    temp_path =
      "/tmp"; // 如果未找到有效的临时路径，则使用默认值"/tmp" (If no valid temporary path is found, use the default value "/tmp")
  }
#endif
  return path(temp_path); // 返回临时路径对象 (Return the temporary path object)
}

/**
 * @brief 在指定的父目录下创建一个临时目录 (Create a temporary directory under the specified parent directory)
 *
 * @param base_name 临时目录的基本名称 (The base name of the temporary directory)
 * @param parent_path 父目录的路径 (The path of the parent directory)
 * @return path 返回创建的临时目录的路径 (Return the path of the created temporary directory)
 */
path create_temp_directory(const std::string & base_name, const path & parent_path)
{
  const auto template_path =
    base_name +
    "XXXXXX"; // 创建一个模板路径，其中"XXXXXX"将被替换为随机字符 (Create a template path where 'XXXXXX' will be replaced with random characters)
  std::string full_template_str =
    (parent_path / template_path)
      .string(); // 将模板路径与父路径连接，并转换为字符串 (Concatenate the template path with the parent path and convert it to a string)
  if (
    !create_directories(
      parent_path)) { // 创建父目录（如果不存在）(Create the parent directory if it does not exist)
    std::error_code ec{errno, std::system_category()};
    errno = 0;
    throw std::system_error(ec, "could not create the parent directory");
  }

#ifdef _WIN32
  errno_t errcode = _mktemp_s(
    &full_template_str[0],
    full_template_str.size() +
      1); // 格式化临时目录名称模板 (Format the temporary directory name template)
  if (errcode) {
    std::error_code ec(static_cast<int>(errcode), std::system_category());
    throw std::system_error(ec, "could not format the temp directory name template");
  }
  const path final_path{full_template_str}; // 创建最终路径对象 (Create the final path object)
  if (!create_directories(final_path)) {    // 创建临时目录 (Create the temporary directory)
    std::error_code ec(static_cast<int>(GetLastError()), std::system_category());
    throw std::system_error(ec, "could not create the temp directory");
  }
#else
  const char * dir_name = mkdtemp(
    &full_template_str[0]); // 格式化并创建临时目录 (Format and create the temporary directory)
  if (dir_name == nullptr) {
    std::error_code ec{errno, std::system_category()};
    errno = 0;
    throw std::system_error(ec, "could not format or create the temp directory");
  }
  const path final_path{dir_name}; // 创建最终路径对象 (Create the final path object)
#endif

  return final_path; // 返回创建的临时目录的路径 (Return the path of the created temporary directory)
}

/**
 * @brief 获取当前工作目录 (Get the current working directory)
 *
 * @return 返回一个表示当前工作目录的路径对象 (Returns a path object representing the current working directory)
 */
path current_path()
{
// 使用宏定义来区分 Windows 平台和其他平台 (Use macro definitions to distinguish between Windows platform and other platforms)
#ifdef _WIN32
// 如果定义了 UNICODE，则抛出错误，因为 rcpputils::fs 不支持 Unicode 路径 (If UNICODE is defined, throw an error because rcpputils::fs does not support Unicode paths)
#ifdef UNICODE
#error "rcpputils::fs does not support Unicode paths"
#endif
  char cwd
    [MAX_PATH]; // 定义一个用于存储当前工作目录的字符数组 (Define a character array for storing the current working directory)
  if (nullptr == _getcwd(cwd, MAX_PATH)) {
#else
  char cwd
    [PATH_MAX]; // 定义一个用于存储当前工作目录的字符数组 (Define a character array for storing the current working directory)
  if (nullptr == getcwd(cwd, PATH_MAX)) {
#endif
    std::error_code ec{
      errno,
      std::
        system_category()}; // 创建一个表示错误码的对象 (Create an object representing the error code)
    errno = 0; // 将 errno 设置为 0 (Set errno to 0)
    throw std::system_error{
      ec,
      "cannot get current working directory"}; // 抛出一个系统错误，表示无法获取当前工作目录 (Throw a system error indicating that the current working directory cannot be obtained)
  }

  return path(
    cwd); // 返回一个表示当前工作目录的路径对象 (Returns a path object representing the current working directory)
}

/**
 * @brief 创建多级目录 (Create multiple directories)
 *
 * @param p 要创建的多级目录路径 (The multi-level directory path to be created)
 * @return 如果成功创建并且最后一个元素是目录，则返回 true，否则返回 false (Returns true if successfully created and the last element is a directory, otherwise returns false)
 */
bool create_directories(const path & p)
{
  path
    p_built; // 定义一个用于构建多级目录的路径对象 (Define a path object for building multi-level directories)
  int status =
    0; // 定义一个表示状态的整数变量，初始值为 0 (Define an integer variable representing the status, with an initial value of 0)

  // 遍历传入的路径中的每个元素，直到遇到非零状态或迭代器到达末尾 (Iterate through each element in the incoming path until a non-zero status is encountered or the iterator reaches the end)
  for (auto it = p.cbegin(); it != p.cend() && status == 0; ++it) {
    if (!p_built.empty() || it->empty()) {
      p_built /= *it;
    } else {
      p_built = *it;
    }
    if (!p_built.exists()) {
#ifdef _WIN32
      status = _mkdir(p_built.string().c_str());
#else
      status = mkdir(p_built.string().c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
#endif
      if (status == -1 && errno == EEXIST) {
        status = 0;
      }
    }
  }
  return status == 0 &&
         p_built
           .is_directory(); // 如果状态为 0 并且最后一个元素是目录，则返回 true，否则返回 false (Returns true if the status is 0 and the last element is a directory, otherwise returns false)
}

/**
 * @brief 删除给定路径的文件或目录（Remove the file or directory at the given path）
 * @param p 要删除的文件或目录的路径（Path to the file or directory to be removed）
 * @return 如果删除成功，则返回 true，否则返回 false（Returns true if removal is successful, otherwise returns false）
 */
bool remove(const path & p)
{
#ifdef _WIN32
  // 定义用于获取文件信息的结构体（Define the structure for getting file information）
  struct _stat s;
  // 检查给定路径的文件信息，并将结果存储在 s 中（Check the file information of the given path and store the result in s）
  if (_stat(p.string().c_str(), &s) == 0) {
    // 判断是否为目录（Check if it is a directory）
    if (s.st_mode & S_IFDIR) {
      // 删除目录（Delete the directory）
      return _rmdir(p.string().c_str()) == 0;
    }
    // 判断是否为常规文件（Check if it is a regular file）
    if (s.st_mode & S_IFREG) {
      // 删除文件（Delete the file）
      return ::remove(p.string().c_str()) == 0;
    }
  }
  // 文件信息检查失败，返回 false（File information check failed, return false）
  return false;
#else
  // 对于非 Windows 系统，直接调用 remove 函数（For non-Windows systems, call the remove function directly）
  return ::remove(p.string().c_str()) == 0;
#endif
}

/**
 * @brief 递归删除给定路径下的所有文件和目录（Recursively remove all files and directories under the given path）
 * @param p 要删除的文件或目录的路径（Path to the file or directory to be removed）
 * @return 如果删除成功，则返回 true，否则返回 false（Returns true if removal is successful, otherwise returns false）
 */
bool remove_all(const path & p)
{
  // 如果给定的路径不是目录，则直接调用 remove 函数（If the given path is not a directory, call the remove function directly）
  if (!is_directory(p)) {
    return rcpputils::fs::remove(p);
  }

#ifdef _WIN32
  // 定义一个双空终止的字符串类型（Define a double null terminated string type）
  size_t length = p.string().size();
  TCHAR * temp_dir = new TCHAR[length + 2];
  memcpy(temp_dir, p.string().c_str(), length);
  temp_dir[length] = '\0';
  temp_dir[length + 1] = '\0'; // 双空终止（double null terminated）

  // 定义文件操作结构体（Define the file operation structure）
  SHFILEOPSTRUCT file_options;
  file_options.hwnd = nullptr;
  file_options.wFunc = FO_DELETE; // 删除（递归）（delete (recursively)）
  file_options.pFrom = temp_dir;
  file_options.pTo = nullptr;
  file_options.fFlags = FOF_NOCONFIRMATION | FOF_SILENT; // 不提示用户（do not prompt user）
  file_options.fAnyOperationsAborted = FALSE;
  file_options.lpszProgressTitle = nullptr;
  file_options.hNameMappings = nullptr;

  // 执行文件操作（Perform file operation）
  auto ret = SHFileOperation(&file_options);
  delete[] temp_dir;

  // 返回操作结果（Return operation result）
  return 0 == ret && false == file_options.fAnyOperationsAborted;
#else
  // 打开目录（Open the directory）
  DIR * dir = opendir(p.string().c_str());
  struct dirent * directory_entry;
  while ((directory_entry = readdir(dir)) != nullptr) {
    // 确保不删除目录中的 ".." 或 "." 条目（可能会删除所有内容）（Make sure not to delete ".." or "." entries in the directory (might delete everything)）
    if (strcmp(directory_entry->d_name, ".") != 0 && strcmp(directory_entry->d_name, "..") != 0) {
      auto sub_path = rcpputils::fs::path(p) / directory_entry->d_name;
      // 如果是目录，递归调用（If it is a directory, call recursively）
      if (sub_path.is_directory() && !rcpputils::fs::remove_all(sub_path)) {
        return false;
        // 如果不是，调用常规 remove 函数（If not, call the regular remove function）
      } else if (!rcpputils::fs::remove(sub_path)) {
        return false;
      }
    }
  }
  // 关闭目录（Close the directory）
  closedir(dir);
  // 目录现在为空，调用 remove 函数（The directory is now empty, call the remove function）
  rcpputils::fs::remove(p);
  // 检查目录是否已被删除（Check if the directory has been removed）
  return !rcpputils::fs::exists(p);
#endif
}

/**
 * @brief 删除文件路径中的扩展名
 * @param file_path 要处理的文件路径
 * @param n_times 要删除的扩展名个数
 * @return 返回删除扩展名后的新路径
 *
 * Remove the extension from a file path.
 * @param file_path The file path to process
 * @param n_times The number of extensions to remove
 * @return The new path with the extension removed
 */
path remove_extension(const path & file_path, int n_times)
{
  // 创建一个新路径，用于存储处理过程中的路径
  // Create a new path to store the processed path during the process
  path new_path(file_path);

  // 循环删除指定次数的扩展名
  // Loop to remove the specified number of extensions
  for (int i = 0; i < n_times; i++) {
    // 获取当前新路径的字符串表示
    // Get the string representation of the current new path
    const auto new_path_str = new_path.string();

    // 查找最后一个点（.）的位置
    // Find the position of the last dot (.)
    const auto last_dot = new_path_str.find_last_of('.');

    // 如果没有找到点（.），说明已经没有扩展名可删除，直接返回当前新路径
    // If no dot (.) is found, there are no more extensions to remove, return the current new path directly
    if (last_dot == std::string::npos) {
      return new_path;
    }

    // 删除最后一个点（.）及之后的内容，生成新的路径
    // Remove the content after the last dot (.) and generate a new path
    new_path = path(new_path_str.substr(0, last_dot));
  }
  return new_path;
}

bool operator==(const path & a, const path & b) { return a.string() == b.string(); }

bool operator!=(const path & a, const path & b) { return !(a == b); }

std::ostream & operator<<(std::ostream & os, const path & p)
{
  os << p.string();
  return os;
}

} // namespace fs
} // namespace rcpputils
