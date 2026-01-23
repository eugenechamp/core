#pragma once
#include <utility>

namespace traits {
  namespace _ {
    
    template<typename T>
    struct decay_functor {
      using type = decay_functor<decltype(&T::operator())>::type;
    };

    template<typename Instance, typename Ret, typename... Args>
    struct decay_functor<Ret(Instance::*)(Args...)> {
      using type = Ret(Args...);
    };

    template<typename Instance, typename Ret, typename... Args>
    struct decay_functor<Ret(Instance::*)(Args...) const> {
      using type = Ret(Args...);
    };

    template<typename Ret, typename... Args>
    struct decay_functor<Ret(Args...)> {
      using type = Ret(Args...);
    };

    template<typename Ret, typename... Args>
    struct decay_functor<Ret(&)(Args...)> {
      using type = Ret(Args...);
    };

    template<typename Ret, typename... Args>
    struct decay_functor<Ret(*)(Args...)> {
      using type = Ret(Args...);
    };
  }
}

template<typename C, typename F>
concept Callable = std::is_function_v<F> && std::is_same_v<F, typename traits::_::decay_functor<C>::type>;

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
    max_align_t dummy; // mb drop this
    char buff[SMALL_BUFF_SZ]{};
    void* ptr;
    Ret(*fn_ptr)(Args...);
  };

public:
  FnImpl() = default;
  
  FnImpl(const FnImpl&) = delete;
  auto operator=(const FnImpl&) -> FnImpl& = delete;

  ~FnImpl() {
    if(destroy_)
    {
      destroy_(&storage_); 
    }
    
    call_ = nullptr;
  }

  template<typename F>
  static inline auto invoker(const Storage* storage, Args&&... args) -> Ret {
    using CallableType = F;
    
    if constexpr (std::is_function_v<CallableType>)
    {
      CallableType* callee = *(CallableType*)(storage->ptr);

      if constexpr (std::is_void_v<Ret>)
      {
        callee(std::forward<Args>(args)...);
      }
      else
      {
        return callee(std::forward<Args>(args)...);
      }
    }
    else if constexpr (sizeof(CallableType) <= SMALL_BUFF_SZ)
    {
      CallableType& callee = *(CallableType*)(storage->buff);

      if constexpr (std::is_void_v<Ret>)
      {
        callee(std::forward<Args>(args)...);
      }
      else
      {
        return callee(std::forward<Args>(args)...);
      }
    }
    else // get it to compile
    {
      CallableType& callee = *(CallableType*)storage->ptr;
      if constexpr (std::is_void_v<Ret>)
      {
        callee(std::forward<Args>(args)...);
      }
      else
      {
        return callee(std::forward<Args>(args)...);
      }
    }
  }

  template<typename F> requires(std::is_invocable_r_v<Ret, F, Args...>)
  FnImpl(F&& callable) {
    
    using CallableType = std::remove_cvref_t<F>;
    
    if constexpr (std::is_function_v<CallableType>)
    {
      storage_.fn_ptr = static_cast<CallableType*>(callable); // ? 
      destroy_ = nullptr;
    }
    else if constexpr (sizeof(CallableType) <= SMALL_BUFF_SZ)
    {
      ::new((void*)(storage_.buff)) CallableType(std::move(callable));

      destroy_ = +[](Storage* storage) {
        CallableType& callee = *(CallableType*)&storage->buff;
        callee.~CallableType();
      };
    }
    else // get it to compile
    {
      ///NOTE: any room for a custom allocator???
      storage_.ptr = (void*) new CallableType(std::move(callable));

      destroy_ = +[](Storage* storage) {
        CallableType& callee = *(CallableType*)storage->ptr;
        callee.~CallableType();
      };
    }

    call_ = &invoker<CallableType>;
  }

  // template<Callable<Ret(Args...)> F>
  // auto operator=(F&&) -> FnImpl& {
  //   return *this;
  // }

  FnImpl(FnImpl&& other)
   : call_{ other.call_ },
    destroy_{ other.destroy_ }
  {
    std::memcpy(&storage_, &other.storage_, sizeof(storage_));

    other.call_    = nullptr;
    other.destroy_ = nullptr;
  }

  auto operator=(FnImpl&& other) noexcept -> FnImpl& {
    if (this == &other)
      return *this;
    
    if(destroy_)
      destroy_(&storage_);

    call_ = other.call_;
    destroy_ = other.destroy_;
    std::memcpy(storage_.buff, other.storage_.buff, sizeof(storage_.buff));

    other.call_    = nullptr;
    other.destroy_ = nullptr;

    return *this;
  }

  [[nodiscard]] auto operator()(Args... args) const -> Ret {
    if constexpr (std::is_void_v<Ret>)
    {
      call_(&storage_, std::forward<Args>(args)...);
    }
    else
    {
      return call_(&storage_, std::forward<Args>(args)...);
    }
  }
  
  template<Callable<Ret(Args...)> F>
  auto target() -> F* {
    
    if (call_ == invoker<F>)
    {
      //return (F*)((void*)storage_.buff);

      if constexpr (std::is_function_v<F>)
      {
        return (F*)(storage_.ptr);
      }
      else if constexpr (sizeof(F) <= SMALL_BUFF_SZ && !std::is_function_v<F>)
      {
        return (F*)(&storage_.buff);
      }
      else
      {
        return (F*)(storage_.ptr);
      }
    }
    else
    {
      return nullptr;
    }
  }

  bool holds() const {
    return false;
  }

  bool empty() const {
    return call_ == nullptr;
  }

  explicit operator bool() const {
    return !empty();
  }

  void swap(FnImpl& fn) {
    std::swap(call_, fn.call_);
    std::swap(destroy_, fn.destroy_);
    std::swap(storage_, fn.storage_);
  }

protected:
  CallFn* call_       = nullptr;
  DestroyFn* destroy_ = nullptr;
  Storage storage_{};
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
class Fn : GetFnImpl<64ULL, F>::Type {
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
};

template<typename F>
class Fn40 : GetFnImpl<40ULL, F>::Type {
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
};

template<typename F>
class Fn48 : GetFnImpl<48ULL, F>::Type {
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
};


template<typename F>
Fn(F&&) -> Fn<typename traits::_::decay_functor<F>::type>;

template<typename F>
Fn32(F&&) -> Fn32<typename traits::_::decay_functor<F>::type>;

template<typename F>
Fn40(F&&) -> Fn40<typename traits::_::decay_functor<F>::type>;

template<typename F>
Fn48(F&&) -> Fn48<typename traits::_::decay_functor<F>::type>;


