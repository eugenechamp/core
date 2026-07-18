#ifndef _CORE_CONCEPTS_HPP_
#define _CORE_CONCEPTS_HPP_
#include <type_traits>

namespace core::traits {
  namespace _ {
    
    template<typename>
    struct decay_functor;

    template<typename Instance, typename Ret, typename... Args>
    struct decay_functor<Ret(Instance::*)(Args...)> {
      using type = Ret(Args...);
    };

    template<typename Instance, typename Ret, typename... Args>
    struct decay_functor<Ret(Instance::*)(Args...) const> {
      using type = Ret(Args...);
    };

    template<typename T>
    struct decay_functor {
      using type = typename decay_functor<decltype(&std::remove_cvref_t<std::decay_t<T>>::operator())>::type;
    };

    template<typename Ret, typename... Args>
    struct decay_functor<Ret(*)(Args...)> {
      using type = Ret(Args...);
    };

    template<typename, typename>
    struct is_invocable_impl {
      static constexpr bool value = false;
    };

    template<typename Fx, typename Ret, typename... Args>
    struct is_invocable_impl<Fx, Ret(Args...)> {
      static constexpr bool value = std::is_invocable_r_v<Ret, Fx, Args...>;
    };
  }
}

template<typename Callable, typename F>
concept Invocable = core::traits::_::is_invocable_impl<Callable, typename core::traits::_::decay_functor<std::remove_cvref_t<std::decay_t<Callable>>>::type>::value;

template<typename C, typename F>
concept Callable = std::is_function_v<F> && std::is_same_v<F, typename core::traits::_::decay_functor<std::remove_cvref_t<std::decay_t<C>>>::type>;

#endif