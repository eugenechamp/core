#ifndef _CORE_MEMORY_
#define _CORE_MEMORY_

#include "pair.hpp"

#include <utility>

template<typename T>
struct DefaultDelete {
  void operator()(T* ptr) noexcept { delete ptr; }
};

// std::unique_ptr equivalent
template<typename T, typename Deleter = DefaultDelete<T>>
class Unique {
public:
  using pointer   = T*;
  using reference = T&;
  using deleter_type = Deleter;

public:
  Unique() = default;
  
  Unique(pointer ptr)
    : pair_{ ptr, Deleter{} }
  { }

  Unique(pointer ptr, Deleter&& deleter)
    : pair_{ ptr, std::move(deleter) }
  { }

  Unique(Unique&& other) noexcept 
    : pair_{ other.release(), std::move(other.get_deleter()) }
  { }

  auto operator=(Unique&& other) noexcept -> Unique& {
    if(this == &other) // prolly mark unlikely
      return *this;
    
    // do move
  }

  Unique(const Unique&)         = delete;
  auto operator=(const Unique&) = delete;
  
  ~Unique() {
    if (valid())
      get_deleter(get());
  }
  
  template<typename... Args>
  static auto make(Args&&... args) -> Unique {
    return Unique{ new T{std::forward<Args>(args)...} };
  }

public:
  bool valid() noexcept { return pair_.first != nullptr; }
  auto get() noexcept -> pointer { return pair_.first; }
  auto get() const noexcept -> const pointer { return pair_.first; }
  
  auto get_deleter() noexcept -> deleter_type& { return pair_.second; }
  auto get_deleter() const noexcept -> const deleter_type& { return pair_.second; }
  
  auto release() noexcept -> pointer { return std::exchange(pair_.first, nullptr); }
  
  void reset() noexcept { 
    pointer old = std::exchange(pair_.first, nullptr);
    get_deleter()(old);
  }
  
  explicit operator bool() { return valid(); }
  auto operator*()  noexcept -> reference { /*TODO: add assertion */ return *pair_.first; }
  auto operator->() noexcept -> pointer { return pair_.first; }

private:
  Pair<T*, Deleter> pair_;
};

#endif /* _CORE_MEMORY_ */