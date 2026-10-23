// RUN: %clang_cc1 -std=c++17 -fassignable-lambdas -triple x86_64-linux-gnu \
// RUN:     -emit-llvm -o - %s | FileCheck %s

// With -fassignable-lambdas the implicit assignment operators of a closure
// type can be odr-used, which never happened before. Check that they mangle
// and emit.

struct Val {
  int n;
  Val();
  Val(const Val &);
  Val &operator=(const Val &);
  Val &operator=(Val &&);
};

// A trivially assignable capture: assignment is a plain memcpy, with no
// out-of-line operator= for the closure type at all.
// CHECK-LABEL: define {{.*}}void @_Z12trivial_copyi(
// CHECK:         call void @llvm.memcpy
// CHECK:       }
void trivial_copy(int i) {
  auto x = [i] { return i; };
  auto y = x;
  y = x;
}

// A non-trivial capture forces an out-of-line implicit operator= for the
// closure type, mangled as the lambda's own scope ('$_0' here) plus 'aS'.
// CHECK-LABEL: define {{.*}}void @_Z15nontrivial_copy3Val(
// CHECK:         call {{.*}}@"_ZZ15nontrivial_copy3ValEN3$_0aSERKS0_"(
// CHECK:         call {{.*}}@"_ZZ15nontrivial_copy3ValEN3$_0aSEOS0_"(
// CHECK:       }
void nontrivial_copy(Val v) {
  auto x = [v] { return v.n; };
  auto y = x;
  y = x;
  y = static_cast<decltype(x) &&>(x);
}

// The closure type's implicit copy assignment operator: memberwise assignment
// of the single capture field.
// CHECK-LABEL: define {{.*}}@"_ZZ15nontrivial_copy3ValEN3$_0aSERKS0_"(
// CHECK:         call {{.*}}@_ZN3ValaSERKS_(
// CHECK:       }

// The closure type's implicit move assignment operator.
// CHECK-LABEL: define {{.*}}@"_ZZ15nontrivial_copy3ValEN3$_0aSEOS0_"(
// CHECK:         call {{.*}}@_ZN3ValaSEOS_(
// CHECK:       }
