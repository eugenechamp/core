#ifndef _CORE_SPAN_
#define _CORE_SPAN_

template<typename T>
class Span {
public:
  using value_type = T;
  using size_type  = size_t;

public:
  Span()  = default;
  ~Span() = default;
  
  auto data() noexcept -> value_type* { 
    return data_; 
  }

  auto data() const noexcept -> const value_type* { 
    return data_; 
  }
  
  auto size() const noexcept -> size_type {
    return size_;
  }

  auto operator[](size_type index) const noexcept -> value_type& {
    return data_[index];
  }
  
  auto operator[](size_type index) const noexcept -> const value_type& {
    return data_[index];
  }

  auto at(size_type index) noexcept -> value_type& {
    return data_[index];
  }
  
  auto at(size_type index) const noexcept -> const value_type& {
    return data_[index];
  }

  auto begin() -> value_type* {
    return data_;
  }
  
  auto end() -> value_type* {
    return data_ + size_;
  }

private:
  T*        data_{};
  size_type size_{};
};

#endif