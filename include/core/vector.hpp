#ifndef _CORE_VECTOR_
#define _CORE_VECTOR_
#include "def.hpp"
#include "optional.hpp"
#include "allocator.hpp"
#include <cstring>
#include <memory>
#include <cassert>
 
template<typename T, typename A = std::allocator<T>>
class [[nodiscard]] Vec {
public:
  using value_type     = T;
  using reference      = T&;
  using pointer        = T*;
  using allocator_type = A;
  using size_type      = u32;
  
  using iterator       = T*;
  using const_iterator = const T*;

public:
  Vec() = default;
  
  Vec(const Vec& other) 
    : m_{ .alloc = other.m_.alloc }
  {
    _do_alloc(other.size());
    
    /// BUGFIX: this is wrong we need to copy each element, not memcopying it, stupido
    _do_memcopy(other.data(), other.size());
    m_.size = other.size();
  }
  
  Vec(Vec&& other) noexcept
    : m_{ other.m_ }
  { 
    other.m_.data      = {};
    other.m_.size      = {};
    other.m_.capacity  = {};
  }
  
  Vec(size_type _capacity, allocator_type&& _alloc)
    : m_{.alloc = CORE_MOVE(_alloc)}
  {
    
  }

  Vec(std::initializer_list<T> ilist, const allocator_type& alloc = {}) 
    : m_{.alloc = alloc}
  {
    _insert_end(ilist.begin(), ilist.end());
  }

  ~Vec() noexcept {
    _tidy_deallocate();
    m_.data     = nullptr;
    m_.size     = {};
    m_.capacity = {};
  }

  auto operator=(Vec&& other) noexcept -> Vec& {
    if(this == &other)
    {
      return *this;
    }
    
    this->_tidy_deallocate();

    m_ = other.m_;

    other.m_.data     = {};
    other.m_.size     = {};
    other.m_.capacity = {};
    
    return *this;
  }
  
  auto operator=(const Vec& other) -> Vec& {
    if(this == &other)
    {
      return *this;
    }

    if (other.size() <= size())
    {
      _do_assign_range(other.begin(), other.end());
      _clear_range(begin() + other.size(), end());
    }
    else if (other.size() > capacity()) // NOTE: >= maybe?
    {
      _tidy_deallocate();
      _do_alloc(other.size());
      _do_assign_range(other.data(), other.data() + other.size());
    }
    else /* if (this->size() < other.size() && other.size() <= this->capacity() ) */
    {
      ptrdiff_t diff = other.size() - size();
      const_iterator last = other.begin() + this->size();

      _do_assign_range(other.begin(), last);
      
      //TODO: replace with insert() later
      for (size_type i = 0; last != other.end(); ++last, ++i)
      {
        ::new((void*)(m_.data + i)) value_type(*last);
      }
    }
    
    m_.size = other.size();

    return *this;
  }

  auto operator=(std::initializer_list<T> ilist) -> Vec& {
    *this = Vec(ilist);
    return *this;
  }

  static auto with_capacity(size_type n) -> Vec {
    Vec vec;
    vec.reserve(n);
    return vec;
  }
  
  static auto with_size(size_type count) -> Vec {
    Vec vec;
    vec.resize(count);
    return vec;
  }

  static consteval auto max_size() noexcept -> size_type
  {
    return max_size_bytes() / sizeof(value_type);
  }

  static consteval auto max_size_bytes() noexcept -> size_type
  {
    return size_type(-1);
  }

public:
  auto at(size_type index) noexcept -> value_type* {
    if (index < size())
      return (m_.data + index);
    
    return nullptr;
  }

  auto at(size_type index) const noexcept -> const value_type* {
    if (index < size())
      return (m_.data + index);

    return nullptr;
  }

  auto operator[](size_type index) noexcept -> value_type& {
    return m_.data[index];
  }

  auto operator[](size_type index) const noexcept -> const value_type& {
    return m_.data[index];
  }

  void push_back(const value_type& element) {
    if (m_.size == m_.capacity)
    {
      _do_realloc(_new_capacity());
    }
    
    ::new ((void*)(m_.data + m_.size)) value_type(element);

    m_.size++;
  }

  void push_back(value_type&& element) {
    if (m_.size == m_.capacity)
    {
      _do_realloc(_new_capacity());
    }

    ::new((void*)(m_.data + m_.size)) value_type(CORE_MOVE(element));
    
    m_.size++;
  }

  auto emplace_back(auto&&... args) -> value_type& {
    if (m_.size == m_.capacity)
    {
      _do_realloc(_new_capacity());
    }

    value_type* val = ::new((void*)(m_.data + m_.size)) value_type(CORE_FWD(args)...);

    m_.size++;

    return *val;
  }

  template<typename Iterator>
  void assign(Iterator first, Iterator last) {
  
  }

  // TODO: implement
  auto insert(const_iterator pos, const value_type& value) -> iterator {
    
    assert(pos >= begin() && pos <= end());

    // _insert_value(&(*pos), value);
    iterator it;
    if(pos == end())
    {
      push_back(value);
      return end() - 1;
      
      ::new((void*)(data() + size())) value_type(CORE_MOVE(back()));
      it = std::move_backward(pos, m_.size - 1, m_.size);
      ::new(m_.size) value_type(value);
    }
    else if(pos == begin())
    {
      
      return begin();
    }
    else 
    {

    }
    
    m_.size++;

    return it;
  }

  auto insert(const_iterator pos, value_type&& value) -> iterator {
    return {};
  }
  
  auto insert(const_iterator pos, size_type count, const value_type& value) -> iterator {
    return {};
  }

  auto insert(const_iterator pos, std::initializer_list<T> ilist) -> iterator {
    return {};

  }
  
  template<typename Iter>
  auto insert(const_iterator pos, Iter first, Iter last) -> iterator {
    return {};
  }

  void pop_swap(iterator iterator) {
    value_type& last = *back();
    value_type& elem = *iterator;
    elem.value_type();

    if constexpr(std::is_trivial_v<value_type>)
    {
      elem = last;
    }
    else
    {
      std::memcpy(&elem, &last, sizeof(value_type));
    }
    m_.size--;
  }

  auto find(const value_type& value) -> iterator {

  }

  auto find(const value_type& value) const -> const iterator {
    
  }
  
  bool in_range(iterator iterator) {
    return iterator >= begin() && iterator < end();
  }

  auto erase(const iterator iterator) {

  }

  auto remove(const value_type& value) -> size_type {
    return {};
  }

  template<typename Predicate>
  auto remove_if(Predicate predicate) -> size_type {
    return {};
  }

  template<typename Container>
  void append(const Container& container) {
    _insert_end(container.begin(), container.end());
  }
  
  template<typename Container>
  void append(Container&& container) {
    _insert_end(std::move_iterator(container.begin()), std::move_iterator(container.end()));
  }

  template<typename Container>
  void prepend(const Container& container) {
    insert(begin(), container.begin(), container.end());
  }
  
  template<typename Container>
  void prepend(Container&& container) {
    insert(begin(), std::move_iterator(container.begin()), std::move_iterator(container.end()));
  }

  // TODO: implement 
  auto emplace(const_iterator it, auto&&... args) {
    
  }

  auto begin() -> iterator {
    return m_.data;
  }

  auto end() -> iterator {
    return m_.data + m_.size;
  }

  auto begin() const -> const_iterator {
    return m_.data;
  }

  auto end() const -> const_iterator {
    return m_.data + m_.size;
  }

  auto data() noexcept -> pointer { 
    return m_.data;
  }

  auto data() const noexcept -> const pointer { 
    return m_.data;
  }
  
  auto size() const noexcept -> size_type { 
    return m_.size;
  }
  
  auto size_bytes() const noexcept -> size_type { 
    return size() * sizeof(value_type); 
  }
  
  auto capacity() const noexcept -> size_type { 
    return m_.capacity;
  }
  
  auto capacity_bytes() const noexcept -> size_type { 
    return capacity() * sizeof(value_type);
  }

  bool empty() const noexcept {
    return m_.size == 0u;
  }
  
  auto back() -> value_type& {
    assert(!empty());

    return *(m_.data + (m_.size - 1));
  }

  auto back() const -> const value_type& {
    assert(!empty());
    return *(m_.data + (m_.size - 1));
  }

  auto front() -> value_type& {
    assert(!empty());
    return *m_.data;
  }

  auto front() const -> const value_type& {
    assert(!empty());
    return *m_.data;
  }

  auto get_allocator() -> allocator_type {
    return m_.alloc;
  }

  void reserve(size_type amount) {
    if(amount <= capacity())
      return;
    _do_realloc(amount);
  }
  
  void resize(size_type newsize) {
    if(newsize > size()) // grow
    {
      if(newsize > capacity())
      {
        size_type n = std::max(newsize, _new_capacity());
        _do_realloc(n);
      }
      
      for(value_type* it = m_.data + m_.size, *end = m_.data + newsize; it < end; ++it)
      {
        ::new((void*)(it)) value_type();
      }
    }
    else // shrink
    {
      _clear_range(m_.data + newsize, m_.data + m_.size);
    }

    m_.size = newsize;
  }

  void clear() noexcept {
    _clear_range(m_.data, m_.data + m_.size);
    m_.size = {};
  }

  void shrink_to_fit() {
    // TODO 
    if (size() == capacity())
      return;
  }

  void swap(Vec& other) {
    _Memdata temp = this->m_;
    this->m_      = other.m_;
    other.m_      = temp;
  }
public:
  void _set_size(size_type newsize) noexcept {
    m_.size = newsize;
  }
  
  auto _emplace_at(size_type pos, auto&&... args) -> value_type& {
    return *::new((void*)(m_.data + pos)) value_type(CORE_FWD(args)...);
  }

private:
  struct _Memdata {
    T* data            = nullptr;
    size_type size     = {};
    size_type capacity = {};
    NO_UNIQUE_ADDRESS allocator_type alloc;
  } m_{};

private:
  void _do_alloc(size_type n) {
    value_type* p = m_.alloc.allocate(n);
    
    m_.data     = p;
    m_.capacity = n;
  }

  void _do_realloc(size_type ncap) {
    value_type* p = m_.alloc.allocate(ncap);
    
    _do_memcopy(p, m_.size);
    
    m_.alloc.deallocate(m_.data, m_.capacity);

    m_.data     = p;
    m_.capacity = ncap;
  }

  void _do_memcopy(value_type* dst, size_type sz) noexcept {
    std::memcpy(dst, m_.data, sz * sizeof(value_type));
  }

  void _destructive_move_overlapped(value_type* dst, size_type sz) noexcept {
    std::memmove(dst, m_.data, sz * sizeof(value_type));
  }
  
  template<std::random_access_iterator Iterator>
  void _do_assign_from_iterator(Iterator first, Iterator last) noexcept {
    _do_assign_range(first, last);
  }

  template<std::input_iterator Iterator>
  void _do_assign_from_iterator(Iterator first, Iterator end) noexcept {
    // bounds checking was done earlier
    for (size_type i = 0; first != end; ++i, ++first)
    {
      m_.data[i] = *first;
    }
  }

  void _do_assign_range(const value_type* first, const value_type* last) noexcept {
    if constexpr (!std::is_trivially_destructible_v<value_type>)
    {
      // bounds checking was done earlier
      for (size_type i = 0; first != last; ++i, ++first)
      {
        m_.data[i] = *first;
      }
    }
    else
    {
      size_type n = last - first;
      std::memcpy(m_.data, first, n * sizeof(value_type));
    }
  }

  auto _new_capacity() const noexcept -> size_type {
    return size_type(!m_.capacity) + m_.capacity * size_type(2);
  }

  // dead code
  void _do_move(_Memdata& m) noexcept {
    m_ = {
      .data = m.data,
      .size = m.size,
      .capacity = m.capacity
      .alloc = m.alloc
    };

    m.data     = nullptr;
    m.size     = {};
    m.capacity = {};
  }
  
  void _clear_range(value_type* start, value_type* end) {
    if constexpr (!std::is_trivially_destructible_v<value_type>)
    {
      for (; start < end; start++)
        start->~value_type();
    }
  }

  void _tidy_deallocate() {
    _clear_range(m_.data, m_.data + m_.size);
    m_.alloc.deallocate(m_.data, capacity());
  }
  
  // TODO: remove that
  // auto _insert_range_end(const value_type* first, const value_type* last) -> iterator {
    
  //   size_type n = last - first;
    
  //   if(size() + n >= capacity())
  //   {
  //     reserve(size() + n);
  //   }

  //   for(size_type i = 0; i < n; i++, ++first)
  //   {
  //     ::new(m_.size + i) value_type(*first);
  //   }

  //   m_.size = m_.size + n;

  //   return end() - 1;
  // }
  
  template<typename Iterator>
  auto _insert_end(Iterator first, Iterator last) -> iterator {
    
    size_type n = size_type(std::distance(first, last));
    
    // TODO: remove this if statement we already check that in reserve()
    if(size() + n >= capacity())
    {
      reserve(size() + n);
    }

    for(size_type i = size(), finish = size() + n; i < finish; ++first, i++)
    {
      ::new((void*)(m_.data + i)) value_type(*first);
    }

    m_.size += n;
    
    return end() - 1;
  }
  
  template<typename InputIterator>
  auto _do_insert(InputIterator first, InputIterator last) -> iterator {
    
  }
};

#endif /* _CORE_VECTOR_ */