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

/*! \file pointer_traits.hpp
 * \brief Additional type traits support for smart pointers
 */

#ifndef RCPPUTILS__POINTER_TRAITS_HPP_
#define RCPPUTILS__POINTER_TRAITS_HPP_

#include <memory>
#include <type_traits>

namespace rcpputils
{

namespace details
{

/**
 * @brief 一个模板结构体，用于判断给定类型是否为智能指针。默认情况下，类型不是智能指针。
 * A template struct to determine if a given type is a smart pointer. By default, the type is not a smart pointer.
 *
 * @tparam T 要判断的类型。The type to be checked.
 */
template <class T>
struct is_smart_pointer_helper : std::false_type
{
};

/**
 * @brief 针对 std::shared_ptr<T> 类型的特化，表示该类型为智能指针。
 * Specialization for std::shared_ptr<T> type, indicating that the type is a smart pointer.
 *
 * @tparam T 模板参数。Template parameter.
 */
template <class T>
struct is_smart_pointer_helper<std::shared_ptr<T>> : std::true_type
{
};

/**
 * @brief 针对 std::unique_ptr<T> 类型的特化，表示该类型为智能指针。
 * Specialization for std::unique_ptr<T> type, indicating that the type is a smart pointer.
 *
 * @tparam T 模板参数。Template parameter.
 */
template <class T>
struct is_smart_pointer_helper<std::unique_ptr<T>> : std::true_type
{
};

/**
 * @brief 一个模板结构体，用于判断给定类型是否为智能指针。
 * A template struct to determine if a given type is a smart pointer.
 *
 * @tparam T 要判断的类型。The type to be checked.
 */
template <class T>
struct is_smart_pointer : is_smart_pointer_helper<typename std::remove_cv<T>::type>
{
};

/**
 * @brief 一个模板结构体，用于移除给定类型的指针。默认情况下，仅移除普通指针。
 * A template struct to remove the pointer of a given type. By default, only raw pointers are removed.
 *
 * @tparam T 要处理的类型。The type to be processed.
 * @tparam is_smart_pointer 指示是否为智能指针。Indicates whether it's a smart pointer or not.
 */
template <class T, bool is_smart_pointer>
struct remove_pointer
{
  using type = typename std::remove_pointer<T>::type;
};

/**
 * @brief 针对智能指针的特化，用于移除智能指针的指针类型。
 * Specialization for smart pointers, used to remove the pointer type of a smart pointer.
 *
 * @tparam T 要处理的类型。The type to be processed.
 */
template <class T>
struct remove_pointer<T, true>
{
  using type = typename std::remove_pointer<
    decltype(std::declval<typename std::remove_volatile<T>::type>().get())>::type;
};
} // namespace details

/// 类型特征，用于验证 T 是否为指针类型或智能指针类型
/// Type traits for validating if T is of type pointer or smart pointer
/**
 * 与标准库中的指针类型特征 `std::is_pointer<T>` 相比
 * https://en.cppreference.com/w/cpp/types/is_pointer，此特征还增强了对智能指针类型的检查。
 * Compared to the existing type trait for pointer in the stdlib `std::is_pointer<T>`
 * https://en.cppreference.com/w/cpp/types/is_pointer this trait is enhancing it for
 * checking of smart pointer types as well.
 * 有效的指针类型是 T*、std::shared_pointer<T> 和 std::unique_ptr<T>
 * The valid pointer types are T*, std::shared_pointer<T> and std::unique_ptr<T>
 *
 * 潜在的用例是在传递模板参数时进行静态断言，要求此参数为指针类型，而不指定指针类型（原始、智能）。
 * Potential use cases are for static assert when passing a template parameter requiring this
 * parameter to be of type pointer without specifying which type of pointer (raw, smart).
 *
 * ```
 * class MyType
 * {
 *   template<class T>
 *   MyType(T && arg)
 *   {
 *     static_assert(rcpputils::is_pointer<decltype(arg)>::value, "arg has to be a pointer");
 *
 *     arg->do_stuff();  // with the assert above, this call is guaranteed to work.
 *   }
 * };
 * ```
 */
template <class T> struct is_pointer
{
  /// 表示此对象是否为指针或智能指针。
  /// Indicates whether this object is a pointer or smart pointer.
  static constexpr bool value =
    std::is_pointer<typename std::remove_reference<T>::type>::value ||
    details::is_smart_pointer<typename std::remove_reference<T>::type>::value;
};

/// 从指针或智能指针中推导 T 的数据类型的类型特征。
/// Type traits for deducing the data type of T from a pointer or smart pointer.
/**
 * 与标准库中的指针类型特征 `std::remove_pointer<T>` 相比
 * https://en.cppreference.com/w/cpp/types/remove_pointer，此特征还增强了对智能指针类型的检查。
 * In comparison to the existing type trait for pointer in the stdlib `std::remove_pointer<T>`
 * https://en.cppreference.com/w/cpp/types/remove_pointer this trait is enhancing it for
 * checking of smart pointer types as well.
 * 有效的指针类型是 T*、std::shared_pointer<T> 和 std::unique_ptr<T>
 * The valid pointer types are T*, std::shared_pointer<T> and std::unique_ptr<T>
 *
 */
template <class T> struct remove_pointer
{
  // 使用 `details::remove_pointer` 类型特征来获取 T 的原始数据类型（去除指针或智能指针）
  // Use `details::remove_pointer` type trait to get the underlying data type of T (removing pointer or smart pointer)
  using type = typename details::remove_pointer<
    typename std::remove_reference<T>::type,
    details::is_smart_pointer<T>::value>::type;
};

} // namespace rcpputils

#endif // RCPPUTILS__POINTER_TRAITS_HPP_
