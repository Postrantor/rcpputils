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
// https://github.com/ros/pluginlib/blob/1a4de29fa55173e9b897ca8ff57ebc88c047e0b3/pluginlib/include/pluginlib/impl/split.hpp

/*! \file split.hpp
 * \brief Split string by provided delimiter.
 */

#ifndef RCPPUTILS__SPLIT_HPP_
#define RCPPUTILS__SPLIT_HPP_

#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace rcpputils
{

/// @cond

/// 将指定输入使用分隔符和类型擦除的插入迭代器拆分为 tokens。
/**
 * 返回的向量将包含从输入中拆分出的 tokens
 *
 * \param[in] input 要拆分的输入字符串
 * \param[in] delim 用于拆分输入字符串的分隔符
 * \param[in] it 指向存储容器的迭代器
 * \param[in] skip_empty 从返回向量中删除空字符串
 */
/// Split a specified input into tokens using a delimiter and a type erased insert iterator.
/**
 * The returned vector will contain the tokens split from the input
 *
 * \param[in] input the input string to be split
 * \param[in] delim the delimiter used to split the input string
 * \param[in] it iterator pointing to a storage container
 * \param[in] skip_empty remove empty strings from the return vector
 */
template <
  class InsertIterator,
  typename std::enable_if<std::is_same<
    InsertIterator &,
    decltype(std::declval<InsertIterator>().operator=(std::declval<std::string>()))>::value>::
    type * = nullptr>
void split(const std::string & input, char delim, InsertIterator & it, bool skip_empty = false)
{
  // 创建一个字符串流对象
  // Create a stringstream object
  std::stringstream ss;

  // 将输入字符串设置为字符串流的内容
  // Set the input string as the content of the stringstream
  ss.str(input);

  // 定义一个临时存储子串的字符串变量
  // Define a temporary string variable to store substrings
  std::string item;

  // 使用 getline 函数从字符串流中按分隔符提取子串，并将其存储到 item 中
  // 使用 while 循环，直到字符串流中没有更多子串为止
  // Use the getline function to extract substrings from the stringstream according to the delimiter and store them in item
  // Use a while loop until there are no more substrings in the stringstream
  while (std::getline(ss, item, delim)) {
    // 如果启用了 skip_empty 选项并且当前子串为空，则跳过该子串
    // If the skip_empty option is enabled and the current substring is empty, skip it
    if (skip_empty && item == "") {
      continue;
    }

    // 将非空子串插入到存储容器中
    // Insert non-empty substrings into the storage container
    it = item;
  }
}
/// @endcond

/// 使用分隔符将指定输入拆分为 tokens。
/**
 * 返回的向量将包含从输入中拆分出的 tokens
 *
 * \param[in] input 要拆分的输入字符串
 * \param[in] delim 用于拆分输入字符串的分隔符
 * \param[in] skip_empty 从返回向量中删除空字符串
 * \return 包含 tokens 的向量。
 */
/// Split a specified input into tokens using a delimiter.
/**
 * The returned vector will contain the tokens split from the input
 *
 * \param[in] input the input string to be split
 * \param[in] delim the delimiter used to split the input string
 * \param[in] skip_empty remove empty strings from the return vector
 * \return A vector of tokens.
 */
inline std::vector<std::string>
split(const std::string & input, char delim, bool skip_empty = false)
{
  // 创建一个存储结果的字符串向量
  // Create a string vector to store the result
  std::vector<std::string> result;

  // 创建一个插入迭代器，用于将子串插入到结果向量中
  // Create an insert iterator for inserting substrings into the result vector
  auto it = std::back_inserter(result);

  // 调用前面定义的 split 函数来拆分输入字符串
  // Call the previously defined split function to split the input string
  split(input, delim, it, skip_empty);

  // 返回包含 tokens 的结果向量
  // Return the result vector containing tokens
  return result;
}
} // namespace rcpputils

#endif // RCPPUTILS__SPLIT_HPP_
