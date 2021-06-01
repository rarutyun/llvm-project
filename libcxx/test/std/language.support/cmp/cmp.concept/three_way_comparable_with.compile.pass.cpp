//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17
// UNSUPPORTED: libcpp-no-concepts

// template<class T, class U, class Cat = partial_ordering>
// concept three_way_comparable_with = // see below

#include <compare>

#include "compare_types.h"

template <class T, class U, typename Cat = std::partial_ordering>
constexpr bool check_three_way_comparable_with() {
  constexpr bool result = std::three_way_comparable_with<T, U, Cat>;
  static_assert(std::three_way_comparable_with<U, T, Cat> == result);
  static_assert(std::three_way_comparable_with<T, U const, Cat> == result);
  static_assert(std::three_way_comparable_with<T const, U const, Cat> == result);
  static_assert(std::three_way_comparable_with<T, U const&, Cat> == result);
  static_assert(std::three_way_comparable_with<T const, U const&, Cat> == result);
  static_assert(std::three_way_comparable_with<T, U const&&, Cat> == result);
  static_assert(std::three_way_comparable_with<T const, U const&&, Cat> == result);
  if constexpr (!std::is_void_v<T>) {
    static_assert(std::three_way_comparable_with<T&, U const, Cat> == result);
    static_assert(std::three_way_comparable_with<T const&, U const, Cat> == result);
    static_assert(std::three_way_comparable_with<T&, U const&, Cat> == result);
    static_assert(std::three_way_comparable_with<T const&, U const&, Cat> == result);
    static_assert(std::three_way_comparable_with<T&, U const&&, Cat> == result);
    static_assert(std::three_way_comparable_with<T const&, U const&&, Cat> == result);
    static_assert(std::three_way_comparable_with<T&&, U const, Cat> == result);
    static_assert(std::three_way_comparable_with<T const&&, U const, Cat> == result);
    static_assert(std::three_way_comparable_with<T&&, U const&, Cat> == result);
    static_assert(std::three_way_comparable_with<T const&&, U const&, Cat> == result);
    static_assert(std::three_way_comparable_with<T&&, U const&&, Cat> == result);
    static_assert(std::three_way_comparable_with<T const&&, U const&&, Cat> == result);
  }
  return result;
}

namespace fundamentals {
static_assert(check_three_way_comparable_with<int, int>());
static_assert(check_three_way_comparable_with<int, char>());
static_assert(check_three_way_comparable_with<int, wchar_t>());
static_assert(check_three_way_comparable_with<int, double>());
static_assert(check_three_way_comparable_with<int*, int*>());

static_assert(check_three_way_comparable_with<int, int, std::strong_ordering>());
static_assert(check_three_way_comparable_with<int, char, std::strong_ordering>());
static_assert(check_three_way_comparable_with<int, wchar_t, std::strong_ordering>());

static_assert(check_three_way_comparable_with<int, int, std::weak_ordering>());
static_assert(check_three_way_comparable_with<int, char, std::weak_ordering>());
static_assert(check_three_way_comparable_with<int, wchar_t, std::weak_ordering>());

static_assert(!check_three_way_comparable_with<int, bool>());
static_assert(!check_three_way_comparable_with<int, int*>());
static_assert(!check_three_way_comparable_with<int, int[5]>());
static_assert(!check_three_way_comparable_with<int, int (*)()>());
static_assert(!check_three_way_comparable_with<int, int (&)()>());
struct S {};
static_assert(!check_three_way_comparable_with<int, int S::*>());
static_assert(!check_three_way_comparable_with<int, int (S::*)()>());
static_assert(!check_three_way_comparable_with<int, int (S::*)() noexcept>());
static_assert(!check_three_way_comparable_with<int, int (S::*)() const>());
static_assert(
    !check_three_way_comparable_with<int, int (S::*)() const noexcept>());
static_assert(!check_three_way_comparable_with<int, int (S::*)() volatile>());
static_assert(
    !check_three_way_comparable_with<int, int (S::*)() volatile noexcept>());
static_assert(
    !check_three_way_comparable_with<int, int (S::*)() const volatile>());
static_assert(!check_three_way_comparable_with<
              int, int (S::*)() const volatile noexcept>());
static_assert(!check_three_way_comparable_with<int, int (S::*)() &>());
static_assert(!check_three_way_comparable_with<int, int (S::*)() & noexcept>());
static_assert(!check_three_way_comparable_with<int, int (S::*)() const&>());
static_assert(
    !check_three_way_comparable_with<int, int (S::*)() const & noexcept>());
static_assert(!check_three_way_comparable_with<int, int (S::*)() volatile&>());
static_assert(
    !check_three_way_comparable_with<int, int (S::*)() volatile & noexcept>());
static_assert(
    !check_three_way_comparable_with<int, int (S::*)() const volatile&>());
static_assert(!check_three_way_comparable_with<int, int (S::*)() const volatile &
                                                       noexcept>());
static_assert(!check_three_way_comparable_with<int, int (S::*)() &&>());
static_assert(!check_three_way_comparable_with < int,
              int (S::*)() && noexcept > ());
static_assert(!check_three_way_comparable_with<int, int (S::*)() const&&>());
static_assert(!check_three_way_comparable_with < int,
              int (S::*)() const&& noexcept > ());
static_assert(!check_three_way_comparable_with<int, int (S::*)() volatile&&>());
static_assert(!check_three_way_comparable_with < int,
              int (S::*)() volatile&& noexcept > ());
static_assert(
    !check_three_way_comparable_with<int, int (S::*)() const volatile&&>());
static_assert(!check_three_way_comparable_with < int,
              int (S::*)() const volatile&& noexcept > ());
static_assert(!check_three_way_comparable_with<int*, int[5]>());
static_assert(!check_three_way_comparable_with<int[5], int[5]>());
static_assert(!check_three_way_comparable_with<std::nullptr_t, int>());
static_assert(!check_three_way_comparable_with<std::nullptr_t, int*>());
static_assert(!check_three_way_comparable_with<std::nullptr_t, int[5]>());
static_assert(!check_three_way_comparable_with<std::nullptr_t, int (*)()>());
static_assert(!check_three_way_comparable_with<std::nullptr_t, int (&)()>());
static_assert(!check_three_way_comparable_with<std::nullptr_t, int (S::*)()>());
static_assert(!check_three_way_comparable_with<void, int>());
static_assert(!check_three_way_comparable_with<void, int*>());
static_assert(!check_three_way_comparable_with<void, std::nullptr_t>());
static_assert(!check_three_way_comparable_with<void, int[5]>());
static_assert(!check_three_way_comparable_with<void, int (*)()>());
static_assert(!check_three_way_comparable_with<void, int (&)()>());
static_assert(!check_three_way_comparable_with<void, int S::*>());
static_assert(!check_three_way_comparable_with<void, int (S::*)()>());
}

namespace user_defined {
struct S {
    bool operator==(int) const;
    std::strong_ordering operator<=>(int) const;
    operator int() const;

    bool operator==(const S&) const = default;
    auto operator<=>(const S&) const = default;
};

static_assert(check_three_way_comparable_with<S, int>());
static_assert(check_three_way_comparable_with<S, int, std::strong_ordering>());
static_assert(check_three_way_comparable_with<S, int, std::weak_ordering>());
}

int main(int, char**) { return 0; }
