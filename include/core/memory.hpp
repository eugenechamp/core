#ifndef _CORE_MEMORY_
#define _CORE_MEMORY_

#include "def.hpp"

#include <utility>
#include <memory>

template<typename T>
struct DefaultDelete {
  void operator()(T* ptr) noexcept { 
    delete ptr; 
  }
};

// std::unique_ptr equivalent
template<typename T, typename Deleter = DefaultDelete<T>>
class [[nodiscard]] Unique {
public:
  using pointer   = T*;
  using reference = T&;
  using deleter_type = Deleter;

public:
  Unique() = default;
  
  explicit Unique(pointer ptr)
    : ptr_{ ptr }, deleter_{}
  { }

  Unique(pointer ptr, Deleter&& deleter)
    : ptr_{ ptr }, deleter_{ std::move(deleter) }
  { }

  Unique(std::nullptr_t)
    : Unique()
  { }

  Unique(Unique&& other) noexcept 
    : ptr_{ other.release() }, deleter_{ std::move(other.deleter_) }
  { }

  auto operator=(Unique&& other) noexcept -> Unique& {
    if(this == &other) // prolly mark unlikely
      return *this;
    
    deleter_(ptr_);
    ptr_      = other.release();
    deleter_  = std::move(other.deleter_);
    
    return *this;
  }

  auto operator=(std::nullptr_t) noexcept -> Unique& {
    reset();
    return *this;
  }

  Unique(const Unique&)         = delete;
  auto operator=(const Unique&) = delete;
  
  ~Unique() noexcept {
    if (ptr_)
    {
      deleter_(ptr_);
      ptr_ = nullptr;
    }
  }
  
  template<typename... Args>
  static auto make(Args&&... args) -> Unique {
    return Unique{ new T(std::forward<Args>(args)...) };
  }

public:
  bool valid() const noexcept { 
    return ptr_ != nullptr; 
  }

  auto get() noexcept -> pointer { 
    return ptr_; 
  }

  auto get() const noexcept -> const pointer { 
    return ptr_; 
  }
  
  auto get_deleter() const noexcept -> deleter_type { 
    return deleter_; 
  }
  
  [[nodiscard]] auto release() noexcept -> pointer { 
    return std::exchange(ptr_, nullptr); 
  }
  
  void reset() noexcept { 
    deleter_(release());
  }
  
  void swap(Unique& other) {
    using std::swap;
    swap(ptr_, other.ptr_);
    swap(deleter_, other.deleter_);
  }

  explicit operator bool() const { 
    return valid(); 
  }
  auto operator*() noexcept -> reference { 
    /*TODO: add assertion */ 
    return *ptr_; 
  }
  auto operator*() const noexcept -> const T& {
    /*TODO: add assertion */ 
    return *ptr_; 
  }
  auto operator->() const noexcept -> const pointer { 
    return ptr_; 
  }
  auto operator->() noexcept -> pointer {
    return ptr_; 
  }

private:
  pointer ptr_{ nullptr };
  NO_UNIQUE_ADDRESS deleter_type deleter_;
};

template<typename T>
using Shared = std::shared_ptr<T>;

template<typename T>
using Weak = std::weak_ptr<T>;

#endif /* _CORE_MEMORY_ */