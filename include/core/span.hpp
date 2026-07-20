#ifndef _CORE_SPAN_
#define _CORE_SPAN_

template<typename T>
class Span {
public:
  using value_type = T;
  using size_type  = size_t;

public:
  constexpr Span()  = default;
  constexpr ~Span() = default;
  
  constexpr auto data() noexcept -> value_type* { 
    return data_;
  }

  constexpr auto data() const noexcept -> const value_type* { 
    return data_; 
  }
  
  constexpr auto size() const noexcept -> size_type {
    return size_;
  }
  
  constexpr auto operator[](size_type index) noexcept -> value_type& {
    // ASSERT(index < size(), Span<{}> out of bounds access, core::meta::nameof<value_type>());
    return data_[index];
  }
  
  constexpr auto operator[](size_type index) const noexcept -> const value_type& {
    // ASSERT(index < size(), Span<{}> out of bounds access, core::meta::nameof<value_type>());
    return data_[index];
  }

  constexpr auto at(size_type index) noexcept -> value_type& {
    return data_[index];
  }
  
  constexpr auto at(size_type index) const noexcept -> const value_type& {
    return data_[index];
  }

  constexpr auto begin() -> value_type* {
    return data_;
  }
  
  constexpr auto end() -> value_type* {
    return data_ + size_;
  }

private:
  T*        data_{};
  size_type size_{};
};

#endif