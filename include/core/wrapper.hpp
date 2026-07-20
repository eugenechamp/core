#pragma once
#include "def.hpp"
#include <iosfwd>
#include <concepts>

template<typename From, typename To>
concept NoNarrowing = requires(To&& to) {
  { From{ CORE_FWD(to) } };
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
  using value_type = T;
  
public:
  explicit constexpr Wrapper()  = default;
  constexpr ~Wrapper()          = default;
  
  constexpr Wrapper(Wrapper&&)                         = default;
  constexpr Wrapper(const Wrapper&)                    = default;
  constexpr auto operator=(Wrapper&&) -> Wrapper&      = default;
  constexpr auto operator=(const Wrapper&) -> Wrapper& = default;

  template<typename Arg, typename... Args> requires(NoNarrowing<T, Arg>)
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

  template<std::derived_from<Wrapper<T>> WrapperType>
  friend FORCE_INLINE constexpr bool operator==(const WrapperType& lhs, const WrapperType& rhs) const
    requires(std::equality_comparable<T>)
  {
    return value_ == other.value_;
  }
  
  FORCE_INLINE constexpr bool operator<(const Wrapper& other) const
    requires(HasLessThan<T>)
  {
    return value_ < other.value_;
  }

  FORCE_INLINE constexpr bool operator>(const Wrapper& other) const
    requires(HasGreaterThan<T>) 
  {
    return value_ > other.value_;
  }
  
  FORCE_INLINE constexpr bool operator<=(const Wrapper& other) const
    requires(HasLessThanOrEqual<T>) 
  {
    return value_ <= other.value_;
  }

  FORCE_INLINE constexpr bool operator>=(const Wrapper& other) const
    requires(HasGreaterThanOrEqual<T>) 
  {
    return value_ >= other.value_;
  }
    
private:
  value_type value_;
};


template<typename T>
requires std::is_base_of_v<Wrapper<typename T::value_type>, T>
struct std::hash<T> {
  auto operator()(const T& key) const noexcept -> size_t {
    return std::hash<typename T::value_type>{}(key.value());
  }
};

template<typename T>
concept OutStreamable = requires(std::ostream& out, const T& value) {
  { out << value } -> std::convertible_to<std::ostream&>;
};

template<typename T>
concept InStreamable = requires(std::istream& in, T& value) {
  { in >> value } -> std::convertible_to<std::istream&>;
};

template<typename T> requires(
  std::derived_from<T, Wrapper<typename T::value_type>> 
  && OutStreamable<typename T::value_type>
)
auto operator<<(std::ostream& out, const T& source) -> std::ostream& {
  return out << source.value();
}

template<typename T> requires(
  std::derived_from<Wrapper<typename T::value_type>, T> &&
  InStreamable<typename T::value_type>
)
auto operator>>(std::istream& in, T& source) -> std::istream&
{
  return in >> source.value();
}