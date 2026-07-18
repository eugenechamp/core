#ifndef CORE_STRING_HASH_HPP
#define CORE_STRING_HASH_HPP

#ifdef CORE_PRAGMA_ONCE
  #pragma once
#endif

#include "def.hpp"

namespace core {

  namespace _ {
    template<typename>
    struct fnv1a_constants;
    
    template<>
    struct fnv1a_constants<u32> {
      static constexpr u32 offset = 2166136261u;
      static constexpr u32 prime  = 16777619u;
    };
  
    template<>
    struct fnv1a_constants<u64> {
      static constexpr u64 offset = 14695981039346656037ull;
      static constexpr u64 prime  = 1099511628211ull;
    };
  }

  template<typename IdType>
  struct fnv1a {
    using HashType = IdType;

    static constexpr auto hash(StringView str) -> HashType {
      constexpr auto prime = _::fnv1a_constants<HashType>::prime;
      HashType hash        = _::fnv1a_constants<HashType>::offset;
      for (auto&& c : str)
        hash = (hash ^ static_cast<HashType>(c)) * prime;

      return hash;
    }
  };
}

template<typename IdType>
class BasicStringHash {
public:
  using HashType = IdType;

  constexpr BasicStringHash()
  : value_{ core::fnv1a<HashType>::hash("") }
  {}
  
  constexpr ~BasicStringHash() = default;

  explicit constexpr BasicStringHash(StringView string)
    : value_{ core::fnv1a<HashType>::hash(string) }
  {}

  constexpr BasicStringHash(const BasicStringHash&) = default;
  constexpr BasicStringHash(BasicStringHash&&)      = default;

  auto operator=(const BasicStringHash&) -> BasicStringHash & = default;
  auto operator=(BasicStringHash&&) -> BasicStringHash & = default;

  constexpr bool operator==(const BasicStringHash&) const = default;
  constexpr bool operator!=(const BasicStringHash&) const = default;
  constexpr bool operator<(const BasicStringHash&) const  = default;
  constexpr bool operator>(const BasicStringHash&) const  = default;

  constexpr auto value() const -> HashType {
    return value_;
  }

  constexpr bool is_empty() const {
    return *this == BasicStringHash("");
  }

  /* explicit */ 
  constexpr operator HashType() {
    return value();
  }

private:
  HashType value_;
};

using StringHash   = BasicStringHash<u64>;
using StringHash32 = BasicStringHash<u32>;

template<typename HashType>
struct std::hash<BasicStringHash<HashType>> {
  auto operator()(const BasicStringHash<HashType>& sh) const noexcept -> size_t {
    return std::hash<HashType>()(sh.value());
  }
};

namespace core::experimental {
  constexpr auto sh32(StringView str) -> StringHash32 {
    return StringHash32(str);
  }

  constexpr auto sh64(StringView str) -> StringHash {
    return StringHash(str);
  }
  
  constexpr auto shv32(StringView str) -> StringHash32::HashType {
    return StringHash32(str).value();
  }

  constexpr auto shv64(StringView str) -> StringHash::HashType {
    return StringHash(str).value();
  }
}

namespace core::sh_literals {
  constexpr auto operator""_sh32(const char* string, size_t len) -> StringHash32 {
    return StringHash32({string, len});
  }
  
  constexpr auto operator""_sh64(const char* string, size_t len) -> StringHash {
    return StringHash({string, len});
  }
}

#endif /* CORE_STRING_HASH_HPP */