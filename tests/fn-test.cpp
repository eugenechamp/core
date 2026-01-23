#include <core/def.hpp>
#include <core/function.hpp>
#include <gtest/gtest.h>

static_assert(sizeof(Fn<void()>) == sizeof(std::function<void()>));
static_assert(sizeof(Fn32<void()>) == 32);
static_assert(sizeof(Fn40<void()>) == 40);
static_assert(sizeof(Fn48<void()>) == 48);

//
// template<typename F>
// using Fn = std::function<F>;

void callme() {
  SUCCEED();
}

TEST(Fn, Default) {
  Fn<int()> fn = [] { return 42; };

  EXPECT_TRUE(fn);
  //EXPECT_FALSE(fn.empty());
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
  Fn<int()> fn = [] { return 11; };
  Fn<int()> fn2 = [] { return 22; };
  
  fn = std::move(fn2);

  EXPECT_EQ(fn(), 22);
}

i32 new_val() { return 42; }

TEST(Fn, CapturingLambda) {
  i32 n = 0;

  Fn<i32()> get_new_value = new_val;
  i32 new_value = get_new_value();
  
  Fn<i32(i32)> fn = [&n](i32 new_value) -> i32 {
    i32 old = n; 
    n = new_value; 
    
    return old;
  };

  i32 old = fn(new_value);

  EXPECT_TRUE(old == 0);
  EXPECT_TRUE(n != 0);
  EXPECT_TRUE(n == new_value);
}

struct Functor {
  void operator()() { std::cout << __func__ << "\n"; }

  ~Functor() {
    std::cout << __func__ << '\n'; 
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
  Fn fn = has_target_test;
  
  EXPECT_TRUE(fn.target<decltype(has_target_test)>() == has_target_test);
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
};

TEST(Fn, HasFunctorTarget) {
  Foo foo;
  std::string name = foo.name();

  Fn<std::string()> fn = MemberFn(&Foo::name, &foo);
  MemberFn* target = fn.target<MemberFn>();

  EXPECT_TRUE(target);
  EXPECT_EQ(target->func, &Foo::name);
  EXPECT_EQ(target->instance, &foo);
  EXPECT_EQ(fn(), name);
  EXPECT_EQ((*target)(), name);
}

TEST(Fn, BigFunctor) {
  
  struct BigFunctor {
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

  Fn fn = BigFunctor("Hello World ");
  (void)fn();
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
