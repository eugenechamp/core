#ifndef _CORE_TS_QUEUE_HPP_
#define _CORE_TS_QUEUE_HPP_

#include<condition_variable>
#include<mutex>
#include<queue>

template<typename T>
class TsQueue {
public:
  TsQueue() = default;

  void push(const T& element) {
    std::unique_lock lock{mtx_};
    queue_.push(element);
    condition_.notify_one();
  }

  void push(T&& element) {
    std::unique_lock lock{mtx_};
    queue_.push(CORE_MOVE(element));
    condition_.notify_one();
  }

  void wait_pop(T& value) {
    std::unique_lock lock{mtx_};
    condition_.wait(lock, [this] { return !queue_.empty(); });
    value = std::move(queue_.front());
    queue_.pop();
  }

  bool try_pop(T& value) {
    std::unique_lock lock(mtx_);
    
    if(queue_.empty())
      return false;
    
    value = std::move(queue_.front());
    queue_.pop();
    
    return true;
  }

  bool empty() const {
    std::unique_lock lock(mtx_);
    return queue_.empty();
  }

private:
  std::queue<T> queue_;
  mutable std::mutex mtx_;
  std::condition_variable condition_;
};

#endif /* _CORE_TS_QUEUE_HPP_ */