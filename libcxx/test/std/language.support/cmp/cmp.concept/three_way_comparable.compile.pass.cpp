//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17
// UNSUPPORTED: libcpp-no-concepts

// template<class T, class Cat = partial_ordering>
// concept three_way_comparable = // see below

#include <compare>

#include "compare_types.h"

namespace fundamentals {
// with default ordering
static_assert(std::three_way_comparable<int>);
static_assert(std::three_way_comparable<double>);
static_assert(std::three_way_comparable<void*>);
static_assert(std::three_way_comparable<char*>);
static_assert(std::three_way_comparable<char const*>);
static_assert(std::three_way_comparable<char volatile*>);
static_assert(std::three_way_comparable<char const volatile*>);
static_assert(std::three_way_comparable<wchar_t&>);
static_assert(std::three_way_comparable<char8_t const&>);
static_assert(std::three_way_comparable<char16_t volatile&>);
static_assert(std::three_way_comparable<char32_t const volatile&>);
static_assert(std::three_way_comparable<unsigned char&&>);
static_assert(std::three_way_comparable<unsigned short const&&>);
static_assert(std::three_way_comparable<unsigned int volatile&&>);
static_assert(std::three_way_comparable<unsigned long const volatile&&>);

// with explicit ordering
static_assert(std::three_way_comparable<int, std::strong_ordering>);
static_assert(std::three_way_comparable<int, std::weak_ordering>);
static_assert(std::three_way_comparable<double, std::partial_ordering>);
static_assert(!std::three_way_comparable<double, std::weak_ordering>);
static_assert(std::three_way_comparable<void*, std::strong_ordering>);
static_assert(std::three_way_comparable<void*, std::weak_ordering>);
static_assert(std::three_way_comparable<char*, std::strong_ordering>);
static_assert(std::three_way_comparable<char*, std::weak_ordering>);
static_assert(std::three_way_comparable<char const*, std::strong_ordering>);
static_assert(std::three_way_comparable<char const*, std::weak_ordering>);
static_assert(std::three_way_comparable<char volatile*, std::strong_ordering>);
static_assert(std::three_way_comparable<char volatile*, std::weak_ordering>);
static_assert(std::three_way_comparable<char const volatile*, std::strong_ordering>);
static_assert(std::three_way_comparable<char const volatile*, std::weak_ordering>);
static_assert(std::three_way_comparable<wchar_t&, std::strong_ordering>);
static_assert(std::three_way_comparable<wchar_t&, std::weak_ordering>);
static_assert(std::three_way_comparable<char8_t const&, std::strong_ordering>);
static_assert(std::three_way_comparable<char8_t const&, std::weak_ordering>);
static_assert(std::three_way_comparable<char16_t volatile&, std::strong_ordering>);
static_assert(std::three_way_comparable<char16_t volatile&, std::weak_ordering>);
static_assert(std::three_way_comparable<char32_t const volatile&, std::strong_ordering>);
static_assert(std::three_way_comparable<char32_t const volatile&, std::weak_ordering>);
static_assert(std::three_way_comparable<unsigned char&&, std::strong_ordering>);
static_assert(std::three_way_comparable<unsigned char&&, std::weak_ordering>);
static_assert(std::three_way_comparable<unsigned short const&&, std::strong_ordering>);
static_assert(std::three_way_comparable<unsigned short const&&, std::weak_ordering>);
static_assert(std::three_way_comparable<unsigned int volatile&&, std::strong_ordering>);
static_assert(std::three_way_comparable<unsigned int volatile&&, std::weak_ordering>);
static_assert(std::three_way_comparable<unsigned long const volatile&&, std::strong_ordering>);
static_assert(std::three_way_comparable<unsigned long const volatile&&, std::weak_ordering>);

static_assert(!std::three_way_comparable<int[5]>);
static_assert(!std::three_way_comparable<int (*)(int)>);
static_assert(!std::three_way_comparable<int (&)(int)>);
static_assert(!std::three_way_comparable<int (*)(int) noexcept>);
static_assert(!std::three_way_comparable<int (&)(int) noexcept>);
static_assert(!std::three_way_comparable<std::nullptr_t>);

struct S {};
static_assert(!std::three_way_comparable<int S::*>);
static_assert(!std::three_way_comparable<int (S::*)()>);
static_assert(!std::three_way_comparable<int (S::*)() noexcept>);
static_assert(!std::three_way_comparable<int (S::*)() &>);
static_assert(!std::three_way_comparable<int (S::*)() & noexcept>);
static_assert(!std::three_way_comparable<int (S::*)() &&>);
static_assert(!std::three_way_comparable<int (S::*)() && noexcept>);
static_assert(!std::three_way_comparable<int (S::*)() const>);
static_assert(!std::three_way_comparable<int (S::*)() const noexcept>);
static_assert(!std::three_way_comparable<int (S::*)() const&>);
static_assert(!std::three_way_comparable<int (S::*)() const & noexcept>);
static_assert(!std::three_way_comparable<int (S::*)() const&&>);
static_assert(!std::three_way_comparable<int (S::*)() const && noexcept>);
static_assert(!std::three_way_comparable<int (S::*)() volatile>);
static_assert(!std::three_way_comparable<int (S::*)() volatile noexcept>);
static_assert(!std::three_way_comparable<int (S::*)() volatile&>);
static_assert(!std::three_way_comparable<int (S::*)() volatile & noexcept>);
static_assert(!std::three_way_comparable<int (S::*)() volatile&&>);
static_assert(!std::three_way_comparable<int (S::*)() volatile && noexcept>);
static_assert(!std::three_way_comparable<int (S::*)() const volatile>);
static_assert(!std::three_way_comparable<int (S::*)() const volatile noexcept>);
static_assert(!std::three_way_comparable<int (S::*)() const volatile&>);
static_assert(!std::three_way_comparable<int (S::*)() const volatile & noexcept>);
static_assert(!std::three_way_comparable<int (S::*)() const volatile&&>);
static_assert(
    !std::three_way_comparable<int (S::*)() const volatile && noexcept>);

static_assert(!std::three_way_comparable<void>);
} // namespace fundamentals

namespace user_defined {

struct S {
    auto operator<=>(const S&) const = default;
};

static_assert(std::three_way_comparable<S>);
static_assert(std::three_way_comparable<S, std::strong_ordering>);
static_assert(std::three_way_comparable<S, std::partial_ordering>);
}

int main(int, char**) { return 0; }
