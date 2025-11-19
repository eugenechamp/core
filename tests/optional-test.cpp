#include<gtest/gtest.h>

#include<core/optional.hpp>
#include<string>

std::string msg = "this is the message, and it\'s very long";

auto get_opt(bool success) -> Optional<std::string> {
  if(success)
    return msg;
  else
    return {};
}

auto get_opt_ref(bool success) -> Optional<const std::string&> {
  if(success)
    return msg;
  else
    return {};
}

TEST(Optional, Default) {
  Optional<std::string> opt;
  EXPECT_FALSE(opt);
  EXPECT_FALSE(opt.has_value());
}

TEST(Optional, Copy) {
  Optional<std::string> copy;
  Optional<std::string> opt;
  
  opt = Optional{ std::string{ "Copy me" } };
  copy = opt;

  EXPECT_EQ(opt.value(), copy.value());
}

TEST(Optional, Move) {
  Optional<std::string> other;
  Optional opt = Optional{ std::string{ "Copy me" } };

  EXPECT_TRUE(opt.has_value());

  other = std::move(opt);
  EXPECT_TRUE(other.has_value());
  EXPECT_FALSE(opt.has_value());
}

TEST(Optional, HasValue) {
  Optional<std::string> opt = get_opt(true);
  EXPECT_TRUE(opt);
  EXPECT_TRUE(opt.has_value());
}

TEST(Optional, ValueOr) {
  std::string err = "Error";
  EXPECT_TRUE(get_opt(true).value_or(err) == msg);
  EXPECT_TRUE(get_opt(false).value_or(err) == err);
}

TEST(Optional, NullOpt) {
  Optional<std::string> opt = Nullopt{};

  EXPECT_FALSE(opt);
  EXPECT_FALSE(opt.has_value());
}

TEST(Optional, RefHasValue) {
  Optional opt = get_opt_ref(true);
  EXPECT_TRUE(opt);
  EXPECT_TRUE(opt.has_value());

  EXPECT_EQ(opt.value().data(), msg.data()); // expect pointers to point to the same memory location
}