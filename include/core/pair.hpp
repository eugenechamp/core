#ifndef _CORE_PAIR_
#define _CORE_PAIR_

#include "def.hpp"

template<typename First, typename Second = First>
struct Pair {
  using first_type  = First;
  using second_type = Second;
  
  NO_UNIQUE_ADDRESS First   first;
  NO_UNIQUE_ADDRESS Second second;
};

template<typename F, typename S = F>
using TiePair = Pair<F&, S&>;

template<typename F, typename S>
constexpr bool operator==(const Pair<F, S>& lhs, const Pair<F, S>& rhs) noexcept {
  return lhs.first == rhs.first && lhs.second == rhs.second; 
}

template<typename F, typename S>
constexpr bool operator!=(const Pair<F, S>& lhs, const Pair<F, S>& rhs) noexcept {
  return !(lhs == rhs);
}

namespace core {
} // core

#endif // _CORE_PAIR_
