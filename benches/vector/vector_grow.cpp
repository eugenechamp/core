#include <benchmark/benchmark.h>
#include <core/vector.hpp>
#include <core/function.hpp>

struct alignas(16) Vec4 {
  float x, y, z, w;
};

struct Vec4x4 {
  Vec4 vecs[4];
};

template<typename Vector>
static void bm_vector_grow_realloc(benchmark::State& state) {
  u32 size = state.range(0);
  for(auto _ : state)
  {
    Vector vec;
    vec.resize(size);
    vec.push_back(typename Vector::value_type{});
    benchmark::DoNotOptimize(vec);
  }
}
BENCHMARK(bm_vector_grow_realloc<Vec<Vec4>>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_vector_grow_realloc<std::vector<Vec4>>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_vector_grow_realloc<Vec<std::string>>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_vector_grow_realloc<std::vector<std::string>>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_vector_grow_realloc<Vec<std::unique_ptr<std::string>>>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_vector_grow_realloc<std::vector<std::unique_ptr<std::string>>>)->Arg(1000)->Arg(10'000)->Arg(100'000);

template<typename Vector>
static void bm_move_push_back(benchmark::State& state) {
  u32 size = state.range(0);
  for(auto _ : state)
  {
    Vector vec;
    for(u32 i = 0; i < size; i++)
    {
      vec.push_back(typename Vector::value_type{});
    }
    benchmark::DoNotOptimize(vec);
  }
}
BENCHMARK(bm_move_push_back<Vec<Vec4>>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_move_push_back<Vec<std::string>>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_move_push_back<Vec<std::unique_ptr<std::string>>>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_move_push_back<std::vector<Vec4>>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_move_push_back<std::vector<std::string>>)->Arg(1000)->Arg(10'000)->Arg(100'000);

template<typename Vector>
static void bm_push_back_reserve(benchmark::State& state) {
  u32 size = state.range(0); 
  
  for (auto _ : state)
  {
    Vector vec;
    vec.reserve(size);
    for(u32 i = 0; i < size; i++)
    {
      vec.push_back(typename Vector::value_type{});
    }
    benchmark::DoNotOptimize(vec);
  }
}
BENCHMARK(bm_push_back_reserve<Vec<Vec4>>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_push_back_reserve<Vec<Vec4x4>>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_push_back_reserve<Vec<std::string>>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_push_back_reserve<Vec<Fn<void()>>>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_push_back_reserve<std::vector<Vec4>>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_push_back_reserve<std::vector<std::string>>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_push_back_reserve<std::vector<Fn<void()>>>)->Arg(1000)->Arg(10'000)->Arg(100'000);

template<typename T>
static void bm_emplace_at_reserve_unsafe(benchmark::State& state) {
  u32 size = state.range(0); 
  
  for (auto _ : state)
  {
    Vec<T> vec;
    vec.reserve(size);
    vec._set_size(size);
    for(u32 i = 0; i < size; i++)
    {
      vec._emplace_at(i, T{});
    }
    benchmark::DoNotOptimize(vec);
  }
}
BENCHMARK(bm_emplace_at_reserve_unsafe<Vec4>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_emplace_at_reserve_unsafe<Vec4x4>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_emplace_at_reserve_unsafe<std::string>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_emplace_at_reserve_unsafe<Fn<void()>>)->Arg(1000)->Arg(10'000)->Arg(100'000);

template<typename Vector>
static void bm_resize_construct(benchmark::State& state) {
  u32 size = state.range(0);
  
  for (auto _ : state)
  {
    Vector vec;
    vec.resize(size);
    for(u32 i = 0; i < size; i++)
    {
      vec[i] = typename Vector::value_type{};
    }
    benchmark::DoNotOptimize(vec);
  }
}
BENCHMARK(bm_resize_construct<Vec<Vec4>>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_resize_construct<Vec<Vec4x4>>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_resize_construct<Vec<std::string>>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_resize_construct<Vec<Fn<void()>>>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_resize_construct<std::vector<Vec4>>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_resize_construct<std::vector<std::string>>)->Arg(1000)->Arg(10'000)->Arg(100'000);
BENCHMARK(bm_resize_construct<std::vector<Fn<void()>>>)->Arg(1000)->Arg(10'000)->Arg(100'000);

// template<typename Vector>
// static void bm_resize_construct(benchmark::State& state) {
  
//   constexpr auto L2_SIZE = 256 * 1000; 

  
//   u32 size = state.range(0); 
  
//   for (auto _ : state)
//   {
//     Vector vec;
//     vec.resize(size);
//     for(u32 i = 0; i < size; i++)
//     {
//       vec[i] = typename Vector::value_type{};
//     }
//     benchmark::DoNotOptimize(vec);
//   }
// }


BENCHMARK_MAIN();