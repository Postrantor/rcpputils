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

#ifndef RCPPUTILS__FILESYSTEM_HELPER_HPP_
#define RCPPUTILS__FILESYSTEM_HELPER_HPP_

#include <string>
#include <vector>

#include "rcpputils/visibility_control.hpp"

namespace rcpputils
{
namespace fs
{

#ifdef _WIN32
#define RCPPUTILS_IMPL_OS_DIRSEP '\\'
#else
#define RCPPUTILS_IMPL_OS_DIRSEP '/'
#endif

/**
 * \def kPreferredSeparator
 *
 * A definition for this platforms string path separator
 */
// 定义此平台的字符串路径分隔符
static constexpr const char kPreferredSeparator = RCPPUTILS_IMPL_OS_DIRSEP;

// 取消定义 RCPPUTILS_IMPL_OS_DIRSEP
#undef RCPPUTILS_IMPL_OS_DIRSEP

/**
 * \brief Drop-in replacement for [std::filesystem::path](https://en.cppreference.com/w/cpp/filesystem/path).
 *
 * It must conform to the same standard described and cannot include methods that are not
 * incorporated there.
 */
// 用于替换 [std::filesystem::path](https://en.cppreference.com/w/cpp/filesystem/path) 的类。
// 它必须符合相同的标准，并且不能包含未在该处实现的方法。
class path
{
public:
  /**
   * \brief Constructs an empty path.
   *
   * \details 构造一个空路径。
   */
  // 构建一个空路径
  RCPPUTILS_PUBLIC
  path() = default;

  /**
 * \brief 从 std::string 路径的转换构造函数。 (Conversion constructor from a std::string path.)
 *
 * \param[in] p 一个用平台的字符串路径分隔符分割的字符串路径。 (A string path split by the platform's string path separator.)
 */
  RCPPUTILS_PUBLIC
  path(const std::string &
         p); // NOLINT(runtime/explicit): 这是一个转换构造函数 (this is a conversion constructor)

  /**
 * \brief 拷贝构造函数。 (Copy constructor.)
 */
  RCPPUTILS_PUBLIC path(const path & p) = default;

  /**
 * \brief 拷贝赋值运算符。 (Copy assignment operator.)
 *
 * \return 对复制路径的引用。 (Reference to the copied path.)
 */
  RCPPUTILS_PUBLIC path & operator=(const path &) = default;

  /**
 * \brief 使用此系统的路径分隔符获取路径。 (Get the path delimited using this system's path separator.)
 *
 * \return 路径作为字符串 (The path as a string)
 */
  RCPPUTILS_PUBLIC std::string string() const;

  /**
 * \brief 检查此路径是否存在。 (Check if this path exists.)
 *
 * \return 如果路径存在，则返回 true，否则返回 false。 (True if the path exists, false otherwise.)
 */
  RCPPUTILS_PUBLIC bool exists() const;

  /**
 * \brief 检查路径是否存在且它是一个目录。 (Check if the path exists and it is a directory.)
 *
 * \return 如果路径是现有目录，则返回 true，否则返回 false。 (True if the path is an existing directory, false otherwise.)
 */
  RCPPUTILS_PUBLIC bool is_directory() const noexcept;

  /**
 * \brief 检查路径是否是常规文件。 (Check if the path is a regular file.)
 *
 * \return 如果文件是现有的常规文件，则返回 true，否则返回 false。 (True if the file is an existing regular file, false otherwise.)
 */
  RCPPUTILS_PUBLIC bool is_regular_file() const noexcept;

  /**
 * \brief 返回文件的大小（以字节为单位）。
 * Return the size of the file in bytes.
 *
 * \return 文件的大小（以字节为单位）
 * \return size of file in bytes
 * \throws std::system_error
 */
  RCPPUTILS_PUBLIC uint64_t file_size() const;

  /**
 * \brief 检查路径是否为空。
 * Check if the path is empty.
 *
 * \return 如果路径为空，则返回 True；否则返回 False。
 * \return True if the path is empty, false otherwise.
 */
  RCPPUTILS_PUBLIC bool empty() const;

  /**
 * \brief 检查路径是否为绝对路径。
 * Check if the path is an absolute path.
 *
 * \return 如果路径是绝对路径，则返回 True；否则返回 False。
 * \return True if the path is absolute, false otherwise.
 */
  RCPPUTILS_PUBLIC bool is_absolute() const;

  /**
 * \brief Const 迭代器指向此路径的第一个元素。
 * Const iterator to first element of this path.
 *
 * \return 一个指向第一个元素的 const 迭代器。
 * \return A const iterator to the first element.
 */
  RCPPUTILS_PUBLIC std::vector<std::string>::const_iterator cbegin() const;

  /**
 * Const 迭代器指向此路径最后一个元素之后的位置。
 * Const iterator to one past the last element of this path.
 *
 * \return 一个指向路径最后一个元素之后的 const 迭代器。
 * \return A const iterator to one past the last element of the path.
 */
  RCPPUTILS_PUBLIC std::vector<std::string>::const_iterator cend() const;

  /**
 * \brief 获取此路径的父目录。
 * Get the parent directory of this path.
 *
 * \return 指向父目录的路径。
 * \return A path to the parent directory.
 */
  RCPPUTILS_PUBLIC path parent_path() const;

  /**
 * \brief 获取此路径中的最后一个元素。
 * Get the last element in this path.
 *
 * 如果此路径指向目录，则返回目录名称。
 * If this path points to a directory, it will return the directory name.
 *
 * \return 此路径中的最后一个元素
 * \return The last element in this path
 */
  RCPPUTILS_PUBLIC path filename() const;

  /**
 * \brief 获取包括并跟随最后一个 '.' 的组件的相对路径。
 * Get a relative path to the component including and following the last '.'.
 *
 * \return 字符串扩展名
 * \return The string extension
 */
  RCPPUTILS_PUBLIC path extension() const;

  /**
 * \brief 将路径和字符串连接成一个单一的路径。 (Concatenate a path and a string into a single path.)
 *
 * \param[in] other 要连接的字符串组件 (the string component to concatenate)
 * \return 这个路径和其他路径的组合 (The combined path of this and other.)
 */
  RCPPUTILS_PUBLIC path operator/(const std::string & other) const;

  /**
 * \brief 将字符串组件附加到此路径。 (Append a string component to this path.)
 *
 * \param[in] other 要附加的字符串组件 (the string component to append)
 * \return *this
 */
  RCPPUTILS_PUBLIC path & operator/=(const std::string & other);

  /**
 * \brief 将两个路径连接在一起。 (Concatenate two paths together.)
 *
 * \param[in] other 要附加的路径 (the path to append)
 * \return 组合后的路径。 (The combined path.)
 */
  RCPPUTILS_PUBLIC path operator/(const path & other) const;

  /**
 * \brief 将字符串组件附加到此路径。 (Append a string component to this path.)
 *
 * \param[in] other 要附加的字符串组件 (the string component to append)
 * \return *this
 */
  RCPPUTILS_PUBLIC path & operator/=(const path & other);

private:
  std::string path_;
  std::vector<std::string> path_as_vector_;
};

/**
 * \brief 检查路径是否为常规文件。
 * \brief Check if the path is a regular file.
 *
 * \param[in] p 要检查的路径
 * \param[in] p The path to check
 * \return 如果路径存在，则返回 true，否则返回 false。
 * \return True if the path exists, false otherwise.
 */
RCPPUTILS_PUBLIC bool is_regular_file(const path & p) noexcept;

/**
 * \brief 检查路径是否为目录。
 * \brief Check if the path is a directory.
 *
 * \param[in] p 要检查的路径
 * \param[in] p The path to check
 * \return 如果路径是现有目录，则返回 true，否则返回 false。
 * \return True if the path is an existing directory, false otherwise.
 */
RCPPUTILS_PUBLIC bool is_directory(const path & p) noexcept;

/**
 * \brief 获取路径的文件大小。
 * \brief Get the file size of the path.
 *
 * \param[in] p 要获取文件大小的路径。
 * \param[in] p The path to get the file size of.
 * \return 以字节为单位的文件大小。
 * \return The file size in bytes.
 *
 * \throws std::sytem_error
 */
RCPPUTILS_PUBLIC uint64_t file_size(const path & p);

/**
 * \brief 检查路径是否存在。
 * \brief Check if a path exists.
 *
 * \param[in] path_to_check 要检查的路径。
 * \param[in] path_to_check The path to check.
 * \return 如果路径存在，则返回 true，否则返回 false。
 * \return True if the path exists, false otherwise.
 */
RCPPUTILS_PUBLIC bool exists(const path & path_to_check);

/**
 * \brief 获取临时目录中的位置路径（如果可用）。
 * \brief Get a path to a location in the temporary directory, if it's available.
 *
 * 此操作不会创建任何目录。
 * This does not create any directories.
 * 在 Windows 上，此操作使用 "GetTempPathA"
 * On Windows, this uses "GetTempPathA"
 * 在非 Windows 上，此操作首选环境变量 TMPDIR，然后回退到 /tmp
 * On non-Windows, this prefers the environment variable TMPDIR, falling back to /tmp
 *
 * \return 用于存储临时文件和目录的目录路径。
 * \return A path to a directory for storing temporary files and directories.
 */
RCPPUTILS_PUBLIC path temp_directory_path();

/**
 * \brief 在 "parent" 中构造一个具有格式 base_nameXXXXXX 的唯一命名临时目录。
 * \brief Construct a uniquely named temporary directory, in "parent", with format base_nameXXXXXX
 *
 * 如果成功，输出保证是新创建的目录。
 * The output, if successful, is guaranteed to be a newly-created directory.
 * 底层实现一直生成路径，直到找到一个不存在的路径。
 * The underlying implementation keeps generating paths until one that does not exist is found.
 * 这可以保证返回的目录中没有现有文件。
 * This guarantees that there will be no existing files in the returned directory.
 *
 * \param[in] base_name 用户指定的创建目录部分
 * \param[in] base_name User-specified portion of the created directory
 * \param[in] parent_path 将创建的目录的父路径
 * \param[in] parent_path The parent path of the directory that will be created
 * \return 具有 base_name 和 6 个字符唯一后缀的新创建目录的路径
 * \return A path to a newly-created directory with base_name and a 6-character unique suffix
 *
 * \throws std::system_error 如果任何操作系统 API 不成功。
 * \throws std::system_error If any OS APIs do not succeed.
 */
RCPPUTILS_PUBLIC path create_temp_directory(
  const std::string & base_name, const path & parent_path = temp_directory_path());

/**
 * \brief 返回当前工作目录。(Return current working directory.)
 *
 * \return 当前工作目录。(The current working directory.)
 *
 * \throws std::system_error
 */
RCPPUTILS_PUBLIC path current_path();

/**
 * \brief 使用给定的路径p创建一个目录。(Create a directory with the given path p.)
 *
 * 此函数会递归地构建目录，并在目录已经创建时跳过它们。
 * (This builds directories recursively and will skip directories if they are already created.)
 * \param[in] p 要创建目录的路径。(The path at which to create the directory.)
 * \return 如果目录已存在或已创建，则返回true，否则返回false。
 *         (Return true if the directory already exists or is created, false otherwise.)
 */
RCPPUTILS_PUBLIC bool create_directories(const path & p);

/**
 * \brief 删除路径p处的文件或目录。(Remove the file or directory at the path p.)
 *
 * \param[in] p 要删除的对象的路径。(The path of the object to remove.)
 * \return 如果文件存在且成功删除，则返回true，否则返回false。
 *         (true if the file exists and it was successfully removed, false otherwise.)
 */
RCPPUTILS_PUBLIC bool remove(const path & p);

/**
 * \brief 删除路径p处的目录及其内容。(Remove the directory at the path p and its content.)
 *
 * 除了\sa remove之外，remove_all还会删除目录及其包含的文件。
 * (Additionally to \sa remove, remove_all removes a directory and its containing files.)
 *
 * \param[in] p 要删除的目录的路径。(The path of the directory to remove.)
 * \return 如果目录存在且成功删除，则返回true，否则返回false。
 *         (true if the directory exists and it was successfully removed, false otherwise.)
 */
RCPPUTILS_PUBLIC bool remove_all(const path & p);

/**
 * \brief 从路径中删除扩展名。(Remove extension(s) from a path.)
 *
 * 扩展名定义为从路径末尾到第一个句点(.)字符的文本。
 * (An extension is defined as text starting from the end of a path to the first period (.) character.)
 *
 * \param[in] file_path 文件路径字符串。(The file path string.)
 * \param[in] n_times 要删除的扩展名数量（如果有多个扩展名）。(The number of extensions to remove if there are multiple extensions.)
 * \return 路径对象。(The path object.)
 */
RCPPUTILS_PUBLIC path remove_extension(const path & file_path, int n_times = 1);

/**
 * \brief 比较两个路径是否相等。(Compare two paths for equality.)
 *
 * \return 如果两个路径作为字符串相等，则返回True。(True if both paths are equal as strings.)
 */
RCPPUTILS_PUBLIC bool operator==(const path & a, const path & b);
RCPPUTILS_PUBLIC bool operator!=(const path & a, const path & b);

/**
 * \brief 将路径转换为字符串以供ostream使用，例如在日志记录或字符串格式化中。
 *        (Convert the path to a string for ostream usage, such as in logging or string formatting.)
 *
 * \param[in] os 要发送路径字符串的流。(The stream to send the path string to)
 * \param[in] p 要字符串化的路径。(The path to stringify)
 * \return 串联起来的ostream。(The ostream, for chaining)
 */
RCPPUTILS_PUBLIC std::ostream & operator<<(std::ostream & os, const path & p);

} // namespace fs
} // namespace rcpputils

#endif // RCPPUTILS__FILESYSTEM_HELPER_HPP_
