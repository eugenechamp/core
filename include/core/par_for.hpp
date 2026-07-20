#ifndef _CORE_PAR_FOR_HPP_
#define _CORE_PAR_FOR_HPP_
#include "def.hpp"
#include "concepts.hpp"
#include "thread_pool.hpp"
#include <span>
#include <vector>

namespace core {
  
  template<typename Container, Callable<void(typename Container::value_type&)> Fx>
  void par_for(Container& container, Fx&& fn) {
    
  }

  template<typename Container, Callable<void(std::span<typename Container::value_type>)> Fx>
  void par_for(Container& container, Fx&& fn) {
    using value_type = typename Container::value_type;
    ThreadPool& pool = ThreadPool::instance();
    
    u32 team_count = pool.size();
    u32 batch_size = container.size() / (team_count + 1);
    
    struct alignas(8) CompletionFlag {
      std::atomic_bool flag;
      char padded[64 - sizeof(std::atomic_bool)];
    };

    /*Vec<CompletionFlag> results;
    results.resize(team_count);*/

    // std::atomic<u32> done = team_count;

    Vec<Future<void>> completed;
    completed.reserve(team_count); // mb one extra, team_count - 1?

    for(u32 i = 0; i < team_count; i++)
    {
      u32 begin = batch_size * (i + 1);
      u32 end = std::min(container.size(), begin + batch_size);
     
      completed.push_back(pool.schedule([begin, end, callable = fn, &container]() mutable {
        auto* base = container.begin();
        callable(std::span(base + begin, base + end));
      }));

     //pool.schedule([begin, end, callable = fn, &container, &done]() mutable {
     //  auto* base = container.begin();
     //  callable(std::span(base + begin, base + end));
     //  done.fetch_sub(1);
     //});
    }
    //

    //done.wait(0u);

   /* while(done.load() > 0u)
    {}*/
    
    fn(std::span(container.begin(), container.begin() + batch_size));
    
    for(auto& future : completed)
    {
      future.wait();
    }
  }

  template<typename Container, Invocable<void(std::span<typename Container::value_type>)> Fx>
  void par_for(u32 invocation_count, Container& container, Fx&& fn) {

  }

}

#endif