// Note: the -NOT patterns below are quoted so that they match only a cc1
// argument, and not this test's own file name on the command line.

// Off by default, and not forwarded to cc1 unless asked for.
// RUN: %clang -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-DEFAULT %s
// CHECK-DEFAULT-NOT: "-fassignable-lambdas"

// RUN: %clang -### -c -fassignable-lambdas %s 2>&1 \
// RUN:   | FileCheck -check-prefix=CHECK-ON %s
// CHECK-ON: "-fassignable-lambdas"

// The negative form cancels an earlier positive one, and is itself never
// forwarded.
// RUN: %clang -### -c -fno-assignable-lambdas %s 2>&1 \
// RUN:   | FileCheck -check-prefix=CHECK-OFF %s
// RUN: %clang -### -c -fassignable-lambdas -fno-assignable-lambdas %s 2>&1 \
// RUN:   | FileCheck -check-prefix=CHECK-OFF %s
// CHECK-OFF-NOT: "-fassignable-lambdas"
// CHECK-OFF-NOT: "-fno-assignable-lambdas"

// RUN: %clang -### -c -fno-assignable-lambdas -fassignable-lambdas %s 2>&1 \
// RUN:   | FileCheck -check-prefix=CHECK-ON %s

// Only meaningful for C++; not forwarded when compiling C.
// RUN: %clang -### -c -xc -fassignable-lambdas %s 2>&1 \
// RUN:   | FileCheck -check-prefix=CHECK-C %s
// CHECK-C-NOT: "-fassignable-lambdas"
