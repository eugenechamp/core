#include <core/def.hpp>
#include <core/vector.hpp>
#include <gtest/gtest.h>

static_assert(sizeof(Vec<i32>) == 16u, "it is requiered that Vec<T> is 16 bytes in size instead of 24");

TEST(VectorTest, GrowOnPushBack) {
  Vec<i32> vec;
  size_t size = 10;
  for(auto i = 0ull; i < size; i++)
  {
    vec.push_back(69);
  }
  
  std::unique_ptr<int[]> array;

  EXPECT_EQ(vec.size(), size);
}

TEST(VectorTest, GetFront) {
  Vec<i32> vec;
  vec.push_back(42);
  vec.push_back(69);
  
  EXPECT_EQ(vec.front(), 42);
}

TEST(VectorTest, Back) {
  Vec<i32> vec;
  vec.push_back(42);
  vec.push_back(42);
  vec.push_back(42);
  vec.push_back(42);
  vec.push_back(69);
  
  EXPECT_EQ(vec[vec.size() - 1], 69);
  EXPECT_EQ(vec.back(), 69);
  //EXPECT_EQ(*vec.at(vec.size() - 1), 69);
}

TEST(VectorTest, Resize) {
  Vec<i32> vec;
  vec.resize(100);
  
  EXPECT_EQ(vec.size(), 100);
  vec.resize(77);
  EXPECT_EQ(vec.size(), 77);
  vec.resize(0);
  EXPECT_EQ(vec.size(), 0);
}

TEST(VectorTest, Reserve) {
  Vec<i32> vec;
  vec.reserve(100);
  
  EXPECT_EQ(vec.capacity(), 100);
  EXPECT_EQ(vec.size(), 0);
}

TEST(VectorTest, Empty) {
  Vec<i32> vec;
  EXPECT_EQ(vec.empty(), true);
  vec.reserve(10);
  EXPECT_EQ(vec.empty(), true);
  vec.push_back(42);
  EXPECT_EQ(vec.empty(), false);
}

TEST(VectorTest, Clear) {
  Vec<i32> vec;
  
  for (auto i = 0; i < 1'000; i++)
  {
    vec.emplace_back(i * i);
  }

  ASSERT_EQ(vec.size(), 1'000);
  ASSERT_TRUE(!vec.empty());
  
  vec.clear();
  EXPECT_TRUE(vec.empty());
}

TEST(VectorTest, InitializerList) {
  Vec<i32> vec = {1,2,3,4,5,6};
  EXPECT_EQ(vec.size(), 6);
}

TEST(VectorTest, Copy) {
  auto sum = [](const Vec<i32>& vec) -> i32 {
    i32 result = 0;
    for (auto n : vec)
    {
      result += n;
    }
    return result;
  };
  
  Vec<i32> vec  = Vec{ 1, 2, 3, 4, 5, 6 };
  Vec<i32> copy = vec;

  EXPECT_EQ(vec.size(), copy.size());
  EXPECT_EQ(sum(vec), sum(copy));
  
  Vec<i32> seven = Vec{ 1,2,3,4,5,6,7 };
  vec = seven;

  EXPECT_EQ(seven.size(), vec.size());
  EXPECT_EQ(sum(vec), sum(seven));
}

TEST(VectorTest, Move) {
  Vec<i32> vec;
  
  EXPECT_EQ(vec.size(), 0);
  EXPECT_TRUE(vec.empty());
  
  vec = Vec {1, 2, 3, 4, 5, 6};

  EXPECT_EQ(vec.size(), 6);
  EXPECT_TRUE(!vec.empty());
  
  Vec<i32> moved = std::move(vec);
  
  EXPECT_EQ(moved.size(), 6);
  EXPECT_TRUE(!moved.empty());

  EXPECT_EQ(vec.size(), 0);
  EXPECT_TRUE(vec.empty());
}

// TEST(VectorTest, Insert) {
//   Vec<i32> vec {1, 2, 8, 9};
//   Vec<i32> vec2 {3, 4, 5, 6};

//   vec.insert(vec.begin() + 2, vec2.begin(), vec2.end());
//   EXPECT_EQ(vec.size(), 8);

//   vec.insert(vec.begin() + 6, 7);
//   EXPECT_EQ(vec.size(), 9);

//   vec.insert(vec.begin() + 6, 3u, 7);
//   EXPECT_EQ(vec.size(), 12);
  
//   vec.insert(vec.end(), {10, 11, 12});
//   EXPECT_EQ(vec.size(), 15);
// }

TEST(VectorTest, Append) {
  Vec<i32> vec { 1, 2, 3, };
  auto vec_size = vec.size();

  Vec<i32> vec_two { 3, 4, 5, 6 };
  auto vec_two_size = vec_two.size();

  vec.append(vec_two);

  EXPECT_EQ(vec.size(), vec_size + vec_two_size);
  EXPECT_EQ(vec[3], 3);
  EXPECT_EQ(vec[4], 4);
  EXPECT_EQ(vec[5], 5);
  EXPECT_EQ(vec[6], 6);
}

TEST(VectorTest, EndCheck) {
  Vec<i32> vec { 1, 2, 3, };
  for(auto num : vec)
  {
    std::cout << num << '\n';
  }
}


TEST(VectorTest, std_function) {
  
  Vec<std::function<void()>> fxs;
  fxs.emplace_back([&](){});
  fxs.emplace_back([&](){});
  fxs.emplace_back([&](){});
  
  for(auto& fx : fxs)
  {
    fx();
  }
}