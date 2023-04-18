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

/*! \file find_and_replace.hpp
 * \brief Find and replace functionality for std::strings
 */

#ifndef RCPPUTILS__FIND_AND_REPLACE_HPP_
#define RCPPUTILS__FIND_AND_REPLACE_HPP_

#include <memory>
#include <string>

namespace rcpputils
{

/// 查找并替换字符串中的所有指定子串为另一个字符串。
/**
 * \param[in] input 输入字符串。
 * \param[in] find 要替换的子串。
 * \param[in] replace 用于替换每个 `find` 出现的字符串。
 * \return 返回一个将输入字符串中所有 `find` 字符串替换为 `replace` 字符串后的副本。
 */
/// Find and replace all instances of a string with another string.
/**
 * \param[in] input The input string.
 * \param[in] find The substring to replace.
 * \param[in] replace The string to substitute for each occurrence of `find`.
 * \return A copy of the input string with all instances of the string `find` replaced with the
 *   string `replace`.
 */
template <
  class CharT,
  class Traits = std::char_traits<CharT>,
  class Allocator = std::allocator<CharT>>
std::basic_string<CharT, Traits, Allocator> find_and_replace(
  const std::basic_string<CharT, Traits, Allocator> & input,
  const std::basic_string<CharT, Traits, Allocator> & find,
  const std::basic_string<CharT, Traits, Allocator> & replace)
{
  // 创建一个与输入字符串相同的输出字符串
  // Create an output string identical to the input string
  std::basic_string<CharT, Traits, Allocator> output = input;

  // 获取要查找和替换的字符串的长度
  // Get the length of the strings to find and replace
  const std::size_t find_len = find.length();
  const std::size_t replace_len = replace.length();

  // 如果查找的字符串与替换的字符串相同，则直接返回输出字符串
  // If the string to find is the same as the string to replace, return the output string directly
  if (find == replace) {
    return output;
  }

  // 如果要查找的字符串长度为0，则直接返回输出字符串
  // If the length of the string to find is 0, return the output string directly
  if (0u == find_len) {
    return output;
  }

  // 查找输入字符串中第一个子串的位置
  // Find the position of the first substring in the input string
  std::size_t pos = output.find(find);

  // 当找到子串时，执行替换操作，并继续查找下一个子串
  // When the substring is found, perform the replacement operation and continue to find the next substring
  while (pos != std::basic_string<CharT, Traits, Allocator>::npos) {
    // 替换找到的子串
    // Replace the found substring
    output.replace(pos, find_len, replace);

    // 继续查找下一个子串
    // Continue to find the next substring
    pos = output.find(find, pos + replace_len);
  }

  // 返回替换后的字符串
  // Return the replaced string
  return output;
}

namespace detail
{
/// 将固定长度字符数组转换为 std::basic_string。
/**
 * \param[in] char_string 长度为 Length 的字符数组。
 * \return 返回一个将输入字符数组转换为 std::basic_string 的字符串。
 */
/// Convert a fixed-length character array to std::basic_string.
/**
 * \param[in] char_string A character array of length Length.
 * \return A string that converts the input character array to std::basic_string.
 */
template <typename CharT, std::size_t Length>
std::basic_string<CharT> normalize_to_basic_string(const CharT (&char_string)[Length])
{
  // 使用输入的固定长度字符数组创建并返回一个 std::basic_string
  // Create and return a std::basic_string using the input fixed-length character array
  return std::basic_string<CharT>(char_string);
}

/// 将类似字符串的对象直接返回，用于规范化为 std::basic_string。
/**
 * \param[in] string_like 类似字符串的对象。
 * \return 返回输入的类似字符串的对象。
 */
/// Directly return a string-like object for normalization to std::basic_string.
/**
 * \param[in] string_like A string-like object.
 * \return The input string-like object.
 */
template <typename StringLikeT> StringLikeT && normalize_to_basic_string(StringLikeT && string_like)
{
  // 直接返回输入的类似字符串的对象
  // Directly return the input string-like object
  return string_like;
}
} // namespace detail

/// 查找并替换字符串中所有指定子串的实例。
/**
 * \param[in] input 输入字符串。
 * \param[in] find 要替换的子串。
 * \param[in] replace 用于替换每个 `find` 出现的字符串。
 * \return 返回一个输入字符串的副本，其中所有的 `find` 字符串实例都被 `replace` 字符串替换。
 */
/// Find and replace all instances of a string with another string.
/**
 * \param[in] input The input string.
 * \param[in] find The substring to replace.
 * \param[in] replace The string to substitute for each occurrence of `find`.
 * \return A copy of the input string with all instances of the string `find` replaced with the
 *   string `replace`.
 */
template <typename InputT, typename FindT, typename ReplaceT>
auto find_and_replace(InputT && input, FindT && find, ReplaceT && replace)
{
  // 使用 detail::normalize_to_basic_string 将输入转换为 std::basic_string
  // Convert the input to std::basic_string using detail::normalize_to_basic_string
  auto input_str = detail::normalize_to_basic_string(input);

  // 使用转换后的 input_str、find 和 replace 调用 find_and_replace 函数，并返回结果
  // Call the find_and_replace function with the converted input_str, find, and replace, and return the result
  return find_and_replace<typename decltype(input_str)::value_type>(
    input_str, detail::normalize_to_basic_string(find), detail::normalize_to_basic_string(replace));
}
} // namespace rcpputils

#endif // RCPPUTILS__FIND_AND_REPLACE_HPP_
