#ifndef _CORE_EXPECTED_
#define _CORE_EXPECTED_

template<typename E>
class Unexpected {
public:
  Unexpected() = delete;

  Unexpected(E&& err)
    : err_{std::move(err)}
  { }

private:
  T err_;
};

template<typename T, typename E>
class Expected {
  friend Unexpected<E>;
public:
  Expected() = default;
  
  Expected(Unexpected<E>&& unexpected)
    : m_{ std::move(unexpected.error_) }
  { }
  
  ~Expected() {
    if(has_value())
    {
      m_.value_.~T();
    }
    else
    {
      m_.error_.~E();
    }
  }
private:
  union {
    T value_;
    E error_;
  } m_;
  bool has_value_;
};

#endif /* _CORE_EXPECTED_ */