#include <core/def.hpp>
#include <core/memory.hpp>
#include <gtest/gtest.h>

static_assert(sizeof(Unique<i32>) == sizeof(i32*));

TEST(Unique, RAII) {
  struct RAII {
    ~RAII() {
      SUCCEED();
    }
  };
  Unique<RAII> ptr(new RAII);
}

TEST(Unique, Default) {
  Unique<i32> default_initialized;
  
  EXPECT_FALSE(default_initialized);
  EXPECT_FALSE(default_initialized.valid());

  Unique<i32> explicitly_nulled = nullptr;

  EXPECT_FALSE(explicitly_nulled);
  EXPECT_FALSE(explicitly_nulled.valid());
}

TEST(Unique, Construct) {
  Unique<std::string> ptr{ new std::string };
  
  EXPECT_TRUE(ptr);
}

TEST(Unique, Move) {
  Unique ptr = Unique<std::string>(new std::string("Hello, World"));

  Unique<std::string> move_to = nullptr;
  move_to = std::move(ptr);

  EXPECT_TRUE(move_to);
  EXPECT_FALSE(ptr);
}

struct CheckRelease {
  CheckRelease(bool& is_released)
    : is_released{is_released}
  { 
    is_released = false;
  }

  ~CheckRelease() {
    is_released = true;
  }

  bool& is_released;
};

TEST(Unique, Reset) {
  {
    bool is_released = false;
    Unique ptr = Unique<CheckRelease>(new CheckRelease(is_released));

    ptr = Unique<CheckRelease>();

    EXPECT_FALSE(ptr);
    EXPECT_TRUE(is_released);
  }

  {
    bool is_released = false;
    Unique ptr = Unique<CheckRelease>(new CheckRelease(is_released));

    ptr.reset();

    EXPECT_FALSE(ptr);
    EXPECT_TRUE(is_released);
  }

  {
    bool is_released = false;
    Unique ptr = Unique<CheckRelease>(new CheckRelease(is_released));
    
    ptr = nullptr;

    EXPECT_FALSE(ptr);
    EXPECT_TRUE(is_released);
  }

}

