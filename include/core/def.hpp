#ifndef _CORE_DEF_
#define _CORE_DEF_

#include <cstddef>
#include <cstdint>
#include <string_view>

using i8 = int8_t;
using u8 = int8_t;
 
using i16 = int16_t;
using u16 = uint16_t;

using i32 = int32_t; 
using u32 = uint32_t;

using i64 = int64_t;
using u64 = uint64_t;

using f32 = float; 
using f64 = double;

#ifndef CORE_MOVE
  #define CORE_MOVE(...) static_cast<std::remove_reference_t<decltype(__VA_ARGS__)>&&>(__VA_ARGS__)
#else
  #error "CORE_MOVE" macro redefinition
#endif

#ifndef CORE_FWD
  #define CORE_FWD(...) static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__)
#else
  #error "CORE_FWD" macro redefinition
#endif

#ifndef NO_UNIQUE_ADDRESS
  #if defined(__clang__) && defined(_MSC_VER)
    #define NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
  #elif defined(__GNUC__) || defined(__clang__)
    #define NO_UNIQUE_ADDRESS [[no_unique_address]]
  #elif defined(_MSC_VER)
    #define NO_UNIQUE_ADDRESS [[msvc::no_unique_address]] [[no_unique_address]]
  #else
    #define NO_UNIQUE_ADDRESS
  #endif
#else
  #error "NO_UNIQUE_ADDRESS redefinition" 
#endif

#ifndef INL
  #if defined(__GNUC__) || defined(__clang__)
    #define INL __attribute__((always_inline)) inline
  #elif defined(_MSC_VER)
    #define INL [[msvc::forceinline]]
  #else
    #define INL inline
  #endif
#endif

#ifndef FORCE_INLINE
  #if defined(_MSC_VER)
    #define FORCE_INLINE __forceinline
  #elif defined(__GNUC__) || defined(__clang__)
    #define FORCE_INLINE [[gnu::forceinline]] inline
  #endif
#else
  #error "FORCE_INLINE redefinition" 
#endif

using StringView = std::string_view;

#endif /* _CORE_DEF_ */ 

