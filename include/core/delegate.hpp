#pragma once

struct TypeInfo {

};

struct TypeId {
  template<typename T>
  static constexpr auto of() -> TypeInfo {
    return TypeInfo {
      
    };
  }

  bool is_unique() {

  }
};

enum struct TypeIndex {};


template<typename>
class Delegate;

template<typename Ret, typename... Args>
class Delegate<Ret(Args...)> {
  auto operator()(Args...) -> Ret {

  }
};

template<typename, typename>
class MultiDelegate;

template<typename Ret, typename... Args, typename Alloc>
class MultiDelegate<Ret(Args...), Alloc = std::allocator<Delegate<Ret(Args...)>>> {
  using DelegateType = Delegate<Ret(Args...)>;

  template<auto callable>
  bool subscribe() {

  }

  template<auto callable, typename Instance>
  bool subscribe(Instance* payload) {

  }

  template<typename Callable>
  bool subscribe(Callable&&) {

  }

  auto operator()(Args...) -> Ret {

  }
  
private:
  Vec<DelegateType, Alloc> calls_;
};
