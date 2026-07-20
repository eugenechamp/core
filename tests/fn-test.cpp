#include <core/def.hpp>
#include <core/function.hpp>
#include <gtest/gtest.h>

static_assert(sizeof(Fn<void()>) == sizeof(std::function<void()>));
static_assert(sizeof(Fn24<void()>) == 24ULL);
static_assert(sizeof(Fn32<void()>) == 32ULL);
static_assert(sizeof(Fn40<void()>) == 40ULL);
static_assert(sizeof(Fn48<void()>) == 48ULL);

void callme() {
  SUCCEED();
}

TEST(Fn, Default) {
  Fn<int()> fn = [] () -> int { return 42; };
  
  EXPECT_TRUE(fn);
  EXPECT_FALSE(fn.empty());
  EXPECT_EQ(fn(), 42);
}

TEST(Fn, Call) {
  Fn<void()> fn = [] { SUCCEED(); };
  fn();
}

TEST(Fn, FunctionPointer) {
  Fn<void()> fn = callme;
  fn();
}

TEST(Fn, Move) {
  Fn<int()> fn = []() -> int { return 11; };
  Fn<int()> fn2 = []() -> int { return 22; };
  
  void(*ptr)() = callme;
  
  Fn<void()> ffff;
  ffff = ptr;
  ffff();

  fn = std::move(fn2);

  EXPECT_EQ(fn(), 22);
}

auto new_val() -> i32 { return 42; }

TEST(Fn, CapturingLambda) {
  i32 n = 0;

  Fn<auto()->i32> get_new_value = new_val;

  i32 new_value = get_new_value();
  
  Fn<auto(i32)->i32> fn = [&n](i32 new_value) -> i32 {
    i32 old = n; 
    // n = new_value; 
    
    return old;
  };

  i32 old = fn(new_value);

  EXPECT_TRUE(old == 0);
  EXPECT_TRUE(n != 0);
  EXPECT_TRUE(n == new_value);
}

struct Functor {
  void operator()() { SUCCEED(); }

  ~Functor() {
    SUCCEED();
  }
};

TEST(Fn, DestructorCall) {
  Fn fn = Functor();
  fn();
}

void has_target_test() { std::cout << __func__; }
void has_target_test_2() { std::cout << __func__; }
int has_target_test_3() { std::cout << __func__; return 42; }

TEST(Fn, HasTarget) {
  auto* ptr = has_target_test;
  Fn<void()> fn = ptr;
  
  EXPECT_TRUE(fn.target<decltype(has_target_test)>() == has_target_test);
  EXPECT_TRUE(fn.target<void()>() != nullptr);
  EXPECT_TRUE(fn.target<void()>() == has_target_test);
  EXPECT_TRUE(fn.target<void()>() != has_target_test_2);
}

struct Foo {
  std::string name() const {
    return "Foo";
  }
};

struct MemberFn {
  std::string(Foo::* func)() const;
  Foo* instance;

  std::string operator()() const {
    return (instance->*func)();
  }

  bool operator==(const MemberFn& that) const {
    return func == that.func && instance == that.instance;
  }
};

TEST(Fn, HasFunctorTarget) {
  Foo foo;
  std::string name = foo.name();
  
  auto lambda = []() -> std::string { return "lambda\n"; };
  
  Fn<std::string()> fn = MemberFn(&Foo::name, &foo);
  
  EXPECT_FALSE(fn.target<std::string()>());
  EXPECT_FALSE(fn.target<decltype(lambda)>());
  
  MemberFn* target = fn.target<MemberFn>();

  EXPECT_TRUE(target);
  
  EXPECT_EQ(target->func, &Foo::name);
  EXPECT_EQ(target->instance, &foo);
  EXPECT_TRUE(fn.holds(MemberFn(&Foo::name, &foo)));
  
  EXPECT_EQ(fn(), name);
  EXPECT_EQ((*target)(), name);
}

TEST(Fn, HasLambda) {
  auto lambda = []() -> std::string { return "lambda"; };
  using Lambda = decltype(lambda);

  Fn<std::string()> fn = lambda;

  Lambda* target = fn.target<Lambda>();

  EXPECT_TRUE(target);
  EXPECT_EQ(fn(), lambda());
  EXPECT_EQ((*target)(), lambda());
}

TEST(Fn, BigFunctor) {
  
  struct alignas(8) BigFunctor {
    explicit BigFunctor(std::string_view sv) {
      
      size_t paste_count = sizeof(str) / sv.size();

      for(size_t i = 0; i < paste_count; i++)
      {
        for(size_t ic{0}; auto c : sv) 
        {
          size_t offset = sv.size() * i;
          str[offset + ic++] = c;
        }
      }
    }

    char* operator()() {
      SUCCEED();
      return str;
    }

    char str[256]{};
  };

  Fn<char*()> fn = BigFunctor("Hello World");
  (void)fn();
}

TEST(Fn, reset) {
  Fn<size_t(const std::string&)> fn;
   
  fn = [](const std::string& str) -> size_t { return str.size(); };
  
  EXPECT_FALSE(fn.empty());
  EXPECT_TRUE(fn);

  fn.reset();

  EXPECT_TRUE(fn.empty());
  EXPECT_FALSE(fn);
}

void one() {}
void two() {}

TEST(Fn, F__K_MSVC){
  static_assert(one != two); // false with optimizations on at runtime

  // you see that sh*t????
  // #if _DEBUG
  //   EXPECT_FALSE(one == two);
  // #else 
  //   EXPECT_TRUE(one == two);
  // #endif
}