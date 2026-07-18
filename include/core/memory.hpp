#ifndef _CORE_MEMORY_
#define _CORE_MEMORY_

#include "def.hpp"

#include <utility>
#include <memory>

#ifdef USE_STD_SMART_POINTERS

template<typename T>
struct DefaultDelete = std::default_delete<T>;

template<typename T, typename Deleter = DefaultDelete<T>>
using Unique = std::unique_ptr<T, Deleter>;
  
template<typename T>
using Shared = std::shared_ptr<T>;

template<typename T>
using Weak = std::weak_ptr<T>;

#else

template<typename T>
struct DefaultDelete {
  void operator()(T* ptr) noexcept { 
    delete ptr; 
  }
};

template<typename T>
struct DefaultDelete<T[]> {
  void operator()(T* ptr) noexcept { 
    delete[] ptr; 
  }
};



// std::unique_ptr equivalent
template<typename T, typename Deleter>
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
    : ptr_{ ptr }, deleter_{ CORE_MOVE(deleter) }
  { }

  Unique(std::nullptr_t)
    : Unique()
  { }

  Unique(Unique&& other) noexcept 
    : ptr_{ other.release() }, deleter_{ CORE_MOVE(other.deleter_) }
  { }

  auto operator=(Unique&& other) noexcept -> Unique& {
    if(this == &other)
      return *this;
    
    deleter_(ptr_);
    ptr_      = other.release();
    deleter_  = CORE_MOVE(other.deleter_);
    
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
  
  [[nodiscard]] auto get_deleter() const noexcept -> deleter_type { 
    return deleter_; 
  }
  
  [[nodiscard]] auto release() noexcept -> pointer { 
    pointer old = ptr_;
    ptr_        = nullptr;
    return old;
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

// template<typename T, typename Deleter>
// class [[nodiscard]] Unique<T[], Deleter> {
// public:
//   using pointer   = T*;
//   using reference = T&;
//   using deleter_type = Deleter;

// public:
//   Unique() = default;
//   Unique(const Unique&)         = delete;
//   auto operator=(const Unique&) = delete;

//   explicit Unique(pointer ptr)
//     : ptr_{ ptr }, deleter_{}
//   { }

//   FORCE_INLINE Unique(pointer ptr, Deleter&& deleter)
//     : ptr_{ ptr }, deleter_{ CORE_MOVE(deleter) }
//   { }

//   FORCE_INLINE Unique(std::nullptr_t)
//     : Unique()
//   { }

//   FORCE_INLINE Unique(Unique&& other) noexcept 
//     : ptr_{ other.release() }, deleter_{ CORE_MOVE(other.deleter_) }
//   { }

//   FORCE_INLINE auto operator=(Unique&& other) noexcept -> Unique& {
//     if(this == &other)
//       return *this;
    
//     deleter_(ptr_);
//     ptr_      = other.release();
//     deleter_  = CORE_MOVE(other.deleter_);
    
//     return *this;
//   }

//   FORCE_INLINE auto operator=(std::nullptr_t) noexcept -> Unique& {
//     reset();
//     return *this;
//   }
  
//   ~Unique() noexcept {
//     if (ptr_)
//     {
//       deleter_(ptr_);
//       ptr_ = nullptr;
//     }
//   }

// public:
//   FORCE_INLINE bool valid() const noexcept { 
//     return ptr_ != nullptr; 
//   }

//   FORCE_INLINE auto get() noexcept -> pointer { 
//     return ptr_; 
//   }

//   FORCE_INLINE auto get() const noexcept -> const pointer { 
//     return ptr_; 
//   }
  
//   FORCE_INLINE [[nodiscard]] auto get_deleter() const noexcept -> deleter_type { 
//     return deleter_; 
//   }
  
//   FORCE_INLINE [[nodiscard]] auto release() noexcept -> pointer { 
//     pointer old = ptr_;
//     ptr_        = nullptr;
//     return old;
//   }
  
//   FORCE_INLINE void reset() noexcept { 
//     deleter_(release());
//   }
  
//   FORCE_INLINE void swap(Unique& other) {
//     using std::swap;
//     swap(ptr_, other.ptr_);
//     swap(deleter_, other.deleter_);
//   }

//   FORCE_INLINE explicit operator bool() const { 
//     return valid(); 
//   }

//   FORCE_INLINE auto operator*() noexcept -> reference { 
//     /*TODO: add assertion */ 
//     return *ptr_; 
//   }

//   FORCE_INLINE auto operator*() const noexcept -> const T& {
//     /* TODO: add assertion */ 
//     return *ptr_; 
//   }

//   FORCE_INLINE auto operator->() const noexcept -> const pointer { 
//     return ptr_; 
//   }
  
//   FORCE_INLINE auto operator->() noexcept -> pointer {
//     return ptr_; 
//   }

// private:
//   pointer ptr_{ nullptr };
//   NO_UNIQUE_ADDRESS deleter_type deleter_;
// };

template<typename T, typename Deleter = DefaultDelete<T>>
class Unique;

template<typename T>
using Shared = std::shared_ptr<T>;

template<typename T>
using Weak = std::weak_ptr<T>;

#endif /* USE_STD_SMART_POINTERS */

#endif /* _CORE_MEMORY_ */