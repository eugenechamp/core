#ifndef _CORE_TUPLE_HPP_

#ifdef CORE_USE_STD_TUPLE
  #include <tuple>
#else 
  #include <tuplet/tuple.hpp>
#endif

namespace core::_ {
#ifdef CORE_USE_STD_TUPLE


template<typename... Ts>
using TupleBase = std::tuple<Ts...>;

#else

template<typename... Ts>
using TupleBase = tuplet::tuple<Ts...>;

#endif
}


namespace core {
#ifdef CORE_USE_STD_TUPLE
  using std::get;
  using std::apply;
  using std::tie;
  using std::forward_as_tuple;
#else
  using tuplet::get;
  using tuplet::apply;
  using tuplet::tie;
  using tuplet::forward_as_tuple;
#endif
}

template<typename... Ts>
using Tuple = tuplet::tuple<Ts...>;

#if 0 
template<typename... Ts>
struct Tuple : core::_::TupleBase<Ts...> {
  using Super = core::_::TupleBase<Ts...>;
  using Super::Super;
  
  Tuple(Ts&&... ts)
    : Super{ CORE_FWD(ts)... }
  { }

  template<typename T>
  auto get() -> T& {
    return core::get<T>(*this);
  }

  template<size_t idx>
  auto get() -> decltype(auto) {
    return core::get<idx>(*this);
  }
  
  template<typename T>
  auto get() const -> const T& {
    return core::get<T>(*this);
  }

  template<size_t idx>
  auto get() const -> decltype(auto) {
    return core::get<idx>(*this);
  }
};

template <class... Ts>
Tuple(Ts...) -> Tuple<tuplet::unwrap_ref_decay_t<Ts>...>;
#endif

#define _CORE_TUPLE_HPP_

#endif //_CORE_TUPLE_HPP_