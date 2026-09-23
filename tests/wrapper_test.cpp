#include <gtest/gtest.h>
#include <core/wrapper.hpp>

struct Quantity: Wrapper<u32> {
  using Wrapper::Wrapper;
};

struct Quality: Wrapper<u32> {
  using Wrapper::Wrapper;
};

struct Price: Wrapper<f64> {
  using Wrapper::Wrapper;
};

struct FirstName: Wrapper<std::string> {
  using Wrapper::Wrapper;
};

struct LastName: Wrapper<std::string> {
  using Wrapper::Wrapper;
};

static_assert(!std::same_as<FirstName, LastName>);
static_assert(std::same_as<FirstName::value_type, LastName::value_type>);
static_assert(!std::same_as<FirstName, std::string>);

void print_full_name(const FirstName& name, const LastName& surname) {
  std::cout << name << " " << surname << '\n';
}

void purchase_product(Quantity quantity, Price price) {
  std::cout << "Purchase has been made Price: " << price << " Quantity: " << quantity << "\n";
}

TEST(WrapperTest, PrintName) {
  print_full_name(FirstName{ "Walter" }, LastName{ "White" });
  purchase_product(Quantity{ 10u }, Price{ 975.0 });
}

TEST(WrapperTest, ValueTypeConversion) {
  // explicit cast to u32 to construct Quantity from Quality!!!
  // doesn't make sense, but because Wrapper<T> defines explicit value_type() operator it's possible here
  Quality q{ 42u };
  Quantity quant{ q };

  constexpr u32 number{ Quality(32u) }; // you can see how you can initialize a u32 variable with Quality;
  (void)number;

  // Quantity quant1 = q; // <-- will not compile no suitable user-defined conversion from "Quality" to "Quantity" exists
}