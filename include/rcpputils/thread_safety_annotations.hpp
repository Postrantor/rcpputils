// Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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

#ifndef RCPPUTILS__THREAD_SAFETY_ANNOTATIONS_HPP_
#define RCPPUTILS__THREAD_SAFETY_ANNOTATIONS_HPP_

#include <mutex>

/*! \file thread_safety_annotations.hpp
 * \brief 使线程安全属性仅在 clang+libcxx 下可用.
 * \brief Enable thread safety attributes only with clang+libcxx.
 *
 * 技术上，它们可以在没有 libcxx 的情况下使用 clang，手动注释线程安全原语，但这种用例会导致针对未注释的 libstdc++ 类型进行注释的错误。
 * Technically they would work with clang without libcxx, on manually-annotated thread safety primitives, but this use case causes the error of annotating against non-annotated libstdc++ types.
 * 希望对其线程库进行注释的用户需要为此单独定义这些宏。
 * Users that wish to annotate their threading library will need to define these macros separately for that case.
 *
 * 使用其他编译器编译时可以安全地擦除属性。
 * The attributes can be safely erased when compiling with other compilers.
 *
 * 从 llvm 的线程安全性注释文档中获取了宏描述，请参阅 [他们的文档](https://clang.llvm.org/docs/ThreadSafetyAnalysis.html) 获取更多信息
 * Macro descriptions were obtained from llvm's thread safety annotation documentation, see [their documentation](https://clang.llvm.org/docs/ThreadSafetyAnalysis.html) for more info
 */

// 为避免与其他项目发生冲突，给所有宏添加前缀。
// Prefixing all macros to avoid potential conflict with other projects.

/**
 * \def RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL(x)
 * \brief 使线程安全属性仅在 clang 下可用.
 * \brief Enable thread safety attributes only with clang.
 *
 * 使用其他编译器编译时可以安全地擦除属性。
 * The attributes can be safely erased when compiling with other compilers.
 * https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#mutex-h
 */
#if defined(__clang__) && defined(_LIBCPP_HAS_THREAD_SAFETY_ANNOTATIONS) && (!defined(SWIG))
#define RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL(x) __attribute__((x))
#else
#define RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL(x) // 无操作
#define RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL(x) // no-op
#endif

/**
 * \brief 定义为否定功能
 * \brief Defined for negation functionality
 *
 * libcxx 没有定义此运算符，需要负能力
 * libcxx does not define this operator, needed for negative capabilities
 * TODO 在这里，直到有人有更好的想法
 * TODO Here until someone has a better idea
 */
inline const std::mutex & operator!(const std::mutex & a) { return a; }

/**
 * \def RCPPUTILS_TSA_CAPABILITY
 * \brief 类的属性，表示类的对象可以用作功能。
 * Attribute on classes, which specifies that objects of the class can be used as a capability.
 *
 * 字符串参数在错误消息中指定功能的类型，例如 "mutex"。请参阅上面给出的 Container 示例，或者 mutex.h 中的 Mutex 类：
 * The string argument specifies the kind of capability in error messages, e.g. "mutex". See the Container example given above, or the Mutex class in mutex.h at:
 * https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#capability-string
 */
#define RCPPUTILS_TSA_CAPABILITY(x) RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL(capability(x))

/**
 * \def RCPPUTILS_TSA_SCOPED_CAPABILITY
 * \brief 实现 RAII 风格锁定的类的属性，在构造函数中获取功能，并在析构函数中释放
 * Attribute on classes that implement RAII-style locking, in which a capability is acquired in the constructor, and released in the destructor
 *
 * 这些类需要特殊处理，因为构造函数和析构函数通过不同的名称引用功能；请参见 mutex.h 中的 MutexLocker 类：
 * Such classes require special handling because the constructor and destructor refer to the capability via different names; see the MutexLocker class in mutex.h, at:
 * https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#scoped-capability
 */
#define RCPPUTILS_TSA_SCOPED_CAPABILITY RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL(scoped_lockable)

/**
 * \def RCPPUTILS_TSA_GUARDED_BY
 * \brief 数据成员的属性，声明数据成员受给定功能保护。
 * Attribute on data members, which declares that the data member is protected by the given capability.
 *
 * 对数据的读操作需要共享访问，而写操作需要独占访问。https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#guarded-by-c-and-pt-guarded-by-c
 * Read operations on the data require shared access, while write operations require exclusive access. https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#guarded-by-c-and-pt-guarded-by-c
 */
#define RCPPUTILS_TSA_GUARDED_BY(x) RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL(guarded_by(x))

/**
 * \def RCPPUTILS_TSA_PT_GUARDED_BY
 * \brief 类似于 RCPPUTILS_TSA_GUARDED_BY，但适用于指针和智能指针。
 * Similar to RCPPUTILS_TSA_GUARDED_BY, but is intended for use on pointers and smart pointers.
 *
 * 数据成员本身没有约束，但它指向的数据受给定功能保护。
 * There is no constraint on the data member itself, but the data that it points to is protected by the given capability.
 * https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#guarded-by-c-and-pt-guarded-by-c
 */
#define RCPPUTILS_TSA_PT_GUARDED_BY(x) RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL(pt_guarded_by(x))

/**
 * \def RCPPUTILS_TSA_ACQUIRED_BEFORE
 * \brief 成员声明上的属性，特别是互斥锁或其他功能的声明。
 * Attribute on member declarations, specifically declarations of mutexes or other capabilities.
 *
 * 这些声明强制执行互斥锁必须按照某种顺序获取，以防止死锁。
 * These declarations enforce a particular order in which the mutexes must be acquired, in order to prevent deadlock.
 * https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#acquired-before-acquired-after
 */
#define RCPPUTILS_TSA_ACQUIRED_BEFORE(...)                                                         \
  RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL(acquired_before(__VA_ARGS__))

/**
 * \def RCPPUTILS_TSA_ACQUIRED_AFTER
 * \brief 成员声明上的属性，特别是互斥锁或其他功能的声明。
 * Attribute on member declarations, specifically declarations of mutexes or other capabilities.
 *
 * 这些声明强制执行互斥锁必须按照某种顺序获取，以防止死锁。
 * These declarations enforce a particular order in which the mutexes must be acquired, in order to prevent deadlock.
 * https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#acquired-before-acquired-after
 */
#define RCPPUTILS_TSA_ACQUIRED_AFTER(...)                                                          \
  RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL(acquired_after(__VA_ARGS__))

/**
 * \def RCPPUTILS_TSA_REQUIRES
 * \brief 函数或方法上的属性，声明调用线程必须对给定功能具有独占访问权限。
 * Attribute on functions or methods, which declares that the calling thread must have exclusive access to the given capabilities.
 *
 * 可以指定多个功能。在进入函数时必须持有这些功能，并且在退出时仍然持有。
 * More than one capability may be specified. The capabilities must be held on entry to the function, and must still be held on exit.
 * https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#requires-requires-shared
 */
#define RCPPUTILS_TSA_REQUIRES(...)                                                                \
  RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL(requires_capability(__VA_ARGS__))

/**
 * \def RCPPUTILS_TSA_REQUIRES_SHARED
 * \brief 函数或方法上的属性，声明调用线程必须对给定功能具有共享访问权限。
 * Attribute on functions or methods, which declares that the calling thread must have shared access to the given capabilities.
 *
 * 可以指定多个功能。在进入函数时必须持有这些功能，并且在退出时仍然持有。
 * More than one capability may be specified. The capabilities must be held on entry to the function, and must still be held on exit.
 * https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#requires-requires-shared
 */
#define RCPPUTILS_TSA_REQUIRES_SHARED(...)                                                         \
  RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL(requires_shared_capability(__VA_ARGS__))

/**
 * \def RCPPUTILS_TSA_ACQUIRE
 * \brief 函数或方法上的属性，声明该函数获取功能但不释放它。
 * Attribute on functions or methods, which declares that the function acquires a capability, but does not release it.
 *
 * 调用者在进入时不能持有给定的功能，在退出时将持有该功能。
 * The caller must not hold the given capability on entry, and it will hold the capability on exit.
 * https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#acquire-acquire-shared-release-release-shared
 */
#define RCPPUTILS_TSA_ACQUIRE(...)                                                                 \
  RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL(acquire_capability(__VA_ARGS__))

/**
 * \def RCPPUTILS_TSA_ACQUIRE_SHARED
 * \brief Attribute on functions or methods, which declares that the function acquires a shared
 * capability, but does not release it.
 *
 * The caller must not hold the given capability on entry, and it will hold the capability on exit.
 * https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#acquire-acquire-shared-release-release-shared
 *
 * 定义在函数或方法上的属性，表示该函数获取一个共享能力，但不释放它。
 * 调用者在进入时不能持有给定的能力，在退出时将持有该能力。
 */
#define RCPPUTILS_TSA_ACQUIRE_SHARED(...)                                                          \
  RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL(acquire_shared_capability(__VA_ARGS__))

/**
 * \def RCPPUTILS_TSA_RELEASE
 * \brief Declare that the function releases the given capability.
 *
 * The caller must hold the capability on entry, and will no longer hold it on exit. It does not
 * matter whether the given capability is shared or exclusive.
 * https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#acquire-acquire-shared-release-release-shared
 *
 * 声明该函数释放给定的能力。
 * 调用者在进入时必须持有该能力，在退出时将不再持有。无论给定的能力是共享的还是独占的都没有关系。
 */
#define RCPPUTILS_TSA_RELEASE(...)                                                                 \
  RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL(release_capability(__VA_ARGS__))

/**
 * \def RCPPUTILS_TSA_RELEASE_SHARED
 * \brief Declare that the function releases the given shared capability.
 *
 * The caller must hold the shared capability on entry, and will no longer hold it on exit. It does
 * not matter whether the given capability is shared or exclusive.
 * https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#acquire-acquire-shared-release-release-shared
 *
 * 声明该函数释放给定的共享能力。
 * 调用者在进入时必须持有共享能力，在退出时将不再持有。无论给定的能力是共享的还是独占的都没有关系。
 */
#define RCPPUTILS_TSA_RELEASE_SHARED(...)                                                          \
  RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL(release_shared_capability(__VA_ARGS__))

/**
 * \def RCPPUTILS_TSA_TRY_ACQUIRE
 * \brief Attempts to RCPPUTILS_TSA_ACQUIRE, but its return value will indicate whether it
 * succeeded or failed.
 *
 * https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#try-acquire-bool-try-acquire-shared-bool
 *
 * 尝试执行 RCPPUTILS_TSA_ACQUIRE，但其返回值将表示操作成功还是失败。
 */
#define RCPPUTILS_TSA_TRY_ACQUIRE(...)                                                             \
  RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL(try_acquire_capability(__VA_ARGS__))

/**
 * \def RCPPUTILS_TSA_TRY_ACQUIRE_SHARED
 * \brief Attempts to RCPPUTILS_TSA_ACQUIRE_SHARED, but its return value will indicate whether it
 * succeeded or failed.
 *
 * https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#try-acquire-bool-try-acquire-shared-bool
 *
 * 尝试执行 RCPPUTILS_TSA_ACQUIRE_SHARED，但其返回值将表示操作成功还是失败。
 */
#define RCPPUTILS_TSA_TRY_ACQUIRE_SHARED(...)                                                      \
  RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL(try_acquire_shared_capability(__VA_ARGS__))

/**
 * \def RCPPUTILS_TSA_EXCLUDES
 * \brief attribute prevents deadlock, by making sure that a mutex is not held.
 *
 * However, EXCLUDES is an optional attribute, and does not provide the same safety guarantee as
 * RCPPUTILS_TSA_REQUIRES. In particular:
 *   - A function which acquires a capability does not have to exclude it.
 *   - A function which calls a function that excludes a capability does not have transitively
 *   - exclude that capability.
 * https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#negative-capabilities
 *
 * 属性通过确保互斥量不被持有来防止死锁。
 * 然而，EXCLUDES 是一个可选属性，并不提供与 RCPPUTILS_TSA_REQUIRES 相同的安全保证。具体来说：
 *   - 获取能力的函数不必排除它。
 *   - 调用排除某个能力的函数的函数不需要传递地排除该能力。
 */
#define RCPPUTILS_TSA_EXCLUDES(...)                                                                \
  RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL(locks_excluded(__VA_ARGS__))

/**
 * \def RCPPUTILS_TSA_ASSERT_CAPABILITY
 * \brief 执行运行时测试以查看调用线程是否具有给定的功能。(Perform a run-time test to see whether the calling thread holds the given capability.)
 *
 * 如果没有该功能，函数将失败（不返回）(The function is assumed to fail (no return) if the capability is not held)
 * https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#assert-capability-and-assert-shared-capability
 */
#define RCPPUTILS_TSA_ASSERT_CAPABILITY(...)                                                       \
  RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL(assert_capability(                                    \
    __VA_ARGS__)) // 使用RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL宏来实现assert_capability属性

/**
 * \def RCPPUTILS_TSA_ASSERT_SHARED_CAPABILITY
 * \brief 执行运行时测试，以查看调用线程是否具有给定的共享功能。(Perform a run-time test to see whether the calling thread holds the given shared capability.)
 *
 * 如果没有该功能，函数将失败（不返回）(The function is assumed to fail (no return) if the capability is not held.)
 * https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#assert-capability-and-assert-shared-capability
 */
#define RCPPUTILS_TSA_ASSERT_SHARED_CAPABILITY(...)                                                \
  RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL(assert_shared_capability(                             \
    __VA_ARGS__)) // 使用RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL宏来实现assert_shared_capability属性

/**
 * \def RCPPUTILS_TSA_RETURN_CAPABILITY
 * \brief 函数或方法上的属性，声明该函数返回对给定功能的引用。(Attribute on functions or methods, which declares that the function returns a reference to the given capability.)
 *
 * 用于返回互斥锁的getter方法。(It is used to annotate getter methods that return mutexes.)
 * https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#return-capability-c
 */
#define RCPPUTILS_TSA_RETURN_CAPABILITY(x)                                                         \
  RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL(                                                      \
    lock_returned(x)) // 使用RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL宏来实现lock_returned属性

/**
 * \def RCPPUTILS_TSA_NO_THREAD_SAFETY_ANALYSIS
 * \brief 函数或方法上的属性，用于关闭该方法的线程安全检查。(Attribute on functions or methods, which turns off thread safety checking for that method.)
 *
 * RCPPUTILS_TSA_NO_THREAD_SAFETY_ANALYSIS为以下两种情况提供了一个逃生舱口：(RCPPUTILS_TSA_NO_THREAD_SAFETY_ANALYSIS provides an escape hatch for functions which are)
 * (1) 故意线程不安全的函数；(deliberately thread-unsafe)
 * (2) 线程安全的函数，但对于分析来说太复杂。(thread-safe, but too complicated for the analysis to understand.)
 * 下面将介绍(2)的原因。(Reasons for (2) will be described in the Known Limitations, below.)
 * https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#return-capability-c
 */
#define RCPPUTILS_TSA_NO_THREAD_SAFETY_ANALYSIS                                                    \
  RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL(                                                      \
    no_thread_safety_analysis) // 使用RCPPUTILS_THREAD_ANNOTATION_ATTRIBUTE_IMPL宏来实现no_thread_safety_analysis属性

#endif // RCPPUTILS__THREAD_SAFETY_ANNOTATIONS_HPP_
