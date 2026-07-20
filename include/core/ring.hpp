#pragma once
#include"vector.hpp"

template<typename T, typename Container = Vec<T>>
class Ring {
public:
  void push_back();
  void reserve(size_type amount);
  
  auto size() -> size_type {
    return data_.size();
  }
  
  auto capacity() -> size_type {
    return data_.size();
  }
  
  auto back() -> size_type {

  }

private:
  Container data_;
};