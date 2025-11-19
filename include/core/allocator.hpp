#ifndef _CORE_ALLOCATOR_
#define _CORE_ALLOCATOR_

// template<typename T>
// class Alloc {
//   using value_type = T;
  
//   static T* allocate(size_type size) {
//     if(std::is_trivial_v<T>) 
//     {
//       return (T*)malloc(size);
//     }
//     else 
//     {
//       return new T[size];
//     }
//   } 
  
//   static T* deallocate(value_type* ptr, size_type size) {
//     if(std::is_trivial_v<T>)
//     {
//       free(ptr);
//     }
//     else 
//     {
//       return delete[] ptr;
//     }
//   }
  
//   static void construct(T* place, T&& value) {
//     ::new(place) value_type(std::forward<T>(value));
//   }
// };

#endif /* _CORE_ALLOCATOR_ */ 