#include<benchmark/benchmark.h>

#include<core/function.hpp>
#include<core/def.hpp>

static void std_function_call_operator(benchmark::State& state) {
  std::function<void()> fn = []{};
  for (auto _ : state)
  {
    fn();
    benchmark::DoNotOptimize(fn);
  }
}
BENCHMARK(std_function_call_operator);

static void core_fn_call_operator(benchmark::State& state) {
  Fn<void()> fn = []() {};
  for (auto _ : state)
  {
    fn();
    benchmark::DoNotOptimize(fn);
  }
}
BENCHMARK(core_fn_call_operator);

static void core_fn48_call_operator(benchmark::State& state) {
  Fn48<void()> fn = []() {};
  for (auto _ : state)
  {
    fn();
    benchmark::DoNotOptimize(fn);
  }
}
BENCHMARK(core_fn48_call_operator);

static void core_fn32_call_operator(benchmark::State& state) {
  Fn32<void()> fn = []{};
  for (auto _ : state)
  {
    fn();
    benchmark::DoNotOptimize(fn);
  }
}
BENCHMARK(core_fn32_call_operator);

static void core_fn24_call_operator(benchmark::State& state) {
  Fn24<void()> fn = []{};
  for (auto _ : state)
  {
    fn();
    benchmark::DoNotOptimize(fn);
  }
}
BENCHMARK(core_fn24_call_operator);

static void function_pointer_call(benchmark::State& state) {
  void(*fn)() = []{};
  for (auto _ : state)
  {
    fn();
    benchmark::DoNotOptimize(fn);
  }
}
BENCHMARK(function_pointer_call);

template<int NUM, typename TFn>
static void populate(std::vector<TFn>& vec) {
  if constexpr(NUM > 0)
  {
    vec.emplace_back([]{ return NUM; });
    populate<NUM - 1>(vec);
  }
  else 
  {
    return;
  }
}

static i32 get_num() {
  return 42;
}

template<typename Fx>
static void call_fn(benchmark::State& state) {
  std::vector<Fx> vec;
  // populate<100>(vec);
  for(i32 i {}; i < 10'000; i++)
  {
    vec.push_back(Fx([i]{return i;}));
  }
  i32 result{};
  for(auto _ : state)
  {
    for(auto& fn : vec)
    {
      result += fn();
    }
    benchmark::DoNotOptimize(result);
  }
} 
BENCHMARK(call_fn<std::function<i32()>>);
BENCHMARK(call_fn<Fn<i32()>>);
BENCHMARK(call_fn<Fn48<i32()>>);
BENCHMARK(call_fn<Fn40<i32()>>);
BENCHMARK(call_fn<Fn32<i32()>>);
BENCHMARK(call_fn<Fn24<i32()>>);


