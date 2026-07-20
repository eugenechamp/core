#include<core/thread_pool.hpp>
#include<core/span.hpp>
#include<core/par_for.hpp>
#include<iostream>
#include<span>

u64 sum(const Vec<i32>& span) {
  u64 sum{};
  for(auto element : span) 
  {
    sum += element;
  }
  return sum;
}

int main() {
  ThreadPool& thread_pool = ThreadPool::instance();
  
  std::atomic<u32> counter;

  for(auto i = 0; i < 1'000; i++)
  {
    thread_pool.schedule([&counter]() {
      counter.fetch_add(1, std::memory_order_relaxed);
    });
  }
  
  //thread_pool.join_all();
  // std::cout << counter.load();


  Vec<i32> integers;
  integers.resize(10'000'000);

  core::par_for(integers, [](std::span<i32> span) {
    std::cout <<"Thread is invoked with id:"<< std::this_thread::get_id() << "\n";
    for(auto& element : span) 
    {
      element += 5;
    }
  });

  std::cout << (sum(integers));

}