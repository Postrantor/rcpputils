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

/*! \file join.hpp
 * \brief Join container of objects into single string
 */

#ifndef RCPPUTILS__JOIN_HPP_
#define RCPPUTILS__JOIN_HPP_

#include <algorithm>
#include <cstring>
#include <iterator>
#include <sstream>
#include <string>

namespace rcpputils
{

/// 加入一个容器中的值，通过给定的分隔符转换为字符串 (Join values in a container turned into strings by a given delimiter)
/**
 * \param[in] container 是要转换为字符串并连接的值的集合。 (is a collection of values to be turned into string and joined.)
 * \param[in] delim 是用于连接转换为字符串的值的分隔符。 (is a delimiter to join values turned into strings.)
 * \tparam CharT 是字符串字符类型。 (is the string character type.)
 * \tparam ValueT 是容器值类型。 (is the container value type.)
 * \tparam AllocatorT 是容器分配器类型。 (is the container allocator type.)
 * \tparam ContainerT 是容器模板类型。 (is the container template type.)
 * \return 连接后的字符串 (joined string)
 */
template <
  typename CharT,
  typename ValueT,
  typename AllocatorT,
  template <typename T, class A>
  class ContainerT>
std::basic_string<CharT> join(const ContainerT<ValueT, AllocatorT> & container, const CharT * delim)
{
  // 创建一个基本输出字符串流对象 (Create a basic output string stream object)
  std::basic_ostringstream<CharT> s;

  // 将容器中的元素复制到输出字符串流中，并使用分隔符分隔 (Copy elements from the container to the output string stream using the delimiter)
  std::copy(container.begin(), container.end(), std::ostream_iterator<ValueT, CharT>(s, delim));

  // 获取字符串流中的字符串并将其存储在结果中 (Get the string from the string stream and store it in the result)
  std::basic_string<CharT> result = s.str();

  // 如果提供了分隔符，则从结果中删除最后一个分隔符 (If a delimiter is provided, remove the last delimiter from the result)
  if (delim) {
    result = result.substr(0, result.length() - strlen(delim));
  }

  // 返回连接后的字符串 (Return the joined string)
  return result;
}

} // namespace rcpputils

#endif // RCPPUTILS__JOIN_HPP_
