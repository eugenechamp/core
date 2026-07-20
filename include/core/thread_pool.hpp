#ifndef _CORE_THREAD_POOL_HPP_
#define _CORE_THREAD_POOL_HPP_

#include "vector.hpp"
#include "concepts.hpp"
#include "function.hpp"
#include "ts_queue.hpp"
#include <future>
#include <thread>

template<typename T>
using Future = std::future<T>;

using Thread = std::thread;
using Jthread = std::jthread;

class ThreadPool {
private:
  ThreadPool();

public:
  static auto instance() -> ThreadPool& {
    static ThreadPool thread_pool;
    return thread_pool;
  }

  ~ThreadPool();
  
  auto size() {
    return workers_.size();
  }

  template<typename Fx>
  auto schedule(Fx&& task) -> Future<std::invoke_result_t<Fx>> {
    using result_type = std::invoke_result_t<Fx>;
#if 1
    std::packaged_task packaged_task(CORE_MOVE(task));

    Future future = packaged_task.get_future();

    tasks_.push([task = CORE_MOVE(packaged_task)]() mutable {
      task();
    });

#else
    std::promise<result_type> promise;
    Future future = promise.get_future();

    tasks_.push([promise = CORE_MOVE(promise), task = CORE_MOVE(task)]() mutable {
      if constexpr (std::is_void_v<result_type>)
      {
        task();
        promise.set_value();
      }
      else
      {
        promise.set_value(task());
      }
    })

#endif // 0

    
    return future;
  }

  static auto get_thread_count() -> u32;
  void worker_thread();
private:
  // think about thread and job lifetimes
  TsQueue<Fn<void()>> tasks_;
  Vec<Jthread> workers_;
  std::condition_variable wakeup_condition_;
  std::mutex enqueue_mtx_;
  std::mutex sleep_mtx_;
  std::atomic_bool done_;
};

#endif