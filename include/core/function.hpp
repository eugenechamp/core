#pragma once
#include "def.hpp"
#include "concepts.hpp"
#include <utility>
#include <cstring>

template<size_t SIZE, typename Ret, typename... Args>
struct FnImpl {
  using Type = Ret(Args...);
  using ResultType = Ret;

private:
  union Storage;

  using CallFn    = Ret(const Storage*, Args&&...);
  using DestroyFn = void(Storage*);
  
  static constexpr size_t SMALL_BUFF_SZ = SIZE - sizeof(CallFn*) - sizeof(DestroyFn*);
  
  union Storage {
    std::max_align_t dummy; // mb drop this
    char buff[SMALL_BUFF_SZ];
    void* ptr;
    Ret(*fn_ptr)(Args...);
  };

public:
  FnImpl() noexcept = default;
  
  FnImpl(const FnImpl&)                    = delete;
  auto operator=(const FnImpl&) -> FnImpl& = delete;

  ~FnImpl() noexcept {
    reset();
  }

  template<typename F>
  static inline auto invoker(const Storage* storage, Args&&... args) -> Ret {
    using CallableType = F;
    
    if constexpr (std::is_function_v<CallableType>)
    {
      CallableType* callee = *(CallableType*)(storage->ptr);

      if constexpr (std::is_void_v<Ret>)
      {
        callee(CORE_FWD(args)...);
      }
      else
      {
        return callee(CORE_FWD(args)...);
      }
    }
    else if constexpr (sizeof(CallableType) <= SMALL_BUFF_SZ)
    {
      CallableType& callee = *(CallableType*)(storage->buff);

      if constexpr (std::is_void_v<Ret>)
      {
        callee(CORE_FWD(args)...);
      }
      else
      {
        return callee(CORE_FWD(args)...);
      }
    }
    else
    {
      CallableType& callee = *(CallableType*)storage->ptr;
      if constexpr (std::is_void_v<Ret>)
      {
        callee(CORE_FWD(args)...);
      }
      else
      {
        return callee(CORE_FWD(args)...);
      }
    }
  }

  template<Callable<Ret(Args...)> F>
  FnImpl(F&& callable) {
    
    using CallableType = std::remove_cvref_t<std::decay_t<F>>;
    
    if constexpr (std::is_function_v<std::remove_pointer_t<CallableType>>) // might be unnecessary
    {
      storage_.fn_ptr = static_cast<CallableType>(callable); // ? 
      destroy_ = nullptr;
    }
    else if constexpr (sizeof(CallableType) <= SMALL_BUFF_SZ)
    {
      ::new((void*)(storage_.buff)) CallableType(CORE_MOVE(callable));

      destroy_ = +[](Storage* storage) {
        CallableType& callee = *(CallableType*)&storage->buff;
        callee.~CallableType();
      };
    }
    else
    {
      ///NOTE: any room for a custom allocator???
      storage_.ptr = (void*) new CallableType(CORE_MOVE(callable));

      destroy_ = +[](Storage* storage) {
        CallableType& callee = *(CallableType*)storage->ptr;
        callee.~CallableType();
      };
    }

    call_ = &invoker<std::remove_pointer_t<CallableType>>;
  }

  FnImpl(FnImpl&& other) noexcept
   : call_{ other.call_ },
    destroy_{ other.destroy_ }
  {
    std::memcpy(storage_.buff, other.storage_.buff, sizeof(storage_.buff));
    other.destroy_ = nullptr;
    // other.call_ = nullptr;
  }

  auto operator=(FnImpl&& other) noexcept -> FnImpl& {
    if (this == &other)
      return *this;
    
    if(destroy_)
      destroy_(&storage_);

    call_    = other.call_;
    destroy_ = other.destroy_;
      
    std::memcpy(storage_.buff, other.storage_.buff, sizeof(storage_.buff));
    other.destroy_ = nullptr;

    return *this;
  }

  FORCE_INLINE [[nodiscard]] auto operator()(Args... args) const -> Ret {
    //assert(!is_empty());
    if constexpr (std::is_void_v<Ret>)
    {
      (void)call_(&storage_, CORE_FWD(args)...);
    }
    else
    {
      return call_(&storage_, CORE_FWD(args)...);
    }
  }
   
  template<Callable<Ret(Args...)> Fx>
  auto target() const -> const Fx* {
    
    using CallableType = std::remove_reference_t<std::decay_t<Fx>>;
    if (call_ == &invoker<std::remove_pointer_t<CallableType>>)
    {
      if constexpr (std::is_function_v<std::remove_pointer_t<CallableType>>)
      {
        return (CallableType)(storage_.ptr);
      }
      else if constexpr (sizeof(CallableType) <= SMALL_BUFF_SZ)
      {
        return (CallableType*)(&storage_.buff);
      }
      else
      {
        return (CallableType*)(storage_.ptr);
      }
    }
    else
    {
      return nullptr;
    }
  }

  template<Callable<Ret(Args...)> Fx>
  auto target() -> Fx* {
    using CallableType = std::remove_cvref_t<Fx>;
    
    if (call_ == invoker<CallableType>)
    {
      if constexpr (std::is_function_v<std::remove_pointer_t<CallableType>>)
      {
        return (CallableType*)(storage_.ptr);
      }
      else if constexpr (sizeof(CallableType) <= SMALL_BUFF_SZ)
      {
        return (CallableType*)(&storage_.buff);
      }
      else
      {
        return (CallableType*)(storage_.ptr);
      }
    }
    else
    {
      return nullptr;
    }
  }
  
  template<Callable<Ret(Args...)> Fx>
  bool holds(const Fx& fn) const {
    
    const Fx* ptr = target<Fx>();
    
    if(ptr && *ptr == fn)
    {
      return true;
    }
    
    return false;
  }

  bool empty() const {
    return destroy_ == nullptr;
  }

  explicit operator bool() const {
    return !empty();
  }

  void reset() {
    if (destroy_)
    {
      destroy_(&storage_);
      destroy_ = nullptr;
      // call_    = nullptr;
    }
  }

  void swap(FnImpl& fn) {
    std::swap(call_, fn.call_);
    std::swap(destroy_, fn.destroy_);
    std::swap(storage_.buff, fn.storage_.buff);
  }

protected:
  CallFn* call_       = nullptr;
  DestroyFn* destroy_ = nullptr;
  Storage storage_;
};

template<size_t, typename>
struct GetFnImpl {
  static_assert(false, "\'Fn\' only accepts function types as template arguments");
};

template<size_t SIZE, typename Ret, typename... Args>
struct GetFnImpl<SIZE, Ret(Args...)> {
  using Type = FnImpl<SIZE, Ret, Args...>;
};

template<typename F>
class Fn : public GetFnImpl<64ULL, F>::Type {
private:
  using Base = GetFnImpl<64ULL, F>::Type;
  using Type = Base::Type;

public:
  using ResultType = Base::ResultType;

public:
  using Base::Base;
  
  using Base::operator();
  using Base::operator bool;
  using Base::empty;
  using Base::holds;
  using Base::target;
  using Base::reset;
  using Base::swap;
};

template<typename T>
using Fn64 = Fn<T>;

template<typename F>
class Fn24 : public GetFnImpl<24ULL, F>::Type {
private:
  using Base = GetFnImpl<24ULL, F>::Type;
  using Type = Base::Type;

public:
  using ResultType = Base::ResultType;

public:
  using Base::Base;
  
  using Base::operator();
  using Base::operator bool;
  using Base::empty;
  using Base::holds;
  using Base::target;
  using Base::reset;
  using Base::swap;
};

template<typename F>
class Fn32 : public GetFnImpl<32ULL, F>::Type {
private:
  using Base = GetFnImpl<32ULL, F>::Type;
  using Type = Base::Type;

public:
  using ResultType = Base::ResultType;

public:
  using Base::Base;
  
  using Base::operator();
  using Base::operator bool;
  using Base::empty;
  using Base::holds;
  using Base::target;
  using Base::reset;
  using Base::swap;
};

template<typename F>
class Fn40 : public GetFnImpl<40ULL, F>::Type {
private:
  using Base = GetFnImpl<40ULL, F>::Type;
  using Type = Base::Type;

public:
  using ResultType = Base::ResultType;

public:
  using Base::Base;
  
  using Base::operator();
  using Base::operator bool;
  using Base::empty;
  using Base::holds;
  using Base::target;
  using Base::reset;
  using Base::swap;
};

template<typename F>
class Fn48 : public GetFnImpl<48ULL, F>::Type {
private:
  using Base = GetFnImpl<48ULL, F>::Type;
  using Type = Base::Type;

public:
  using ResultType = Base::ResultType;

public:
  using Base::Base;
  
  using Base::operator();
  using Base::operator bool;
  using Base::empty;
  using Base::holds;
  using Base::target;
  using Base::reset;
  using Base::swap;
};

template<typename F>
Fn(F&&) -> Fn<typename core::traits::_::decay_functor<F>::type>;

template<typename F>
Fn32(F&&) -> Fn32<typename core::traits::_::decay_functor<F>::type>;

template<typename F>
Fn40(F&&) -> Fn40<typename core::traits::_::decay_functor<F>::type>;

template<typename F>
Fn48(F&&) -> Fn48<typename core::traits::_::decay_functor<F>::type>;

// template<typename F>
// struct MemFn;

// template<typename Ret, typename Instance, typename... Args>
// struct MemFn<Ret(Instance::*)(Args...)> {
//   using callable_type = Ret(Instance::*)(Args...);
  
//   MemFn(callable_type fn, Instance* instance) {

//   }

//   auto operator()(Args... args) -> Ret {
//     if constexpr(std::is_void_v<Ret>)
//     {
//       instance->*fn(CORE_FWD(args)...);
//     }
//     else 
//     {
//       return instance->*fn(CORE_FWD(args)...);
//     }
//   }

// };

// template<typename Ret, typename Instance, typename... Args>
// struct MemFn<Ret(Instance::*)(Args...) const>;

// template<typename Ret, typename Instance, typename... Args>
// struct MemFn<Ret(Instance::*)(Args...) const noexcept>;