// RUN: %clang_cc1 -std=c++2a -fassignable-lambdas -verify=expected,cxx20 %s
// RUN: %clang_cc1 -std=c++17 -fassignable-lambdas -verify=expected,cxx17 %s

// With -fassignable-lambdas, the copy and move assignment operators of a
// closure type follow the ordinary rules applied to its capture fields,
// instead of always being deleted when the lambda has a capture.
//
// Default-construction is deliberately unaffected: a lambda with a capture
// remains non-default-constructible even when it is assignable.

template <typename T>
constexpr bool copy_assignable = __is_assignable(T &, const T &);
template <typename T>
constexpr bool move_assignable = __is_assignable(T &, T &&);
template <typename T>
constexpr bool trivially_copy_assignable =
    __is_trivially_assignable(T &, const T &);

// A by-value capture of a trivially assignable type gives a trivially
// assignable closure type.
void value_capture(int i) {
  auto x = [i] { return i; };
  using X = decltype(x);
  static_assert(copy_assignable<X>);
  static_assert(move_assignable<X>);
  static_assert(trivially_copy_assignable<X>);
  x = x;
  x = static_cast<X &&>(x);
}

void copy_capture_default(int i) {
  auto x = [=] { return i; };
  using X = decltype(x);
  static_assert(copy_assignable<X>);
  static_assert(move_assignable<X>);
  static_assert(trivially_copy_assignable<X>);
  x = x;
}

// A capture by reference gives a reference member, which deletes both
// assignment operators -- exactly as for any other class.
void reference_capture(int i) {
  auto x = [&i] { return i; };
  using X = decltype(x);
  static_assert(!copy_assignable<X>);
  static_assert(!move_assignable<X>);
}

void reference_capture_default(int i) {
  auto x = [&] { return i; };
  using X = decltype(x);
  static_assert(!copy_assignable<X>);
  static_assert(!move_assignable<X>);
}

// A by-copy capture of a const object gives a const member.
void const_capture() {
  const int c = 0;
  auto x = [c] { return c; };
  using X = decltype(x);
  static_assert(!copy_assignable<X>);
  static_assert(!move_assignable<X>);
}

struct NoCopyAssign {
  NoCopyAssign();
  NoCopyAssign(const NoCopyAssign &);
  NoCopyAssign &operator=(const NoCopyAssign &) = delete;
};

void non_assignable_capture(NoCopyAssign n) {
  auto x = [n] {};
  using X = decltype(x);
  static_assert(!copy_assignable<X>);
  static_assert(!move_assignable<X>);
}

struct MoveOnly {
  MoveOnly();
  MoveOnly(MoveOnly &&);
  MoveOnly &operator=(MoveOnly &&);
  MoveOnly(const MoveOnly &) = delete;
  MoveOnly &operator=(const MoveOnly &) = delete;
};

// A move-only capture leaves the closure type move-assignable but not
// copy-assignable, like a class with a move-only member.
void move_only_capture() {
  MoveOnly m;
  auto x = [m = static_cast<MoveOnly &&>(m)] {};
  using X = decltype(x);
  static_assert(!copy_assignable<X>);
  static_assert(move_assignable<X>);
  x = static_cast<X &&>(x);
}

// [this] captures *this by reference. Although it is stored as a pointer, it
// is treated like any other by-reference capture: not assignable.
struct S {
  int n;
  void capture_this() {
    auto x = [this] { return n; }; // expected-note {{lambda expression begins here}}
    using X = decltype(x);
    static_assert(!copy_assignable<X>);
    static_assert(!move_assignable<X>);
    x = x; // expected-error {{cannot be assigned because its copy assignment operator is implicitly deleted}}
  }
  void capture_this_with_default() {
    auto x = [&, this] { return n; }; // equivalent to [&]
    static_assert(!copy_assignable<decltype(x)>);
    static_assert(!move_assignable<decltype(x)>);
  }
  void capture_this_implicitly() {
    auto x = [&] { return n; }; // captures *this by reference
    static_assert(!copy_assignable<decltype(x)>);
    static_assert(!move_assignable<decltype(x)>);
  }
  void capture_this_copy_default() {
    // [=] also captures *this by reference (deprecated in C++20).
    auto x = [=] { return n; }; // cxx20-warning {{implicit capture of 'this' with a capture default of '=' is deprecated}} \
                                // cxx20-note {{add an explicit capture of 'this'}}
    static_assert(!copy_assignable<decltype(x)>);
    static_assert(!move_assignable<decltype(x)>);
  }
  void capture_this_with_other() {
    int i = 0;
    auto x = [this, i] { return n + i; };
    static_assert(!copy_assignable<decltype(x)>);
    static_assert(!move_assignable<decltype(x)>);
  }
  // An init-capture copies the pointer by value, which is an ordinary pointer
  // member: assignable, and assignment rebinds it.
  void capture_this_pointer_by_copy() {
    auto x = [self = this] { return self->n; };
    static_assert(copy_assignable<decltype(x)>);
    static_assert(trivially_copy_assignable<decltype(x)>);
  }
  void capture_this_const() const {
    auto x = [this] { return n; };
    static_assert(!copy_assignable<decltype(x)>);
    static_assert(!move_assignable<decltype(x)>);
  }
  // [*this] captures a copy, so it follows the assignability of S.
  void capture_star_this() {
    auto x = [*this] { return n; };
    static_assert(copy_assignable<decltype(x)>);
    static_assert(move_assignable<decltype(x)>);
  }
  // ... which in a const member function is a const S member.
  void capture_star_this_const() const {
    auto x = [*this] { return n; };
    static_assert(!copy_assignable<decltype(x)>);
    static_assert(!move_assignable<decltype(x)>);
  }
};

// Assignability must not confer default-constructibility.
void default_construction_unaffected(int i) {
  auto x = [i] { return i; }; // expected-note 2{{candidate constructor}}
  decltype(x) y;              // expected-error {{no matching constructor}}
}

// A captureless lambda becomes assignable in every language mode with this
// flag, not just C++20. Default-construction still follows the standard rule.
void captureless() {
  auto x = [] {}; // cxx17-note 2{{candidate constructor}}
  using X = decltype(x);
  static_assert(copy_assignable<X>);
  static_assert(move_assignable<X>);
  decltype(x) y; // cxx17-error {{no matching constructor}}
  (void)y;
}

// Generic lambdas: exercises instantiation of the implicit assignment
// operators of a closure type inside a template.
template <typename T>
void generic(T t) {
  auto x = [t](auto &&u) { return u; };
  using X = decltype(x);
  static_assert(copy_assignable<X>);
  static_assert(move_assignable<X>);
  x = x;
  x = static_cast<X &&>(x);
}

void use_generic() { generic(42); }

// Assigning a closure type that is still dependent, and whose capture is
// itself a closure type. These shapes are what previously needed a
// TypeSourceInfo workaround for the implicit copy constructor of a lambda in
// a template instantiation, so pin that the assignment operators cope too.
template <typename F>
void assign_dependent(F f) {
  f = f;
  f = static_cast<F &&>(f);
  f.operator()(0);
}

template <typename F>
void capture_lambda(F f) {
  assign_dependent([f](auto i) { f.operator()(i); });
}

void use_capture_lambda() { capture_lambda([](auto i) {}); }
