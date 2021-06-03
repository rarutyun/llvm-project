//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14, c++17
// UNSUPPORTED: libcpp-no-concepts

// <compare>

// compare_three_way_result, compare_three_way_result_t

#include <compare>
#include <type_traits>

template <class T, class U>
concept compare_three_way_result_type_defined = requires {
    typename std::compare_three_way_result<T, U>::type;
};

template <class T, class U>
concept compare_three_way_result_t_defined = requires {
    typename std::compare_three_way_result_t<T, U>;
};

struct OtherOperand {
    friend std::strong_ordering operator<=>( const OtherOperand&, const OtherOperand& ) = delete;
};

template <class SameOperandsOrdering, class DifferentOperandsOrdering>
struct Operand {
    SameOperandsOrdering operator<=>( const Operand& ) const = default;
    friend DifferentOperandsOrdering operator<=>( const Operand&, const OtherOperand& ) {
        return DifferentOperandsOrdering::equivalent;
    }
};

template <class SameOperandsOrdering, class DifferentOperandsOrdering>
constexpr void do_compare_three_way_result_positive_test() {
    using OperandType = Operand<SameOperandsOrdering, DifferentOperandsOrdering>;
    static_assert(std::is_same_v<std::compare_three_way_result_t<OperandType>, SameOperandsOrdering>);
    static_assert(std::is_same_v<std::compare_three_way_result_t<OperandType, OperandType>, SameOperandsOrdering>);
    static_assert(std::is_same_v<std::compare_three_way_result_t<std::add_lvalue_reference_t<OperandType>, std::add_lvalue_reference_t<OperandType>>,
                                SameOperandsOrdering>);
    static_assert(std::is_same_v<std::compare_three_way_result_t<OperandType, OtherOperand>, DifferentOperandsOrdering>);
    static_assert(std::is_same_v<std::compare_three_way_result_t<OtherOperand, OperandType>, DifferentOperandsOrdering>);
    static_assert(std::is_same_v<std::compare_three_way_result_t<std::add_lvalue_reference_t<OtherOperand>, std::add_lvalue_reference_t<OperandType>>,
                                DifferentOperandsOrdering>);

    static_assert(std::is_same_v<std::compare_three_way_result_t<OperandType>,
                                typename std::compare_three_way_result<OperandType>::type>);
    static_assert(std::is_same_v<std::compare_three_way_result_t<OperandType, OperandType>,
                                typename std::compare_three_way_result<OperandType, OperandType>::type>);
    static_assert(std::is_same_v<std::compare_three_way_result_t<OperandType, OtherOperand>,
                                typename std::compare_three_way_result<OperandType, OtherOperand>::type>);
    static_assert(std::is_same_v<std::compare_three_way_result_t<OtherOperand, OperandType>,
                                typename std::compare_three_way_result<OtherOperand, OperandType>::type>);
    static_assert(std::is_same_v<std::compare_three_way_result_t<std::add_lvalue_reference_t<OperandType>,
                                                                std::add_lvalue_reference_t<OperandType>>,
                                typename std::compare_three_way_result<OperandType, OperandType>::type>);
}

constexpr void do_compare_three_way_result_negative_test() {
    static_assert(!compare_three_way_result_type_defined<OtherOperand, OtherOperand>);
    static_assert(!compare_three_way_result_type_defined<OtherOperand, int>);
    static_assert(!compare_three_way_result_type_defined<int, OtherOperand>);

    static_assert(!compare_three_way_result_t_defined<OtherOperand, OtherOperand>);
    static_assert(!compare_three_way_result_t_defined<OtherOperand, int>);
    static_assert(!compare_three_way_result_t_defined<int, OtherOperand>);
}

int main(int, char**) {
    do_compare_three_way_result_positive_test<std::strong_ordering, std::weak_ordering>();
    do_compare_three_way_result_positive_test<std::weak_ordering, std::partial_ordering>();
    do_compare_three_way_result_positive_test<std::partial_ordering, std::strong_ordering>();
    do_compare_three_way_result_negative_test();
}
