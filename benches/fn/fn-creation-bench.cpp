#include<benchmark/benchmark.h>
#include<core/function.hpp>
#include<core/vector.hpp>
#include<core/def.hpp>

// static void func() {}

// template<typename Fx>
// static void default_ctor(benchmark::State& state) {
//   for (auto _ : state)
//   {
//     Fx fn;
//     benchmark::DoNotOptimize(fn);
//   }
// }

// BENCHMARK(default_ctor<std::function<void()>>);
// BENCHMARK(default_ctor<Fn<void()>>);
// BENCHMARK(default_ctor<Fn48<void()>>);
// BENCHMARK(default_ctor<Fn40<void()>>);
// BENCHMARK(default_ctor<Fn32<void()>>);
// BENCHMARK(default_ctor<Fn24<void()>>);

// template<typename Fx>
// static void move_ctor(benchmark::State& state) {
//   for (auto _ : state)
//   {
//     Fx fn = Fx{[]{}};
//     benchmark::DoNotOptimize(fn);
    
//   }
// }
// BENCHMARK(move_ctor<std::function<void()>>);
// BENCHMARK(move_ctor<Fn<void()>>);
// BENCHMARK(move_ctor<Fn48<void()>>);
// BENCHMARK(move_ctor<Fn40<void()>>);
// BENCHMARK(move_ctor<Fn32<void()>>);
// BENCHMARK(move_ctor<Fn24<void()>>);
// template<typename Fx>
// static void function_pointer_assignment(benchmark::State& state) {
//   for (auto _ : state)
//   {
//     Fx fn = func;
//     benchmark::DoNotOptimize(fn);
//   }
// }
// BENCHMARK(function_pointer_assignment<std::function<void()>>);
// BENCHMARK(function_pointer_assignment<Fn<void()>>);
// BENCHMARK(function_pointer_assignment<Fn48<void()>>);
// BENCHMARK(function_pointer_assignment<Fn40<void()>>);
// BENCHMARK(function_pointer_assignment<Fn32<void()>>);
// BENCHMARK(function_pointer_assignment<Fn24<void()>>);

// template<typename Fx>
// static void pointer_assignment_ctor(benchmark::State& state) {
//   for (auto _ : state)
//   {
//     Fx fn = func;
//     benchmark::DoNotOptimize(fn);
//   }
// }
// BENCHMARK(pointer_assignment_ctor<std::function<void()>>);
// BENCHMARK(pointer_assignment_ctor<Fn<void()>>);
// BENCHMARK(pointer_assignment_ctor<Fn48<void()>>);
// BENCHMARK(pointer_assignment_ctor<Fn40<void()>>);
// BENCHMARK(pointer_assignment_ctor<Fn32<void()>>);
// BENCHMARK(pointer_assignment_ctor<Fn24<void()>>);

// template<typename Fx>
// static void rvalue_assignment(benchmark::State& state) {
//   Fx fn;
//   for (auto _ : state)
//   {
//     fn = []{};
//     benchmark::DoNotOptimize(fn);
//   }
// }
// BENCHMARK(rvalue_assignment<std::function<void()>>);
// BENCHMARK(rvalue_assignment<Fn<void()>>);
// BENCHMARK(rvalue_assignment<Fn48<void()>>);
// BENCHMARK(rvalue_assignment<Fn40<void()>>);
// BENCHMARK(rvalue_assignment<Fn32<void()>>);
// BENCHMARK(rvalue_assignment<Fn24<void()>>);

// template<typename Fx>
// static void move_assignment(benchmark::State& state) {
//   Fx fn;
//   for (auto _ : state)
//   {
//     fn = Fx([]{});
//     benchmark::DoNotOptimize(fn);
//   }
// }
// BENCHMARK(move_assignment<std::function<void()>>);
// BENCHMARK(move_assignment<Fn<void()>>);
// BENCHMARK(move_assignment<Fn48<void()>>);
// BENCHMARK(move_assignment<Fn40<void()>>);
// BENCHMARK(move_assignment<Fn32<void()>>);
// BENCHMARK(move_assignment<Fn24<void()>>);

// template<typename Fx>
// static void function_swap(benchmark::State& state) {
//   Fx fn = []{ return 1; };
//   Fx fn2 = []{ return 2; };

//   for (auto _ : state)
//   {
//     fn.swap(fn2);
//     benchmark::DoNotOptimize(fn);
//     benchmark::DoNotOptimize(fn2);
//   }
// }
// BENCHMARK(function_swap<std::function<i32()>>);
// BENCHMARK(function_swap<Fn<i32()>>);
// BENCHMARK(function_swap<Fn48<i32()>>);
// BENCHMARK(function_swap<Fn40<i32()>>);
// BENCHMARK(function_swap<Fn32<i32()>>);
// BENCHMARK(function_swap<Fn24<i32()>>);

// static auto get_num() -> i32 {
//   return 42;
// }

// static void std_function_target(benchmark::State& state) {

//   std::function<i32()> func = get_num;

//   for(auto _ : state) 
//   {
//     auto* ptr = func.target<i32(*)()>();
//     if(ptr && *ptr == &get_num) 
//     {
//       benchmark::DoNotOptimize(ptr);
//     }
//     benchmark::DoNotOptimize(ptr);
//   }

//   benchmark::DoNotOptimize(func);
// }
// BENCHMARK(std_function_target);

// static void core_fn_target(benchmark::State& state) {

//   Fn<i32()> func = get_num;

//   for(auto _ : state) 
//   {
//     auto* ptr = func.target<i32(*)()>();
//     if(ptr && *ptr == &get_num)
//     {
//       benchmark::DoNotOptimize(ptr);
//     }
//     benchmark::DoNotOptimize(ptr);
//   }
  
//   benchmark::DoNotOptimize(func);
// }
// BENCHMARK(core_fn_target);

template<typename Fx>
static void add_function_to_vector(benchmark::State& state) {

  for(auto _ : state) 
  {
    Vec<Fx> funcs;
    funcs.reserve(10'000u);
    for(u32 i{}; i < funcs.capacity(); i++)
    {
      // funcs[i] = Fx([]{});
      funcs.push_back(Fx([]{}));
    }

    benchmark::DoNotOptimize(funcs);
  }
}
BENCHMARK(add_function_to_vector<Fn<void()>>);
BENCHMARK(add_function_to_vector<std::function<void()>>);

template<typename Fx>
static void add_function_to_vector_and_iterate(benchmark::State& state) {

  Vec<Fx> funcs;
  funcs.reserve(10'000u);
  for(u32 i{}; i < funcs.capacity(); i++)
  {
    funcs.push_back(Fx([]{}));
  }

  for(auto _ : state) 
  {
    for(auto& fn : funcs)
    {
      fn();
    }
    
    benchmark::DoNotOptimize(funcs);
  }
}
BENCHMARK(add_function_to_vector_and_iterate<Fn<void()>>);
BENCHMARK(add_function_to_vector_and_iterate<std::function<void()>>);

// template<typename Fx>
// static void add_function_to_vector_std(benchmark::State& state) {
//   std::vector<Fx> funcs;
//   funcs.reserve(10'000u);

//   for(auto _ : state) 
//   {
//     for(i32 i{}; i < funcs.capacity(); i++)
//     {
//       funcs.push_back(Fx());
//     }
//     benchmark::DoNotOptimize(funcs);
//     funcs.resize(0);
//   }
// }
// BENCHMARK(add_function_to_vector_std<std::function<void()>>);
// BENCHMARK(add_function_to_vector_std<Fn<void()>>);

static void reverse_vector_fn(benchmark::State& state) {
  Vec<Fn<u32()>> funcs;
  funcs.reserve(10'000u);
  for(u32 i{}; i < funcs.capacity(); i++)
  {
    funcs.push_back(Fn([i]{ return i; }));
  }

  for(auto _ : state)
  {
    auto end = funcs.size() - 1;
    auto mid = funcs.size() / 2;
    for(u32 i {}; i != mid; i++)
    {
      auto& lhs = funcs[i];
      auto& rhs = funcs[end - i];
      lhs.swap(rhs);
    }
  }
  // benchmark::DoNotOptimize(funcs);
}
BENCHMARK(reverse_vector_fn);
static void reverse_vector_std_function(benchmark::State& state) {
  Vec<std::function<u32()>> funcs;
  funcs.reserve(10'000u);
  for(u32 i{}; i < funcs.capacity(); i++)
  {
    funcs.push_back(std::function([i]{ return i; }));
  }

  for(auto _ : state)
  {
    auto end = funcs.size() - 1;
    auto mid = funcs.size() / 2;
    for(u32 i {}; i != mid; i++)
    {
      auto& lhs = funcs[i];
      auto& rhs = funcs[end - i];
      lhs.swap(rhs);
    }
  }
  // benchmark::DoNotOptimize(funcs);
}
BENCHMARK(reverse_vector_std_function);