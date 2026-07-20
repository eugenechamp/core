#ifndef _CORE_OPTIONAL_
#define _CORE_OPTIONAL_
#pragma once

#include <utility>
#include "def.hpp"

struct Nullopt {};

template<typename T>
class Optional {
public:
  using value_type = T;

public:
  Optional() 
    : has_value_{false}
  { }
  
  ~Optional() = default;
  
  Optional(const Optional&)                    = default;
  auto operator=(const Optional&) -> Optional& = default;

  Optional(Optional&& other) noexcept {
    if(other.has_value())
    {
      value_ = std::move(other.value());
      has_value_ = true;
      
      other.has_value_ = false;
    }
  }

  auto operator=(Optional&& other) noexcept -> Optional& {
    if(this == &other)
      return *this;
    
    if(other.has_value()) 
    {
      value_ = std::move(other.value());
      has_value_ = true;

      other.has_value_ = false;
    }

    return *this;
  }

  Optional& operator=(Nullopt) {
    if(has_value()) 
    {
      value_ = {};
    }
    has_value_ = false;
  }

  Optional(const T& val)
    : value_{ val }, has_value_{true}
  { }

  Optional(T&& val)
    : value_{ std::move(val) }, has_value_{true}
  { }

  Optional(Nullopt)
    : has_value_{ false }
  { }
  
  explicit operator bool() const noexcept { 
    return has_value(); 
  }
  
  bool has_value() const noexcept {
    return has_value_;
  }

  auto value() -> value_type& {
    return value_;
  }

  auto value_or(const value_type& val) -> value_type {
    if(!has_value())
    {
      return val;
    }
    return value_;
  }

private:
  // NOTE: may default initialize the value, shouldn't be an issue at all, 
  // but consider using "alignas(T) char[sizeof(T)]" with placement new if perf hurts
  T value_;
  bool has_value_ = false;
};

template<typename T>
class Optional<T&> {
public:
  using value_type = T&;
  
public:
  Optional() = default;
  ~Optional() = default;

  Optional(const Optional&)                    = default;
  auto operator=(const Optional&) -> Optional& = default;

  Optional(Optional&&) noexcept                    = default;
  auto operator=(Optional&&) noexcept -> Optional& = default;

  Optional(value_type val)
    : value_{ &val }
  { }

  Optional(Nullopt)
    : value_{ nullptr }
  { }

  bool has_value() const noexcept {
    return value_;
  }
  
  explicit operator bool() const noexcept { 
    return has_value(); 
  }
  
  auto value() -> value_type {
    return *value_;
  }
  
  auto value() const -> const value_type {
    return *value_;
  }

  auto value_or(const value_type val) const -> value_type {
    if (!has_value())
    {
      return val;
    }
    return *value_;
  }

private:
  T* value_ = nullptr;
};

#endif /* _CORE_OPTIONAL_ */