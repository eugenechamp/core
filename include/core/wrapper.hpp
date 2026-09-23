#pragma once
#include "def.hpp"
#include <iosfwd>
#include <concepts>

template<typename From, typename To>
concept NoNarrowing = requires(From&& from) {
  { To{ CORE_FWD(from) } };
};

template<typename T>
concept HasLessThan = requires(const T& a, const T& b) {
  { a < b } -> std::convertible_to<bool>;
};

template<typename T>
concept HasLessThanOrEqual = requires(const T& a, const T& b) {
  { a <= b } -> std::convertible_to<bool>;
};

template<typename T>
concept HasGreaterThan = requires(const T& a, const T& b) {
  { a > b } -> std::convertible_to<bool>;
};

template<typename T>
concept HasGreaterThanOrEqual = requires(const T& a, const T& b) {
  { a >= b } -> std::convertible_to<bool>;
};

template<typename T>
struct Wrapper {
  using value_type  = T;
  
public:
  explicit constexpr Wrapper()  = default;
  constexpr ~Wrapper()          = default;
  
  constexpr Wrapper(Wrapper&&)                         = default;
  constexpr Wrapper(const Wrapper&)                    = default;
  constexpr auto operator=(Wrapper&&) -> Wrapper&      = default;
  constexpr auto operator=(const Wrapper&) -> Wrapper& = default;

  template<typename Arg, typename... Args> requires(NoNarrowing<Arg, T>)
  FORCE_INLINE explicit constexpr Wrapper(Arg&& arg, Args&&... args)
    : value_{ CORE_FWD(arg), CORE_FWD(args)... }
  {}
  
  FORCE_INLINE explicit constexpr operator value_type() {
    return value();
  }

  FORCE_INLINE constexpr auto operator*() -> value_type& {
    return value_;
  }
  
  FORCE_INLINE constexpr auto operator*() const -> const value_type& {
    return value_;
  }

  FORCE_INLINE constexpr auto operator->() -> value_type* {
    return &value_;
  }
  
  FORCE_INLINE constexpr auto operator->() const -> const value_type* {
    return &value_;
  }
  
  FORCE_INLINE constexpr auto value() -> value_type& {
    return value_;
  }
  
  FORCE_INLINE constexpr auto value() const -> const value_type& {
    return value_;
  }

  template<std::derived_from<Wrapper> WrapperType>
  FORCE_INLINE friend constexpr bool operator==(const WrapperType& lhs, const WrapperType& rhs)
    requires(std::equality_comparable<T>)
  {
    return lhs.value_ == rhs.value_;
  }

  template<std::derived_from<Wrapper> WrapperType>
  FORCE_INLINE friend constexpr bool operator<(const WrapperType& lhs, const WrapperType& rhs)
    requires(HasLessThan<T>)
  {
    return lhs.value_ < rhs.value_;
  }

  template<std::derived_from<Wrapper> WrapperType>
  FORCE_INLINE friend constexpr bool operator>(const WrapperType& lhs, const Wrapper& rhs)
    requires(HasGreaterThan<T>) 
  {
    return lhs.value_ > rhs.value_;
  }

  template<std::derived_from<Wrapper> WrapperType>
  FORCE_INLINE friend constexpr bool operator<=(const WrapperType& lhs, const WrapperType& rhs)
    requires(HasLessThanOrEqual<T>) 
  {
    return lhs.value_ <= rhs.value_;
  }

  template<std::derived_from<Wrapper> WrapperType>
  FORCE_INLINE friend constexpr bool operator>=(const WrapperType& lhs, const WrapperType& rhs)
    requires(HasGreaterThanOrEqual<T>) 
  {
    return lhs.value_ >= rhs.value_;
  }
    
private:
  value_type value_;
};

template<typename WrapperType>
requires std::is_base_of_v<Wrapper<typename WrapperType::value_type>, WrapperType>
struct std::hash<WrapperType> {
  auto operator()(const WrapperType& key) const noexcept -> size_t {
    return std::hash<typename WrapperType::value_type>{}(key.value());
  }
};

template<typename T>
concept OutStreamable = requires(std::ostream& out, const T& value) {
  { out << value } -> std::convertible_to<std::ostream&>;
};

template<typename T>
concept InStreamable = requires(std::istream& in, const T& value) {
  { in >> value } -> std::convertible_to<std::istream&>;
};

template<typename WrapperType> requires(
  std::derived_from<WrapperType, Wrapper<typename WrapperType::value_type>> 
  && OutStreamable<typename WrapperType::value_type>
)
auto operator<<(std::ostream& out, const WrapperType& source) -> std::ostream& {
  return out << source.value();
}

template<typename WrapperType> requires(
  std::derived_from<WrapperType, Wrapper<typename WrapperType::value_type>> &&
  InStreamable<typename WrapperType::value_type>
)
auto operator>>(std::istream& in, WrapperType& source) -> std::istream&
{
  return in >> source.value();
}