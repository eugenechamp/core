#include"core/thread_pool.hpp"
#include"core/vector.hpp"
#include<thread>
#include<condition_variable>
#include<mutex>
#include<deque>

ThreadPool::ThreadPool()
  : workers_(Vec<Jthread>::with_capacity(get_thread_count())),
    done_{ false }
{ 
  for(u32 i{}; i < workers_.capacity(); i++)
  {
    workers_.emplace_back([this] { worker_thread(); });
  }
}

ThreadPool::~ThreadPool() {
  done_ = true;
}

u32 ThreadPool::get_thread_count()
{
  return u32((Thread::hardware_concurrency() / 2) - 1);
}

void ThreadPool::worker_thread() {
  while (!done_)
  {
    Fn<void()> task;
    if (tasks_.try_pop(task)) 
    {
      task();
    }
    else
    {
      std::this_thread::yield();
    }
  }
}