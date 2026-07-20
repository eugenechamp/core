#ifndef CORE_STRING_ID_HPP
#define CORE_STRING_ID_HPP
#include "string_hash.hpp"
#include <string>
#include "memory.hpp"
#include "hashmap.hpp"

class GlobalStringPool;

class StringID {
public:
  constexpr StringID(StringView string)
    : hash_{ string }
  {

  }

  constexpr ~StringID() = default;

  constexpr auto id() const -> StringHash {
    return hash_;
  }

  constexpr auto string() const -> StringView {
    return *str_;
  }

  constexpr StringID(const StringID&) = default;
  constexpr StringID(StringID&&)      = default;

  constexpr auto operator=(const StringID&) -> StringID& = default;
  constexpr auto operator=(StringID&&) -> StringID&      = default;

  constexpr bool operator==(const StringID& sid) const {
    return str_ == sid.str_;
  }
  
  constexpr bool operator!=(const StringID& sid) const {
    return !(*this == sid);
  }
  
  constexpr bool operator==(StringView string) const {
    if (std::is_constant_evaluated())
    {
      return (hash_ == StringHash(string)) || (*str_ == string);
    }
    else
    {
      return (*str_ == string);
    }
  }

  constexpr bool operator!=(StringView string) const {
    return !(*this == string);
  }

private:
  StringHash hash_;
  std::string* str_;
};

#endif /* CORE_STRING_ID_HPP */